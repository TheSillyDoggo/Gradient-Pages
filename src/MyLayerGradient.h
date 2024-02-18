#include <Geode/Geode.hpp>

using namespace geode::prelude;

class MyLayerGradient : public cocos2d::CCLayerColor
{
public:
    virtual bool init();
    CREATE_FUNC(MyLayerGradient);

    void setStartColor(const cocos2d::ccColor3B& startColor);
    void setEndColor(const cocos2d::ccColor3B& endColor);
};