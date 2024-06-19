#include <Geode/Geode.hpp>
#include <Geode/modify/GJGarageLayer.hpp>
#include "ShopListAlert.hpp"

using namespace geode::prelude;

class $modify(ModLayer, GJGarageLayer){
	bool init(){
		if(!GJGarageLayer::init()) return false;

		NodeIDs::provideFor(this);

		auto menu = this->getChildByID("shards-menu");

		auto spr = CircleButtonSprite::createWithSprite("bigShopIcon.png"_spr, 1, CircleBaseColor::Gray, CircleBaseSize::Small);
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
		geode::createQuickPopup(
			"Warning",
			"It's recommended to load the textures of the Icons by cycling through the Icon Kit first before opening this, if you haven't done it, you'll run the risk of the Mod Crashing.\nDo you wish to proceed?",
			"No", "Yes",
			[](auto, bool btn2) {
				if (btn2) {
					ShoppingListAlert::create()->show();
				}
			}
		);
	}
};
