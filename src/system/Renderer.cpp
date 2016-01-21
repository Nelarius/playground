#include "system/Renderer.h"
#include "system/Material.h"
#include "opengl/VertexArrayObjectFactory.h"
#include "component/Include.h"
#include "utils/Log.h"
#include "utils/Assert.h"

#include <cmath>
#include <utility>

namespace {

const float DegreesToRads = 3.141592653f / 180.0f;
const float RadsToDegrees = 180.0f / 3.141592653f;
const float Pi = 3.141592653f;

// TODO: this needs to be exposed in a public API
const std::size_t MaxDebugLines = 1000u;

}

namespace pg {
namespace system {

Renderer::Renderer( Context& context )
:   System< Renderer >(),
    cameraEntity_{},
    lightEntity_{},
    defaultProjection_{},
    debugLines_{},
    lineLifeTimes_{},
    lineBuffer_{GL_ARRAY_BUFFER},
    lineBufferArray_{0},
    context_{ context },
    debug_{ false } {
    defaultProjection_ = math::Matrix4f::Perspective( 70.0f, 1.5f, 0.1f, 100.0f );

    // set up the debug buffers and VAOs
    lineBuffer_.dataStore(MaxDebugLines, 6 * sizeof(float), NULL, GL_STREAM_DRAW);
    opengl::VertexArrayObjectFactory fact{&lineBuffer_, context_.shaderManager.get("basic")};
    fact.addStandardAttribute(opengl::VertexAttribute::Vertex);
    lineBufferArray_ = fact.getVao();
}

void Renderer::configure( ecs::EventManager& events ) {
    events.subscribe< ecs::ComponentAssignedEvent<component::Camera> >( *this );
    events.subscribe< ecs::ComponentAssignedEvent<component::PointLight> >( *this );
    events.subscribe< ToggleDebugRenderer >( *this );
    events.subscribe<RenderDebugLine>(*this);
}

void Renderer::receive( const ecs::ComponentAssignedEvent< component::Camera >& event ) {
    cameraEntity_ = event.entity;
}

void Renderer::receive( const ecs::ComponentAssignedEvent< component::PointLight >& event ) {
    lightEntity_ = event.entity;
}

void Renderer::receive( const ToggleDebugRenderer& debug ) {
    debug_ = debug.show;
}

void Renderer::receive(const RenderDebugLine& line) {
    if (debugLines_.size() < MaxDebugLines) {
        debugLines_.push_back(math::Line{ line.start, line.end });
        lineLifeTimes_.push_back(line.lifeTime);
    }
    else {
        LOG_INFO << "There are already " << debugLines_.size() << " debug lines. No more can be added!";
    }
}

void Renderer::update(
    ecs::EntityManager& entities,
    ecs::EventManager& events,
    float dt 
) {
    ///////////////////////////////////////////////////////////////////////////////////////////
    /// DEBUG RENDERER
    ///////////////////////////////////////////////////////////////////////////////////////////
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

    ///////////////////////////////////////////////////////////////////////////////////////////
    /// REGULAR RENDERER STARTS HERE AGAIN
    ///////////////////////////////////////////////////////////////////////////////////////////
    math::Matrix4f cameraMatrix{ defaultProjection_ };
    math::Vector3f cameraPos{};
    math::Vector3f lightPos{};
    math::Vector3f lightIntensity{};
    float attenuation = 1.0f;
    float ambientCoefficient = 0.5f;

    if ( cameraEntity_.isValid() ) {
        float aspectRatio = float( context_.window->width() ) / context_.window->height();
        auto transform = cameraEntity_.component< component::Transform >();
        auto view = math::Matrix4f::Translation( transform->position )
                    * math::Matrix4f::Rotation( transform->rotation )
                    * math::Matrix4f::Scale( transform->scale );
        auto camera = cameraEntity_.component< component::Camera >();
        auto proj = math::Matrix4f::Perspective( 
            camera->verticalFov, 
            aspectRatio,
            camera->nearPlane,
            camera->farPlane
        );
        cameraMatrix = proj * view.inverse();
    }

    if ( lightEntity_.isValid() ) {
        lightPos = lightEntity_.component< component::Transform >()->position;
        lightIntensity = lightEntity_.component< component::PointLight >()->intensity;
        attenuation = lightEntity_.component< component::PointLight >()->attenuation;
        ambientCoefficient = lightEntity_.component< component::PointLight >()->ambientCoefficient;
    }

     /*
      * Then, iterate over renderables
      */
    for ( ecs::Entity entity: entities.join< component::Transform, component::Renderable>() ) {
        auto renderable = entity.component< component::Renderable>();
        auto transform = entity.component< component::Transform>();
        auto shader = renderable->shader;
        shader->use();
        shader->setUniform( 
            "model",
            math::Matrix4f::Translation( transform->position )
            * math::Matrix4f::Rotation( transform->rotation )
            * math::Matrix4f::Scale( transform->scale )
        );
        shader->setUniform( "camera", cameraMatrix );
        for ( const auto& it: renderable->material.uniforms ) {
            shader->setUniform( it.first.c_str(), it.second );
        }
        shader->setUniform( "pointLight.position", lightPos );
        shader->setUniform( "pointLight.intensity", lightIntensity );
        shader->setUniform( "pointLight.attenuation", attenuation );
        shader->setUniform( "pointLight.ambientCoefficient", ambientCoefficient );
        renderable->vao.bind();
        glDrawArrays( GL_TRIANGLES, 0, renderable->vbo->count() / renderable->vao.elementsPerIndex() );
        renderable->vao.unbind();
        shader->stopUsing();
    }

    if ( debug_ ) {
        debugRender_( entities, events, cameraMatrix );
    }
}

void Renderer::setSpecularUniforms_( const math::Vector3f& pos, opengl::Program* p ) {
    p->setUniform( "cameraPosition", pos );
}

///////////////////////////////////////////////////////////////////////////////////////////
/// DEBUG RENDERER
///////////////////////////////////////////////////////////////////////////////////////////
void Renderer::debugRender_( ecs::EntityManager& entities, ecs::EventManager& events, const math::Matrix4f& cameraMatrix ) {
    // create rendering state
    auto* shader = context_.shaderManager.get( "basic" );
    auto* vbo = context_.meshManager.get( "data/cube.obj" );

    opengl::VertexArrayObjectFactory factory{ vbo, shader };
    factory.addAttribute( "vertex", 3, GL_FLOAT, GL_FALSE, 24 );    // skip the normals
    auto vao = factory.getVao();

    shader->use();
    shader->setUniform( "color", math::Vector3f{ 1.0f, 0.2f, 0.2f } );
    shader->setUniform( "camera", cameraMatrix );
    /// BOUNDING BOXES
    ///////////////////////////////////////////////////////////
    for ( ecs::Entity entity: entities.join< component::Transform, math::AABox >() ) {
        auto t = entity.component< component::Transform >();
        auto bb = entity.component<math::AABox>();
        math::Vector3f min = t->scale.hadamard( bb->min );
        math::Vector3f max = t->scale.hadamard( bb->max );
        math::Vector3f center = 0.5f * (min + max);
        math::Vector3f scale{ max.x - min.x, max.y - min.y, max.z - min.z };
        math::Matrix4f S = math::Matrix4f::Scale( scale );              // scale to current model dimensions
        math::Matrix4f R = math::Matrix4f::Rotation( t->rotation );     // rotate to world coords
        math::Matrix4f Tl = math::Matrix4f::Translation(center);        // translate to local coords
        math::Matrix4f Tw = math::Matrix4f::Translation(t->position);   // translate to world coords
        math::Matrix4f TRS = Tw * R  * Tl * S;

        shader->use();
        shader->setUniform( "model", TRS );
        vao.bind();
        glDrawArrays( GL_LINES, 0, vbo->count() / vao.elementsPerIndex() );
        vao.unbind();
    }

    /// LINES
    ///////////////////////////////////////////////////////////
    const std::size_t LineCount = debugLines_.size();
    math::Vector3f* lines = (math::Vector3f*)lineBuffer_.mapBufferRange(0, LineCount * 6u * sizeof(float), GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT);
    for (unsigned i = 0u; i < 2u * LineCount; i += 2u) {
        unsigned index = i / 2u;
        lines[i]      = debugLines_[index].origin;
        lines[i + 1u] = debugLines_[index].end;
    }
    lineBuffer_.unmapBuffer();

    shader->setUniform("color", math::Vector3f{0.9f, 0.6f, 0.15f});
    shader->setUniform("model", math::Matrix4f{});
    lineBufferArray_.bind();
    glDrawArrays(GL_LINES, 0, debugLines_.size());
    lineBufferArray_.unbind();

    shader->stopUsing();
}

}
}
