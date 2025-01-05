#include <Geode/Geode.hpp>
#include <Geode/modify/GJGarageLayer.hpp>
#include <Geode/modify/GJShopLayer.hpp>
#include "layers/ShopListAlert.hpp"

using namespace geode::prelude;

// GARAGE LAYER
class $modify(ModLayer, GJGarageLayer){
	bool init(){
		if(!GJGarageLayer::init()) return false;
		auto sl_garageButton = Mod::get()->getSettingValue<bool>("garage-button");

		if(sl_garageButton){
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
		}

		return true;
	}

	void onShopListButton(CCObject *){
		ShopRewardsListAlert::create(ShopType(-1))->show();
	}
};

// SHOP LAYER
class $modify(ShopLayer, GJShopLayer){
	bool init(ShopType p0){
		if(!GJShopLayer::init(p0)) return false;
		auto winSize = CCDirector::sharedDirector()->getWinSize();

		NodeIDs::provideFor(this);

		auto menu = CCMenu::create();
		menu->setPosition({30.0f, 30.0f});
		menu->setID("Shopping-List-Menu");
		menu->setZOrder(5);
		
		auto button = CCMenuItemSpriteExtra::create(
			CCSprite::create("SL_ModInfo.png"_spr),
			this,
			menu_selector(ShopLayer::onInfoButton));
		button->setID("Shopping-List-Button");
		
		menu->addChild(button);
		menu->updateLayout();
		this->addChild(menu);
		return true;
	}

	void onInfoButton(CCObject * sender){
		//  log::debug("Value {}", sender->getTag());

		ShopRewardsListAlert::create(m_type)->show();

		//	ShopRewardsListAlert::create()->show();
	}
};