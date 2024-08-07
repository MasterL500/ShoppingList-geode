#include "ShopListAlert.hpp"
#include "ShopData.hpp"

//  Structure for Icons
struct IconParameters : public CCObject
{
    bool p_selected;
    int p_iconPrice;
    int p_iconType;
    int p_iconID;
    int p_shopID;

    IconParameters(int type, int id, int price, int shop, bool selected) : p_iconType(type), p_iconID(id), p_iconPrice(price), p_shopID(shop), p_selected(selected) {
        this->autorelease();
    }
};

bool ShoppingListAlert::setup()
{
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    loadData();

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
        menu_selector(ShoppingListAlert::onInfoButton)
    );

    infoButton->setPosition(infoMenu->getContentSize());
    infoButton->setID("info-button");
    infoMenu->addChild(infoButton);
    this->addChild(infoMenu);

    //  Navigation Menu
    auto navMenu = CCMenu::create();
    navMenu->setPosition({winSize.width / 2 + 110.0f, winSize.height / 2 - 104.0f});
    navMenu->setLayout(RowLayout::create()
            ->setGap(6.f));
    navMenu->setID("navigation-menu");
    this->addChild(navMenu);

    //  Navigation Menu (Buttons)
    for (int ii = 1; ii <= 5; ii++){
        createNavButton(navMenu, ii, ii == 1);
    };

    //  Arrow Buttons for shops with more than One page
    auto pageNavMenu = CCMenu::create();
    pageNavMenu->setLayout(RowLayout::create()
                ->setGap(460.0f)
                ->setGrowCrossAxis(true)
                ->setCrossAxisOverflow(false));
    pageNavMenu->setID("page-menu");
    this->addChild(pageNavMenu);

    auto prevBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
        this,
        menu_selector(ShoppingListAlert::onPageButton));
    prevBtn->setID("arrow-prev-button");
    prevBtn->setTag(1);

    auto nextSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    nextSpr->setFlipX(true);

    auto nextBtn = CCMenuItemSpriteExtra::create(
        nextSpr,
        this,
        menu_selector(ShoppingListAlert::onPageButton));
    nextBtn->setID("arrow-next-button");
    nextBtn->setScale(-1);
    nextBtn->setTag(2);

    pageNavMenu->addChild(prevBtn);
    pageNavMenu->addChild(nextBtn);
    //  pageNavMenu->setVisible(false);
    pageNavMenu->updateLayout();
    prevBtn->setVisible(false);

    //  Creates the Menu
    auto iconMenu = CCMenu::create();
    iconMenu->setContentSize({540.0f, 220.0f});
    iconMenu->setLayout(RowLayout::create()
            ->setGap(24.0f)
            ->setAutoScale(false)
            ->setGrowCrossAxis(true)
            ->setCrossAxisOverflow(false)
            ->setCrossAxisLineAlignment(AxisAlignment::Even));
    iconMenu->setID("icon-menu");
    iconMenu->setPositionY(winSize.height / 2 + 8.f);
    iconMenu->setScale(0.8f);
    this->addChild(iconMenu);

    createIconPage(1, 1);

    //  Select Mode (Menu)
    auto selectMenu = CCMenu::create();
    selectMenu->setContentSize({400.f, 212.f});
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

    //  Orbs Pricing (When Select Mode is enabled)
    auto orbsPrice = CCLabelBMFont::create(std::to_string(m_totalManaOrbs).c_str(), "bigFont.fnt");
    orbsPrice->setPosition({winSize.width / 2 - 160.0f, winSize.height / 2 - 104.0f});
    orbsPrice->setAnchorPoint({0.0f, 0.5f});
    orbsPrice->setID("orbs-label");
    orbsPrice->setScale(0.55f);
    this->addChild(orbsPrice);

    auto orbsIcon = CCSprite::createWithSpriteFrameName("currencyOrbIcon_001.png");
    orbsIcon->setPositionY(winSize.height / 2 - 105.0f);
    orbsIcon->setPositionX(orbsPrice->getPositionX() - 12.0f);
    orbsIcon->setID("orbs-icon");
    this->addChild(orbsIcon);

    auto diamondsPrice = CCLabelBMFont::create(std::to_string(m_totalDiamonds).c_str(), "bigFont.fnt");
    diamondsPrice->setPosition({winSize.width / 2 - 160.0f + orbsPrice->getContentWidth() + 25.0f, winSize.height / 2 - 104.0f});
    diamondsPrice->setAnchorPoint({0.0f, 0.5f});
    diamondsPrice->setID("diamonds-label");
    diamondsPrice->setScale(0.55f);
    this->addChild(diamondsPrice);

    auto diamondsIcon = CCSprite::createWithSpriteFrameName("currencyDiamondIcon_001.png");
    diamondsIcon->setPositionY(winSize.height / 2 - 105.0f);
    diamondsIcon->setPositionX(diamondsPrice->getPositionX() - 15.0f);
    diamondsIcon->setID("diamonds-icon");
    this->addChild(diamondsIcon);

    diamondsPrice->setVisible(false);
    diamondsIcon->setVisible(false);
    orbsPrice->setVisible(false);
    orbsIcon->setVisible(false);

    /*
    auto selectAllSpr = ButtonSprite::create("Select All", 100, false, "bigFont.fnt", "GJ_button_04.png", 24, 0.25f);
    auto selectAllBtn = CCMenuItemSpriteExtra::create(
        selectAllSpr,
        this,
        nullptr
    );

    selectMenu->addChild(selectAllBtn);
    */

    this->m_noElasticity = true;
    this->setTitle("The Shop");
    return true;
};

void ShoppingListAlert::loadData(){
    auto gsm = GameStatsManager::sharedState();
    //  log::info("Function called");
    for(auto ii = SHOPS.begin(); ii != SHOPS.end(); ii++){
        //  log::info("hello {}", ii->first);
        for(auto jj = ii->second.begin(); jj != ii->second.end(); jj++){
            //  log::info("-- hai {}", jj->first);
            for(auto &kk: jj->second){
                //  log::info("-- greetings {}", kk);
                if (gsm->isItemUnlocked(UnlockType{jj->first}, kk)){
                    m_itemCount[ii->first]++;
                };
            }
        }
    };
};

void ShoppingListAlert::createIconPage(int ID, int index)
{
    auto menu = this->getChildByID("icon-menu");
    auto iconMenu = static_cast<CCMenu *>(menu);

    iconMenu->removeAllChildren();
    iconMenu->updateLayout();
    iconMenu->setTag(ID);

    switch (ID){
    case 1:
        if (index == 1)
        {
            // THE SHOP (Icons)
            createItem(iconMenu, 0x1, {{73, 1000}, {77, 1000}, {79, 500}, {86, 1000}, {102, 1000}, {107, 1000}, {109, 4000}, {113, 4000}, {140, 3000}, {162, 3000}, {191, 3000}}, false);
            createItem(iconMenu, 0x4, {{27, 2000}}, false);
            createItem(iconMenu, 0x5, {{19, 1500}, {20, 1000}, {40, 4000}, {57, 3000}}, false);
            createItem(iconMenu, 0x6, {{23, 2500}, {25, 2000}, {40, 3500}}, false);
            createItem(iconMenu, 0x7, {{21, 500}, {35, 4000}, {38, 5000}}, false);
            createItem(iconMenu, 0x8, {{12, 3000}, {33, 3500}}, false);
            createItem(iconMenu, 0x9, {{2, 2000}, {18, 4000}}, false);
            createItem(iconMenu, 0xD, {{7, 5000}}, false);
        }
        else
        {
            //  THE SHOP (Extras)
            createItem(iconMenu, 0x2, {{33, 1000}, {35, 1000}, {36, 1000}, {84, 1500}, {97, 2500}, {99, 1500}}, false);
            createItem(iconMenu, 0x3, {{28, 1000}, {39, 1000}, {49, 2500}, {88, 2000}, {104, 2000}}, false);
            createItem(iconMenu, 0xB, {{8, 7000}, {11, 7000}}, false);
        }
        break;

    case 2:
        if (index == 1)
        {
            //  SECRET SHOP (Icons)
            createItem(iconMenu, 0x1, {{85, 2500}, {94, 3000}, {96, 2000}, {110, 5000}, {136, 5000}, {206, 2000}, {225, 3000}, {229, 5000}}, false);
            createItem(iconMenu, 0x4, {{58, 4000}, {77, 4000}}, false);
            createItem(iconMenu, 0x5, {{25, 2000}, {60, 2500}, {107, 3000}}, false);
            createItem(iconMenu, 0x6, {{21, 4000}, {83, 3000}, {104, 2500}}, false);
            createItem(iconMenu, 0x7, {{18, 2000}, {78, 2500}}, false);
            createItem(iconMenu, 0x8, {{14, 3000}, {45, 3500}}, false);
            createItem(iconMenu, 0x9, {{3, 3000}, {58, 3500}, {69, 5000}}, false);
            createItem(iconMenu, 0xD, {{12, 6000}, {27, 6000}}, false);
        }
        else
        {
            //  SECRET SHOP (Extras)
            createItem(iconMenu, 0x2, {{39, 1000}, {40, 1000}, {49, 1000}, {94, 1000}}, false);
            createItem(iconMenu, 0x3, {{32, 1000}, {41, 1000}, {85, 1500}, {86, 1000}, {70, 1000}}, false);
            createItem(iconMenu, 0xA, {{7, 4000}}, false);
            createItem(iconMenu, 0xB, {{13, 10000}, {16, 15000}}, false);
            createItem(iconMenu, 0xC, {{4, 1000}}, false);
        }
        break;

    case 3:
        if (index == 1)
        {
            //  COMMUNITY SHOP (Icons)
            createItem(iconMenu, 0x1, {{117, 4000}, {118, 2000}, {119, 2000}, {120, 4000}, {121, 2000}, {122, 2000}, {123, 6000}, {124, 8000}, {125, 4000}, {126, 2000}, {127, 6000}, {128, 8000}, {129, 6000}, {130, 4000}, {131, 4000}, {132, 4000}, {133, 8000}, {134, 6000}, {135, 6000}}, false);
            createItem(iconMenu, 0x4, {{36, 2000}, {37, 2000}, {38, 6000}, {39, 2000}, {40, 4000}, {41, 4000}, {42, 6000}, {43, 2000}, {44, 6000}, {45, 8000}, {46, 4000}, {47, 6000}, {48, 4000}}, false);
            createItem(iconMenu, 0x5, {{30, 4000}, {31, 6000}, {32, 6000}, {33, 2000}, {34, 4000}, {35, 8000}, {36, 6000}, {37, 2000}}, false);
        }
        else
        {
            createItem(iconMenu, 0x5, {{38, 2000}}, false);
            createItem(iconMenu, 0x6, {{29, 6000}, {30, 6000}, {31, 4000}, {32, 6000}, {33, 4000}, {34, 2000}, {35, 2000}}, false);
            createItem(iconMenu, 0x7, {{24, 2000}, {25, 4000}, {26, 2000}, {27, 2000}, {28, 6000}, {29, 6000}, {30, 4000}, {31, 6000}, {32, 2000}, {33, 6000}}, false);
            createItem(iconMenu, 0x8, {{18, 2000}, {19, 6000}, {20, 2000}, {21, 4000}, {22, 6000}, {23, 2000}, {24, 6000}, {25, 4000}}, false);
            createItem(iconMenu, 0x9, {{11, 4000}, {12, 6000}, {13, 4000}, {14, 8000}, {15, 2000}, {16, 6000}}, false);
        }
        break;

    case 4:
        if (index == 1)
        {
            //  THE MECHANIC (Icons)
            createItem(iconMenu, 0x1, {{168, 5000}, {185, 6000}, {190, 5000}, {230, 6000}, {253, 7000}, {358, 6000}, {360, 6000}, {480, 5000}}, false);
            createItem(iconMenu, 0x4, {{80, 6000}, {111, 6000}, {126, 7000}, {144, 5000}, {164, 7000}}, false);
            createItem(iconMenu, 0x5, {{49, 6000}, {54, 5000}, {64, 5000}, {65, 5000}}, false);
            createItem(iconMenu, 0x6, {{86, 7000}, {95, 7000}, {107, 6000}, {132, 6000}, {136, 5000}}, false);
            createItem(iconMenu, 0x7, {{68, 5000}, {77, 7000}, {85, 6000}}, false);
            createItem(iconMenu, 0x8, {{52, 9000}, {68, 7000}}, false);
            createItem(iconMenu, 0x9, {{50, 6000}, {56, 6000}}, false);
            createItem(iconMenu, 0xD, {{6, 6000}}, false);
        }
        else
        {
            //  THE MECHANIC (Extras)
            createItem(iconMenu, 0xC, {{18, 15000}, {19, 15000}, {20, 15000}}, false);
            createItem(iconMenu, 0xF, {{2, 20000}, {3, 20000}, {4, 20000}, {5, 20000}, {6, 20000}}, false);
            createItem(iconMenu, 0xB, {{19, 15000}}, false);
            createItem(iconMenu, 0xC, {{16, 40000}}, false);
        }
        break;

    case 5:
        if (index == 1)
        {
            //  DIAMOND SHOP (Icons)
            createItem(iconMenu, 0x1, {{181, 300}, {186, 500}, {196, 400}, {203, 600}, {250, 300}, {263, 600}, {290, 400}, {314, 400}, {421, 500}, {443, 600}, {450, 500}, {456, 400}, {463, 400}, {464, 500}, {465, 400}, {471, 400}, {474, 300}, {478, 400}}, true);
            createItem(iconMenu, 0x4, {{62, 500}, {63, 300}, {66, 500}, {81, 600}, {87, 400}, {125, 500}, {129, 400}, {131, 500}, {168, 600}}, true);
            createItem(iconMenu, 0x5, {{53, 600}, {55, 300}, {81, 500}, {98, 300}, {112, 500}, {113, 400}, {116, 500}}, true);
            createItem(iconMenu, 0x6, {{45, 500}, {52, 500}, {78, 300}, {80, 400}, {100, 600}, {140, 600}}, true);
        }
        else
        {
            createItem(iconMenu, 0x6, {{149, 600}, {141, 500}}, true);
            createItem(iconMenu, 0x7, {{56, 400}, {65, 300}, {75, 500}, {80, 300}, {82, 500}, {87, 400}}, true);
            createItem(iconMenu, 0x8, {{34, 500}, {49, 500}, {51, 500}, {62, 300}, {63, 600}}, true);
            createItem(iconMenu, 0x9, {{22, 400}, {29, 500}, {54, 300}, {60, 400}, {64, 500}, {65, 300}, {66, 500}}, true);
            createItem(iconMenu, 0xD, {{10, 500}, {30, 500}, {40, 600}, {41, 400}, {42, 500}}, true);
            createItem(iconMenu, 0xE, {{5, 500}}, true);

            //  DIAMOND SHOP (Extras)
            createItem(iconMenu, 0x2, {{64, 100}, {71, 100}, {74, 100}, {77, 100}, {98, 100}, {100, 100}}, true);
            createItem(iconMenu, 0x3, {{43, 100}, {59, 100}, {72, 100}, {77, 100}, {89, 100}, {106, 100}}, true);
            createItem(iconMenu, 0xB, {{18, 1500}}, true);
            createItem(iconMenu, 0xC, {{17, 2000}}, true);
        }
        break;
    }
};

void ShoppingListAlert::createNavButton(CCMenu *menu, int tag, bool active)
{
    //  Sprite based on the tag of the button
    auto sprName = (tag == 1) ? "SL_ShopKeeper01.png"_spr
                : (tag == 2) ? "SL_ShopKeeper02.png"_spr
                : (tag == 3) ? "SL_ShopKeeper03.png"_spr
                : (tag == 4) ? "SL_ShopKeeper04.png"_spr
                : "SL_ShopKeeper05.png"_spr;

    //  Base color based on whenever the current page is on
    auto baseColor = (active) ? IconSelectBaseColor::Selected : IconSelectBaseColor::Unselected;

    auto buttonSpr = IconSelectButtonSprite::createWithSprite(sprName, 1.25F, baseColor);

    if(!active && m_tagged[tag - 1] > 0){
        auto mark = CCSprite::create("SL_ExMark.png"_spr);
        mark->setPosition({27.f, 10.f});
        mark->setScale(0.75f);

        buttonSpr->addChild(mark);
    }

    //  Button
    auto button = CCMenuItemSpriteExtra::create(
        buttonSpr,
        this,
        menu_selector(ShoppingListAlert::onNavButton));

    //  Adds ID and Tag
    button->setID("shop-button-" + std::to_string(tag));
    button->setTag(tag);

    //  Adds button to menu and updates layout.
    menu->addChild(button);
    menu->updateLayout();
};

//  When a Navigation button was pressed
void ShoppingListAlert::onNavButton(CCObject *sender)
{
    auto menu = this->getChildByID("navigation-menu");
    auto navMenu = static_cast<CCMenu *>(menu);
    auto tag = sender->getTag();

    m_currentPage = tag;

    auto shopTitle = (tag == 1) ? "The Shop" 
            : (tag == 2) ? "Scratch's Shop"
            : (tag == 3)   ? "Community Shop"
            : (tag == 4)   ? "The Mechanic"
            : "Diamond Shop";
    this->setTitle(shopTitle);

    navMenu->removeAllChildren();
    navMenu->updateLayout();

    for (int ii = 1; ii <= 5; ii++)
    {
        createNavButton(navMenu, ii, tag == ii);
    };

    auto iconPage = this->getChildByID("icon-menu");
    auto iconMenu = static_cast<CCMenu *>(iconPage);
    iconMenu->removeAllChildren();
    iconMenu->updateLayout();

    createIconPage(tag, 1);

    auto pageMenu = this->getChildByID("page-menu");
    auto prevArrow = pageMenu->getChildByID("arrow-prev-button");
    auto nextArrow = pageMenu->getChildByID("arrow-next-button");
    nextArrow->setVisible(true);
    prevArrow->setVisible(false);
};

//  When the Arrow buttons are pressed
void ShoppingListAlert::onPageButton(CCObject * sender){
    auto tag = sender->getTag();

    auto pageMenu = this->getChildByID("page-menu");
    auto prevArrow = pageMenu->getChildByID("arrow-prev-button");
    auto nextArrow = pageMenu->getChildByID("arrow-next-button");
    nextArrow->setVisible(tag == 1);
    prevArrow->setVisible(tag == 2);

    auto iconPage = this->getChildByID("icon-menu");
    auto iconMenu = static_cast<CCMenu *>(iconPage);

    iconMenu->removeAllChildren();
    iconMenu->updateLayout();

    createIconPage(m_currentPage, tag);
};


void ShoppingListAlert::onSelectButton(CCObject * sender){
    CCMenuItemToggler * toggler = static_cast<CCMenuItemToggler *>(sender);
    m_selectMode = !toggler->isToggled();

    auto diamondsPrice = static_cast<CCLabelBMFont *>(this->getChildByID("diamonds-label"));
    auto diamondsIcon = static_cast<CCSprite *>(this->getChildByID("diamonds-icon"));
    auto orbsPrice = static_cast<CCLabelBMFont *>(this->getChildByID("orbs-label"));
    auto orbsIcon = static_cast<CCSprite *>(this->getChildByID("orbs-icon"));

    diamondsPrice->setVisible(!toggler->isToggled());
    diamondsIcon->setVisible(!toggler->isToggled());
    orbsPrice->setVisible(!toggler->isToggled());
    orbsIcon->setVisible(!toggler->isToggled());

    //  log::debug("Select mode: {}", m_selectMode);
};

void ShoppingListAlert::createItem(CCMenu *menu, int type, std::map<int, int> icons, bool isDiamondShop)
{
    for (auto const &[iconID, price] : icons)
    {   
        //  std::vector<int>::iterator it = std::find(m_taggedItems.begin(), m_taggedItems.end(), iconID + type * 1000 + menu->getTag() * 100000);
        //  auto found = (it != m_taggedItems.end());

        auto found = false;
        for (int ii : m_taggedItems){
            if(ii == iconID + type * 1000 + menu->getTag() * 100000) found = true;
        }

        auto noCheckmark = Mod::get()->getSettingValue<bool>("disable-checkmark");
        auto gsm = GameStatsManager::sharedState();
        UnlockType iconType{type};

        auto iconSpr = GJItemIcon::createBrowserItem(
            iconType,
            iconID);

        if (gsm->isItemUnlocked(iconType, iconID) && !noCheckmark)
        {
            auto checkmark = CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png");
            checkmark->setPosition(iconSpr->getContentSize() / 2);
            checkmark->setScale(0.75f);

            iconSpr->darkenStoreItem({30, 30, 30});
            iconSpr->addChild(checkmark);
        }
        else
        {
            if (iconType == UnlockType::Col1 || iconType == UnlockType::Col2)
            {
                auto colorType = (iconType == UnlockType::Col1) ? CCLabelBMFont::create("1", "bigFont.fnt") : CCLabelBMFont::create("2", "bigFont.fnt");
                colorType->setPosition(iconSpr->getContentSize() / 2);
                colorType->setScale(0.5f);

                iconSpr->addChild(colorType);
            }

            auto value = CCLabelBMFont::create(std::to_string(price).c_str(), "bigFont.fnt");
            value->setPosition({iconSpr->getContentSize().width / 2, iconSpr->getContentSize().height / 2 - 22.5f});
            value->setID("icon-price");
            value->setScale(0.4f);

            if(found)value->setColor({ 0, 255, 255 });

            iconSpr->addChild(value);
        };

        //  Creates the Icon as a button itself.
        auto iconButton = CCMenuItemSpriteExtra::create(
            iconSpr,
            this,
            menu_selector(ShoppingListAlert::onIcon)
        );

        //	Passes the user-defined IconParameters for its callback
        iconButton->setUserObject(new IconParameters(type, iconID, price, menu->getTag(), found));

        menu->addChild(iconButton);
        menu->updateLayout();
    }
};

//	When a specific Icon Button is pressed
void ShoppingListAlert::onIcon(CCObject *sender){
    auto parameters = static_cast<IconParameters *>(static_cast<CCNode *>(sender)->getUserObject());
    auto noCheckmark = Mod::get()->getSettingValue<bool>("disable-checkmark");
    UnlockType iconType{parameters->p_iconType};
    auto gsm = GameStatsManager::sharedState();

    //  log::debug("Select mode: {}", m_selectMode);
    //  log::debug("Tag {}", parameters->p_shopID);

    if(m_selectMode){
        if(!gsm->isItemUnlocked(iconType, parameters->p_iconID) || noCheckmark){
            auto btn = static_cast<CCMenuItemSpriteExtra *>(sender);
            auto icon = static_cast<GJItemIcon *>(btn->getChildren()->objectAtIndex(0));
            auto label = static_cast<CCLabelBMFont *>(icon->getChildByID("icon-price"));
            auto arrayID = parameters->p_iconID + parameters->p_iconType * 1000 + parameters->p_shopID * 100000;

            if(parameters->p_selected){
                //  log::debug("Icon Deselected");

                if(parameters->p_shopID != 5){
                    m_totalManaOrbs -= parameters->p_iconPrice;
                } else {
                    m_totalDiamonds -= parameters->p_iconPrice;
                }

                m_tagged[parameters -> p_shopID - 1]--;

                label->setColor({ 255, 255, 255 });

                //  std::vector<int>::iterator it = std::find(m_taggedItems.begin(), m_taggedItems.end(), arrayID);
                //  m_taggedItems.erase(it);

                std::vector<int>::iterator it;
                for(it = m_taggedItems.begin(); it != m_taggedItems.end() && *it != arrayID; it++);

                if (it != m_taggedItems.end()){
                    m_taggedItems.erase(it);
                }
            } else {
                //  log::debug("Icon Selected");

                if(parameters->p_shopID != 5){
                    m_totalManaOrbs += parameters->p_iconPrice;
                } else {
                    m_totalDiamonds += parameters->p_iconPrice;
                }

                m_tagged[parameters -> p_shopID - 1]++;

                label->setColor({ 0, 255, 255 });
                m_taggedItems.push_back(arrayID);
            }

            parameters->p_selected = !parameters->p_selected;

            auto orbsPrice = static_cast<CCLabelBMFont *>(this->getChildByID("orbs-label"));
            orbsPrice->setString(std::to_string(m_totalManaOrbs).c_str());

            auto diamondsPrice = static_cast<CCLabelBMFont *>(this->getChildByID("diamonds-label"));
            auto diamondsIcon = static_cast<CCSprite *>(this->getChildByID("diamonds-icon"));

            diamondsPrice->setString(std::to_string(m_totalDiamonds).c_str());
            diamondsPrice->setPositionX(orbsPrice->getPositionX() + orbsPrice->getScaledContentSize().width + 30.0f);
            diamondsIcon->setPositionX(diamondsPrice->getPositionX() - 15.0f);

            auto size = std::to_string(m_totalManaOrbs).length();
        }else{
            FLAlertLayer::create("Nope", "You already bought this item.", "OK")->show();
        }

        //  log::debug("Set: {}", m_taggedItems);
    } else {
        ItemInfoPopup::create(parameters->p_iconID, iconType)->show();
        //  log::debug("Icon Popup");
    }
};

//	When the Info Button is pressed, gives a quick summary of the Player's stats in the Treasure Room.
void ShoppingListAlert::onInfoButton(CCObject * sender){
    std::string info =
        "<cr>The Shop:</c> " + std::to_string(m_itemCount[0]) + " out of " + std::to_string(m_itemTotal[0]) +
        "\n<cg>Secret Shop:</c> " + std::to_string(m_itemCount[1]) + " out of " + std::to_string(m_itemTotal[1]) +
        "\n<cy>Community Shop:</c> " + std::to_string(m_itemCount[2]) + " out of " + std::to_string(m_itemTotal[2]) +
        "\n<cp>The Mechanic: </c> " + std::to_string(m_itemCount[3]) + " out of " + std::to_string(m_itemTotal[3]) +
        "\n<cb>Diamond Shop:</c> " + std::to_string(m_itemCount[4]) + " out of " + std::to_string(m_itemTotal[4]);
	FLAlertLayer::create("Shops", info.c_str(), "OK")->show();
};

ShoppingListAlert *ShoppingListAlert::create()
{
    auto ret = new ShoppingListAlert();

    if (ret && ret->init(450.f, 260.f))
    {
        ret->autorelease();
        return ret;
    };

    CC_SAFE_DELETE(ret);
    return nullptr;
};