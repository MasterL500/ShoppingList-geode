#include <Geode/Geode.hpp>
#include <Geode/binding/DialogLayer.hpp>
#include <Geode/modify/GJGarageLayer.hpp>
#include <Geode/modify/GJShopLayer.hpp>
#include "ShopListAlert.hpp"

using namespace geode::prelude;

class $modify(ModLayer, GJGarageLayer){
	bool init(){
		if(!GJGarageLayer::init()) return false;

		NodeIDs::provideFor(this);

		auto menu = this->getChildByID("shards-menu");

		auto spr = CircleButtonSprite::createWithSprite("SL_ShopIcon.png"_spr, 1, CircleBaseColor::Gray, CircleBaseSize::Small);
		auto button = CCMenuItemSpriteExtra::create(
			spr,
			this,
			menu_selector(ModLayer::onShopListButton)
		);
		button->setID("Shopping-List-Button");

		menu->addChild(button);
		menu->updateLayout();
		return true;
	}

	void onShopListButton(CCObject *){
		ShoppingListAlert::create()->show();
	}
};

class $modify(ShopLayer, GJShopLayer){
	bool init(ShopType p0){
		if(!GJShopLayer::init(p0)) return false;
		auto winSize = CCDirector::sharedDirector()->getWinSize();

		NodeIDs::provideFor(this);

		auto menu = CCMenu::create();
		menu->setPosition({30.0f, 30.0f});
		menu->setID("Treasure-Checklist-Menu");
		menu->setZOrder(5);
		
		auto button = CCMenuItemSpriteExtra::create(
			CCSprite::create("SL_ModInfo.png"_spr),
			this,
			menu_selector(ShopLayer::onInfoButton));
		button->setID("Treasure-Checklist-Button");
		
		menu->addChild(button);
		menu->updateLayout();
		this->addChild(menu);
		return true;
		}

		void onInfoButton(CCObject *){
			ShoppingListAlert::create()->show();
		}
	};