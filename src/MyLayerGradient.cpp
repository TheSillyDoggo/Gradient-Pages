/*#include "MyLayerGradient.h"

bool MyLayerGradient::init()
{
    if (!CCLayerColor::init())
        return false;

        auto macNode = CCNode::create();
        macNode->setContentSize(size);
        macNode->setZOrder(-69);
        macNode->setAnchorPoint(ccp(0.5f, 0.5f));
        macNode->setPosition(CCDirector::get()->getWinSize() / 2);
        macNode->setID("gradient"_spr);

        int numSteps = Mod::get()->getSettingValue<int64_t>("gradient-quality");

        ccColor3B startColor = ccc3(255, 0, 0);
        ccColor3B endColor = ccc3(0, 0, 255);

        if (Mod::get()->getSettingValue<bool>("use-custom-colours"))
        {
            startColor = Mod::get()->getSettingValue<ccColor3B>("primary-colour");
            endColor = Mod::get()->getSettingValue<ccColor3B>("secondary-colour");
        }
        else
        {
            startColor = GameManager::get()->colorForIdx(GameManager::get()->m_playerColor.value());
            endColor = GameManager::get()->colorForIdx(GameManager::get()->m_playerColor2.value());
        }

        for (int i = 0; i < numSteps; i++) 
        {
            ccColor3B color = lerpColor(startColor, endColor, (i * 1.0f / numSteps * 1.0f));

            CCSprite *sprite = CCSprite::create("pixel.png");
            sprite->setColor(color);

            sprite->setPosition(ccp(size.x / 2, size.y - ((((size.y / sprite->getContentSize().width) / numSteps)) * (i + 1)) / 2));
            sprite->setAnchorPoint(ccp(0.5f, 0));
            sprite->setScaleX(size.x / sprite->getContentSize().width);
            sprite->setScaleY((size.y / sprite->getContentSize().width) / numSteps);

            macNode->addChild(sprite);
        }

        if (Mod::get()->getSettingValue<bool>("reverse-order"))
            macNode->setScaleY(-1);

        auto darken = CCScale9Sprite::createWithSpriteFrameName("square-fill.png"_spr);
        darken->setID("darken"_spr);
        darken->setContentSize(size - ccp(15, 15));
        darken->setZOrder(0);
        darken->setPosition(size / 2);

        auto outline = CCScale9Sprite::createWithSpriteFrameName("square-outline.png"_spr);
        outline->setPosition(size / 2);
        outline->setContentSize(size);
        outline->setZOrder(1);
        outline->setID("outline"_spr);
        
        macNode->addChild(darken);
        macNode->addChild(outline);

    return true;
}

void MyLayerGradient::setStartColor(const ccColor3B& startColor)
{
    
}

void MyLayerGradient::setEndColor(const ccColor3B& endColor)
{
    
}*/