#include "system/DebugRenderSystem.h"
#include "opengl/Use.h"
#include "opengl/VertexAttributes.h"
#include "app/Context.h"
#include <GL/glew.h>

namespace {

const std::size_t MaxTransientElements = 500u;

const float cubePoints[] = {
    -0.500000, -0.500000,  0.500000,    // 0
    0.500000, -0.500000,  0.500000,     // 1
    -0.500000,  0.500000,  0.500000,    // 2
    0.500000,  0.500000,  0.500000,     // 3
    -0.500000,  0.500000, -0.500000,    // 4
    0.500000,  0.500000, -0.500000,     // 5
    -0.500000, -0.500000, -0.500000,    // 6
    0.500000, -0.500000, -0.500000      // 7
};

const GLuint cubeLines[] = {
    0u, 2u, 2u, 4u, 4u, 6u, 6u, 0u,
    0u, 1u, 1u, 3u, 3u, 2u, 2u, 0u,
    6u, 7u, 7u, 5u, 5u, 4u, 4u, 6u,
    7u, 1u, 1u, 3u, 3u, 5u, 5u, 7u,

};

const GLuint cubeTris[] = {
    0u, 1u, 3u, 3u, 2u, 0u,
    5u, 7u, 6u, 6u, 4u, 5u,
    5u, 4u, 2u, 2u, 3u, 5u,
    7u, 1u, 0u, 0u, 6u, 7u,
    6u, 0u, 2u, 2u, 4u, 6u,
    5u, 3u, 1u, 1u, 7u, 5u
};

}

namespace pg {
namespace system {

DebugRenderSystem::DebugRenderSystem(Context& context)
    : context_{ context },
    defaultProjection_{},
    staticDebugLines_{},
    transientDebugLines_{},
    lineLifeTimes_{},
    staticDebugBoxes_{},
    transientDebugBoxes_{},
    boxLifeTimes_{},
    lineBuffer_{ GL_ARRAY_BUFFER },
    lineBufferArray_{},
    showLines_{ false },
    showBoundingBoxes_{ false },
    showDebugBoxes_{ false } {
    defaultProjection_ = math::Matrix4f::perspective(70.0f, 1.5f, 0.1f, 100.0f);

    // set up the debug buffers and VAOs
    lineBuffer_.dataStore(MaxTransientElements, 6 * sizeof(float), NULL, GL_STREAM_DRAW);
    lineBuffer_.bind();
    lineBufferArray_ = opengl::VertexAttributes{
        {unsigned(context_.shaderManager.get("basic")->attribute("vertex")), opengl::AttributeType::Float, 3}
    };
    lineBuffer_.unbind();

    // generate cube objects
    cubeVbo_ = 0u;
    glGenBuffers(1, &cubeVbo_);
    PG_ASSERT(cubeVbo_);
    GLint old;
    glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &old);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubePoints), cubePoints, GL_STATIC_DRAW);
    opengl::Program* basicShader = context_.shaderManager.get("basic");
    basicShader->use();
    glGenVertexArrays(1, &cubeVao_);
    glGetIntegerv(GL_ARRAY_BUFFER, &old);
    glBindVertexArray(cubeVao_);
    PG_ASSERT(cubeVao_);
    GLint index = glGetAttribLocation(basicShader->object(), "vertex");
    glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(index);
    glBindVertexArray(cubeVao_);
    basicShader->stopUsing();
    glBindBuffer(GL_ARRAY_BUFFER, old);
}

DebugRenderSystem::~DebugRenderSystem() {
    glDeleteBuffers(1, &cubeVbo_);
    glDeleteVertexArrays(1, &cubeVao_);
}

void DebugRenderSystem::configure(ecs::EventManager& events) {
    events.subscribe<ecs::ComponentAssignedEvent<component::Camera>>(*this);
    events.subscribe<ShowDebugLines>(*this);
    events.subscribe<ShowBoundingBoxes>(*this);
    events.subscribe<ShowDebugBoxes>(*this);
    events.subscribe<RenderDebugLine>(*this);
    events.subscribe<RenderDebugBox>(*this);
}

void DebugRenderSystem::update(ecs::EntityManager& entities, ecs::EventManager& events, float dt) {
    updateTransientElements_(lineLifeTimes_, transientDebugLines_, dt);
    updateTransientElements_(boxLifeTimes_, transientDebugBoxes_, dt);

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

    {
        opengl::UseProgram use{ *shader };
        shader->setUniform("color", math::Vec3f{ 1.0f, 0.2f, 0.2f });
        shader->setUniform("camera", cameraMatrix);
        if (showBoundingBoxes_) {
            /// BOUNDING BOXES
            ///////////////////////////////////////////////////////////
            for (ecs::Entity entity : entities.join< component::Transform, math::AABoxf >()) {
                auto t = entity.component< component::Transform >();
                auto bb = entity.component<math::AABoxf>();
                math::Vec3f min = t->scale.hadamard(bb->min);
                math::Vec3f max = t->scale.hadamard(bb->max);
                math::Vec3f center = 0.5f * (min + max);
                math::Vec3f scale{ max.x - min.x, max.y - min.y, max.z - min.z };
                math::Matrix4f S = math::Matrix4f::scale(scale);                // scale to current model dimensions
                math::Matrix4f R = math::Matrix4f::rotation(t->rotation);       // rotate to world coords
                math::Matrix4f Tl = math::Matrix4f::translation(center);        // translate to local coords
                math::Matrix4f Tw = math::Matrix4f::translation(t->position);   // translate to world coords
                math::Matrix4f TRS = Tw * R  * Tl * S;

                shader->setUniform("model", TRS);
                {
                    GLint old;
                    glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &old);
                    glBindVertexArray(cubeVao_);
                    glDrawElements(GL_LINES, 32, GL_UNSIGNED_INT, cubeLines);
                    glBindVertexArray(old);
                }
            }
        }

        const std::size_t PointCount = 2u*staticDebugLines_.size() + 2u*transientDebugLines_.size();
        if (showLines_ && PointCount > 0u) {
            /// LINES
            ///////////////////////////////////////////////////////////
            lineBuffer_.bind();
            math::Vec3f* lines = (math::Vec3f*)lineBuffer_.mapBufferRange(0, PointCount * 6u * sizeof(float), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
            for (unsigned i = 0u; i < 2u * staticDebugLines_.size(); i += 2u) {
                unsigned index = i / 2u;
                lines[i] = staticDebugLines_[index].origin;
                lines[i + 1u] = staticDebugLines_[index].end;
            }
            for (auto i = 2u * staticDebugLines_.size(); i < PointCount; i += 2u) {
                unsigned index = (i / 2u) - staticDebugLines_.size();
                lines[i] = transientDebugLines_[index].origin;
                lines[i + 1u] = transientDebugLines_[index].end;
            }
            lineBuffer_.unmapBuffer();

            shader->setUniform("color", math::Vec3f{ 0.9f, 0.6f, 0.15f });
            shader->setUniform("model", math::Matrix4f{});
            {
                opengl::UseArray array(lineBufferArray_);
                glDrawArrays(GL_LINES, 0, PointCount);
            }
            lineBuffer_.unbind();
        }

        if (showDebugBoxes_) {
            glBindBuffer(GL_ARRAY_BUFFER, cubeVbo_);
            for (auto& box : staticDebugBoxes_) {
                math::Matrix4f M = math::Matrix4f::translation(box.position) * math::Matrix4f::scale(box.scale);
                shader->setUniform("model", M);
                shader->setUniform("color", box.color);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, cubeTris);
            }
            for (auto& box : transientDebugBoxes_) {
                math::Matrix4f M = math::Matrix4f::translation(box.position) * math::Matrix4f::scale(box.scale);
                shader->setUniform("model", M);
                shader->setUniform("color", box.color);
                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, cubeTris);
            }
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }
    }
}

void DebugRenderSystem::receive(const ecs::ComponentAssignedEvent<component::Camera>& event) {
    cameraEntity_ = event.entity;
}

void DebugRenderSystem::receive(const ShowDebugLines& event) {
    showLines_ = event.show;
}

void DebugRenderSystem::receive(const ShowBoundingBoxes& event) {
    showBoundingBoxes_ = event.show;
}

void DebugRenderSystem::receive(const ShowDebugBoxes& event) {
    showDebugBoxes_ = event.show;
}

void DebugRenderSystem::receive(const RenderDebugLine& line) {
    addDebugLine(line);
}

void DebugRenderSystem::receive(const RenderDebugBox& box) {
    addDebugBox(box);
}

void DebugRenderSystem::addDebugLine(const RenderDebugLine& line) {
    if (line.lifeTime == 0.f) {
        staticDebugLines_.push_back(math::Linef{ line.start, line.end });
    }
    else {
        transientDebugLines_.push_back(math::Linef{ line.start, line.end });
    }
}

void DebugRenderSystem::addDebugBox(const RenderDebugBox& box) {
    if (box.lifeTime == 0.f) {
        staticDebugBoxes_.push_back(box);
    }
    else {
        transientDebugBoxes_.push_back(box);
        boxLifeTimes_.push_back(box.lifeTime);
    }
}

}
}
