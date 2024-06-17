#include "ShopListAlert.hpp"
#include "utils.hpp"

//	Icon Parameters (to create the info pop-up when pressed
struct IconParameters : public CCObject{
	UnlockType p_iconType;
	int p_iconId;

	IconParameters(UnlockType icon, int id) : p_iconType(icon), p_iconId(id){
		this->autorelease();
	}
};

//  Mod's Popup set-up
bool ShopListAlert::setup(){
    auto showMiscRewards = Mod::get()->getSettingValue<bool>("misc-rewards");
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    this->setTitle("The Shop");

    //  Background for the Icon list
    auto iconListBG = CCScale9Sprite::create("square02b_001.png", {0, 0, 80, 80});
    iconListBG->setPosition({winSize.width / 2.f, winSize.height / 2.f + 8.f});
    iconListBG->setColor({ 133, 68, 41 });
    iconListBG->setContentSize({400, 165});
	this->addChild(iconListBG);

    //  Info Button menu
    auto infoMenu = CCMenu::create();
    infoMenu->setContentSize({420.f, 260.f});
    infoMenu->ignoreAnchorPointForPosition(false);
    infoMenu->setID("info-menu");

    //  Info Button itself
    auto infoButton = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png"),
        this,
        nullptr
    );

    infoButton->setPosition(infoMenu->getContentSize() - 20.f);
    infoButton->setID("info-button");
    infoMenu->addChild(infoButton);
    this->addChild(infoMenu);

    //  Creates the menu that features buttons with the Chest sprites. (OPTIMIZED)
    auto shopMenu = CCMenu::create();
    shopMenu->setPositionY(winSize.height / 2 - 100.f);
    shopMenu->setLayout(RowLayout::create()
            ->setGap(8.f)
    );
    shopMenu->setID("shop-menu");
    this->addChild(shopMenu);

    //  Creates the buttons for the Menu. By default the first button is Selected.
    for(int ii = 1; ii <= 5; ii++){
        createShopButton(shopMenu, ii, ii == 1);
    };

    //  Creates a menu for each page available (PLANNING TO OPTIMIZE)
    auto iconPage_1 = MLUtils::createIconPage({520.f, 220.f}, 1, true);
    auto iconPage_2 = MLUtils::createIconPage({520.f, 220.f}, 2, false);
    auto iconPage_3 = MLUtils::createIconPage({520.f, 220.f}, 3, false);
    auto iconPage_4 = MLUtils::createIconPage({520.f, 220.f}, 4, false);
    auto iconPage_5 = MLUtils::createIconPage({520.f, 220.f}, 5, false);

    // THE SHOP - CUBES
    addShopIcon(iconPage_1, 0x1, 73, 1000, false);
    addShopIcon(iconPage_1, 0x1, 77, 1000, false);
    addShopIcon(iconPage_1, 0x1, 79, 500, false);
    addShopIcon(iconPage_1, 0x1, 86, 1000, false);
    addShopIcon(iconPage_1, 0x1, 102, 1000, false);
    addShopIcon(iconPage_1, 0x1, 107, 1000, false);
    addShopIcon(iconPage_1, 0x1, 109, 4000, false);
    addShopIcon(iconPage_1, 0x1, 113, 4000, false);
    addShopIcon(iconPage_1, 0x1, 140, 3000, false);
    addShopIcon(iconPage_1, 0x1, 162, 3000, false);
    addShopIcon(iconPage_1, 0x1, 191, 3000, false);
    // THE SHOP - SHIPS
    addShopIcon(iconPage_1, 0x4, 27, 2000, false);
    // THE SHOP - BALLS
    addShopIcon(iconPage_1, 0x5, 19, 1500, false);
    addShopIcon(iconPage_1, 0x5, 20, 1000, false);
    addShopIcon(iconPage_1, 0x5, 40, 4000, false);
    addShopIcon(iconPage_1, 0x5, 57, 3000, false);
    // THE SHOP - UFOS
    addShopIcon(iconPage_1, 0x6, 23, 2500, false);
    addShopIcon(iconPage_1, 0x6, 25, 2000, false);
    addShopIcon(iconPage_1, 0x6, 40, 3500, false);
    // THE SHOP - WAVES
    addShopIcon(iconPage_1, 0x7, 21, 500, false);
    addShopIcon(iconPage_1, 0x7, 35, 4000, false);
    addShopIcon(iconPage_1, 0x7, 38, 5000, false);
    // THE SHOP - ROBOTS
    addShopIcon(iconPage_1, 0x8, 12, 3000, false);
    addShopIcon(iconPage_1, 0x8, 33, 3500, false);
    // THE SHOP - SPIDERS
    addShopIcon(iconPage_1, 0x9, 2, 2000, false);
    addShopIcon(iconPage_1, 0x9, 18, 4000, false);
    // THE SHOP - SWINGS
    addShopIcon(iconPage_1, 0xD, 7, 5000, false);
    // THE SHOP - EXTRAS
    addShopIcon(iconPage_1, 0x2, 33, 1000, false);
    addShopIcon(iconPage_1, 0x2, 35, 1000, false);
    addShopIcon(iconPage_1, 0x2, 36, 1000, false);
    addShopIcon(iconPage_1, 0x2, 39, 1000, false);
    addShopIcon(iconPage_1, 0x2, 84, 1500, false);
    addShopIcon(iconPage_1, 0x2, 97, 2500, false);
    addShopIcon(iconPage_1, 0x2, 99, 1500, false);
    addShopIcon(iconPage_1, 0x3, 28, 1000, false);
    addShopIcon(iconPage_1, 0x3, 39, 1000, false);
    addShopIcon(iconPage_1, 0x3, 49, 2500, false);
    addShopIcon(iconPage_1, 0x3, 88, 2000, false);
    addShopIcon(iconPage_1, 0x3, 104, 2000, false);
    addShopIcon(iconPage_1, 0xB, 8, 7000, false);
    addShopIcon(iconPage_1, 0xB, 11, 7000, false);

    //  SECRET SHOP - CUBES
    addShopIcon(iconPage_2, 0x1, 85, 2500, false);
    addShopIcon(iconPage_2, 0x1, 94, 3000, false);
    addShopIcon(iconPage_2, 0x1, 96, 2000, false);
    addShopIcon(iconPage_2, 0x1, 110, 5000, false);
    addShopIcon(iconPage_2, 0x1, 136, 5000, false);
    addShopIcon(iconPage_2, 0x1, 206, 2000, false);
    addShopIcon(iconPage_2, 0x1, 225, 3000, false);
    addShopIcon(iconPage_2, 0x1, 229, 5000, false);
    //  SECRET SHOP - SHIPS
    addShopIcon(iconPage_2, 0x4, 58, 4000, false);
    addShopIcon(iconPage_2, 0x4, 138, 4000, false);
    //  SECRET SHOP - BALLS
    addShopIcon(iconPage_2, 0x5, 25, 2000, false);
    addShopIcon(iconPage_2, 0x5, 60, 2500, false);
    addShopIcon(iconPage_2, 0x5, 107, 3000, false);
    //  SECRET SHOP - UFOS
    addShopIcon(iconPage_2, 0x6, 21, 4000, false);
    addShopIcon(iconPage_2, 0x6, 83, 3000, false);
    addShopIcon(iconPage_2, 0x6, 104, 2500, false);
    //  SECRET SHOP - WAVES
    addShopIcon(iconPage_2, 0x7, 18, 2000, false);
    addShopIcon(iconPage_2, 0x7, 78, 2500, false);
    //  SECRET SHOP - ROBOTS
    addShopIcon(iconPage_2, 0x8, 14, 3000, false);
    addShopIcon(iconPage_2, 0x8, 45, 3500, false);
    //  SECRET SHOP - SPIDERS
    addShopIcon(iconPage_2, 0x9, 3, 3000, false);
    addShopIcon(iconPage_2, 0x9, 58, 3500, false);
    addShopIcon(iconPage_2, 0x9, 69, 5000, false);
    //  SECRET SHOP - SWINGS
    addShopIcon(iconPage_2, 0xD, 12, 6000, false);
    addShopIcon(iconPage_2, 0xD, 27, 6000, false);

    //  COMMUNITY SHOP - CUBES
    addShopIcon(iconPage_3, 0x1, 117, 4000, false);
    addShopIcon(iconPage_3, 0x1, 118, 2000, false);
    addShopIcon(iconPage_3, 0x1, 119, 2000, false);
    addShopIcon(iconPage_3, 0x1, 120, 4000, false);
    addShopIcon(iconPage_3, 0x1, 121, 2000, false);
    addShopIcon(iconPage_3, 0x1, 122, 2000, false);
    addShopIcon(iconPage_3, 0x1, 123, 6000, false);
    addShopIcon(iconPage_3, 0x1, 124, 8000, false);
    addShopIcon(iconPage_3, 0x1, 125, 4000, false);
    addShopIcon(iconPage_3, 0x1, 126, 2000, false);
    addShopIcon(iconPage_3, 0x1, 127, 6000, false);
    addShopIcon(iconPage_3, 0x1, 128, 8000, false);
    addShopIcon(iconPage_3, 0x1, 129, 6000, false);
    addShopIcon(iconPage_3, 0x1, 130, 4000, false);
    addShopIcon(iconPage_3, 0x1, 131, 4000, false);
    addShopIcon(iconPage_3, 0x1, 132, 4000, false);
    addShopIcon(iconPage_3, 0x1, 133, 8000, false);
    addShopIcon(iconPage_3, 0x1, 134, 6000, false);
    addShopIcon(iconPage_3, 0x1, 135, 6000, false);
    //  COMMUNITY SHOP - SHIPS
    addShopIcon(iconPage_3, 0x4, 36, 2000, false);
    addShopIcon(iconPage_3, 0x4, 37, 2000, false);
    addShopIcon(iconPage_3, 0x4, 38, 6000, false);
    addShopIcon(iconPage_3, 0x4, 39, 2000, false);
    addShopIcon(iconPage_3, 0x4, 40, 4000, false);
    addShopIcon(iconPage_3, 0x4, 41, 4000, false);
    addShopIcon(iconPage_3, 0x4, 42, 6000, false);
    addShopIcon(iconPage_3, 0x4, 43, 2000, false);
    addShopIcon(iconPage_3, 0x4, 44, 6000, false);
    addShopIcon(iconPage_3, 0x4, 45, 8000, false);
    addShopIcon(iconPage_3, 0x4, 46, 4000, false);
    addShopIcon(iconPage_3, 0x4, 47, 6000, false);
    addShopIcon(iconPage_3, 0x4, 48, 4000, false);
    //  COMMUNITY SHOP - BALLS
    addShopIcon(iconPage_3, 0x5, 30, 4000, false);
    addShopIcon(iconPage_3, 0x5, 31, 6000, false);
    addShopIcon(iconPage_3, 0x5, 32, 6000, false);
    addShopIcon(iconPage_3, 0x5, 33, 2000, false);
    addShopIcon(iconPage_3, 0x5, 34, 4000, false);
    addShopIcon(iconPage_3, 0x5, 35, 8000, false);
    addShopIcon(iconPage_3, 0x5, 36, 6000, false);
    addShopIcon(iconPage_3, 0x5, 37, 2000, false);
    addShopIcon(iconPage_3, 0x5, 38, 2000, false);
    //  COMMUNITY SHOP - UFOS
    addShopIcon(iconPage_3, 0x6, 29, 6000, false);
    addShopIcon(iconPage_3, 0x6, 30, 6000, false);
    addShopIcon(iconPage_3, 0x6, 31, 4000, false);
    addShopIcon(iconPage_3, 0x6, 32, 6000, false);
    addShopIcon(iconPage_3, 0x6, 33, 4000, false);
    addShopIcon(iconPage_3, 0x6, 34, 2000, false);
    addShopIcon(iconPage_3, 0x6, 35, 2000, false);
    //  COMMUNITY SHOP - WAVES
    addShopIcon(iconPage_3, 0x7, 24, 2000, false);
    addShopIcon(iconPage_3, 0x7, 25, 4000, false);
    addShopIcon(iconPage_3, 0x7, 26, 2000, false);
    addShopIcon(iconPage_3, 0x7, 27, 2000, false);
    addShopIcon(iconPage_3, 0x7, 28, 6000, false);
    addShopIcon(iconPage_3, 0x7, 29, 6000, false);
    addShopIcon(iconPage_3, 0x7, 30, 4000, false);
    addShopIcon(iconPage_3, 0x7, 31, 6000, false);
    addShopIcon(iconPage_3, 0x7, 32, 2000, false);
    addShopIcon(iconPage_3, 0x7, 33, 6000, false);
    //  COMMUNITY SHOP - ROBOTS
    addShopIcon(iconPage_3, 0x8, 18, 2000, false);
    addShopIcon(iconPage_3, 0x8, 19, 6000, false);
    addShopIcon(iconPage_3, 0x8, 20, 2000, false);
    addShopIcon(iconPage_3, 0x8, 21, 4000, false);
    addShopIcon(iconPage_3, 0x8, 22, 6000, false);
    addShopIcon(iconPage_3, 0x8, 23, 2000, false);
    addShopIcon(iconPage_3, 0x8, 24, 6000, false);
    addShopIcon(iconPage_3, 0x8, 25, 4000, false);
    //  COMMUNITY SHOP - SPIDERS
    addShopIcon(iconPage_3, 0x9, 11, 4000, false);
    addShopIcon(iconPage_3, 0x9, 12, 6000, false);
    addShopIcon(iconPage_3, 0x9, 13, 4000, false);
    addShopIcon(iconPage_3, 0x9, 14, 8000, false);
    addShopIcon(iconPage_3, 0x9, 15, 2000, false);
    addShopIcon(iconPage_3, 0x9, 16, 6000, false);

    //  THE MECHANIC - CUBES
    addShopIcon(iconPage_4, 0x1, 168, 5000, false);
    addShopIcon(iconPage_4, 0x1, 185, 6000, false);
    addShopIcon(iconPage_4, 0x1, 190, 5000, false);
    addShopIcon(iconPage_4, 0x1, 230, 6000, false);
    addShopIcon(iconPage_4, 0x1, 253, 7000, false);
    addShopIcon(iconPage_4, 0x1, 358, 6000, false);
    addShopIcon(iconPage_4, 0x1, 360, 6000, false);
    addShopIcon(iconPage_4, 0x1, 480, 5000, false);
    //  THE MECHANIC - SHIPS
    addShopIcon(iconPage_4, 0x4, 80, 6000, false);
    addShopIcon(iconPage_4, 0x4, 111, 6000, false);
    addShopIcon(iconPage_4, 0x4, 126, 7000, false);
    addShopIcon(iconPage_4, 0x4, 144, 5000, false);
    addShopIcon(iconPage_4, 0x4, 164, 7000, false);
    //  THE MECHANIC - BALLS
    addShopIcon(iconPage_4, 0x5, 49, 6000, false);
    addShopIcon(iconPage_4, 0x5, 54, 5000, false);
    addShopIcon(iconPage_4, 0x5, 64, 5000, false);
    addShopIcon(iconPage_4, 0x5, 65, 5000, false);
    //  THE MECHANIC - UFOS
    addShopIcon(iconPage_4, 0x6, 86, 7000, false);
    addShopIcon(iconPage_4, 0x6, 95, 7000, false);
    addShopIcon(iconPage_4, 0x6, 107, 6000, false);
    addShopIcon(iconPage_4, 0x6, 132, 6000, false);
    addShopIcon(iconPage_4, 0x6, 136, 5000, false);
    //  THE MECHANIC - WAVES
    addShopIcon(iconPage_4, 0x7, 68, 5000, false);
    addShopIcon(iconPage_4, 0x7, 77, 7000, false);
    addShopIcon(iconPage_4, 0x7, 85, 6000, false);
    //  THE MECHANIC - ROBOTS
    addShopIcon(iconPage_4, 0x8, 52, 9000, false);
    addShopIcon(iconPage_4, 0x8, 68, 7000, false);
    //  THE MECHANIC - SPIDERS
    addShopIcon(iconPage_4, 0x9, 50, 6000, false);
    addShopIcon(iconPage_4, 0x9, 56, 6000, false);
    //  THE MECHANIC - SWINGS
    addShopIcon(iconPage_4, 0xD, 6, 6000, false);
    //  THE MECHANIC - EXTRAS
    addShopIcon(iconPage_4, 0xC, 18, 15000, false);
    addShopIcon(iconPage_4, 0xC, 19, 15000, false);
    addShopIcon(iconPage_4, 0xC, 20, 15000, false);
    addShopIcon(iconPage_4, 0xF, 2, 20000, false);
    addShopIcon(iconPage_4, 0xF, 3, 20000, false);
    addShopIcon(iconPage_4, 0xF, 4, 20000, false);
    addShopIcon(iconPage_4, 0xF, 5, 20000, false);
    addShopIcon(iconPage_4, 0xF, 6, 20000, false);
    addShopIcon(iconPage_4, 0xB, 19, 15000, false);
    addShopIcon(iconPage_4, 0xC, 16, 40000, false);

    //  DIAMOND SHOP - CUBES
    addShopIcon(iconPage_5, 0x1, 181, 300, true);
    addShopIcon(iconPage_5, 0x1, 186, 500, true);
    addShopIcon(iconPage_5, 0x1, 196, 400, true);
    addShopIcon(iconPage_5, 0x1, 203, 600, true);
    addShopIcon(iconPage_5, 0x1, 250, 300, true);
    addShopIcon(iconPage_5, 0x1, 263, 600, true);
    addShopIcon(iconPage_5, 0x1, 290, 400, true);
    addShopIcon(iconPage_5, 0x1, 314, 400, true);
    addShopIcon(iconPage_5, 0x1, 421, 500, true);
    addShopIcon(iconPage_5, 0x1, 443, 600, true);
    addShopIcon(iconPage_5, 0x1, 450, 500, true);
    addShopIcon(iconPage_5, 0x1, 456, 400, true);
    addShopIcon(iconPage_5, 0x1, 463, 400, true);
    addShopIcon(iconPage_5, 0x1, 464, 500, true);
    addShopIcon(iconPage_5, 0x1, 465, 400, true);
    addShopIcon(iconPage_5, 0x1, 471, 400, true);
    addShopIcon(iconPage_5, 0x1, 474, 300, true);
    addShopIcon(iconPage_5, 0x1, 478, 400, true);

    //  DIAMOND SHOP - SHIPS
    addShopIcon(iconPage_5, 0x4, 62, 500, true);
    addShopIcon(iconPage_5, 0x4, 63, 300, true);
    addShopIcon(iconPage_5, 0x4, 66, 500, true);
    addShopIcon(iconPage_5, 0x4, 81, 600, true);
    addShopIcon(iconPage_5, 0x4, 87, 400, true);
    addShopIcon(iconPage_5, 0x4, 125, 500, true);
    addShopIcon(iconPage_5, 0x4, 129, 400, true);
    addShopIcon(iconPage_5, 0x4, 131, 500, true);
    addShopIcon(iconPage_5, 0x4, 168, 600, true);

    //  DIAMOND SHOP - BALLS
    addShopIcon(iconPage_5, 0x5, 53, 600, true);
    addShopIcon(iconPage_5, 0x5, 55, 300, true);
    addShopIcon(iconPage_5, 0x5, 81, 500, true);
    addShopIcon(iconPage_5, 0x5, 98, 300, true);
    addShopIcon(iconPage_5, 0x5, 112, 500, true);
    addShopIcon(iconPage_5, 0x5, 113, 400, true);
    addShopIcon(iconPage_5, 0x5, 116, 500, true);

    //  DIAMOND SHOP - UFOS
    addShopIcon(iconPage_5, 0x6, 45, 500, true);
    addShopIcon(iconPage_5, 0x6, 52, 500, true);
    addShopIcon(iconPage_5, 0x6, 78, 300, true);
    addShopIcon(iconPage_5, 0x6, 80, 400, true);
    addShopIcon(iconPage_5, 0x6, 100, 600, true);
    addShopIcon(iconPage_5, 0x6, 140, 600, true);
    addShopIcon(iconPage_5, 0x6, 149, 600, true);
    addShopIcon(iconPage_5, 0x6, 141, 500, true);

    //  DIAMOND SHOP - WAVES
    addShopIcon(iconPage_5, 0x7, 56, 400, true);
    addShopIcon(iconPage_5, 0x7, 65, 300, true);
    addShopIcon(iconPage_5, 0x7, 75, 500, true);
    addShopIcon(iconPage_5, 0x7, 80, 300, true);
    addShopIcon(iconPage_5, 0x7, 82, 500, true);
    addShopIcon(iconPage_5, 0x7, 87, 400, true);

    //  DIAMOND SHOP - ROBOTS
    addShopIcon(iconPage_5, 0x8, 34, 500, true);
    addShopIcon(iconPage_5, 0x8, 49, 500, true);
    addShopIcon(iconPage_5, 0x8, 51, 500, true);
    addShopIcon(iconPage_5, 0x8, 62, 300, true);
    addShopIcon(iconPage_5, 0x8, 63, 600, true);

    //  DIAMOND SHOP - SPIDERS
    addShopIcon(iconPage_5, 0x9, 22, 400, true);
    addShopIcon(iconPage_5, 0x9, 29, 500, true);
    addShopIcon(iconPage_5, 0x9, 54, 300, true);
    addShopIcon(iconPage_5, 0x9, 60, 400, true);
    addShopIcon(iconPage_5, 0x9, 64, 500, true);
    addShopIcon(iconPage_5, 0x9, 65, 300, true);
    addShopIcon(iconPage_5, 0x9, 66, 500, true);

    //  DIAMOND SHOP - SWINGS
    addShopIcon(iconPage_5, 0xD, 10, 500, true);
    addShopIcon(iconPage_5, 0xD, 30, 500, true);
    addShopIcon(iconPage_5, 0xD, 40, 600, true);
    addShopIcon(iconPage_5, 0xD, 41, 400, true);
    addShopIcon(iconPage_5, 0xD, 42, 500, true);

    //  DIAMOND SHOP - JETPACKS
    addShopIcon(iconPage_5, 0xE, 5, 500, true);

    //  DIAMOND SHOP - EXTRAS
    addShopIcon(iconPage_5, 0x2, 64, 100, true);
    addShopIcon(iconPage_5, 0x2, 71, 100, true);
    addShopIcon(iconPage_5, 0x2, 77, 100, true);
    addShopIcon(iconPage_5, 0x2, 98, 100, true);
    addShopIcon(iconPage_5, 0x2, 100, 100, true);
    addShopIcon(iconPage_5, 0x3, 45, 100, true);
    addShopIcon(iconPage_5, 0x3, 59, 100, true);
    addShopIcon(iconPage_5, 0x3, 72, 100, true);
    addShopIcon(iconPage_5, 0x3, 74, 100, true);
    addShopIcon(iconPage_5, 0x3, 77, 100, true);
    addShopIcon(iconPage_5, 0x3, 89, 100, true);
    addShopIcon(iconPage_5, 0x3, 106, 100, true);
    addShopIcon(iconPage_5, 0xB, 18, 1500, true);
    addShopIcon(iconPage_5, 0xC, 17, 2000, true);

    //  MLUtils::addIcons(iconPage_1, menu_selector(ShopListAlert::onIconButton), 0x1, 73);
 
    //	Adds all of the Icon Pages into the main menu.
	this->addChild(iconPage_1);
    this->addChild(iconPage_2);
    this->addChild(iconPage_3);
    this->addChild(iconPage_4);
    this->addChild(iconPage_5);

    this->m_noElasticity = true;
    return true;
};

void ShopListAlert::createShopButton(CCMenu* menu, int tag, bool isActive){
    auto baseColor = (isActive) ? IconSelectBaseColor::Selected : IconSelectBaseColor::Unselected;

    auto sprName = (tag == 1) ? "shopKeeper_1.png"_spr :
                (tag == 2) ? "shopKeeper_2.png"_spr :
                (tag == 3) ? "shopKeeper_3.png"_spr :
                (tag == 4) ? "shopKeeper_4.png"_spr :
                "shopKeeper_5.png"_spr;

    auto button = CCMenuItemSpriteExtra::create(
        IconSelectButtonSprite::createWithSprite(sprName, 1.25F, baseColor),
        this,
        menu_selector(ShopListAlert::onShopButton)
    );

    button->setID("shop-button-" + std::to_string(tag));
    button->setTag(tag);

    menu->addChild(button);
    menu->updateLayout();
};

void ShopListAlert::onShopButton(CCObject * sender){
    auto menu = this->getChildByID("shop-menu");
    auto btnMenu = static_cast<CCMenu*>(menu);
    auto tag = sender->getTag();

    auto type = (tag == 1) ? "The Shop" : (tag == 2) ? "Scratch's Shop" : (tag == 3) ? "Community Shop" : (tag == 4) ? "The Mechanic" : "Diamond Shop";
    this->setTitle(type);

    menu->removeAllChildren();
    menu->updateLayout();

    for(int ii = 1; ii <= 5; ii++){
        this->getChildByID("icon-page-" + std::to_string(ii))->setVisible(tag == ii);
        createShopButton(btnMenu, ii, tag == ii);
    };
};

void ShopListAlert::addShopIcon(CCMenu* menu, int type, int iconID, int price, bool isDiamondShop){
    auto noCheckmark = Mod::get()->getSettingValue<bool>("disable-checkmark");
    auto gsm = GameStatsManager::sharedState();
    UnlockType iconType{type};

    auto iconSpr = GJItemIcon::createBrowserItem(
        iconType,
        iconID
    );

    if(gsm->isItemUnlocked(iconType, iconID) && !noCheckmark){
        auto checkmark = CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png");
        checkmark->setPosition(iconSpr->getContentSize() / 2);
		checkmark->setScale(0.75f);

        iconSpr->darkenStoreItem({ 30, 30, 30 });
		iconSpr->addChild(checkmark);
    } else {
        if(iconType == UnlockType::Col1 || iconType == UnlockType::Col2){
            auto colorType = (iconType == UnlockType::Col1) ? CCLabelBMFont::create("1", "bigFont.fnt") : CCLabelBMFont::create("2", "bigFont.fnt");
            colorType->setPosition(iconSpr->getContentSize() / 2);
            colorType->setScale(0.5f);

            iconSpr->addChild(colorType);
        }
    
        auto value = CCLabelBMFont::create(std::to_string(price).c_str(), "bigFont.fnt");
        value->setPosition({iconSpr->getContentSize().width / 2, iconSpr->getContentSize().height / 2 - 22.5f});
		value->setScale(0.4f);

		iconSpr->addChild(value);
    };

    //  Creates the Icon as a button itself.
    auto iconButton = CCMenuItemSpriteExtra::create(
        iconSpr,
        this,
        nullptr //  menu_selector(ShopListAlert::onIconButton)
    );

    //	Passes the user-defined IconParameters for its callback
    iconButton->setUserObject(new IconParameters(iconType, iconID));

    menu->addChild(iconButton);
    menu->updateLayout();
};

//	When a specific Icon Button is pressed
void ShopListAlert::onIconButton(CCObject *sender){
    auto parameters = static_cast<IconParameters *>(
        static_cast<CCNode *>(sender)->getUserObject());

    ItemInfoPopup::create(parameters->p_iconId, parameters->p_iconType)->show();
};

//  Function to add Icons (Base of the variadic function)
template <typename T>
void ShopListAlert::addIcons(CCMenu* menu, int type, const T &iconID){
    auto noCheckmark = Mod::get()->getSettingValue<bool>("disable-checkmark");
    auto gm = GameStatsManager::sharedState();
    UnlockType iconType{type};

    auto iconSpr = GJItemIcon::createBrowserItem(
        iconType,
        iconID
	);

    if(gm->isItemUnlocked(iconType, iconID) && !noCheckmark){
        auto checkmark = CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png");
        checkmark->setPosition(iconSpr->getContentSize() / 2);
		checkmark->setScale(0.75f);

        iconSpr->darkenStoreItem({ 30, 30, 30 });
		iconSpr->addChild(checkmark);
    } else if(iconType == UnlockType::Col1 || iconType == UnlockType::Col2){
        auto value = (iconType == UnlockType::Col1) ? CCLabelBMFont::create("1", "bigFont.fnt") : CCLabelBMFont::create("2", "bigFont.fnt");
        value->setPosition(iconSpr->getContentSize() / 2);
		value->setScale(0.5f);

		iconSpr->addChild(value);
    };

    //  Creates the Icon as a button itself.
    auto iconButton = CCMenuItemSpriteExtra::create(
        iconSpr,
        this,
        menu_selector(ShopListAlert::onIconButton)
    );

    //	Passes the user-defined IconParameters for its callback
    iconButton->setUserObject(new IconParameters(iconType, iconID));

    menu->addChild(iconButton);
    menu->updateLayout();
};

//  Function to add Icons (Recursive part of the variadic function)
template <typename T, typename... ARGS>
void ShopListAlert::addIcons(CCMenu* menu, int type, const T &iconID, const ARGS &...args){
    auto noCheckmark = Mod::get()->getSettingValue<bool>("disable-checkmark");
    auto gm = GameStatsManager::sharedState();
    UnlockType iconType{type};

    auto iconSpr = GJItemIcon::createBrowserItem(
        iconType,
        iconID
	);

    if(gm->isItemUnlocked(iconType, iconID) && !noCheckmark){
        auto checkmark = CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png");
        checkmark->setPosition(iconSpr->getContentSize() / 2);
		checkmark->setScale(0.75f);

        iconSpr->darkenStoreItem({ 30, 30, 30 });
		iconSpr->addChild(checkmark);
    } else if(iconType == UnlockType::Col1 || iconType == UnlockType::Col2){
        auto value = (iconType == UnlockType::Col1) ? CCLabelBMFont::create("1", "bigFont.fnt") : CCLabelBMFont::create("2", "bigFont.fnt");
        value->setPosition(iconSpr->getContentSize() / 2);
		value->setScale(0.5f);

		iconSpr->addChild(value);
    };

    //  Creates the Icon as a button itself.
    auto iconButton = CCMenuItemSpriteExtra::create(
        iconSpr,
        this,
        menu_selector(ShopListAlert::onIconButton)
    );

    //	Passes the user-defined IconParameters for its callback
    iconButton->setUserObject(new IconParameters(iconType, iconID));

    menu->addChild(iconButton);
    menu->updateLayout();

    addIcons(menu, type, args...);
};

ShopListAlert * ShopListAlert::create() {
    auto ret = new ShopListAlert();

    if (ret && ret->init(420.f, 260.f)){
        ret->autorelease();
        return ret;
    };

    CC_SAFE_DELETE(ret);
    return nullptr;
};