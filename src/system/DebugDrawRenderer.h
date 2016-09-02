#pragma once

#include "DebugDraw.hpp"
#include "math/Matrix.h"
#include "GL/glew.h"

namespace pg {

struct Context;

class DebugDrawRenderer :  public dd::RenderInterface {
public:
    // This aren't necessarily needed
    // void beginDraw()    override;
    // void endDraw()      override;
    DebugDrawRenderer(Context& context);
    virtual ~DebugDrawRenderer();

    void drawPointList(const dd::DrawVertex* points, int count, bool depthEnabled)                      override;
    void drawLineList(const dd::DrawVertex* lines, int count, bool depthEnabled)                        override;
    void drawGlyphList(const dd::DrawVertex* glyphs, int count, dd::GlyphTextureHandle glyphTexture)    override;

    dd::GlyphTextureHandle createGlyphTexture(int width, int height, const void* pixels)                override;
    void destroyGlyphTexture(dd::GlyphTextureHandle glyphTexture)                                       override;

    inline void setProjectionMatrix(const math::Matrix4f& mat) { mvpMatrix = mat; }

private:
    void setupShaderPrograms();
    void setupVertexBuffers();
    
    Context& context;

    // The "model-view-projection" matrix for the scene.
    // In this demo, it consists of the camera's view and projection matrices only.
    math::Matrix4f mvpMatrix;

    GLuint linePointProgram;
    GLint  linePointProgram_MvpMatrixLocation;

    GLuint textProgram;
    GLint  textProgram_GlyphTextureLocation;
    GLint  textProgram_ScreenDimensions;

    GLuint linePointVAO;
    GLuint linePointVBO;

    GLuint textVAO;
    GLuint textVBO;

};

}
