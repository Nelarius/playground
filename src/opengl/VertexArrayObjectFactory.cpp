#include "opengl/VertexArrayObjectFactory.h"
#include "opengl/Enum.h"
#include "opengl/BufferObject.h"
#include "opengl/Program.h"
#include "utils/Log.h"
#include "utils/Assert.h"

namespace pg {
namespace opengl {


VertexArrayObjectFactory::VertexArrayObjectFactory(BufferObject* buffer, Program* program)
    : attributeQueue_(),
    standardAttributeQueue_(),
    buffer_(buffer),
    program_(program) {}

void VertexArrayObjectFactory::addAttribute(
    const std::string& attrib,
    GLint size,
    GLenum type,
    bool normalized,
    GLsizei stride,
    std::size_t offset) {
    if (!standardAttributeQueue_.empty()) {
        LOG_ERROR << "Adding custom and standard attributes to the same vertex array object is prohibited!";
        return;
    }
    attributeQueue_.emplace_back(
        attrib,
        size,
        type,
        normalized,
        stride,
        offset
        );
}

void VertexArrayObjectFactory::addStandardAttribute(VertexAttribute attribute) {
    if (!attributeQueue_.empty()) {
        LOG_ERROR << "Adding standard and custom attributes to the same vertex array object is prohibited!";
        return;
    }
    GLint program;
    glGetIntegerv(GL_CURRENT_PROGRAM, &program);

    switch (attribute) {
    case VertexAttribute::Vertex: {
        standardAttributeQueue_.emplace_back(
            "vertex", 3, GL_FLOAT, false, 0, 0u
            );
        break;
    }
    case VertexAttribute::Normal: {
        standardAttributeQueue_.emplace_back(
            "normal", 3, GL_FLOAT, false, 0, 0u
            );
        break;
    }
    case VertexAttribute::Color: {
        standardAttributeQueue_.emplace_back(
            "color", 3, GL_FLOAT, false, 0, 0u
            );
        break;
    }
    case VertexAttribute::UVCoordinate: {
        standardAttributeQueue_.emplace_back(
            "uvcoordinate", 2, GL_FLOAT, false, 0, 0u
            );
        break;
    }

    default: LOG_ERROR << "Trying to add incorrect attribute to vertex array object.";
    }
}

VertexArrayObject VertexArrayObjectFactory::getVao() {
    buffer_->bind();
    program_->use();

    // for custom attributes, the stride may be larger than the actual number
    // of elements used. Hence, we need to get the number of elements per
    // index needs to be deduced from the stride.
    int elements = standardAttributeQueue_.empty() ?
        attributeQueue_[0].stride / SizeOfGlType(attributeQueue_[0].type) :
        accumulateAttributes_(standardAttributeQueue_);
    VertexArrayObject vao{ elements };
    if (!standardAttributeQueue_.empty()) {
        vao.bind();
        enableStandardAttributes_();
        standardAttributeQueue_.clear();
        vao.unbind();
    }
    else if (!attributeQueue_.empty()) {
        vao.bind();
        enableAttributes_();
        attributeQueue_.clear();
        vao.unbind();
    }
    buffer_->unbind();
    program_->stopUsing();
    return vao;
}

/*
 * This class creates a vertex attribute pointer for each attribute,
 * and then enables the array.
 *
 * Does not clear the queue.
 * */
void VertexArrayObjectFactory::enableStandardAttributes_() {
    int strideAccumulator{};
    /*
     * Get the total stride
     * */
    for (auto& attrib : standardAttributeQueue_) {
        strideAccumulator += attrib.size;
    }
    /*
     * enable the attributes
     * */
    std::size_t offsetAccumulator{ 0u };
    for (auto& attrib : standardAttributeQueue_) {
        GLint index = glGetAttribLocation(program_->object(), attrib.name.c_str());
        PG_ASSERT(index != -1);
        glVertexAttribPointer(
            index,
            attrib.size,
            attrib.type,
            attrib.normalized,
            strideAccumulator * sizeof(float),
            (const GLvoid*)(offsetAccumulator * sizeof(float))
            );
        offsetAccumulator += attrib.size;
        glEnableVertexAttribArray(index);

    }
}

void VertexArrayObjectFactory::enableAttributes_() {
    for (auto& attrib : attributeQueue_) {
        GLint index = glGetAttribLocation(program_->object(), attrib.name.c_str());
        PG_ASSERT(index != -1);
        glVertexAttribPointer(
            index,
            attrib.size,
            attrib.type,
            attrib.normalized,
            attrib.stride,
            (const GLvoid*)attrib.offset
            );
        glEnableVertexAttribArray(index);
    }
}

int VertexArrayObjectFactory::accumulateAttributes_(const std::vector<VertexArrayObjectFactory::Attribute>& attributes) const {
    int accumulator{ 0 };
    for (auto& attrib : attributes) {
        accumulator += attrib.size;
    }
    return accumulator;
}

}   // opengl
}   // pg
