#include <Geode/Geode.hpp>
#include <Geode/modify/ProfilePage.hpp>
#include <Geode/modify/FriendsProfilePage.hpp>
#include <Geode/modify/FRequestProfilePage.hpp>
#include <Geode/modify/MessagesProfilePage.hpp>
#include <Geode/modify/LevelInfoLayer.hpp>
#include <Geode/modify/InfoLayer.hpp>
#include <Geode/modify/ItemInfoPopup.hpp>
#include <Geode/modify/ShardsPage.hpp>
#include <Geode/modify/CommunityCreditsPage.hpp>
#include <Geode/modify/MoreSearchLayer.hpp>
#include <Geode/modify/GJCommentListLayer.hpp>
#include <Geode/modify/GJAccountSettingsLayer.hpp>
#include <Geode/modify/DemonFilterSelectLayer.hpp>
#include <Geode/modify/SetIDPopup.hpp>
#include <Geode/modify/ChallengesPage.hpp>

using namespace geode::prelude;

#include "MyLayerGradient.h"

class GradientPages
{
	public:
		static inline GJUserScore* score = nullptr;
};

class $modify(ProfilePage) {

	bool init(int accountID, bool idk)
	{
		bool a = ProfilePage::init(accountID, idk);

		if (!Mod::get()->getSettingValue<bool>("apply-profiles") || Loader::get()->getLoadedMod("bitz.customprofiles"))
			return a;

		if (GradientPages::score)
		{
			if (GradientPages::score->m_accountID != accountID)
			{
				GradientPages::score = nullptr;
			}
		}

		auto l = reinterpret_cast<CCLayer*>(this->getChildren()->objectAtIndex(0));

		//auto gradient = MyLayerGradient::create();
		auto gradient = MyLayerGradient::create();
		
		gradient->setStartColor({255, 0, 0});
		gradient->setEndColor({0, 255, 0});
		gradient->setZOrder(-1);
		gradient->setID("gradient"_spr);
		gradient->setOpacity(0);

		gradient->setPosition(CCDirector::get()->getWinSize() / 2);
		gradient->setContentSize(ccp(440, 290));
		gradient->ignoreAnchorPointForPosition(false);

		if (Mod::get()->getSettingValue<bool>("reverse-order"))
			gradient->setScaleY(-1);

		auto darken = CCScale9Sprite::createWithSpriteFrameName("square-fill.png"_spr);
		darken->setID("darken"_spr);
		darken->setContentSize(gradient->getContentSize() - ccp(15, 15));
		darken->setZOrder(0);
		darken->setPosition(gradient->getPosition());
		darken->setAnchorPoint(gradient->getAnchorPoint());
		darken->setOpacity(0);

		auto bg = CCScale9Sprite::createWithSpriteFrameName("square-outline.png"_spr);
		bg->setPosition(CCDirector::get()->getWinSize() / 2);
		bg->setContentSize(ccp(440, 290));
		bg->setZOrder(1);
		bg->setID("bg"_spr);

		l->addChild(bg);
		l->addChild(darken);
		l->addChild(gradient);
		
		if (GradientPages::score == nullptr)
		{
			log::info("hasn't loaded profile info yet :(");
		}
		else
		{
			gradient->setStartColor(GameManager::get()->colorForIdx(GradientPages::score->m_color1));
			gradient->setEndColor(GameManager::get()->colorForIdx(GradientPages::score->m_color2));

			gradient->setOpacity(255);
			darken->setOpacity(255);
			bg->setOpacity(255);
		}

		return a;
	}

	virtual void loadPageFromUserInfo(GJUserScore* score)
	{
		GradientPages::score = score;

		ProfilePage::loadPageFromUserInfo(score);

		if (!Mod::get()->getSettingValue<bool>("apply-profiles") || Loader::get()->getLoadedMod("bitz.customprofiles"))
			return;

		log::info("loadPageFromUserInfo");

		log::info("colour 1: {}", score->m_color1);
		log::info("colour 2: {}", score->m_color2);
		
		auto l = reinterpret_cast<CCLayer*>(this->getChildren()->objectAtIndex(0));

		if (l)
		{
			auto g = reinterpret_cast<MyLayerGradient*>(l->getChildByID("gradient"_spr));
			auto d = reinterpret_cast<CCScale9Sprite*>(l->getChildByID("darken"_spr));

			if (g)
			{
				g->setStartColor(GameManager::get()->colorForIdx(score->m_color1));
				g->setEndColor(GameManager::get()->colorForIdx(score->m_color2));
				
				if (g->getOpacity() == 0)
				{
					#ifdef GEODE_IS_MACOS
					g->setOpacity(255);
					#else
					g->runAction(CCFadeIn::create(0.25f));
					#endif
				}

				if (d)
				{
					if (d->getOpacity() == 0)
					{
						#ifdef GEODE_IS_MACOS
						d->setOpacity(255);
						#else
						d->runAction(CCFadeIn::create(0.25f));
						#endif
					}
				}
			}
		}
	}

};

class $modify (GJAccountSettingsLayer)
{
	bool init(int idk)
	{
		if (!GJAccountSettingsLayer::init(idk))
			return false;

		if (!Mod::get()->getSettingValue<bool>("apply-profiles") || Loader::get()->getLoadedMod("bitz.customprofiles"))
			return true;

		auto l = reinterpret_cast<CCLayer*>(this->getChildren()->objectAtIndex(0));
		l->sortAllChildren();
		reinterpret_cast<CCNode*>(l->getChildren()->objectAtIndex(0))->setVisible(false);

		auto gradient = MyLayerGradient::create();
		
		gradient->setStartColor({255, 0, 0});
		gradient->setEndColor({0, 255, 0});
		gradient->setZOrder(-3);
		gradient->setID("gradient"_spr);

		gradient->setPosition(CCDirector::get()->getWinSize() / 2);
		gradient->setContentSize(ccp(400, 300));
		gradient->ignoreAnchorPointForPosition(false);

		if (Mod::get()->getSettingValue<bool>("reverse-order"))
			gradient->setScaleY(-1);

		auto darken = CCScale9Sprite::createWithSpriteFrameName("square-fill.png"_spr);
		darken->setID("darken"_spr);
		darken->setContentSize(gradient->getContentSize() - ccp(15, 15));
		darken->setZOrder(-2);
		darken->setPosition(gradient->getPosition());
		darken->setAnchorPoint(gradient->getAnchorPoint());

		auto bg = CCScale9Sprite::createWithSpriteFrameName("square-outline.png"_spr);
		bg->setPosition(CCDirector::get()->getWinSize() / 2);
		bg->setContentSize(gradient->getContentSize());
		bg->setZOrder(1);
		bg->setID("bg"_spr);

		l->addChild(bg);
		l->addChild(darken);
		l->addChild(gradient);

		if (Mod::get()->getSettingValue<bool>("use-custom-colours"))
		{
			gradient->setStartColor(Mod::get()->getSettingValue<ccColor3B>("primary-colour"));
			gradient->setEndColor(Mod::get()->getSettingValue<ccColor3B>("secondary-colour"));
		}
		else
		{
			gradient->setStartColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerColor.value()));
			gradient->setEndColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerColor2.value()));
		}

		if (Mod::get()->getSettingValue<bool>("reverse-order"))
			gradient->setScaleY(-1);

		gradient->setOpacity(255);
		bg->setOpacity(255);

		return true;
	}
};

class $modify(FRequestProfilePage) {

	bool init(bool p0)
	{
		if (!FRequestProfilePage::init(p0))
			return false;

		if (!Mod::get()->getSettingValue<bool>("apply-profiles"))
			return true;

		auto l = reinterpret_cast<CCLayer*>(this->getChildren()->objectAtIndex(0));

		auto gradient = MyLayerGradient::create();
		
		gradient->setStartColor({255, 0, 0});
		gradient->setEndColor({0, 255, 0});
		gradient->setZOrder(-1);
		gradient->setID("gradient"_spr);

		gradient->setPosition(CCDirector::get()->getWinSize() / 2);
		gradient->setContentSize(ccp(440, 290));
		gradient->ignoreAnchorPointForPosition(false);

		if (Mod::get()->getSettingValue<bool>("reverse-order"))
			gradient->setScaleY(-1);

		auto darken = CCScale9Sprite::createWithSpriteFrameName("square-fill.png"_spr);
		darken->setID("darken"_spr);
		darken->setContentSize(gradient->getContentSize() - ccp(15, 15));
		darken->setZOrder(0);
		darken->setPosition(gradient->getPosition());
		darken->setAnchorPoint(gradient->getAnchorPoint());

		auto bg = CCScale9Sprite::createWithSpriteFrameName("square-outline.png"_spr);
		bg->setPosition(CCDirector::get()->getWinSize() / 2);
		bg->setContentSize(ccp(440, 290));
		bg->setZOrder(1);
		bg->setID("bg"_spr);

		l->addChild(bg);
		l->addChild(darken);
		l->addChild(gradient);

		if (Mod::get()->getSettingValue<bool>("use-custom-colours"))
		{
			gradient->setStartColor(Mod::get()->getSettingValue<ccColor3B>("primary-colour"));
			gradient->setEndColor(Mod::get()->getSettingValue<ccColor3B>("secondary-colour"));
		}
		else
		{
			gradient->setStartColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerColor.value()));
			gradient->setEndColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerColor2.value()));
		}

		if (Mod::get()->getSettingValue<bool>("reverse-order"))
			gradient->setScaleY(-1);

		gradient->setOpacity(255);
		bg->setOpacity(255);

		return true;
	}

};

class $modify(MessagesProfilePage) {

	bool init(bool p0)
	{
		if (!MessagesProfilePage::init(p0))
			return false;

		if (!Mod::get()->getSettingValue<bool>("apply-profiles"))
			return true;

		auto l = reinterpret_cast<CCLayer*>(this->getChildren()->objectAtIndex(0));

		auto gradient = MyLayerGradient::create();
		
		gradient->setStartColor({255, 0, 0});
		gradient->setEndColor({0, 255, 0});
		gradient->setZOrder(-1);
		gradient->setID("gradient"_spr);

		gradient->setPosition(CCDirector::get()->getWinSize() / 2);
		gradient->setContentSize(ccp(440, 290));
		gradient->ignoreAnchorPointForPosition(false);

		if (Mod::get()->getSettingValue<bool>("reverse-order"))
			gradient->setScaleY(-1);

		auto darken = CCScale9Sprite::createWithSpriteFrameName("square-fill.png"_spr);
		darken->setID("darken"_spr);
		darken->setContentSize(gradient->getContentSize() - ccp(15, 15));
		darken->setZOrder(0);
		darken->setPosition(gradient->getPosition());
		darken->setAnchorPoint(gradient->getAnchorPoint());

		auto bg = CCScale9Sprite::createWithSpriteFrameName("square-outline.png"_spr);
		bg->setPosition(CCDirector::get()->getWinSize() / 2);
		bg->setContentSize(ccp(440, 290));
		bg->setZOrder(1);
		bg->setID("bg"_spr);

		l->addChild(bg);
		l->addChild(darken);
		l->addChild(gradient);

		if (Mod::get()->getSettingValue<bool>("use-custom-colours"))
		{
			gradient->setStartColor(Mod::get()->getSettingValue<ccColor3B>("primary-colour"));
			gradient->setEndColor(Mod::get()->getSettingValue<ccColor3B>("secondary-colour"));
		}
		else
		{
			gradient->setStartColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerColor.value()));
			gradient->setEndColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerColor2.value()));
		}

		if (Mod::get()->getSettingValue<bool>("reverse-order"))
			gradient->setScaleY(-1);

		gradient->setOpacity(255);
		bg->setOpacity(255);

		return true;
	}

};

class $modify(FriendsProfilePage) {

	bool init(UserListType p0)
	{
		if (!FriendsProfilePage::init(p0))
			return false;

		if (!Mod::get()->getSettingValue<bool>("apply-profiles"))
			return true;

		auto l = reinterpret_cast<CCLayer*>(this->getChildren()->objectAtIndex(0));

		auto gradient = MyLayerGradient::create();
		
		gradient->setStartColor({255, 0, 0});
		gradient->setEndColor({0, 255, 0});
		gradient->setZOrder(-1);
		gradient->setID("gradient"_spr);

		gradient->setPosition(CCDirector::get()->getWinSize() / 2);
		gradient->setContentSize(ccp(440, 290));
		gradient->ignoreAnchorPointForPosition(false);

		if (Mod::get()->getSettingValue<bool>("reverse-order"))
			gradient->setScaleY(-1);

		auto darken = CCScale9Sprite::createWithSpriteFrameName("square-fill.png"_spr);
		darken->setID("darken"_spr);
		darken->setContentSize(gradient->getContentSize() - ccp(15, 15));
		darken->setZOrder(0);
		darken->setPosition(gradient->getPosition());
		darken->setAnchorPoint(gradient->getAnchorPoint());

		auto bg = CCScale9Sprite::createWithSpriteFrameName("square-outline.png"_spr);
		bg->setPosition(CCDirector::get()->getWinSize() / 2);
		bg->setContentSize(ccp(440, 290));
		bg->setZOrder(1);
		bg->setID("bg"_spr);

		l->addChild(bg);
		l->addChild(darken);
		l->addChild(gradient);

		if (Mod::get()->getSettingValue<bool>("use-custom-colours"))
		{
			gradient->setStartColor(Mod::get()->getSettingValue<ccColor3B>("primary-colour"));
			gradient->setEndColor(Mod::get()->getSettingValue<ccColor3B>("secondary-colour"));
		}
		else
		{
			gradient->setStartColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerColor.value()));
			gradient->setEndColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerColor2.value()));
		}

		if (Mod::get()->getSettingValue<bool>("reverse-order"))
			gradient->setScaleY(-1);

		gradient->setOpacity(255);
		bg->setOpacity(255);

		return true;
	}

};

class $modify(InfoLayer) {

	bool init(GJGameLevel* level, GJUserScore* score, GJLevelList* p2)
	{
		if (!InfoLayer::init(level, score, p2))
			return false;

		if (!Mod::get()->getSettingValue<bool>("apply-info-layer") || Loader::get()->getLoadedMod("bitz.customprofiles"))
			return true;

		auto l = reinterpret_cast<CCLayer*>(this->getChildren()->objectAtIndex(0));

		auto gradient = MyLayerGradient::create();
		
		gradient->setStartColor({255, 0, 0});
		gradient->setEndColor({0, 255, 0});
		gradient->setZOrder(-2);
		gradient->setID("gradient"_spr);

		gradient->setPosition(CCDirector::get()->getWinSize() / 2);
		gradient->setContentSize(ccp(440, 290));
		gradient->ignoreAnchorPointForPosition(false);

		auto darken = CCScale9Sprite::createWithSpriteFrameName("square-fill.png"_spr);
		darken->setID("darken"_spr);
		darken->setContentSize(gradient->getContentSize() - ccp(15, 15));
		darken->setZOrder(0);
		darken->setPosition(gradient->getPosition());
		darken->setAnchorPoint(gradient->getAnchorPoint());

		auto bg = CCScale9Sprite::createWithSpriteFrameName("square-outline.png"_spr);
		bg->setPosition(CCDirector::get()->getWinSize() / 2);
		bg->setContentSize(ccp(440, 290));
		bg->setZOrder(1);
		bg->setID("bg"_spr);

		l->addChild(bg);
		l->addChild(darken);
		l->addChild(gradient);

		if (Mod::get()->getSettingValue<bool>("use-custom-colours"))
		{
			gradient->setStartColor(Mod::get()->getSettingValue<ccColor3B>("primary-colour"));
			gradient->setEndColor(Mod::get()->getSettingValue<ccColor3B>("secondary-colour"));
		}
		else
		{
			gradient->setStartColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerColor.value()));
			gradient->setEndColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerColor2.value()));
		}

		if (Mod::get()->getSettingValue<bool>("reverse-order"))
			gradient->setScaleY(-1);

		gradient->setOpacity(255);
		bg->setOpacity(255);

		reinterpret_cast<CCNode*>(l->getChildren()->objectAtIndex(0))->setVisible(false);

		if (this->m_list->getContentSize().height <= 140)
		{
			reinterpret_cast<CCNodeRGBA*>(l->getChildren()->objectAtIndex(3))->setColor({0, 0, 0});
			reinterpret_cast<CCNodeRGBA*>(l->getChildren()->objectAtIndex(3))->setOpacity(100);
		}		

		return true;
	}

};

#ifndef GEODE_IS_MACOS

class $modify (GJCommentListLayer)
{
	static GJCommentListLayer* create(BoomListView* p0, char const* p1, cocos2d::_ccColor4B p2, float p3, float p4, bool p5)
	{
		auto a = GJCommentListLayer::create(p0, p1, p2, p3, p4, p5);

		if (Loader::get()->getLoadedMod("bitz.customprofiles"))
			return a;

		if (!(Mod::get()->getSettingValue<bool>("apply-profiles") || Mod::get()->getSettingValue<bool>("apply-info-layer")))
			return a;

		for (size_t i = 0; i < 4; i++)
		{
			reinterpret_cast<CCNode*>(a->getChildren()->objectAtIndex(i))->setVisible(false);
		}

		auto spr = CCScale9Sprite::createWithSpriteFrameName("comment-outline.png"_spr);
		spr->setContentSize(a->getContentSize() + ccp(1, 1));
		spr->setAnchorPoint(ccp(0, 0));
		spr->setPosition(0, -1);
		a->addChild(spr, 69);

		return a;
	}
};

class $modify (ItemInfoPopup)
{
	bool init(int p0, UnlockType p1)
	{
		if (!ItemInfoPopup::init(p0, p1))
			return false;

		if (!Mod::get()->getSettingValue<bool>("apply-unlock"))
			return true;

		auto l = reinterpret_cast<CCLayer*>(this->getChildren()->objectAtIndex(0));
		l->sortAllChildren();
		reinterpret_cast<CCNode*>(l->getChildren()->objectAtIndex(0))->setVisible(false);

		auto gradient = MyLayerGradient::create();
		
		gradient->setStartColor({255, 0, 0});
		gradient->setEndColor({0, 255, 0});
		gradient->setZOrder(-3);
		gradient->setID("gradient"_spr);

		gradient->setPosition(CCDirector::get()->getWinSize() / 2);
		gradient->setContentSize(ccp(300, 230));
		gradient->ignoreAnchorPointForPosition(false);

		if (Mod::get()->getSettingValue<bool>("reverse-order"))
			gradient->setScaleY(-1);

		auto darken = CCScale9Sprite::createWithSpriteFrameName("square-fill.png"_spr);
		darken->setID("darken"_spr);
		darken->setContentSize(gradient->getContentSize() - ccp(15, 15));
		darken->setZOrder(-2);
		darken->setPosition(gradient->getPosition());
		darken->setAnchorPoint(gradient->getAnchorPoint());

		auto bg = CCScale9Sprite::createWithSpriteFrameName("square-outline.png"_spr);
		bg->setPosition(CCDirector::get()->getWinSize() / 2);
		bg->setContentSize(gradient->getContentSize());
		bg->setZOrder(1);
		bg->setID("bg"_spr);

		l->addChild(bg);
		l->addChild(darken);
		l->addChild(gradient);

		if (Mod::get()->getSettingValue<bool>("use-custom-colours"))
		{
			gradient->setStartColor(Mod::get()->getSettingValue<ccColor3B>("primary-colour"));
			gradient->setEndColor(Mod::get()->getSettingValue<ccColor3B>("secondary-colour"));
		}
		else
		{
			gradient->setStartColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerColor.value()));
			gradient->setEndColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerColor2.value()));
		}

		if (Mod::get()->getSettingValue<bool>("reverse-order"))
			gradient->setScaleY(-1);

		gradient->setOpacity(255);
		bg->setOpacity(255);

		return true;
	}
};

class $modify (ShardsPage)
{
	virtual bool init()
	{
		if (!ShardsPage::init())
			return false;

		if (!Mod::get()->getSettingValue<bool>("apply-shards"))
			return true;

		auto l = reinterpret_cast<CCLayer*>(this->getChildren()->objectAtIndex(0));
		l->sortAllChildren();
		reinterpret_cast<CCNode*>(l->getChildren()->objectAtIndex(0))->setVisible(false);

		auto gradient = MyLayerGradient::create();
		
		gradient->setStartColor({255, 0, 0});
		gradient->setEndColor({0, 255, 0});
		gradient->setZOrder(-3);
		gradient->setID("gradient"_spr);

		gradient->setPosition(CCDirector::get()->getWinSize() / 2);
		gradient->setContentSize(ccp(420, 280));
		gradient->ignoreAnchorPointForPosition(false);

		if (Mod::get()->getSettingValue<bool>("reverse-order"))
			gradient->setScaleY(-1);

		auto darken = CCScale9Sprite::createWithSpriteFrameName("square-fill.png"_spr);
		darken->setID("darken"_spr);
		darken->setContentSize(gradient->getContentSize() - ccp(15, 15));
		darken->setZOrder(-2);
		darken->setPosition(gradient->getPosition());
		darken->setAnchorPoint(gradient->getAnchorPoint());

		auto bg = CCScale9Sprite::createWithSpriteFrameName("square-outline.png"_spr);
		bg->setPosition(CCDirector::get()->getWinSize() / 2);
		bg->setContentSize(gradient->getContentSize());
		bg->setZOrder(1);
		bg->setID("bg"_spr);

		l->addChild(bg);
		l->addChild(darken);
		l->addChild(gradient);

		if (Mod::get()->getSettingValue<bool>("use-custom-colours"))
		{
			gradient->setStartColor(Mod::get()->getSettingValue<ccColor3B>("primary-colour"));
			gradient->setEndColor(Mod::get()->getSettingValue<ccColor3B>("secondary-colour"));
		}
		else
		{
			gradient->setStartColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerColor.value()));
			gradient->setEndColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerColor2.value()));
		}

		if (Mod::get()->getSettingValue<bool>("reverse-order"))
			gradient->setScaleY(-1);

		gradient->setOpacity(255);
		bg->setOpacity(255);

		return true;
	}
};

class $modify (CommunityCreditsPage)
{
	virtual bool init()
	{
		if (!CommunityCreditsPage::init())
			return false;

		if (!Mod::get()->getSettingValue<bool>("apply-credits"))
			return true;

		auto l = reinterpret_cast<CCLayer*>(this->getChildren()->objectAtIndex(0));
		l->sortAllChildren();
		reinterpret_cast<CCNode*>(l->getChildren()->objectAtIndex(0))->setVisible(false);

		auto gradient = MyLayerGradient::create();
		
		gradient->setStartColor({255, 0, 0});
		gradient->setEndColor({0, 255, 0});
		gradient->setZOrder(-3);
		gradient->setID("gradient"_spr);

		gradient->setPosition(CCDirector::get()->getWinSize() / 2);
		gradient->setContentSize(ccp(420, 280));
		gradient->ignoreAnchorPointForPosition(false);

		if (Mod::get()->getSettingValue<bool>("reverse-order"))
			gradient->setScaleY(-1);

		auto darken = CCScale9Sprite::createWithSpriteFrameName("square-fill.png"_spr);
		darken->setID("darken"_spr);
		darken->setContentSize(gradient->getContentSize() - ccp(15, 15));
		darken->setZOrder(-2);
		darken->setPosition(gradient->getPosition());
		darken->setAnchorPoint(gradient->getAnchorPoint());

		auto bg = CCScale9Sprite::createWithSpriteFrameName("square-outline.png"_spr);
		bg->setPosition(CCDirector::get()->getWinSize() / 2);
		bg->setContentSize(gradient->getContentSize());
		bg->setZOrder(1);
		bg->setID("bg"_spr);

		l->addChild(bg);
		l->addChild(darken);
		l->addChild(gradient);

		if (Mod::get()->getSettingValue<bool>("use-custom-colours"))
		{
			gradient->setStartColor(Mod::get()->getSettingValue<ccColor3B>("primary-colour"));
			gradient->setEndColor(Mod::get()->getSettingValue<ccColor3B>("secondary-colour"));
		}
		else
		{
			gradient->setStartColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerColor.value()));
			gradient->setEndColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerColor2.value()));
		}

		if (Mod::get()->getSettingValue<bool>("reverse-order"))
			gradient->setScaleY(-1);

		gradient->setOpacity(255);
		bg->setOpacity(255);

		return true;
	}
};

class $modify (DemonFilterSelectLayer)
{
	virtual bool init()
	{
		if (!DemonFilterSelectLayer::init())
			return false;

		if (!Mod::get()->getSettingValue<bool>("apply-demon"))
			return true;

		auto l = reinterpret_cast<CCLayer*>(this->getChildren()->objectAtIndex(0));
		l->sortAllChildren();
		reinterpret_cast<CCNode*>(l->getChildren()->objectAtIndex(0))->setVisible(false);

		auto gradient = MyLayerGradient::create();
		
		gradient->setStartColor({255, 0, 0});
		gradient->setEndColor({0, 255, 0});
		gradient->setZOrder(-3);
		gradient->setID("gradient"_spr);

		gradient->setPosition(CCDirector::get()->getWinSize() / 2);
		gradient->setContentSize(ccp(380, 180));
		gradient->ignoreAnchorPointForPosition(false);

		if (Mod::get()->getSettingValue<bool>("reverse-order"))
			gradient->setScaleY(-1);

		auto darken = CCScale9Sprite::createWithSpriteFrameName("square-fill.png"_spr);
		darken->setID("darken"_spr);
		darken->setContentSize(gradient->getContentSize() - ccp(15, 15));
		darken->setZOrder(-2);
		darken->setPosition(gradient->getPosition());
		darken->setAnchorPoint(gradient->getAnchorPoint());

		auto bg = CCScale9Sprite::createWithSpriteFrameName("square-outline.png"_spr);
		bg->setPosition(CCDirector::get()->getWinSize() / 2);
		bg->setContentSize(gradient->getContentSize());
		bg->setZOrder(1);
		bg->setID("bg"_spr);

		l->addChild(bg);
		l->addChild(darken);
		l->addChild(gradient);

		if (Mod::get()->getSettingValue<bool>("use-custom-colours"))
		{
			gradient->setStartColor(Mod::get()->getSettingValue<ccColor3B>("primary-colour"));
			gradient->setEndColor(Mod::get()->getSettingValue<ccColor3B>("secondary-colour"));
		}
		else
		{
			gradient->setStartColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerColor.value()));
			gradient->setEndColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerColor2.value()));
		}

		if (Mod::get()->getSettingValue<bool>("reverse-order"))
			gradient->setScaleY(-1);

		gradient->setOpacity(255);
		bg->setOpacity(255);

		return true;
	}
};

class $modify (MoreSearchLayer)
{
	virtual bool init()
	{
		if (!MoreSearchLayer::init())
			return false;

		if (!Mod::get()->getSettingValue<bool>("apply-filter"))
			return true;

		auto l = reinterpret_cast<CCLayer*>(this->getChildren()->objectAtIndex(0));
		l->sortAllChildren();
		reinterpret_cast<CCNode*>(l->getChildren()->objectAtIndex(0))->setVisible(false);

		auto gradient = MyLayerGradient::create();
		
		gradient->setStartColor({255, 0, 0});
		gradient->setEndColor({0, 255, 0});
		gradient->setZOrder(-3);
		gradient->setID("gradient"_spr);

		gradient->setPosition(CCDirector::get()->getWinSize() / 2);
		gradient->setContentSize(ccp(440, 290));
		gradient->ignoreAnchorPointForPosition(false);

		if (Mod::get()->getSettingValue<bool>("reverse-order"))
			gradient->setScaleY(-1);

		auto darken = CCScale9Sprite::createWithSpriteFrameName("square-fill.png"_spr);
		darken->setID("darken"_spr);
		darken->setContentSize(gradient->getContentSize() - ccp(15, 15));
		darken->setZOrder(-2);
		darken->setPosition(gradient->getPosition());
		darken->setAnchorPoint(gradient->getAnchorPoint());

		auto bg = CCScale9Sprite::createWithSpriteFrameName("square-outline.png"_spr);
		bg->setPosition(CCDirector::get()->getWinSize() / 2);
		bg->setContentSize(gradient->getContentSize());
		bg->setZOrder(1);
		bg->setID("bg"_spr);

		l->addChild(bg);
		l->addChild(darken);
		l->addChild(gradient);

		if (Mod::get()->getSettingValue<bool>("use-custom-colours"))
		{
			gradient->setStartColor(Mod::get()->getSettingValue<ccColor3B>("primary-colour"));
			gradient->setEndColor(Mod::get()->getSettingValue<ccColor3B>("secondary-colour"));
		}
		else
		{
			gradient->setStartColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerColor.value()));
			gradient->setEndColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerColor2.value()));
		}

		if (Mod::get()->getSettingValue<bool>("reverse-order"))
			gradient->setScaleY(-1);

		gradient->setOpacity(255);
		bg->setOpacity(255);

		return true;
	}
};

class $modify (ChallengesPage)
{
	virtual bool init()
	{
		if (!ChallengesPage::init())
			return false;

		if (!Mod::get()->getSettingValue<bool>("apply-quests"))
			return true;

		auto l = reinterpret_cast<CCLayer*>(this->getChildren()->objectAtIndex(0));
		l->sortAllChildren();
		reinterpret_cast<CCNode*>(l->getChildren()->objectAtIndex(0))->setVisible(false);

		auto gradient = MyLayerGradient::create();
		
		gradient->setStartColor({255, 0, 0});
		gradient->setEndColor({0, 255, 0});
		gradient->setZOrder(-3);
		gradient->setID("gradient"_spr);

		gradient->setPosition(CCDirector::get()->getWinSize() / 2);
		gradient->setContentSize(ccp(420, 280));
		gradient->ignoreAnchorPointForPosition(false);

		if (Mod::get()->getSettingValue<bool>("reverse-order"))
			gradient->setScaleY(-1);

		auto darken = CCScale9Sprite::createWithSpriteFrameName("square-fill.png"_spr);
		darken->setID("darken"_spr);
		darken->setContentSize(gradient->getContentSize() - ccp(15, 15));
		darken->setZOrder(-2);
		darken->setPosition(gradient->getPosition());
		darken->setAnchorPoint(gradient->getAnchorPoint());

		auto bg = CCScale9Sprite::createWithSpriteFrameName("square-outline.png"_spr);
		bg->setPosition(CCDirector::get()->getWinSize() / 2);
		bg->setContentSize(gradient->getContentSize());
		bg->setZOrder(1);
		bg->setID("bg"_spr);

		l->addChild(bg);
		l->addChild(darken);
		l->addChild(gradient);

		if (Mod::get()->getSettingValue<bool>("use-custom-colours"))
		{
			gradient->setStartColor(Mod::get()->getSettingValue<ccColor3B>("primary-colour"));
			gradient->setEndColor(Mod::get()->getSettingValue<ccColor3B>("secondary-colour"));
		}
		else
		{
			gradient->setStartColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerColor.value()));
			gradient->setEndColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerColor2.value()));
		}

		if (Mod::get()->getSettingValue<bool>("reverse-order"))
			gradient->setScaleY(-1);

		gradient->setOpacity(255);
		bg->setOpacity(255);

		return true;
	}
};

class $modify (SetIDPopup)
{
	bool init(int p0, int p1, int p2, gd::string p3, gd::string p4, bool p5, int p6, float p7, bool p8, bool p9)
	{
		if (!SetIDPopup::init(p0, p1, p2, p3, p4, p5, p6, p7, p8, p9))
			return false;

		if (!Mod::get()->getSettingValue<bool>("apply-pagesel"))
			return true;

		auto l = reinterpret_cast<CCLayer*>(this->getChildren()->objectAtIndex(0));
		l->sortAllChildren();
		auto v = reinterpret_cast<CCNode*>(l->getChildren()->objectAtIndex(0));
		v->setVisible(false);

		auto gradient = MyLayerGradient::create();
		
		gradient->setStartColor({255, 0, 0});
		gradient->setEndColor({0, 255, 0});
		gradient->setZOrder(-3);
		gradient->setID("gradient"_spr);

		gradient->setPosition(v->getPosition());
		gradient->setContentSize(ccp(220, 150));
		gradient->ignoreAnchorPointForPosition(false);

		if (Mod::get()->getSettingValue<bool>("reverse-order"))
			gradient->setScaleY(-1);

		auto darken = CCScale9Sprite::createWithSpriteFrameName("square-fill.png"_spr);
		darken->setID("darken"_spr);
		darken->setContentSize(gradient->getContentSize() - ccp(15, 15));
		darken->setZOrder(-2);
		darken->setPosition(gradient->getPosition());
		darken->setAnchorPoint(gradient->getAnchorPoint());

		auto bg = CCScale9Sprite::createWithSpriteFrameName("square-outline.png"_spr);
		bg->setPosition(v->getPosition());
		bg->setContentSize(gradient->getContentSize());
		bg->setZOrder(1);
		bg->setID("bg"_spr);

		l->addChild(bg);
		l->addChild(darken);
		l->addChild(gradient);

		if (Mod::get()->getSettingValue<bool>("use-custom-colours"))
		{
			gradient->setStartColor(Mod::get()->getSettingValue<ccColor3B>("primary-colour"));
			gradient->setEndColor(Mod::get()->getSettingValue<ccColor3B>("secondary-colour"));
		}
		else
		{
			gradient->setStartColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerColor.value()));
			gradient->setEndColor(GameManager::get()->colorForIdx(GameManager::get()->m_playerColor2.value()));
		}

		if (Mod::get()->getSettingValue<bool>("reverse-order"))
			gradient->setScaleY(-1);

		gradient->setOpacity(255);
		bg->setOpacity(255);

		return true;
	}
};

#endif