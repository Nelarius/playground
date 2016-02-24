#include "system/DebugRenderSystem.h"
#include "opengl/Use.h"
#include "opengl/VertexArrayObjectFactory.h"
#include "app/Context.h"
#include <GL/glew.h>

namespace {

// TODO: this needs to be exposed in a public API
const std::size_t MaxDebugLines = 1000u;

}

namespace pg {
namespace system {

DebugRenderSystem::DebugRenderSystem(Context& context)
    : context_{ context },
    defaultProjection_{},
    debugLines_{},
    lineLifeTimes_{},
    lineBuffer_{ GL_ARRAY_BUFFER },
    lineBufferArray_{ 0 },
    showLines_{ false },
    showBoxes_{ false } {
    defaultProjection_ = math::Matrix4f::perspective(70.0f, 1.5f, 0.1f, 100.0f);

    // set up the debug buffers and VAOs
    lineBuffer_.dataStore(MaxDebugLines, 6 * sizeof(float), NULL, GL_STREAM_DRAW);
    opengl::VertexArrayObjectFactory fact{ &lineBuffer_, context_.shaderManager.get("basic") };
    fact.addStandardAttribute(opengl::VertexAttribute::Vertex);
    lineBufferArray_ = fact.getVao();
}

void DebugRenderSystem::configure(ecs::EventManager& events) {
    events.subscribe<ecs::ComponentAssignedEvent<component::Camera>>(*this);
    events.subscribe<ShowDebugLines>(*this);
    events.subscribe<ShowDebugBoxes>(*this);
    events.subscribe<RenderDebugLine>(*this);
}

void DebugRenderSystem::update(ecs::EntityManager& entities, ecs::EventManager& events, float dt) {
    if (lineLifeTimes_.size() != 0u) {
        for (std::size_t i = lineLifeTimes_.size() - 1u; i-- > 0u;) {
            lineLifeTimes_[i] -= dt;
            if (lineLifeTimes_[i] < 0.f) {
                std::swap(lineLifeTimes_[i], lineLifeTimes_.back());
                lineLifeTimes_.pop_back();
                std::swap(debugLines_[i], debugLines_.back());
                debugLines_.pop_back();
            }
        }
    }

    math::Matrix4f cameraMatrix{ defaultProjection_ };
    if (cameraEntity_.isValid()) {
        float aspectRatio = float(context_.window->width()) / context_.window->height();
        auto transform = cameraEntity_.component< component::Transform >();
        auto view = math::Matrix4f::translation(transform->position)
            * math::Matrix4f::rotation(transform->rotation)
            * math::Matrix4f::scale(transform->scale);
        auto camera = cameraEntity_.component< component::Camera >();
        auto proj = math::Matrix4f::perspective(
            camera->verticalFov,
            aspectRatio,
            camera->nearPlane,
            camera->farPlane
            );
        cameraMatrix = proj * view.inverse();
    }

    // create rendering state
    auto* shader = context_.shaderManager.get("basic");
    auto* vbo = context_.meshManager.get("builtin/cube.obj");

    opengl::VertexArrayObjectFactory factory{ vbo, shader };
    factory.addAttribute("vertex", 3, GL_FLOAT, GL_FALSE, 24);    // skip the normals
    auto vao = factory.getVao();

    {
        opengl::UseProgram use{ *shader };
        shader->setUniform("color", math::Vec3f{ 1.0f, 0.2f, 0.2f });
        shader->setUniform("camera", cameraMatrix);
        if (showBoxes_) {
            /// BOUNDING BOXES
            ///////////////////////////////////////////////////////////
            for (ecs::Entity entity : entities.join< component::Transform, math::AABox >()) {
                auto t = entity.component< component::Transform >();
                auto bb = entity.component<math::AABox>();
                math::Vec3f min = t->scale.hadamard(bb->min);
                math::Vec3f max = t->scale.hadamard(bb->max);
                math::Vec3f center = 0.5f * (min + max);
                math::Vec3f scale{ max.x - min.x, max.y - min.y, max.z - min.z };
                math::Matrix4f S = math::Matrix4f::scale(scale);              // scale to current model dimensions
                math::Matrix4f R = math::Matrix4f::rotation(t->rotation);     // rotate to world coords
                math::Matrix4f Tl = math::Matrix4f::translation(center);        // translate to local coords
                math::Matrix4f Tw = math::Matrix4f::translation(t->position);   // translate to world coords
                math::Matrix4f TRS = Tw * R  * Tl * S;

                shader->setUniform("model", TRS);
                {
                    opengl::UseArray array(vao);
                    glDrawArrays(GL_LINES, 0, vbo->count() / vao.elementsPerIndex());
                }
            }
        }

        if (showLines_) {
            /// LINES
            ///////////////////////////////////////////////////////////
            const std::size_t LineCount = debugLines_.size();
            math::Vec3f* lines = (math::Vec3f*)lineBuffer_.mapBufferRange(0, LineCount * 6u * sizeof(float), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
            for (unsigned i = 0u; i < 2u * LineCount; i += 2u) {
                unsigned index = i / 2u;
                lines[i] = debugLines_[index].origin;
                lines[i + 1u] = debugLines_[index].end;
            }
            lineBuffer_.unmapBuffer();

            shader->setUniform("color", math::Vec3f{ 0.9f, 0.6f, 0.15f });
            shader->setUniform("model", math::Matrix4f{});
            {
                opengl::UseArray array(lineBufferArray_);
                glDrawArrays(GL_LINES, 0, debugLines_.size());
            }
        }
    }
}

void DebugRenderSystem::receive(const ecs::ComponentAssignedEvent<component::Camera>& event) {
    cameraEntity_ = event.entity;
}

void DebugRenderSystem::receive(const ShowDebugLines& event) {
    showLines_ = event.show;
}

void DebugRenderSystem::receive(const ShowDebugBoxes& event) {
    showBoxes_ = event.show;
}

void DebugRenderSystem::receive(const RenderDebugLine& line) {
    if (debugLines_.size() < MaxDebugLines) {
        debugLines_.push_back(math::Line{ line.start, line.end });
        lineLifeTimes_.push_back(line.lifeTime);
    }
    else {
        LOG_INFO << "There are already " << debugLines_.size() << " debug lines. No more can be added!";
    }
}

}
}
