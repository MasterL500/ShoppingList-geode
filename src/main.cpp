#include <Geode/Geode.hpp>
#include <Geode/modify/GJGarageLayer.hpp>
#include <Geode/modify/GJShopLayer.hpp>
#include "ShopListAlert.hpp"

using namespace geode::prelude;

// GARAGE LAYER
class $modify(ModLayer, GJGarageLayer)
{
    bool init()
    {
        if (!GJGarageLayer::init())
            return false;

        if (Mod::get()->getSettingValue<bool>("garage-button"))
        {
            NodeIDs::provideFor(this);

            auto menu = this->getChildByID("shards-menu");
            auto spr = CircleButtonSprite::createWithSprite("SL_ShopIcon.png"_spr, 1, CircleBaseColor::Gray, CircleBaseSize::Small);
            auto button = CCMenuItemSpriteExtra::create(
                spr,
                this,
                menu_selector(ModLayer::onShopListButton));
            button->setID("Shopping-List-Button");

            menu->addChild(button);
            menu->updateLayout();
        }

        return true;
    }

    void onShopListButton(CCObject *)
    {
        ShopRewardsListAlert::create(ShopType(-1))->show();
    }
};

// SHOP LAYER
class $modify(ShopLayer, GJShopLayer)
{
    bool init(ShopType p0)
    {
        if (!GJShopLayer::init(p0))
            return false;

        NodeIDs::provideFor(this);

        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto menu = CCMenu::create();
        menu->setPosition({30.0f, 30.0f});
        menu->setID("Shopping-List-Menu");
        menu->setZOrder(5);

        auto button = CCMenuItemSpriteExtra::create(
            CCSprite::create("SL_ModInfo.png"_spr),
            this,
            menu_selector(ShopLayer::onInfo));
        button->setID("Shopping-List-Button");

        menu->addChild(button);
        menu->updateLayout();
        this->addChild(menu);
        return true;
    }

    void onInfo(CCObject *sender)
    {
        ShopRewardsListAlert::create(m_type)->show();
    }
};

$execute
{
    listenForAllSettingChanges([](const std::string_view key, std::shared_ptr<SettingV3> setting)
                               {
		log::debug("Setting: {} Changed", key);

		if (auto shoppingList = static_cast<ShopRewardsListAlert*>(CCScene::get()->getChildByIDRecursive("shopping-list-popup")))
		{
			shoppingList->onNavButton(shoppingList->getChildByIDRecursive("navigation-menu")->getChildByTag(shoppingList->m_currentPage));
		} });
};