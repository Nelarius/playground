#include "json11/json11.hpp"
#include "app/WorldIO.h"
#include "opengl/VertexArrayObject.h"
#include "opengl/VertexArrayObjectFactory.h"
#include "manager/MeshManager.h"
#include "manager/TextFileManager.h"
#include "system/Material.h"
#include "system/RenderSystem.h"
#include "system/Events.h"
#include "component/Include.h"
#include "utils/Log.h"
#include "utils/Assert.h"
#include "utils/File.h"
#include "Wrenly.h"
#include "system/WrenBindings.h"

namespace pg {

WorldIO::WorldIO(Context& context)
    : context_(context)
{}

void WorldIO::read(
    const std::string& file,
    ecs::EntityManager& entities,
    ecs::EventManager& events
    ) {
    // bind the scripting API so that Wren can find the methods
    wren::bindVectorModule();
    wren::bindMathModule();
    wren::bindQuaternionModule();
    wren::bindEntityModule();
    wren::bindComponentModule();
    wren::bindNumberArrayModule();
    wren::bindRingBufferModule();
    wren::bindImguiModule();
    wren::bindUtilsModule();
    wren::bindSystemsModule();

    auto json = pg::fileToString("data/scene.json");
    std::string error{ "" };
    auto scene = json11::Json::parse(json, error).array_items();

    for (auto object : scene) {
        ecs::Entity entity = entities.create();
        auto transform = object["transform"];
        auto renderable = object["renderable"];
        auto script = object["script"];
        auto camera = object["camera"];
        auto pointLight = object["pointLight"];
        auto wrenScript = object["wren"];

        if (!transform.is_null()) {
            auto contents = transform.object_items();
            auto pos = contents["position"].array_items();
            auto rot = contents["rotation"].array_items();
            auto sca = contents["scale"].array_items();
            entity.assign< component::Transform >(
                math::Vec3f(float(pos[0].number_value()), float(pos[1].number_value()), float(pos[2].number_value())),
                math::Quatf(float(rot[0].number_value()), float(rot[1].number_value()), float(rot[2].number_value()), float(rot[3].number_value())),
                math::Vec3f(float(sca[0].number_value()), float(sca[1].number_value()), float(sca[2].number_value()))
                );
        }   // transform

        if (!renderable.is_null()) {
            auto contents = renderable.object_items();
            opengl::BufferObject* buffer = context_.meshManager.get(contents["model"].string_value());
            opengl::Program* shader{ nullptr };
            opengl::VertexArrayObject vao{ 0 };
            system::Material mat;

            if (!contents["material"].is_null()) {
                auto specular = contents["material"].object_items();
                std::unordered_map<std::string, float> uniforms{};
                uniforms.emplace("shininess", float(specular["shininess"].number_value()));
                auto scolor = specular["specularColor"].array_items();
                math::Vec3f specColor(
                    float(scolor[0].number_value()), float(scolor[1].number_value()), float(scolor[2].number_value())
                    );
                auto acolor = specular["ambientColor"].array_items();
                math::Vec3f surfColor(
                    float(acolor[0].number_value()), float(acolor[1].number_value()), float(acolor[2].number_value())
                    );
                auto bcolor = specular["baseColor"].array_items();
                math::Vec3f baseColor(
                    float(bcolor[0].number_value()), float(bcolor[1].number_value()), float(bcolor[2].number_value())
                    );
                uniforms.emplace("specColor_r", specColor.r);
                uniforms.emplace("specColor_g", specColor.g);
                uniforms.emplace("specColor_b", specColor.b);
                uniforms.emplace("ambientColor_r", surfColor.r);
                uniforms.emplace("ambientColor_g", surfColor.g);
                uniforms.emplace("ambientColor_b", surfColor.b);
                uniforms.emplace("baseColor_r", baseColor.r);
                uniforms.emplace("baseColor_g", baseColor.g);
                uniforms.emplace("baseColor_b", baseColor.b);

                mat.type = system::MaterialType::Specular;
                mat.uniforms = uniforms;
                shader = context_.shaderManager.get("specular");
                opengl::VertexArrayObjectFactory factory{ buffer, shader };
                factory.addStandardAttribute(opengl::VertexAttribute::Vertex);
                factory.addStandardAttribute(opengl::VertexAttribute::Normal);
                vao = factory.getVao();
            }
            opengl::VertexArrayObjectFactory factory{ buffer, shader };
            entity.assign< component::Renderable >(buffer, shader, vao, mat);
            const auto& bb = context_.meshManager.getBoundingBox(contents["model"].string_value());
            entity.assign<math::AABox>(bb.min, bb.max);
        }   //renderable

        if (!pointLight.is_null()) {
            auto contents = pointLight.object_items();
            auto intensity = contents["intensity"].array_items();
            entity.assign< component::PointLight >(
                math::Vec3f(float(intensity[0].number_value()), float(intensity[1].number_value()), float(intensity[2].number_value())),
                float(contents["attenuation"].number_value()),
                float(contents["ambientCoefficient"].number_value())
                );
        }

        if (!camera.is_null()) {
            auto contents = camera.object_items();
            // cast doubles (numbers) to float, to avoid narrowing errors
            entity.assign< component::Camera >(
                (float)contents["fov"].number_value(),
                (float)contents["nearPlane"].number_value(),
                (float)contents["farPlane"].number_value(),
                true,
                true
                );
        }   //camera

        if (!wrenScript.is_null()) {
            auto mod = wrenScript.string_value();
            auto path = mod + ".wren";
            wrenly::Wren vm;
            vm.executeString(
                "import \"builtin/entity\" for Entity\n"
                "var entity = Entity.new()\n"
                );
            wrenly::Method set = vm.method("main", "entity", "set_(_)");
            set(int(entity.id().index()));
            wrenly::Result res = vm.executeModule(mod);
            if (res == wrenly::Result::Success) {
                auto activate = vm.method("main", "activate", "call()");
                auto deactivate = vm.method("main", "deactivate", "call()");
                auto update = vm.method("main", "update", "call(_)");
                entity.assign< component::Script >(
                    context_.textFileManager.id(path),
                    std::move(vm),
                    activate,
                    deactivate,
                    update
                    );
            }
            else if (res == wrenly::Result::CompileError) {
                LOG_ERROR << "Entity(" << entity.id().index() << ", " << entity.id().version() << "): there was an error when compiling " << mod << " module.";
            }
            else {
                LOG_ERROR << "Entity(" << entity.id().index() << ", " << entity.id().version() << "): there was a runtime error in " << mod << " module.";
            }
        }   // wren
    }
}

}
