#include "ShopListAlert.hpp"

//  Structure for Icon Parameters (to create the Info pop-up when clicked).
struct IconParameters : public CCObject{
    UnlockType p_iconType;
    bool p_isSelected;
    int p_iconPrice;
    int p_iconId;

    IconParameters(UnlockType icon, int id, int price, bool isSelected) : p_iconType(icon), p_iconId(id), p_iconPrice(price), p_isSelected(isSelected){
        this->autorelease();
    }
};

//  Mod's main setup
bool ShoppingListAlert::setup(){
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    //  Background for the Icon Lists
    auto iconListBG = CCScale9Sprite::create("square02b_001.png", {0, 0, 80, 80});
    iconListBG->setPosition({winSize.width / 2.f, winSize.height / 2.f + 2.f});
    iconListBG->setColor({ 133, 68, 41 });
    iconListBG->setContentSize({430, 176});
	this->addChild(iconListBG);

    //  Info Button (Menu)
    auto infoMenu = CCMenu::create();
    infoMenu->setContentSize({405.f, 215.f});
    infoMenu->ignoreAnchorPointForPosition(false);
    infoMenu->setID("info-menu");

    //  Info Button (Itself)
    auto infoButton = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png"),
        this,
        nullptr
    );

    infoButton->setPosition(infoMenu->getContentSize());
    infoButton->setID("info-button");
    infoMenu->addChild(infoButton);
    this->addChild(infoMenu);

    /*  WORK IN PROGRESS
    //  Select Mode (Menu)
    auto selectMenu = CCMenu::create();
    selectMenu->setContentSize({405.f, 215.f});
    selectMenu->ignoreAnchorPointForPosition(false);
    selectMenu->setID("select-menu");

    auto checkButton = CCMenuItemToggler::createWithStandardSprites(
        this,
        menu_selector(ShoppingListAlert::onSelectButton),
        0.8f
    );

    checkButton->setID("select-button");
    selectMenu->addChild(checkButton);
    this->addChild(selectMenu);

    auto pricing = CCLabelBMFont::create(std::to_string(m_totalPrice).c_str(), "bigFont.fnt");
    pricing->setPosition({winSize.width / 2, winSize.height / 2});
    pricing->setID("price");
    this->addChild(pricing);
    */

    //  Navigation Menu
    auto navMenu = CCMenu::create();
    navMenu->setPositionY(winSize.height / 2 - 104.f);
    navMenu->setLayout(RowLayout::create()
            ->setGap(8.f)
    );
    navMenu->setID("navigation-menu");
    this->addChild(navMenu);

    //  Navigation Menu (Buttons)
    for(int ii = 1; ii <= 5; ii++){
        createNavButton(navMenu, ii, ii == 1);
    };

    //  Arrow Buttons for shops with more than One page
    auto pageNavMenu = CCMenu::create();
    pageNavMenu->setLayout(RowLayout::create()
        ->setGap(480.0f)
        ->setGrowCrossAxis(true)
        ->setCrossAxisOverflow(false)
    );
    pageNavMenu->setID("page-menu");
    this->addChild(pageNavMenu);

    auto prevBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
        this,
        menu_selector(ShoppingListAlert::onPageButton)
    );
    prevBtn->setID("arrow-prev-button");
    prevBtn->setTag(1);

    auto nextSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    nextSpr->setFlipX(true);

    auto nextBtn = CCMenuItemSpriteExtra::create(
        nextSpr,
        this,
        menu_selector(ShoppingListAlert::onPageButton)
    );
    nextBtn->setID("arrow-next-button");
    nextBtn->setScale(-1);
    nextBtn->setTag(2);

    pageNavMenu->addChild(prevBtn);
    pageNavMenu->addChild(nextBtn);
    //  pageNavMenu->setVisible(false);
    pageNavMenu->updateLayout();
    prevBtn->setVisible(false);

    //  Creating Menus
    CCSize defaultSize = {540.0f, 220.0f};
    auto iconPage_1_1 = createPage(defaultSize, 1, 1, true);
    auto iconPage_1_2 = createPage(defaultSize, 1, 2, false);
    auto iconPage_2_1 = createPage(defaultSize, 2, 1, false);
    auto iconPage_2_2 = createPage(defaultSize, 2, 2, false);
    auto iconPage_3_1 = createPage(defaultSize, 3, 1, false);
    auto iconPage_3_2 = createPage(defaultSize, 3, 2, false);
    auto iconPage_4_1 = createPage(defaultSize, 4, 1, false);
    auto iconPage_4_2 = createPage(defaultSize, 4, 2, false);
    auto iconPage_5_1 = createPage(defaultSize, 5, 1, false);
    auto iconPage_5_2 = createPage(defaultSize, 5, 2, false);

    // THE SHOP (Icons)
    addItem(iconPage_1_1, 0x1, {{73, 1000}, {77, 1000}, {79, 500}, {86, 1000}, {102, 1000}, {107, 1000}, {109, 4000}, {113, 4000}, {140, 3000}, {162, 3000}, {191, 3000}}, false);
    addItem(iconPage_1_1, 0x4, {{27, 2000}}, false);
    addItem(iconPage_1_1, 0x5, {{19, 1500}, {20, 1000}, {40, 4000}, {57, 3000}}, false);
    addItem(iconPage_1_1, 0x6, {{23, 2500}, {25, 2000}, {40, 3500}}, false);
    addItem(iconPage_1_1, 0x7, {{21, 500}, {35, 4000}, {38, 5000}}, false);
    addItem(iconPage_1_1, 0x8, {{12, 3000}, {33, 3500}}, false);
    addItem(iconPage_1_1, 0x9, {{2, 2000}, {18, 4000}}, false);
    addItem(iconPage_1_1, 0xD, {{7, 5000}}, false);

    //  THE SHOP (Extras)
    addItem(iconPage_1_2, 0x2, {{33, 1000}, {35, 1000}, {36, 1000}, {39, 1000}, {84, 1500}, {97, 2500}, {99, 1500}}, false);
    addItem(iconPage_1_2, 0x3, {{28, 1000}, {39, 1000}, {49, 2500}, {88, 2000}, {104, 2000}}, false);
    addItem(iconPage_1_2, 0xB, {{8, 7000}, {11, 7000}}, false);

    //  SECRET SHOP (Icons)
    addItem(iconPage_2_1, 0x1, {{85, 2500}, {94, 3000}, {96, 2000}, {110, 5000}, {136, 5000}, {206, 2000}, {225, 3000}, {229, 5000}}, false);
    addItem(iconPage_2_1, 0x4, {{58, 4000}, {138, 4000}}, false);
    addItem(iconPage_2_1, 0x5, {{25, 2000}, {60, 2500}, {107, 3000}}, false);
    addItem(iconPage_2_1, 0x6, {{21, 4000}, {83, 3000}, {104, 2500}}, false);
    addItem(iconPage_2_1, 0x7, {{18, 2000}, {78, 2500}}, false);
    addItem(iconPage_2_1, 0x8, {{14, 3000}, {45, 3500}}, false);
    addItem(iconPage_2_1, 0x9, {{3, 3000}, {58, 3500}, {69, 5000}}, false);
    addItem(iconPage_2_1, 0xD, {{12, 6000}, {27, 6000}}, false);

    //  SECRET SHOP (Extras)
    addItem(iconPage_2_2, 0x2, {{39, 1000}, {40, 1000}, {49, 1000}, {94, 1000}}, false);
    addItem(iconPage_2_2, 0x3, {{32, 1000}, {41, 1000}, {85, 1500}, {86, 1000}, {70, 1000}}, false);
    addItem(iconPage_2_2, 0xA, {{7, 4000}}, false);
    addItem(iconPage_2_2, 0xB, {{13, 10000}, {16, 15000}}, false);
    addItem(iconPage_2_2, 0xC, {{4, 1000}}, false);

    //  COMMUNITY SHOP (Icons)
    addItem(iconPage_3_1, 0x1, {{117, 4000}, {118, 2000}, {119, 2000}, {120, 4000}, {121, 2000}, {122, 2000}, {123, 6000}, {124, 8000}, {125, 4000}, {126, 2000}, {127, 6000}, {128, 8000}, {129, 6000}, {130, 4000}, {131, 4000}, {132, 4000}, {133, 8000}, {134, 6000}, {135, 6000}}, false);
    addItem(iconPage_3_1, 0x4, {{36, 2000}, {37, 2000}, {38, 6000}, {39, 2000}, {40, 4000}, {41, 4000}, {42, 6000}, {43, 2000}, {44, 6000}, {45, 8000}, {46, 4000}, {47, 6000}, {48, 4000}}, false);
    addItem(iconPage_3_1, 0x5, {{30, 4000}, {31, 6000}, {32, 6000}, {33, 2000}, {34, 4000}, {35, 8000}, {36, 6000}, {37, 2000}}, false);

    addItem(iconPage_3_2, 0x5, {{38, 2000}}, false);
    addItem(iconPage_3_2, 0x6, {{29, 6000}, {30, 6000}, {31, 4000}, {32, 6000}, {33, 4000}, {34, 2000}, {35, 2000}}, false);
    addItem(iconPage_3_2, 0x7, {{24, 2000}, {25, 4000}, {26, 2000}, {27, 2000}, {28, 6000}, {29, 6000}, {30, 4000}, {31, 6000}, {32, 2000}, {33, 6000}}, false);
    addItem(iconPage_3_2, 0x8, {{18, 2000}, {19, 6000}, {20, 2000}, {21, 4000}, {22, 6000}, {23, 2000}, {24, 6000}, {25, 4000}}, false);
    addItem(iconPage_3_2, 0x9, {{11, 4000}, {12, 6000}, {13, 4000}, {14, 8000}, {15, 2000}, {16, 6000}}, false);

    //  THE MECHANIC (Icons)
    addItem(iconPage_4_1, 0x1, {{168, 5000}, {185, 6000}, {190, 5000}, {230, 6000}, {253, 7000}, {358, 6000}, {360, 6000}, {480, 5000}}, false);
    addItem(iconPage_4_1, 0x4, {{80, 6000}, {111, 6000}, {126, 7000}, {144, 5000}, {164, 7000}}, false);
    addItem(iconPage_4_1, 0x5, {{49, 6000}, {54, 5000}, {64, 5000}, {65, 5000}}, false);
    addItem(iconPage_4_1, 0x6, {{86, 7000}, {95, 7000}, {107, 6000}, {132, 6000}, {136, 5000}}, false);
    addItem(iconPage_4_1, 0x7, {{68, 5000}, {77, 7000}, {85, 6000}}, false);
    addItem(iconPage_4_1, 0x8, {{52, 9000}, {68, 7000}}, false);
    addItem(iconPage_4_1, 0x9, {{50, 6000}, {56, 6000}}, false);
    addItem(iconPage_4_1, 0xD, {{6, 6000}}, false);

    //  THE MECHANIC (Extras)
    addItem(iconPage_4_2, 0xC, {{18, 15000}, {19, 15000}, {20, 15000}}, false);
    addItem(iconPage_4_2, 0xF, {{2, 20000}, {3, 20000}, {4, 20000}, {5, 20000}, {6, 20000}}, false);
    addItem(iconPage_4_2, 0xB, {{19, 15000}}, false);
    addItem(iconPage_4_2, 0xC, {{16, 40000}}, false);

    //  DIAMOND SHOP (Icons)
    addItem(iconPage_5_1, 0x1, {{181, 300}, {186, 500}, {196, 400}, {203, 600}, {250, 300}, {263, 600}, {290, 400}, {314, 400}, {421, 500}, {443, 600}, {450, 500}, {456, 400}, {463, 400}, {464, 500}, {465, 400}, {471, 400}, {474, 300}, {478, 400}}, true);
    addItem(iconPage_5_1, 0x4, {{62, 500}, {63, 300}, {66, 500}, {81, 600}, {87, 400}, {125, 500}, {129, 400}, {131, 500}, {168, 600}}, true);
    addItem(iconPage_5_1, 0x5, {{53, 600}, {55, 300}, {81, 500}, {98, 300}, {112, 500}, {113, 400}, {116, 500}}, true);
    addItem(iconPage_5_1, 0x6, {{45, 500}, {52, 500}, {78, 300}, {80, 400}, {100, 600}, {140, 600}}, true);

    addItem(iconPage_5_2, 0x6, {{149, 600}, {141, 500}}, true);
    addItem(iconPage_5_2, 0x7, {{56, 400}, {65, 300}, {75, 500}, {80, 300}, {82, 500}, {87, 400}}, true);
    addItem(iconPage_5_2, 0x8, {{34, 500}, {49, 500}, {51, 500}, {62, 300}, {63, 600}}, true);
    addItem(iconPage_5_2, 0x9, {{22, 400}, {29, 500}, {54, 300}, {60, 400}, {64, 500}, {65, 300}, {66, 500}}, true);
    addItem(iconPage_5_2, 0xD, {{10, 500}, {30, 500}, {40, 600}, {41, 400}, {42, 500}}, true);
    addItem(iconPage_5_2, 0xE, {{5, 500}}, true);

    //  DIAMOND SHOP (Extras)
    addItem(iconPage_5_2, 0x2, {{64, 100}, {71, 100}, {77, 100}, {98, 100}, {100, 100}}, true);
    addItem(iconPage_5_2, 0x3, {{45, 100}, {59, 100}, {72, 100}, {74, 100}, {77, 100}, {89, 100}, {106, 100}}, true);
    addItem(iconPage_5_2, 0xB, {{18, 1500}}, true);
    addItem(iconPage_5_2, 0xC, {{17, 2000}}, true);

    //	Adds all pages into Menu
	this->addChild(iconPage_1_1);
    this->addChild(iconPage_1_2);
    this->addChild(iconPage_2_1);
    this->addChild(iconPage_2_2);
    this->addChild(iconPage_3_1);
    this->addChild(iconPage_3_2);
    this->addChild(iconPage_4_1);
    this->addChild(iconPage_4_2);
    this->addChild(iconPage_5_1);
    this->addChild(iconPage_5_2);

    this->m_noElasticity = true;
    this->setTitle("The Shop");
    return true;
};

//  Creates a menu where all Icons will be saved on
CCMenu * ShoppingListAlert::createPage(CCSize size, int ID, int index, bool isVisible){
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto menu = CCMenu::create();

    menu->setContentSize(size);
    menu->setLayout(RowLayout::create()
        ->setGap(24.0f)
        ->setAutoScale(false)
        ->setGrowCrossAxis(true)
        ->setCrossAxisOverflow(false)
        ->setCrossAxisLineAlignment(AxisAlignment::Even)
    );

    menu->setID("icon-page-" + std::to_string(ID) + "-" + std::to_string(index));
    menu->setPositionY(winSize.height / 2 + 8.f);
    menu->setVisible(isVisible);
    menu->setScale(0.8f);

    return menu;
};

//  Creates the Navigation Button
void ShoppingListAlert::createNavButton(CCMenu * menu, int tag, bool isActive){
    //  Sprite based on the tag of the button
    auto sprName = (tag == 1) ? "shopKeeper_1.png"_spr :
                (tag == 2) ? "shopKeeper_2.png"_spr :
                (tag == 3) ? "shopKeeper_3.png"_spr :
                (tag == 4) ? "shopKeeper_4.png"_spr :
                "shopKeeper_5.png"_spr;

    //  Base color based on whenever the current page is on
    auto baseColor = (isActive) ? IconSelectBaseColor::Selected : IconSelectBaseColor::Unselected;

    //  Button
    auto button = CCMenuItemSpriteExtra::create(
        IconSelectButtonSprite::createWithSprite(sprName, 1.25F, baseColor),
        this,
        menu_selector(ShoppingListAlert::onNavButton)
    );

    //  Adds ID and Tag
    button->setID("shop-button-" + std::to_string(tag));
    button->setTag(tag);

    //  Adds button to menu and updates layout.
    menu->addChild(button);
    menu->updateLayout();
};

//  When a Navigation button was pressed
void ShoppingListAlert::onNavButton(CCObject * sender){
    auto menu = this->getChildByID("navigation-menu");
    auto navMenu = static_cast<CCMenu*>(menu);
    auto tag = sender->getTag();
    m_currentPage = tag;

    auto shopTitle = (tag == 1) ? "The Shop" : 
            (tag == 2) ? "Scratch's Shop" :
            (tag == 3) ? "Community Shop" :
            (tag == 4) ? "The Mechanic" :
            "Diamond Shop";
    this->setTitle(shopTitle);
    
    menu->removeAllChildren();
    menu->updateLayout();

    for(int ii = 1; ii <= 5; ii++){
        createNavButton(navMenu, ii, tag == ii);

        auto page_1 = this->getChildByID("icon-page-" + std::to_string(ii) + "-1");
        auto page_2 = this->getChildByID("icon-page-" + std::to_string(ii) + "-2");

        if(page_2) page_2->setVisible(false);
        page_1->setVisible(tag == ii);
    };

    auto pageMenu = this->getChildByID("page-menu");
    auto prevArrow = pageMenu->getChildByID("arrow-prev-button");
    auto nextArrow = pageMenu->getChildByID("arrow-next-button");

    pageMenu->setVisible(true);
    prevArrow->setVisible(false);
    nextArrow->setVisible(true);
};

//  When the Arrow buttons are pressed
void ShoppingListAlert::onPageButton(CCObject * sender){
    auto tag = sender->getTag();

    //  log::info("Page Button Called, value of m_current {}", m_currentPage);

    auto pageMenu = this->getChildByID("page-menu");
    auto prevArrow = pageMenu->getChildByID("arrow-prev-button");
    auto nextArrow = pageMenu->getChildByID("arrow-next-button");

    //  if(m_currentPage == 3 || m_currentPage == 5){
        //  log::info("The condition is True");
        this->getChildByID("icon-page-" + std::to_string(m_currentPage) + "-1")->setVisible(tag == 1);
        this->getChildByID("icon-page-" + std::to_string(m_currentPage) + "-2")->setVisible(tag == 2);

        nextArrow->setVisible(tag == 1);
        prevArrow->setVisible(tag == 2);
    //  }
};

/*  WORK IN PROGRESS
void ShoppingListAlert::onSelectButton(CCObject * sender){
    CCMenuItemToggler * toggler = static_cast<CCMenuItemToggler *>(sender);

    m_selectMode = !toggler->isToggled();
};
*/

void ShoppingListAlert::addItem(CCMenu* menu, int type, std::map<int, int> icons, bool isDiamondShop){
    for (auto const& [iconID, price] : icons){
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
            menu_selector(ShoppingListAlert::onIcon)
        );

        //	Passes the user-defined IconParameters for its callback
        iconButton->setUserObject(new IconParameters(iconType, iconID, price, false));

        menu->addChild(iconButton);
        menu->updateLayout();
    }
};

//	When a specific Icon Button is pressed
void ShoppingListAlert::onIcon(CCObject *sender){
    auto parameters = static_cast<IconParameters *>(
        static_cast<CCNode *>(sender)->getUserObject());

    /*
    if(m_selectMode){
        log::info("Icon Flag: {}", parameters->p_isSelected);

        if(parameters->p_isSelected){
            m_totalPrice -= parameters->p_iconPrice;
        } else {
            m_totalPrice += parameters->p_iconPrice;
        }

        parameters->p_isSelected = !parameters->p_isSelected;

        auto price = static_cast<CCLabelBMFont *>(this->getChildByID("price"));
        price->setString(std::to_string(m_totalPrice).c_str());
    } else {
        ItemInfoPopup::create(parameters->p_iconId, parameters->p_iconType)->show();
    }
    */

    ItemInfoPopup::create(parameters->p_iconId, parameters->p_iconType)->show();
};

ShoppingListAlert * ShoppingListAlert::create() {
    auto ret = new ShoppingListAlert();

    if (ret && ret->init(450.f, 260.f)){
        ret->autorelease();
        return ret;
    };

    CC_SAFE_DELETE(ret);
    return nullptr;
};