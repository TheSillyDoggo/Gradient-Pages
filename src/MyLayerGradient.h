#include <Geode/Geode.hpp>

using namespace geode::prelude;

//sorry this was made by chat gpt because no way i'm reimplementing cclayergradient from scratch (holy shit scratch gd reference)

class MyLayerGradient : public cocos2d::CCLayerColor
{
public:
    virtual bool init();
    CREATE_FUNC(MyLayerGradient);

    void setStartColor(const cocos2d::ccColor3B& startColor);
    void setEndColor(const cocos2d::ccColor3B& endColor);
    void setStartOpacity(GLubyte startOpacity);
    void setEndOpacity(GLubyte endOpacity);
    void setVector(const cocos2d::CCPoint& vector);

    virtual void draw();
    
private:
    cocos2d::ccColor3B _startColor;
    cocos2d::ccColor3B _endColor;
    GLubyte _startOpacity;
    GLubyte _endOpacity;
    cocos2d::CCPoint _vector;
};