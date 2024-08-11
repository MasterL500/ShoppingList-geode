#include "ShopListAlert.hpp"
#include "ShopData.hpp"

//  Structure for Icons
struct IconParameters : public CCObject
{
    bool p_selected;
    bool p_unlocked;
    int p_iconPrice;
    int p_iconType;
    int p_iconID;
    int p_shopID;

    IconParameters(int type, int id, int price, int shop, bool unlocked, bool selected) : p_iconType(type), p_iconID(id), p_iconPrice(price), p_shopID(shop), p_unlocked(unlocked), p_selected(selected) {
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
	m_mainLayer->addChild(iconListBG);

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
    m_mainLayer->addChild(infoMenu);

    //  Navigation Menu
    auto navMenu = CCMenu::create();
    navMenu->setPosition({winSize.width / 2 + 110.0f, winSize.height / 2 - 104.0f});
    navMenu->setLayout(RowLayout::create()
            ->setGap(6.f));
    navMenu->setID("navigation-menu");
    m_mainLayer->addChild(navMenu);

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
    m_mainLayer->addChild(pageNavMenu);

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
    m_mainLayer->addChild(iconMenu);

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
    m_mainLayer->addChild(selectMenu);

    //  Select Options (Menu)
    auto selectOptionsMenu = CCMenu::create();
    selectOptionsMenu->setPosition({winSize.width / 2, winSize.height / 2 - 145.0f});
    selectOptionsMenu->setLayout(RowLayout::create()
        ->setGap(6.f));
    
    auto selectAllSpr = ButtonSprite::create("Select All", 100, false, "bigFont.fnt", "GJ_button_04.png", 24, 0.25f);
    auto selectAllBtn = CCMenuItemSpriteExtra::create(
        selectAllSpr,
        this,
        menu_selector(ShoppingListAlert::onSelectAll)
    );

    auto deselectAllSpr = ButtonSprite::create("Deselect All", 100, false, "bigFont.fnt", "GJ_button_04.png", 24, 0.25f);
    auto deselectAllBtn = CCMenuItemSpriteExtra::create(
        deselectAllSpr,
        this,
        menu_selector(ShoppingListAlert::onDeselectAll)
    );

    selectOptionsMenu->addChild(selectAllBtn);
    selectOptionsMenu->addChild(deselectAllBtn);
    selectOptionsMenu->updateLayout();

    selectOptionsMenu->setID("select-options-menu");
    selectOptionsMenu->setVisible(false);
    m_mainLayer->addChild(selectOptionsMenu);

    //  Orbs Pricing (When Select Mode is enabled)
    auto orbsPrice = CCLabelBMFont::create(std::to_string(m_totalManaOrbs).c_str(), "bigFont.fnt");
    orbsPrice->setPosition({winSize.width / 2 - 160.0f, winSize.height / 2 - 104.0f});
    orbsPrice->setAnchorPoint({0.0f, 0.5f});
    orbsPrice->setID("orbs-label");
    orbsPrice->setScale(0.55f);

    auto orbsIcon = CCSprite::createWithSpriteFrameName("currencyOrbIcon_001.png");
    orbsIcon->setPositionY(winSize.height / 2 - 105.0f);
    orbsIcon->setPositionX(orbsPrice->getPositionX() - 12.0f);
    orbsIcon->setID("orbs-icon");

    auto diamondsPrice = CCLabelBMFont::create(std::to_string(m_totalDiamonds).c_str(), "bigFont.fnt");
    diamondsPrice->setPosition({winSize.width / 2 - 160.0f + orbsPrice->getContentWidth() + 25.0f, winSize.height / 2 - 104.0f});
    diamondsPrice->setAnchorPoint({0.0f, 0.5f});
    diamondsPrice->setID("diamonds-label");
    diamondsPrice->setScale(0.55f);

    auto diamondsIcon = CCSprite::createWithSpriteFrameName("currencyDiamondIcon_001.png");
    diamondsIcon->setPositionY(winSize.height / 2 - 105.0f);
    diamondsIcon->setPositionX(diamondsPrice->getPositionX() - 15.0f);
    diamondsIcon->setID("diamonds-icon");

    m_mainLayer->addChild(orbsPrice);
    m_mainLayer->addChild(orbsIcon);
    m_mainLayer->addChild(diamondsPrice);
    m_mainLayer->addChild(diamondsIcon);

    diamondsPrice->setVisible(false);
    diamondsIcon->setVisible(false);
    orbsPrice->setVisible(false);
    orbsIcon->setVisible(false);

    this->m_noElasticity = true;
    this->setTitle("The Shop");
    return true;
};

void ShoppingListAlert::loadData(){
    auto gsm = GameStatsManager::sharedState();

    for (auto ii = ShopData.begin(); ii != ShopData.end(); ii++){
        //  log::debug("Icon: {} {} - Unlocked?: {}", ii->at(0), ii->at(1), gsm->isItemUnlocked(UnlockType{ii->at(0)}, ii->at(1)));

        if (gsm->isItemUnlocked(UnlockType{ii->at(0)}, ii->at(1))){
            m_itemCount[ii->at(3) - 1]++;
        }
    }

    for (auto jj = ShopDataExtras.begin(); jj != ShopDataExtras.end(); jj++){
        if (gsm->isItemUnlocked(UnlockType{jj->at(0)}, jj->at(1))){
            m_itemCount[jj->at(3) - 1]++;
        }
    }
};

void ShoppingListAlert::createIconPage(int ID, int index)
{
    auto menu = this->getChildByIDRecursive("icon-menu");
    auto iconMenu = static_cast<CCMenu *>(menu);
    auto iconList = ShopData;
    auto extrasList = ShopDataExtras;

    iconMenu->removeAllChildren();
    iconMenu->updateLayout();
    iconMenu->setTag(ID);
    
    for (auto ii = iconList.begin(); ii != iconList.end(); ii++){
        if (ii->at(3) != ID || ii->at(4) != index) continue;

        createItem(iconMenu, ii->at(0), {{ii->at(1), ii->at(2)}}, (ID == 4));
    }

    if(index == 2){
        for (auto jj = extrasList.begin(); jj != extrasList.end(); jj++){
            if (jj->at(3) != ID) continue;

            createItem(iconMenu, jj->at(0), {{jj->at(1), jj->at(2)}}, (ID == 4));
        }
    }
};

void ShoppingListAlert::createNavButton(CCMenu *menu, int tag, bool active)
{
    //  Sprite based on the tag of the button
    auto sprName = (tag == 1) ? "SL_ShopKeeper01.png"_spr
                : (tag == 2) ? "SL_ShopKeeper02.png"_spr
                : (tag == 3) ? "SL_ShopKeeper03.png"_spr
                : (tag == 4) ? "SL_ShopKeeper04.png"_spr
                : (tag == 5) ? "SL_ShopKeeper05.png"_spr
                : "SL_PathIcon.png"_spr;

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
    auto menu = this->getChildByIDRecursive("navigation-menu");
    auto navMenu = static_cast<CCMenu *>(menu);
    auto tag = sender->getTag();

    m_currentPage = tag;

    auto shopTitle = (tag == 1) ? "The Shop" 
            : (tag == 2) ? "Scratch's Shop"
            : (tag == 3) ? "Community Shop"
            : (tag == 4) ? "The Mechanic"
            : (tag == 5) ? "Diamond Shop"
            : "Paths of Power";
    this->setTitle(shopTitle);

    navMenu->removeAllChildren();
    navMenu->updateLayout();

    for (int ii = 1; ii <= 5; ii++)
    {
        createNavButton(navMenu, ii, tag == ii);
    };

    auto iconPage = this->getChildByIDRecursive("icon-menu");
    auto iconMenu = static_cast<CCMenu *>(iconPage);
    iconMenu->removeAllChildren();
    iconMenu->updateLayout();

    createIconPage(tag, 1);

    auto pageMenu = this->getChildByIDRecursive("page-menu");
    auto prevArrow = pageMenu->getChildByIDRecursive("arrow-prev-button");
    auto nextArrow = pageMenu->getChildByIDRecursive("arrow-next-button");
    nextArrow->setVisible(true);
    prevArrow->setVisible(false);
};

//  When the Arrow buttons are pressed
void ShoppingListAlert::onPageButton(CCObject * sender){
    auto tag = sender->getTag();

    auto pageMenu = this->getChildByIDRecursive("page-menu");
    auto prevArrow = pageMenu->getChildByIDRecursive("arrow-prev-button");
    auto nextArrow = pageMenu->getChildByIDRecursive("arrow-next-button");
    nextArrow->setVisible(tag == 1);
    prevArrow->setVisible(tag == 2);

    auto iconPage = this->getChildByIDRecursive("icon-menu");
    auto iconMenu = static_cast<CCMenu *>(iconPage);

    iconMenu->removeAllChildren();
    iconMenu->updateLayout();

    createIconPage(m_currentPage, tag);
};

//  When the Select Button is pressed.
void ShoppingListAlert::onSelectButton(CCObject * sender){
    CCMenuItemToggler * toggler = static_cast<CCMenuItemToggler *>(sender);
    m_selectMode = !toggler->isToggled();

    auto selectOptionsMenu = static_cast<CCMenu *>(this->getChildByIDRecursive("select-options-menu"));
    auto diamondsPrice = static_cast<CCLabelBMFont *>(this->getChildByIDRecursive("diamonds-label"));
    auto diamondsIcon = static_cast<CCSprite *>(this->getChildByIDRecursive("diamonds-icon"));
    auto orbsPrice = static_cast<CCLabelBMFont *>(this->getChildByIDRecursive("orbs-label"));
    auto orbsIcon = static_cast<CCSprite *>(this->getChildByIDRecursive("orbs-icon"));

    selectOptionsMenu->setVisible(!toggler->isToggled());
    diamondsPrice->setVisible(!toggler->isToggled());
    diamondsIcon->setVisible(!toggler->isToggled());
    orbsPrice->setVisible(!toggler->isToggled());
    orbsIcon->setVisible(!toggler->isToggled());

    //  log::debug("Select mode: {}", m_selectMode);
};

//  Selects all the icons found in the current Icon page
void ShoppingListAlert::onSelectAll(CCObject * sender){
    auto noCheckmark = Mod::get()->getSettingValue<bool>("shopping-list-disable-checkmark");
    auto iconPage = this->getChildByIDRecursive("icon-menu");
    auto iconMenu = static_cast<CCMenu *>(iconPage);
    auto icons = iconMenu->getChildren();

    for (unsigned int ii = 0; ii < iconMenu->getChildrenCount(); ii++){
        auto node = icons->objectAtIndex(ii);
        auto parameters = static_cast<IconParameters *>(static_cast<CCNode *>(node)->getUserObject());

        //  log::debug("Icon {} - Selected: {} - Unlocked: {}", parameters->p_iconID, parameters->p_selected, parameters->p_unlocked);

        if(!parameters->p_selected && (!parameters->p_unlocked || noCheckmark)){
            onIcon(node);
        }
    }
};

//  Un-selects all the icons from the current Icon page
void ShoppingListAlert::onDeselectAll(CCObject * sender){
    auto noCheckmark = Mod::get()->getSettingValue<bool>("shopping-list-disable-checkmark");
    auto iconPage = this->getChildByIDRecursive("icon-menu");
    auto iconMenu = static_cast<CCMenu *>(iconPage);
    auto icons = iconMenu->getChildren();

    for (unsigned int ii = 0; ii < iconMenu->getChildrenCount(); ii++){
        auto node = icons->objectAtIndex(ii);
        auto parameters = static_cast<IconParameters *>(static_cast<CCNode *>(node)->getUserObject());

        if(parameters->p_selected){
            onIcon(node);
        }
    }
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

        auto noCheckmark = Mod::get()->getSettingValue<bool>("shopping-list-disable-checkmark");
        auto gsm = GameStatsManager::sharedState();
        auto gm = GameManager::sharedState();
        UnlockType iconType{type};
        IconType iconStatus{type - 1};

        auto iconSpr = GJItemIcon::createBrowserItem(
            iconType,
            iconID);

        //  log::debug("ITEM ID {} - UNLOCK STATE: {}", iconID, gm->isIconUnlocked(iconID, iconStatus));

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
        iconButton->setUserObject(new IconParameters(type, iconID, price, menu->getTag(), gsm->isItemUnlocked(iconType, iconID), found));

        menu->addChild(iconButton);
        menu->updateLayout();

        //  log::debug("0x{}, {}, {}, {}, 1", std::format("{:x}", type), iconID, price, menu->getTag());
    }
};

/*  // TO ADD LATER
void ShoppingListAlert::createPath(CCMenu *menu, int path){
    auto gm = GameManager::sharedState();

    auto pathSprite = CCSprite::createWithSpriteFrameName("pathIcon_01_001.png");
    auto pathShardSprite = CCSprite::createWithSpriteFrameName("fireShardBig_001.png");

    pathSprite->setAnchorPoint({0.5, 0.5});
    pathSprite->addChild(pathShardSprite);

    auto pathButton = CCMenuItemSpriteExtra::create(
        pathSprite,
        this,
        nullptr
    );

    menu->addChild(pathButton);
    menu->updateLayout();

    switch (path){
        case 1:

    }
};
*/

//	When a specific Icon Button is pressed
void ShoppingListAlert::onIcon(CCObject *sender){
    auto parameters = static_cast<IconParameters *>(static_cast<CCNode *>(sender)->getUserObject());
    auto noCheckmark = Mod::get()->getSettingValue<bool>("shopping-list-disable-checkmark");
    UnlockType iconType{parameters->p_iconType};
    auto gsm = GameStatsManager::sharedState();

    //  log::debug("Select mode: {}", m_selectMode);
    //  log::debug("Tag {}", parameters->p_shopID);

    if(m_selectMode){
        if(!gsm->isItemUnlocked(iconType, parameters->p_iconID) || noCheckmark){
            auto btn = static_cast<CCMenuItemSpriteExtra *>(sender);
            auto icon = static_cast<GJItemIcon *>(btn->getChildren()->objectAtIndex(0));
            auto label = static_cast<CCLabelBMFont *>(icon->getChildByIDRecursive("icon-price"));
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

            auto orbsPrice = static_cast<CCLabelBMFont *>(this->getChildByIDRecursive("orbs-label"));
            orbsPrice->setString(std::to_string(m_totalManaOrbs).c_str());

            auto diamondsPrice = static_cast<CCLabelBMFont *>(this->getChildByIDRecursive("diamonds-label"));
            auto diamondsIcon = static_cast<CCSprite *>(this->getChildByIDRecursive("diamonds-icon"));

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