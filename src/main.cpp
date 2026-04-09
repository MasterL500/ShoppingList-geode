#include <Geode/Geode.hpp>
#include <Geode/modify/GJGarageLayer.hpp>
#include <Geode/modify/GJShopLayer.hpp>
#include "ShopListAlert.hpp"

using namespace geode::prelude;

//	Garage Layer (Icon Kit)
class $modify(GarageLayer, GJGarageLayer)
{
	bool init()
	{
		if (!GJGarageLayer::init())
			return false;

		if (Mod::get()->getSettingValue<bool>("garage-button"))
		{
			auto menu = this->getChildByID("shards-menu");
			auto spr = CircleButtonSprite::createWithSprite("ShopListIcon.png"_spr, 1, CircleBaseColor::Gray, CircleBaseSize::Small);
			auto button = CCMenuItemSpriteExtra::create(
				spr,
				this,
				menu_selector(GarageLayer::onShopListButton));
			button->setID("Shopping-List-Button");

			menu->addChild(button);
			menu->updateLayout();
		}

		return true;
	}

	void onShopListButton(CCObject *)
	{
		ShopListAlert::create(ShopType(-1))->show();
	}
};

//	Shop Layer
class $modify(ShopLayer, GJShopLayer)
{
	bool init(ShopType p0)
	{
		if (!GJShopLayer::init(p0))
			return false;

		if (Mod::get()->getSettingValue<bool>("shops-button"))
		{
			auto menu = CCMenu::create();
			menu->setPosition({30.0f, 30.0f});
			menu->setID("Shopping-List-Menu");
			menu->setZOrder(10);

			auto button = CCMenuItemSpriteExtra::create(
				CCSpriteGrayscale::createWithSpriteFrameName("GJ_infoIcon_001.png"),
				this,
				menu_selector(ShopLayer::onInfo));
			button->setID("Shopping-List-Button");

			menu->addChild(button);
			menu->updateLayout();
			this->addChild(menu);
		}

		return true;
	}

	void onInfo(CCObject *sender)
	{
		ShopListAlert::create(m_type)->show();
	}
};

$execute
{
	listenForAllSettingChanges([](const std::string_view key, std::shared_ptr<SettingV3> setting)
							   {
		if (auto shoppingList = static_cast<ShopListAlert*>(CCScene::get()->getChildByIDRecursive("Shopping-List-Popup")))
		{
			shoppingList->onNavButton(shoppingList->getChildByIDRecursive("navigation-menu")->getChildByTag(shoppingList->m_page));
		} });
};