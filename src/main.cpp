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
#include <Geode/modify/SetTextPopup.hpp>
#include <Geode/modify/ChallengesPage.hpp>

using namespace geode::prelude;

class GradientPages
{
	public:
		static inline GJUserScore* score = nullptr;
		static inline CCNode* macNode = nullptr;

		static CCNode* createGradientWithSize(CCPoint size, GJUserScore* score = nullptr)
		{
			auto gradient = CCLayerGradient::create();
			gradient->setContentSize(size);
			gradient->setZOrder(-69);
			gradient->setID("gradient"_spr);

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

			gradient->setPosition(CCDirector::get()->getWinSize() / 2);
			gradient->ignoreAnchorPointForPosition(false);

			if (score)
			{
				gradient->setStartColor(GameManager::sharedState()->colorForIdx(score->m_color1));
				gradient->setEndColor(GameManager::sharedState()->colorForIdx(score->m_color2));
			}

			if (Mod::get()->getSettingValue<bool>("reverse-order"))
			gradient->setScaleY(-1);

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
			
			gradient->addChild(darken);
			gradient->addChild(outline);

			return gradient;
		}

		static ccColor3B lerpColor(const ccColor3B& startColor, const ccColor3B& endColor, float t) {
			t = fmaxf(0.0f, fminf(t, 1.0f));

			return ccc3(startColor.r + (endColor.r - startColor.r) * t,
						startColor.g + (endColor.g - startColor.g) * t,
						startColor.b + (endColor.b - startColor.b) * t);
		}
};

class $modify(ProfilePageExt, ProfilePage) {

	void updateCommentList()
	{
		if (!m_list)
			return;

		if (auto comment = m_list)
		{
			if (Mod::get()->getSettingValue<bool>("gradient-comments"))
			{
				comment->setColor(ccc3(0, 0, 0));
				comment->setOpacity(40);
			}

			CCObject* child = nullptr;
			CCARRAY_FOREACH(comment->getChildren(), child)
			{
				if (typeinfo_cast<CCSprite*>(child))
				{
					as<CCSprite*>(child)->setVisible(false);
				}
			};

			if (auto ss = comment->getChildByID("comment-outline"_spr))
				ss->removeFromParent();

			auto spr = CCScale9Sprite::createWithSpriteFrameName("comment-outline.png"_spr);
			spr->setColor(ccc3(0, 0, 0));
			spr->setOpacity(50);
			spr->setContentSize(comment->getContentSize() + ccp(3.5f, 3.5f)); // i love random numbers that make no sense
			spr->setAnchorPoint(ccp(0.5f, 0.5f));
			spr->setPosition(comment->getContentSize() / 2);
			spr->setID("comment-outline"_spr);
			comment->addChild(spr, 69);
		}

		if (!Mod::get()->getSettingValue<bool>("gradient-comments"))
			return;

		if (!m_list->m_list)
			return;

		if (!m_list->m_list->m_tableView)
			return;

		if (!m_list->m_list->m_tableView->m_contentLayer)
			return;

		if (auto content = m_list->m_list->m_tableView->m_contentLayer) // not sorry for this
		{
			if (!content->getChildren())
				return;
			
			CCObject* child = nullptr;

			CCARRAY_FOREACH(content->getChildren(), child)
			{
				if (auto cmt = typeinfo_cast<CommentCell*>(child))
				{
					cmt->m_backgroundLayer->setColor(ccc3(0, 0, 0));
					cmt->m_backgroundLayer->setOpacity(0);
					if (auto bg = getChildOfType<CCScale9Sprite>(cmt->m_mainLayer, 0))
					{
						bg->setOpacity(50);
						bg->setColor(ccc3(0, 0, 0));
					}
				}
			};
		}
	}

	static ProfilePage* create(int accountID, bool ownProfile)
	{
		auto res = ProfilePage::create(accountID, ownProfile);

		if (res)
		{
			GradientPages::macNode = nullptr;

			if (!Mod::get()->getSettingValue<bool>("apply-profiles") || Loader::get()->getLoadedMod("bitz.customprofiles"))
				return res;

			if (GradientPages::score && GradientPages::score->m_accountID != accountID)
				GradientPages::score = nullptr;

			if (auto icons = getChildOfType<GJCommentListLayer>(res->m_mainLayer, 0))
			{
				if (Mod::get()->getSettingValue<bool>("gradient-comments"))
				{
					icons->setColor(ccc3(0, 0, 0));
					icons->setOpacity(40);
				}

				CCObject* child = nullptr;
				CCARRAY_FOREACH(icons->getChildren(), child)
				{
					if (typeinfo_cast<CCSprite*>(child))
					{
						as<CCSprite*>(child)->setVisible(false);
					}
				};

				auto spr = CCScale9Sprite::createWithSpriteFrameName("comment-outline.png"_spr);
				spr->setColor(ccc3(0, 0, 0));
				spr->setOpacity(50);
				spr->setContentSize(icons->getContentSize() + ccp(3.5f, 3.5f));
				spr->setAnchorPoint(ccp(0.5f, 0.5f));
				spr->setPosition(icons->getContentSize() / 2);
				icons->addChild(spr, 69);
			}

			as<ProfilePageExt*>(res)->updateCommentList();

			auto l = res->m_mainLayer;

			auto gradient = CCLayerGradient::create();
			
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
			darken->setPosition(gradient->getContentSize() / 2);
			darken->setAnchorPoint(gradient->getAnchorPoint());
			darken->setOpacity(0);

			auto bg = CCScale9Sprite::createWithSpriteFrameName("square-outline.png"_spr);
			bg->setPosition(gradient->getContentSize() / 2);
			bg->setContentSize(ccp(440, 290));
			bg->setZOrder(1);
			bg->setID("bg"_spr);

			gradient->addChild(bg, 42069);
			gradient->addChild(darken);
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
		}

		return res;
	}

	virtual void loadPageFromUserInfo(GJUserScore* score)
	{
		ProfilePage::loadPageFromUserInfo(score);

		if (!Mod::get()->getSettingValue<bool>("apply-profiles") || Loader::get()->getLoadedMod("bitz.customprofiles"))
			return;
			
		GradientPages::score = score;

		auto l = m_mainLayer;

		if (l)
		{
			auto g = as<CCLayerGradient*>(l->getChildByID("gradient"_spr));

			if (g)
			{
				auto d = as<CCScale9Sprite*>(g->getChildByID("darken"_spr));

				g->setStartColor(GameManager::get()->colorForIdx(score->m_color1));
				g->setEndColor(GameManager::get()->colorForIdx(score->m_color2));
				
				if (g->getOpacity() == 0)
				{
					g->runAction(CCFadeTo::create(0.25f, 255));
				}

				if (d)
				{
					if (d->getOpacity() == 0)
					{
						d->runAction(CCFadeTo::create(0.25f, 255));
					}
				}
			}
		}
	}

	void setupCommentsBrowser(cocos2d::CCArray* p0)
	{
		ProfilePage::setupCommentsBrowser(p0);

		if (!Mod::get()->getSettingValue<bool>("apply-profiles") || Loader::get()->getLoadedMod("bitz.customprofiles"))
			return;

		updateCommentList();
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

		auto l = m_mainLayer;
		auto v = getChildOfType<CCScale9Sprite>(l, 0);

		l->addChild(GradientPages::createGradientWithSize(v->getContentSize()));
		v->setVisible(false);

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

		auto l = m_mainLayer;
		auto v = getChildOfType<CCScale9Sprite>(l, 0);

		l->addChild(GradientPages::createGradientWithSize(v->getContentSize()));
		v->setVisible(false);

		return true;
	}

	void setupCommentsBrowser(cocos2d::CCArray* p0)
	{
		FRequestProfilePage::setupCommentsBrowser(p0);

		if (!Mod::get()->getSettingValue<bool>("apply-profiles"))
			return;

		auto m_list = getChildOfType<GJCommentListLayer>(m_mainLayer, 0);

		if (!m_list)
			return;

		CCObject* child = nullptr;
		CCARRAY_FOREACH(m_list->getChildren(), child)
		{
			if (typeinfo_cast<CCSprite*>(child))
			{
				as<CCSprite*>(child)->setVisible(false);
			}
		};

		auto spr = CCScale9Sprite::createWithSpriteFrameName("comment-outline.png"_spr);
		spr->setColor(ccc3(0, 0, 0));
		spr->setOpacity(50);
		spr->setContentSize(m_list->getContentSize() + ccp(3.5f, 3.5f));
		spr->setAnchorPoint(ccp(0.5f, 0.5f));
		spr->setPosition(m_list->getContentSize() / 2);
		m_list->addChild(spr, 69);

		if (!Mod::get()->getSettingValue<bool>("gradient-comments"))
			return;

		m_list->setColor(ccc3(0, 0, 0));
		m_list->setOpacity(40);
		
		if (!m_list->m_list)
			return;

		if (!m_list->m_list->m_tableView)
			return;

		if (!m_list->m_list->m_tableView->m_contentLayer)
			return;

		if (auto content = m_list->m_list->m_tableView->m_contentLayer) // not sorry for this
		{
			if (!content->getChildren())
				return;

			int i = 0;
			
			CCARRAY_FOREACH(content->getChildren(), child)
			{
				if (auto cmt = typeinfo_cast<GJUserCell*>(child))
				{
					cmt->m_backgroundLayer->setColor(ccc3(0, 0, 0));
					cmt->m_backgroundLayer->setOpacity(i % 2 ? 50 : 0);
				}

				i++;
			};
		}
	}
};

class $modify(MessagesProfilePage) {

	bool init(bool p0)
	{
		if (!MessagesProfilePage::init(p0))
			return false;

		if (!Mod::get()->getSettingValue<bool>("apply-profiles"))
			return true;

		auto l = m_mainLayer;
		auto v = getChildOfType<CCScale9Sprite>(l, 0);

		l->addChild(GradientPages::createGradientWithSize(v->getContentSize()));
		v->setVisible(false);

		return true;
	}

	void setupCommentsBrowser(cocos2d::CCArray* p0)
	{
		MessagesProfilePage::setupCommentsBrowser(p0);

		if (!Mod::get()->getSettingValue<bool>("apply-profiles"))
			return;

		auto m_list = getChildOfType<GJCommentListLayer>(m_mainLayer, 0);

		if (!m_list)
			return;

		CCObject* child = nullptr;
		CCARRAY_FOREACH(m_list->getChildren(), child)
		{
			if (typeinfo_cast<CCSprite*>(child))
			{
				as<CCSprite*>(child)->setVisible(false);
			}
		};

		auto spr = CCScale9Sprite::createWithSpriteFrameName("comment-outline.png"_spr);
		spr->setColor(ccc3(0, 0, 0));
		spr->setOpacity(50);
		spr->setContentSize(m_list->getContentSize() + ccp(3.5f, 3.5f));
		spr->setAnchorPoint(ccp(0.5f, 0.5f));
		spr->setPosition(m_list->getContentSize() / 2);
		m_list->addChild(spr, 69);

		if (!Mod::get()->getSettingValue<bool>("gradient-comments"))
			return;

		m_list->setColor(ccc3(0, 0, 0));
		m_list->setOpacity(40);
		
		if (!m_list->m_list)
			return;

		if (!m_list->m_list->m_tableView)
			return;

		if (!m_list->m_list->m_tableView->m_contentLayer)
			return;

		if (auto content = m_list->m_list->m_tableView->m_contentLayer) // not sorry for this
		{
			if (!content->getChildren())
				return;

			int i = 0;
			
			CCARRAY_FOREACH(content->getChildren(), child)
			{
				if (auto cmt = typeinfo_cast<GJMessageCell*>(child))
				{
					cmt->m_backgroundLayer->setColor(ccc3(0, 0, 0));
					cmt->m_backgroundLayer->setOpacity(i % 2 ? 50 : 0);
				}

				i++;
			};
		}
	}
};

class $modify(FriendsProfilePage) {

	bool init(UserListType p0)
	{
		if (!FriendsProfilePage::init(p0))
			return false;

		if (!Mod::get()->getSettingValue<bool>("apply-profiles"))
			return true;

		auto l = m_mainLayer;
		auto v = getChildOfType<CCScale9Sprite>(l, 0);

		l->addChild(GradientPages::createGradientWithSize(v->getContentSize()));
		v->setVisible(false);

		return true;
	}

	void setupUsersBrowser(cocos2d::CCArray* p0, UserListType p1)
	{
		FriendsProfilePage::setupUsersBrowser(p0, p1);

		if (!Mod::get()->getSettingValue<bool>("apply-profiles"))
			return;

		auto m_list = getChildOfType<GJCommentListLayer>(m_mainLayer, 0);

		if (!m_list)
			return;

		CCObject* child = nullptr;
		CCARRAY_FOREACH(m_list->getChildren(), child)
		{
			if (typeinfo_cast<CCSprite*>(child))
			{
				as<CCSprite*>(child)->setVisible(false);
			}
		};

		auto spr = CCScale9Sprite::createWithSpriteFrameName("comment-outline.png"_spr);
		spr->setColor(ccc3(0, 0, 0));
		spr->setOpacity(50);
		spr->setContentSize(m_list->getContentSize() + ccp(3.5f, 3.5f));
		spr->setAnchorPoint(ccp(0.5f, 0.5f));
		spr->setPosition(m_list->getContentSize() / 2);
		m_list->addChild(spr, 69);

		if (!Mod::get()->getSettingValue<bool>("gradient-comments"))
			return;

		m_list->setColor(ccc3(0, 0, 0));
		m_list->setOpacity(40);
		
		if (!m_list->m_list)
			return;

		if (!m_list->m_list->m_tableView)
			return;

		if (!m_list->m_list->m_tableView->m_contentLayer)
			return;

		if (auto content = m_list->m_list->m_tableView->m_contentLayer) // not sorry for this
		{
			if (!content->getChildren())
				return;

			int i = 0;
			
			CCARRAY_FOREACH(content->getChildren(), child)
			{
				if (auto cmt = typeinfo_cast<GJUserCell*>(child))
				{
					cmt->m_backgroundLayer->setColor(ccc3(0, 0, 0));
					cmt->m_backgroundLayer->setOpacity(i % 2 ? 50 : 0);
				}

				i++;
			};
		}
	}
};

class $modify(InfoLayer) {

	bool init(GJGameLevel* level, GJUserScore* score, GJLevelList* p2)
	{
		if (!InfoLayer::init(level, score, p2))
			return false;

		if (!Mod::get()->getSettingValue<bool>("apply-info-layer") || Loader::get()->getLoadedMod("bitz.customprofiles"))
			return true;

		auto l = m_mainLayer;
		auto v = getChildOfType<CCScale9Sprite>(l, 0);

		l->addChild(GradientPages::createGradientWithSize(v->getContentSize(), score));
		v->setVisible(false);

		return true;
	}

	void setupCommentsBrowser(cocos2d::CCArray* p0)
	{
		InfoLayer::setupCommentsBrowser(p0);

		if (!Mod::get()->getSettingValue<bool>("apply-info-layer") || Loader::get()->getLoadedMod("bitz.customprofiles"))
			return;

		auto m_list = getChildOfType<GJCommentListLayer>(m_mainLayer, 0);

		if (!m_list)
			return;

		CCObject* child = nullptr;
		CCARRAY_FOREACH(m_list->getChildren(), child)
		{
			if (typeinfo_cast<CCSprite*>(child))
			{
				as<CCSprite*>(child)->setVisible(false);
			}
		};

		auto spr = CCScale9Sprite::createWithSpriteFrameName("comment-outline.png"_spr);
		spr->setColor(ccc3(0, 0, 0));
		spr->setOpacity(50);
		spr->setContentSize(m_list->getContentSize() + ccp(3.5f, 3.5f));
		spr->setAnchorPoint(ccp(0.5f, 0.5f));
		spr->setPosition(m_list->getContentSize() / 2);
		m_list->addChild(spr, 69);

		if (!Mod::get()->getSettingValue<bool>("gradient-comments"))
			return;

		m_list->setColor(ccc3(0, 0, 0));
		m_list->setOpacity(40);
		
		if (!m_list->m_list)
			return;

		if (!m_list->m_list->m_tableView)
			return;

		if (!m_list->m_list->m_tableView->m_contentLayer)
			return;

		if (auto content = m_list->m_list->m_tableView->m_contentLayer) // not sorry for this
		{
			if (!content->getChildren())
				return;

			int i = 0;
			
			CCARRAY_FOREACH(content->getChildren(), child)
			{
				if (auto cmt = typeinfo_cast<CommentCell*>(child))
				{
					cmt->m_backgroundLayer->setColor(ccc3(0, 0, 0));
					cmt->m_backgroundLayer->setOpacity(i % 2 ? 50 : 0);
				}

				i++;
			};
		}
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

		auto l = m_mainLayer;
		auto v = getChildOfType<CCScale9Sprite>(l, 0);

		l->addChild(GradientPages::createGradientWithSize(v->getContentSize()));
		v->setVisible(false);

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

		auto l = m_mainLayer;
		auto v = getChildOfType<CCScale9Sprite>(l, 0);

		l->addChild(GradientPages::createGradientWithSize(v->getContentSize()));
		v->setVisible(false);

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

		auto l = m_mainLayer;
		auto v = getChildOfType<CCScale9Sprite>(l, 0);

		l->addChild(GradientPages::createGradientWithSize(v->getContentSize()));
		v->setVisible(false);

		return true;
	}
};

class $modify (DemonFilterSelectLayer)
{
	static void onModify(auto& self) {
        self.setHookPriority("DemonFilterSelectLayer::init", -6969);
    }

	virtual bool init()
	{
		if (!DemonFilterSelectLayer::init())
			return false;

		if (!Mod::get()->getSettingValue<bool>("apply-demon"))
			return true;

		auto l = m_mainLayer;
		auto v = getChildOfType<CCScale9Sprite>(l, 0);

		l->addChild(GradientPages::createGradientWithSize(v->getContentSize()));
		v->setVisible(false);

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

		auto l = m_mainLayer;
		auto v = getChildOfType<CCScale9Sprite>(l, 0);

		l->addChild(GradientPages::createGradientWithSize(v->getContentSize()));
		v->setVisible(false);

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

		auto l = m_mainLayer;
		auto v = getChildOfType<CCScale9Sprite>(l, 0);

		l->addChild(GradientPages::createGradientWithSize(v->getContentSize()));
		v->setVisible(false);

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

		auto l = m_mainLayer;
		auto v = getChildOfType<CCScale9Sprite>(l, 0);

		auto g = GradientPages::createGradientWithSize(v->getContentSize());
		g->setPosition(v->getPosition());
		l->addChild(g);
		v->setVisible(false);

		return true;
	}
};