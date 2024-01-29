#include "MyLayerGradient.h"

bool MyLayerGradient::init()
{
    if (!CCLayer::init())
        return false;

    // Set default values
    _startColor = ccc3(255, 255, 255);
    _endColor = ccc3(255, 255, 255);
    _startOpacity = 255;
    _endOpacity = 255;
    _vector = ccp(0, -1);

    setShaderProgram(CCShaderCache::sharedShaderCache()->programForKey(kCCShader_PositionColor));
    ccBlendFunc blendFunc = {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA};
    setBlendFunc(blendFunc);

    return true;
}

void MyLayerGradient::setStartColor(const ccColor3B& startColor)
{
    _startColor = startColor;
}

void MyLayerGradient::setEndColor(const ccColor3B& endColor)
{
    _endColor = endColor;
}

void MyLayerGradient::setStartOpacity(GLubyte startOpacity)
{
    _startOpacity = startOpacity;
}

void MyLayerGradient::setEndOpacity(GLubyte endOpacity)
{
    _endOpacity = endOpacity;
}

void MyLayerGradient::setVector(const CCPoint& vector)
{
    //_vector = vector.getNormalized();
    float length = sqrt(vector.x * vector.x + vector.y * vector.y);
    _vector = length > 0 ? ccp(vector.x / length, vector.y / length) : vector;
}

void MyLayerGradient::draw()
{
    CC_NODE_DRAW_SETUP();

    ccColor4F start = ccc4FFromccc3B(_startColor);
    ccColor4F end = ccc4FFromccc3B(_endColor);

    GLfloat vertices[] = {
        //tl
        0, getContentSize().height,
        //bl
        0, 0,
        //tr
        getContentSize().width, getContentSize().height,
        //br
        getContentSize().width, 0,
    };

    GLfloat colors[] = {
        start.r, start.g, start.b, start.a * _startOpacity / 255.0f * (this->getOpacity() / 255.0f),
        end.r, end.g, end.b, end.a * _endOpacity / 255.0f * (this->getOpacity() / 255.0f),
        start.r, start.g, start.b, start.a * _startOpacity / 255.0f * (this->getOpacity() / 255.0f),
        end.r, end.g, end.b, end.a * _endOpacity / 255.0f * (this->getOpacity() / 255.0f),
    };

    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, GL_FALSE, 0, colors);

    ccGLBlendFunc(this->getBlendFunc().src, this->getBlendFunc().dst);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    /*CC_NODE_DRAW_SETUP();

    ccColor3B start = _startColor;
    ccColor3B end = _endColor;

    CCPoint vertices[] = {
        ccp(0, 0),
        ccp(getContentSize().width, 0),
        ccp(0, getContentSize().height),
        ccp(getContentSize().width, getContentSize().height),
    };

    for (int i = 0; i < 4; ++i)
    {
        CCPoint dir = ccpMult(_vector, ccpDot(vertices[i], _vector));
        vertices[i] = dir;
    }

    GLfloat colors[] = {
        start.r / 255.0f, start.g / 255.0f, start.b / 255.0f, _startOpacity / 255.0f * (this->getOpacity() / 255.0f),
        end.r / 255.0f, end.g / 255.0f, end.b / 255.0f, _endOpacity / 255.0f * (this->getOpacity() / 255.0f),
        start.r / 255.0f, start.g / 255.0f, start.b / 255.0f, _startOpacity / 255.0f * (this->getOpacity() / 255.0f),
        end.r / 255.0f, end.g / 255.0f, end.b / 255.0f, _endOpacity / 255.0f * (this->getOpacity() / 255.0f),
    };

    glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_FLOAT, GL_FALSE, 0, colors);

    //ccGLBlendFunc(this->getBlendFunc().src, this->getBlendFunc().dst);

    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);*/
}
