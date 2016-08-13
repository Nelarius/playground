#include "SceneIO.h"
#include "app/Context.h"
#include "component/Include.h"
#include "ecs/Include.h"
#include "manager/TextFileManager.h"
#include "math/Vector.h"
#include "opengl/VertexAttributes.h"
#include "utils/Assert.h"
#include "utils/Json.h"
#include "utils/Log.h"
#include "system/Material.h"
#include "Wren++.h"

#include <cstdio>
#include <vector>
#include <string>
#include <fstream>

namespace pg {

using namespace math;
using namespace component;

void readScene(Context& context, const char* file) {
    JsonParser json(file);

    // parse while the scene array has tokens
    char queryString[64];
    for (int i = 0;; ++i) {
        sprintf(queryString, "entities[%i]", i);
        auto entity = json.query(queryString);
        if (!entity) {
            break;
        }

        ecs::Entity newEntity = context.entityManager.create();

        JsonToken transform = json.query(entity, "transform");
        if (transform) {
            Vec3f position{
                json.query(transform, "position[0]").as<float>(),
                json.query(transform, "position[1]").as<float>(),
                json.query(transform, "position[2]").as<float>()
            };
            Vec3f scale{
                json.query(transform, "scale[0]").as<float>(),
                json.query(transform, "scale[1]").as<float>(),
                json.query(transform, "scale[2]").as<float>()
            };
            Quatf rotation{
                json.query(transform, "rotation[0]").as<float>(),
                json.query(transform, "rotation[1]").as<float>(),
                json.query(transform, "rotation[2]").as<float>(),
                json.query(transform, "rotation[3]").as<float>()
            };
            newEntity.assign<Transform>(position, rotation, scale);
        }

        JsonToken camera = json.query(entity, "camera");
        if (camera) {
            newEntity.assign<Camera>(
                json.query(camera, "fov").as<float>(),
                json.query(camera, "nearPlane").as<float>(),
                json.query(camera, "farPlane").as<float>(),
                true, true
            );
        }

        JsonToken script = json.query(entity, "script");
        if (script) {
            const char* module = script.as<const char*>();
            std::string path(module);
            path.append(".wren");
            wrenpp::VM vm;
            vm.executeString(
                "import \"pg/entity\" for Entity\n"
                "var entity = Entity.new()\n"
                );
            // make sure the method gets destroyed before the vm is moved into the entity's script component
            {
                wrenpp::Method set = vm.method("main", "entity", "set_(_)");
                set(int(newEntity.id().index()));
            }

            wrenpp::Result res = vm.executeModule(module);
            if (res == wrenpp::Result::Success) {
                newEntity.assign<Script>(
                    context.textFileManager.id(path),
                    std::move(vm)
                );
            }
            else if (res == wrenpp::Result::CompileError) {
                LOG_ERROR << "Entity(" << newEntity.id().index() << ", " << newEntity.id().version() << "): there was an error when compiling " << module << " module.";
            }
            else {
                LOG_ERROR << "Entity(" << newEntity.id().index() << ", " << newEntity.id().version() << "): there was a runtime error in " << module << " module.";
            }
        }

        JsonToken renderable = json.query(entity, "renderable");
        if (renderable) {
            const char* modelName = json.query(renderable, "model").as<const char*>();
            opengl::BufferObject* buffer = context.meshManager.get(modelName);
            opengl::Program* shader{ nullptr };
            system::Material mat;

            JsonToken material = json.query(renderable, "material");
            PG_ASSERT(material);
            math::Vec3f specularColor{
                json.query(material, "specularColor[0]").as<float>(),
                json.query(material, "specularColor[1]").as<float>(),
                json.query(material, "specularColor[2]").as<float>()
            };
            math::Vec3f baseColor{
                json.query(material, "baseColor[0]").as<float>(),
                json.query(material, "baseColor[1]").as<float>(),
                json.query(material, "baseColor[2]").as<float>()
            };
            math::Vec3f ambientColor{
                json.query(material, "ambientColor[0]").as<float>(),
                json.query(material, "ambientColor[1]").as<float>(),
                json.query(material, "ambientColor[2]").as<float>()
            };
            mat.ambientColor = ambientColor;
            mat.baseColor = baseColor;
            mat.specularColor = specularColor;
            mat.shininess = json.query(material, "shininess").as<float>();

            shader = context.shaderManager.get("specular");

            buffer->bind();
            opengl::VertexAttributes vao{
                {unsigned(shader->attribute("vertex")), opengl::AttributeType::Float, 3},
                {unsigned(shader->attribute("normal")), opengl::AttributeType::Float, 3}
            };
            buffer->unbind();

            newEntity.assign<component::Renderable>(buffer, shader, vao, mat);
            const auto& boundingBox = context.meshManager.getBoundingBox(modelName);
            newEntity.assign<math::AABoxf>(boundingBox.min, boundingBox.max);
        }
    }
}

void writeScene(Context& context, const char* file) {
    //
}

}
