#include <Geode/Geode.hpp>
#include <Geode/modify/ProfilePage.hpp>
#include <Geode/modify/FriendsProfilePage.hpp>
#include <Geode/modify/InfoLayer.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/modify/GJCommentListLayer.hpp>

using namespace geode::prelude;

class $modify(ProfilePage) {

	bool init(int accountID, bool idk)
	{
		bool a = ProfilePage::init(accountID, idk);

		if (!Mod::get()->getSettingValue<bool>("apply-profiles"))
			return a;

		auto l = reinterpret_cast<CCLayer*>(this->getChildren()->objectAtIndex(0));

		auto gradient = CCLayerGradient::create();
		
		gradient->setStartColor({255, 0, 0});
		gradient->setEndColor({0, 255, 0});
		gradient->setZOrder(-1);
		gradient->setID("gradient"_spr);
		gradient->setOpacity(0);

		gradient->setPosition(CCDirector::get()->getWinSize() / 2);
		gradient->setContentSize(ccp(440, 290));
		gradient->ignoreAnchorPointForPosition(false);

		auto bg = CCScale9Sprite::createWithSpriteFrameName("square-outline.png"_spr);
		bg->setPosition(CCDirector::get()->getWinSize() / 2);
		bg->setContentSize(ccp(440, 290));
		bg->setZOrder(1);
		bg->setID("bg"_spr);

		l->addChild(bg);
		l->addChild(gradient);

		if (m_score == nullptr)
		{
			log::info("hasn't loaded profile info yet :(");
		}
		else
		{
			gradient->setStartColor(GameManager::get()->colorForIdx(m_score->getPlayerColor1()));
			gradient->setEndColor(GameManager::get()->colorForIdx(m_score->getPlayerColor2()));

			gradient->setOpacity(255);
			bg->setOpacity(255);
		}

		return a;
	}

	virtual void loadPageFromUserInfo(GJUserScore* score)
	{
		ProfilePage::loadPageFromUserInfo(score);

		if (!Mod::get()->getSettingValue<bool>("apply-profiles"))
			return;

		log::info("loadPageFromUserInfo");

		log::info("colour 1: {}", score->getPlayerColor1());
		log::info("colour 2: {}", score->getPlayerColor2());
		
		auto l = reinterpret_cast<CCLayer*>(this->getChildren()->objectAtIndex(0));

		if (l != nullptr)
		{
			auto g = reinterpret_cast<CCLayerGradient*>(l->getChildByID("gradient"_spr));

			if (g != nullptr)
			{
				g->setStartColor(GameManager::get()->colorForIdx(score->getPlayerColor1()));
				g->setEndColor(GameManager::get()->colorForIdx(score->getPlayerColor2()));
				
				if (g->getOpacity() == 0)
				{
					g->runAction(CCFadeIn::create(0.25f));
				}
			}
		}
	}

};

class $modify(FriendsProfilePage) {

	bool init(UserListType p0)
	{
		bool a = FriendsProfilePage::init(p0);

		if (!Mod::get()->getSettingValue<bool>("apply-profiles"))
			return a;

		auto l = reinterpret_cast<CCLayer*>(this->getChildren()->objectAtIndex(0));

		auto gradient = CCLayerGradient::create();
		
		gradient->setStartColor({255, 0, 0});
		gradient->setEndColor({0, 255, 0});
		gradient->setZOrder(-1);
		gradient->setID("gradient"_spr);

		gradient->setPosition(CCDirector::get()->getWinSize() / 2);
		gradient->setContentSize(ccp(440, 290));
		gradient->ignoreAnchorPointForPosition(false);

		auto bg = CCScale9Sprite::createWithSpriteFrameName("square-outline.png"_spr);
		bg->setPosition(CCDirector::get()->getWinSize() / 2);
		bg->setContentSize(ccp(440, 290));
		bg->setZOrder(1);
		bg->setID("bg"_spr);

		l->addChild(bg);
		l->addChild(gradient);

		gradient->setStartColor(GameManager::get()->colorForIdx(GameManager::get()->getPlayerColor()));
		gradient->setEndColor(GameManager::get()->colorForIdx(GameManager::get()->getPlayerColor2()));

		gradient->setOpacity(255);
		bg->setOpacity(255);

		return a;
	}

};

class $modify(InfoLayer) {

	bool init(GJGameLevel* level, GJUserScore* score)
	{
		bool a = InfoLayer::init(level, score);

		if (!Mod::get()->getSettingValue<bool>("apply-info-layer"))
			return a;

		auto l = reinterpret_cast<CCLayer*>(this->getChildren()->objectAtIndex(0));

		auto gradient = CCLayerGradient::create();
		
		gradient->setStartColor({255, 0, 0});
		gradient->setEndColor({0, 255, 0});
		gradient->setZOrder(-2);
		gradient->setID("gradient"_spr);

		gradient->setPosition(CCDirector::get()->getWinSize() / 2);
		gradient->setContentSize(ccp(440, 290));
		gradient->ignoreAnchorPointForPosition(false);

		auto bg = CCScale9Sprite::createWithSpriteFrameName("square-outline.png"_spr);
		bg->setPosition(CCDirector::get()->getWinSize() / 2);
		bg->setContentSize(ccp(440, 290));
		bg->setZOrder(1);
		bg->setID("bg"_spr);

		l->addChild(bg);
		l->addChild(gradient);

		gradient->setStartColor(GameManager::get()->colorForIdx(GameManager::get()->getPlayerColor()));
		gradient->setEndColor(GameManager::get()->colorForIdx(GameManager::get()->getPlayerColor2()));

		gradient->setOpacity(255);
		bg->setOpacity(255);

		reinterpret_cast<CCNode*>(l->getChildren()->objectAtIndex(0))->setVisible(false);

		if (this->m_list->getContentSize().height <= 140)
		{
			reinterpret_cast<CCNodeRGBA*>(l->getChildren()->objectAtIndex(3))->setColor({0, 0, 0});
			reinterpret_cast<CCNodeRGBA*>(l->getChildren()->objectAtIndex(3))->setOpacity(100);
		}		

		return a;
	}

};

class $modify (geode::Popup<std::string>)
{
	
};

class $modify (GJCommentListLayer)
{
	static GJCommentListLayer* create(BoomListView* p0, char const* p1, cocos2d::_ccColor4B p2, float p3, float p4, bool p5)
	{
		auto a = GJCommentListLayer::create(p0, p1, p2, p3, p4, p5);

		if (!(Mod::get()->getSettingValue<bool>("apply-profiles") || Mod::get()->getSettingValue<bool>("apply-info-layer")))
			return a;

		for (size_t i = 0; i < 4; i++)
		{
			reinterpret_cast<CCNode*>(a->getChildren()->objectAtIndex(i))->setVisible(false);
		}

		auto spr = CCScale9Sprite::createWithSpriteFrameName("comment-outline.png"_spr);
		spr->setContentSize(a->getContentSize());
		spr->setAnchorPoint(ccp(0, 0));
		a->addChild(spr, 69);

		return a;
	}
};