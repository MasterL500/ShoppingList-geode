#include "ShopListAlert.hpp"
#include "DialogData.hpp"
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

    IconParameters(int type, int id, int price, int shop, bool unlocked, bool selected) : p_iconType(type), p_iconID(id), p_iconPrice(price), p_shopID(shop), p_unlocked(unlocked), p_selected(selected)
    {
        this->autorelease();
    }
};

bool ShopRewardsListAlert::setup(int const &shopType)
{
    auto layerSize = m_mainLayer->getContentSize();
    auto showPathsPage = Mod::get()->getSettingValue<bool>("show-paths");
    loadData();

    /*  DELAYED UNTIL FURTHER NOTICE
    if(shopType == 1 || !Mod::get()->setSavedValue("shown-scratch-dialogue", true)){
        secretDialogue(1);
    }
    */

    //  Background for the Icon Lists
    auto iconListBG = CCScale9Sprite::create("square02b_001.png", {0, 0, 80, 80});
    iconListBG->setPosition({layerSize.width / 2.f, layerSize.height / 2.f + 2.f});
    iconListBG->setColor({133, 68, 41});
    iconListBG->setContentSize({430, 176});
    m_mainLayer->addChild(iconListBG);

    //  Info Button (Menu)
    auto infoMenu = CCMenu::create();
    infoMenu->setContentSize({400.0f, 216.0f});
    infoMenu->setID("info-menu");

    //  Info Button (Itself)
    auto infoButton = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png"),
        this,
        menu_selector(ShopRewardsListAlert::onInfoButton));

    infoButton->setID("info-button");
    infoMenu->addChildAtPosition(infoButton, Anchor::TopRight);
    m_mainLayer->addChildAtPosition(infoMenu, Anchor::Center);

    //  Navigation Menu
    auto navMenu = CCMenu::create();
    navMenu->setPosition({layerSize.width / 2 + 100.0f, layerSize.height / 2 - 104.0f});
    navMenu->setLayout(
        RowLayout::create()
            ->setGap(4.f));
    navMenu->setID("navigation-menu");
    m_mainLayer->addChild(navMenu);

    //  Colors blue the button of the current Shop (If value is -1 it means it's from the Garage)
    auto enabledButton = shopType != -1 ? shopType + 1 : 1;

    //  Navigation Menu (Buttons)
    //  * I know this method to add the paths page is terrible
    for (int ii = 1; ii <= m_totalPages + showPathsPage; ii++)
        createNavButton(navMenu, ii, ii == enabledButton);

    //  Arrow Buttons for shops with more than One page
    auto pageNavMenu = CCMenu::create();
    pageNavMenu->setLayout(RowLayout::create()
                               ->setGap(460.0f)
                               ->setGrowCrossAxis(true)
                               ->setCrossAxisOverflow(false));
    pageNavMenu->setID("page-menu");
    m_mainLayer->addChildAtPosition(pageNavMenu, Anchor::Center);

    //  Previous page (Arrow button to change page within a shop)
    auto prevBtn = CCMenuItemSpriteExtra::create(
        CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
        this,
        menu_selector(ShopRewardsListAlert::onPageButton));
    prevBtn->setID("arrow-prev-button");
    prevBtn->setTag(1);

    //  Next page (Arrow button to change page within a shop)
    auto nextSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    nextSpr->setFlipX(true);
    auto nextBtn = CCMenuItemSpriteExtra::create(
        nextSpr,
        this,
        menu_selector(ShopRewardsListAlert::onPageButton));
    nextBtn->setID("arrow-next-button");
    nextBtn->setScale(-1);
    nextBtn->setTag(2);

    pageNavMenu->addChild(prevBtn);
    pageNavMenu->addChild(nextBtn);
    //  pageNavMenu->setVisible(false);
    pageNavMenu->updateLayout();
    prevBtn->setVisible(false);

    //  Creates the Menu to add the Icons, which clears everytime when the shop changes
    auto iconMenu = CCMenu::create();
    iconMenu->setContentSize({540.0f, 220.0f});
    iconMenu->setLayout(RowLayout::create()
                            ->setGap(24.0f)
                            ->setAutoScale(false)
                            ->setGrowCrossAxis(true)
                            ->setCrossAxisOverflow(false)
                            ->setCrossAxisLineAlignment(AxisAlignment::Even));
    iconMenu->setPosition({layerSize.width / 2, layerSize.height / 2 + 7.5f});
    iconMenu->setID("icon-menu");
    iconMenu->setScale(0.8f);

    m_mainLayer->addChild(iconMenu);

    if (shopType != -1)
        createIconPage(shopType + 1, 1);
    else
        createIconPage(1, 1);

    auto shopTitle = (shopType == 1)   ? "Scratch's Shop"
                     : (shopType == 2) ? "Community Shop"
                     : (shopType == 3) ? "The Mechanic"
                     : (shopType == 4) ? "Diamond Shop"
                                       : "The Shop";
    this->setTitle(shopTitle);

    //  Select Mode (Menu)
    auto selectMenu = CCMenu::create();
    selectMenu->setContentSize({400.f, 212.f});
    selectMenu->ignoreAnchorPointForPosition(false);
    selectMenu->setID("select-menu");

    auto checkButton = CCMenuItemToggler::createWithStandardSprites(
        this,
        menu_selector(ShopRewardsListAlert::onSelectButton),
        0.8f);

    checkButton->setID("select-button");
    selectMenu->addChild(checkButton);
    m_mainLayer->addChildAtPosition(selectMenu, Anchor::Center);

    //  Select Options (Menu)
    auto selectOptionsMenu = CCMenu::create();
    selectOptionsMenu->setPosition({layerSize.width / 2, layerSize.height / 2 - 145.0f});
    selectOptionsMenu->setLayout(RowLayout::create()
                                     ->setGap(6.f));

    //  Select All (Option)
    auto selectAllSpr = ButtonSprite::create("Select All", 100, false, "bigFont.fnt", "GJ_button_04.png", 24, 0.25f);
    auto selectAllBtn = CCMenuItemSpriteExtra::create(
        selectAllSpr,
        this,
        menu_selector(ShopRewardsListAlert::onSelectAll));

    //  Deselect All (Option)
    auto deselectAllSpr = ButtonSprite::create("Deselect All", 100, false, "bigFont.fnt", "GJ_button_04.png", 24, 0.25f);
    auto deselectAllBtn = CCMenuItemSpriteExtra::create(
        deselectAllSpr,
        this,
        menu_selector(ShopRewardsListAlert::onDeselectAll));

    selectOptionsMenu->addChild(selectAllBtn);
    selectOptionsMenu->addChild(deselectAllBtn);
    selectOptionsMenu->updateLayout();

    selectOptionsMenu->setID("select-options-menu");
    selectOptionsMenu->setVisible(false);
    m_mainLayer->addChild(selectOptionsMenu);

    //  Orbs Pricing (When Select Mode is enabled)
    auto orbsPrice = CCLabelBMFont::create(std::to_string(m_totalManaOrbs).c_str(), "bigFont.fnt");
    orbsPrice->setPosition({layerSize.width / 2 - 160.0f, layerSize.height / 2 - 104.0f});
    orbsPrice->setAnchorPoint({0.0f, 0.5f});
    orbsPrice->setID("orbs-label");
    orbsPrice->setScale(0.45f);

    auto orbsIcon = CCSprite::createWithSpriteFrameName("currencyOrbIcon_001.png");
    orbsIcon->setPositionY(layerSize.height / 2 - 105.0f);
    orbsIcon->setPositionX(orbsPrice->getPositionX() - 12.0f);
    orbsIcon->setID("orbs-icon");

    //  Diamonds Pricing (When Select mode is Enabled)
    auto diamondsPrice = CCLabelBMFont::create(std::to_string(m_totalDiamonds).c_str(), "bigFont.fnt");
    diamondsPrice->setPosition({layerSize.width / 2 - 160.0f + orbsPrice->getContentWidth() + 25.0f, layerSize.height / 2 - 104.0f});
    diamondsPrice->setAnchorPoint({0.0f, 0.5f});
    diamondsPrice->setID("diamonds-label");
    diamondsPrice->setScale(0.45f);

    auto diamondsIcon = CCSprite::createWithSpriteFrameName("currencyDiamondIcon_001.png");
    diamondsIcon->setPositionY(layerSize.height / 2 - 105.0f);
    diamondsIcon->setPositionX(diamondsPrice->getPositionX() - 15.0f);
    diamondsIcon->setID("diamonds-icon");

    //  Adds both price labels and icons
    m_mainLayer->addChild(orbsPrice);
    m_mainLayer->addChild(orbsIcon);
    m_mainLayer->addChild(diamondsPrice);
    m_mainLayer->addChild(diamondsIcon);

    diamondsPrice->setVisible(false);
    diamondsIcon->setVisible(false);
    orbsPrice->setVisible(false);
    orbsIcon->setVisible(false);

    //  Experimental feature text (Shown only in the Path's page)
    auto pathText = CCLabelBMFont::create("This is still in development", "chatFont.fnt");
    pathText->setPosition({layerSize.width / 2, layerSize.height / 2 - 78.0f});
    pathText->setColor({0, 0, 0});
    pathText->setScale(0.8f);
    pathText->setID("experimental-info");
    pathText->setVisible(false);

    //  m_mainLayer->addChild(pathText);

    //  Final features
    this->m_noElasticity = true;
    return true;
};

//  Saves up the amount of icons the player has unlocked so far
void ShopRewardsListAlert::loadData()
{
    auto gsm = GameStatsManager::sharedState();
    auto gm = GameManager::sharedState();

    for (auto ii = ShopData.begin(); ii != ShopData.end(); ii++)
    {
        //  log::debug("Icon: {} {} - Unlocked?: {}", ii->at(0), ii->at(1), gsm->isItemUnlocked(UnlockType{ii->at(0)}, ii->at(1)));

        if (gsm->isItemUnlocked(UnlockType{ii->at(0)}, ii->at(1)))
        {
            m_itemCount[ii->at(3) - 1]++;
        }
    }

    for (auto jj = ShopDataExtras.begin(); jj != ShopDataExtras.end(); jj++)
    {
        if (gsm->isItemUnlocked(UnlockType{jj->at(0)}, jj->at(1)))
        {
            m_itemCount[jj->at(3) - 1]++;
        }
    }

    for (auto kk = PathData.begin(); kk != PathData.end(); kk++)
    {
        //  log::debug("Icon: {} {} - Unlocked?: {}", kk->at(0), kk->at(1), gm->isIconUnlocked(kk->at(1), IconType{kk->at(5)}));

        if (gm->isIconUnlocked(kk->at(0), IconType{kk->at(5)}))
            m_itemCount[5]++;
    }
};

void ShopRewardsListAlert::createIconPage(int ID, int index)
{
    auto menu = this->getChildByIDRecursive("icon-menu");
    auto iconMenu = static_cast<CCMenu *>(menu);
    auto iconList = ShopData;
    auto extrasList = ShopDataExtras;

    iconMenu->setContentSize({540.0f, 220.0f});
    iconMenu->removeAllChildren();
    iconMenu->updateLayout();
    iconMenu->setTag(ID);

    for (auto ii = iconList.begin(); ii != iconList.end(); ii++)
    {
        if (ii->at(3) != ID || ii->at(4) != index)
            continue;

        createItem(iconMenu, ii->at(0), {{ii->at(1), ii->at(2)}}, (ID == 4));
    }

    if (index == 2)
    {
        for (auto jj = extrasList.begin(); jj != extrasList.end(); jj++)
        {
            if (jj->at(3) != ID)
                continue;

            createItem(iconMenu, jj->at(0), {{jj->at(1), jj->at(2)}}, (ID == 4));
        }
    }
};

void ShopRewardsListAlert::createNavButton(CCMenu *menu, int tag, bool active)
{
    //  Sprite based on the tag of the button
    auto sprName = (tag == 1)   ? "SL_ShopKeeper01.png"_spr
                   : (tag == 2) ? "SL_ShopKeeper02.png"_spr
                   : (tag == 3) ? "SL_ShopKeeper03.png"_spr
                   : (tag == 4) ? "SL_ShopKeeper04.png"_spr
                   : (tag == 5) ? "SL_ShopKeeper05.png"_spr
                                : "SL_PathIcon.png"_spr;

    //  Base color based on whenever the current page is on
    auto baseColor = (active) ? IconSelectBaseColor::Selected : IconSelectBaseColor::Unselected;

    auto buttonSpr = IconSelectButtonSprite::createWithSprite(sprName, 1.25F, baseColor);

    if (!active && m_tagged[tag - 1] > 0)
    {
        auto mark = CCSprite::create("SL_ExMark.png"_spr);
        mark->setPosition({27.f, 10.f});
        mark->setScale(0.75f);

        buttonSpr->addChild(mark);
    }

    //  Button
    auto button = CCMenuItemSpriteExtra::create(
        buttonSpr,
        this,
        menu_selector(ShopRewardsListAlert::onNavButton));

    //  Adds ID and Tag
    button->setID("shop-button-" + std::to_string(tag));
    button->setEnabled(!active);
    button->setTag(tag);

    //  Adds button to menu and updates layout.
    menu->addChild(button);
    menu->updateLayout();
};

//  When a Navigation button was pressed
void ShopRewardsListAlert::onNavButton(CCObject *sender)
{
    auto showPathsPage = Mod::get()->getSettingValue<bool>("show-paths");
    auto menu = this->getChildByIDRecursive("navigation-menu");
    auto navMenu = static_cast<CCMenu *>(menu);
    auto tag = sender->getTag();

    m_page = tag;

    auto shopTitle = (tag == 1)   ? "The Shop"
                     : (tag == 2) ? "Scratch's Shop"
                     : (tag == 3) ? "Community Shop"
                     : (tag == 4) ? "The Mechanic"
                     : (tag == 5) ? "Diamond Shop"
                                  : "Paths of Power";
    this->setTitle(shopTitle);

    navMenu->removeAllChildren();
    navMenu->updateLayout();

    for (int ii = 1; ii <= m_totalPages + showPathsPage; ii++)
    {
        createNavButton(navMenu, ii, tag == ii);
    };

    auto iconPage = this->getChildByIDRecursive("icon-menu");
    auto iconMenu = static_cast<CCMenu *>(iconPage);
    iconMenu->removeAllChildren();
    iconMenu->updateLayout();

    auto pageMenu = this->getChildByIDRecursive("page-menu");
    auto prevArrow = pageMenu->getChildByIDRecursive("arrow-prev-button");
    auto nextArrow = pageMenu->getChildByIDRecursive("arrow-next-button");
    auto textInfo = this->getChildByIDRecursive("experimental-info");

    if (tag != 6)
    {
        iconMenu->setLayout(RowLayout::create()
                                ->setGap(24.0f)
                                ->setAutoScale(false)
                                ->setGrowCrossAxis(true)
                                ->setCrossAxisOverflow(false)
                                ->setCrossAxisLineAlignment(AxisAlignment::Even));

        createIconPage(tag, 1);
        nextArrow->setVisible(true);
        prevArrow->setVisible(false);
    }
    else
    {
        createPathPage(tag);
        nextArrow->setVisible(false);
        prevArrow->setVisible(false);
    }
};

//  When the Arrow buttons are pressed
void ShopRewardsListAlert::onPageButton(CCObject *sender)
{
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

    createIconPage(m_page, tag);
};

//  When the Select Button is pressed.
void ShopRewardsListAlert::onSelectButton(CCObject *sender)
{
    CCMenuItemToggler *toggler = static_cast<CCMenuItemToggler *>(sender);
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
void ShopRewardsListAlert::onSelectAll(CCObject *sender)
{
    auto noCheckmark = Mod::get()->getSettingValue<bool>("disable-checkmark");
    auto iconPage = this->getChildByIDRecursive("icon-menu");
    auto iconMenu = static_cast<CCMenu *>(iconPage);
    auto icons = iconMenu->getChildren();
    auto tag = iconMenu->getTag();

    log::debug("Tag {}", tag);

    for (unsigned int ii = 0; ii < iconMenu->getChildrenCount(); ii++)
    {
        auto node = icons->objectAtIndex(ii);
        auto parameters = static_cast<IconParameters *>(static_cast<CCNode *>(node)->getUserObject());

        //  log::debug("Icon {} - Selected: {} - Unlocked: {}", parameters->p_iconID, parameters->p_selected, parameters->p_unlocked);

        if (!parameters->p_selected && (!parameters->p_unlocked || noCheckmark))
        {
            if (tag != 6)
            {
                onIcon(node);
            }
            else
            {
                onPath(node);
            }
        }
    }
};

//  Un-selects all the icons from the current Icon page
void ShopRewardsListAlert::onDeselectAll(CCObject *sender)
{
    auto noCheckmark = Mod::get()->getSettingValue<bool>("disable-checkmark");
    auto iconPage = this->getChildByIDRecursive("icon-menu");
    auto iconMenu = static_cast<CCMenu *>(iconPage);
    auto icons = iconMenu->getChildren();

    for (unsigned int ii = 0; ii < iconMenu->getChildrenCount(); ii++)
    {
        auto node = icons->objectAtIndex(ii);
        auto parameters = static_cast<IconParameters *>(static_cast<CCNode *>(node)->getUserObject());

        if (parameters->p_selected)
        {
            onIcon(node);
        }
    }
};

void ShopRewardsListAlert::createItem(CCMenu *menu, int type, std::map<int, int> icons, bool isDiamondShop)
{
    for (auto const &[iconID, price] : icons)
    {
        //  std::vector<int>::iterator it = std::find(m_taggedItems.begin(), m_taggedItems.end(), iconID + type * 1000 + menu->getTag() * 100000);
        //  auto found = (it != m_taggedItems.end());

        auto found = false;
        for (int ii : m_taggedItems)
        {
            if (ii == iconID + type * 1000 + menu->getTag() * 100000)
                found = true;
        }

        auto noCheckmark = Mod::get()->getSettingValue<bool>("disable-checkmark");
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

            if (found)
                value->setColor({0, 255, 255});

            iconSpr->addChild(value);
        };

        //  Creates the Icon as a button itself.
        auto iconButton = CCMenuItemSpriteExtra::create(
            iconSpr,
            this,
            menu_selector(ShopRewardsListAlert::onIcon));

        //	Passes the user-defined IconParameters for its callback
        iconButton->setUserObject(new IconParameters(type, iconID, price, menu->getTag(), gsm->isItemUnlocked(iconType, iconID), found));

        menu->addChild(iconButton);
        menu->updateLayout();

        //  log::debug("0x{}, {}, {}, {}, 1", std::format("{:x}", type), iconID, price, menu->getTag());
    }
};

void ShopRewardsListAlert::createPathPage(int ID)
{
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto menu = this->getChildByIDRecursive("icon-menu");
    auto iconMenu = static_cast<CCMenu *>(menu);
    auto iconList = PathData;

    auto noCheckmark = Mod::get()->getSettingValue<bool>("disable-checkmark");
    auto gsm = GameStatsManager::sharedState();
    auto gm = GameManager::sharedState();

    iconMenu->setContentSize({380.0f, 140.0f});
    iconMenu->removeAllChildren();
    iconMenu->setTag(ID);

    iconMenu->setLayout(RowLayout::create()
                            ->setGap(45.0f)
                            ->setAutoScale(false)
                            ->setGrowCrossAxis(true)
                            ->setCrossAxisOverflow(false)
                            ->setAxisAlignment(AxisAlignment::Between)
                            ->setCrossAxisAlignment(AxisAlignment::Between)
                            ->setCrossAxisLineAlignment(AxisAlignment::Even));

    for (int ii = 1; ii <= 10; ii++)
    {
        auto pathSprite = GJPathSprite::create(ii);
        pathSprite->addShardSprite();

        auto value = CCLabelBMFont::create("25000", "bigFont.fnt");
        value->setPosition({pathSprite->getScaledContentSize().width / 2, pathSprite->getContentSize().height / 2 - 22.5f});
        value->setID("icon-price");
        value->setScale(0.4f);

        pathSprite->addChild(value);

        for (auto jj = iconList.begin(); jj != iconList.end(); jj++)
        {
            if (jj->at(4) != ii)
            {
                continue;
            }

            if (gm->isIconUnlocked(jj->at(1), IconType{jj->at(5)}) && !noCheckmark)
            {
                auto checkmark = CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png");
                checkmark->setPosition({pathSprite->getContentSize().width / 2, pathSprite->getContentSize().height / 2 - 22.5f});
                checkmark->setScale(0.75f);
                value->setVisible(false);

                pathSprite->addChild(checkmark);
            }

            auto found = false;
            for (int kk : m_taggedItems)
            {
                if (kk == jj->at(1) + jj->at(0) * 1000 + menu->getTag() * 100000)
                    found = true;
            }

            UnlockType iconType{jj->at(0)};
            IconType iconStatus{jj->at(0) - 1};

            //  log::debug("Wow it's {}", jj->at(4));

            auto pathButton = CCMenuItemSpriteExtra::create(
                pathSprite,
                this,
                menu_selector(ShopRewardsListAlert::onPath));

            pathButton->setContentSize({40, 40});
            pathSprite->setPosition({20, 20});

            if (found)
                value->setColor({0, 255, 255});

            pathButton->setUserObject(new IconParameters(jj->at(0), jj->at(1), jj->at(2), menu->getTag(), gsm->isItemUnlocked(iconType, jj->at(1)), found));
            iconMenu->addChild(pathButton);
        }

        iconMenu->updateLayout();
    }
};

void ShopRewardsListAlert::onPath(CCObject *sender)
{
    auto parameters = static_cast<IconParameters *>(static_cast<CCNode *>(sender)->getUserObject());
    auto noCheckmark = Mod::get()->getSettingValue<bool>("disable-checkmark");
    auto gsm = GameStatsManager::sharedState();
    auto gm = GameManager::sharedState();

    UnlockType unlockType{parameters->p_iconType};
    IconType iconType{std::max(parameters->p_iconType - 3, 0)};

    if (m_selectMode)
    {
        if (!gm->isIconUnlocked(parameters->p_iconID, iconType) || noCheckmark)
        {
            auto btn = static_cast<CCMenuItemSpriteExtra *>(sender);
            auto icon = static_cast<GJItemIcon *>(btn->getChildren()->objectAtIndex(0));
            auto label = static_cast<CCLabelBMFont *>(icon->getChildByIDRecursive("icon-price"));
            auto arrayID = parameters->p_iconID + parameters->p_iconType * 1000 + parameters->p_shopID * 100000;

            if (parameters->p_selected)
            {
                //  log::debug("Icon Deselected");

                m_totalManaOrbs -= parameters->p_iconPrice;
                m_tagged[parameters->p_shopID - 1]--;

                label->setColor({255, 255, 255});

                //  std::vector<int>::iterator it = std::find(m_taggedItems.begin(), m_taggedItems.end(), arrayID);
                //  m_taggedItems.erase(it);

                std::vector<int>::iterator it;
                for (it = m_taggedItems.begin(); it != m_taggedItems.end() && *it != arrayID; it++)
                    ;

                if (it != m_taggedItems.end())
                {
                    m_taggedItems.erase(it);
                }
            }
            else
            {
                //  log::debug("Icon Selected");

                m_totalManaOrbs += parameters->p_iconPrice;
                m_tagged[parameters->p_shopID - 1]++;

                label->setColor({0, 255, 255});
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
        }
        else
        {
            if (noCheckmark)
                FLAlertLayer::create("Nope", "You already bought this item.", "OK")->show();
        }

        //  log::debug("Set: {}", m_taggedItems);
    }
    else
    {
        //  Saves the Popup to edit the name, descriptions and others
        auto popup = ItemInfoPopup::create(parameters->p_iconID, unlockType);
        std::string iconName = ItemInfoPopup::nameForUnlockType(parameters->p_iconID, unlockType);
        std::string descriptionString = fmt::format("You can <cl>buy</c> this <cg>Path</c> in the <cy>Paths of Power</c>.", iconName);

        //  Changes the Name of the item
        if (auto nameLabel = static_cast<CCLabelBMFont *>(popup->getChildByIDRecursive("name-label"))){
            nameLabel->setString(iconName.c_str());
            nameLabel->setZOrder(20);
        }

        //  Changes the Description of the item
        if (auto descriptionArea = static_cast<TextArea *>(popup->getChildByIDRecursive("description-area")))
            descriptionArea->setString(descriptionString);

        //  Changes the label of the "achievement"
        if (auto achievementLabel = static_cast<CCLabelBMFont *>(popup->getChildByIDRecursive("achievement-label")))
            achievementLabel->setString("Path of Power");

        popup->show();
    }
}

//	When a specific Icon Button is pressed
void ShopRewardsListAlert::onIcon(CCObject *sender)
{
    auto parameters = static_cast<IconParameters *>(static_cast<CCNode *>(sender)->getUserObject());
    auto noCheckmark = Mod::get()->getSettingValue<bool>("disable-checkmark");
    UnlockType iconType{parameters->p_iconType};
    auto gsm = GameStatsManager::sharedState();

    //  log::debug("Select mode: {}", m_selectMode);
    //  log::debug("Tag {}", parameters->p_shopID);

    if (m_selectMode)
    {
        if (!gsm->isItemUnlocked(iconType, parameters->p_iconID) || noCheckmark)
        {
            auto btn = static_cast<CCMenuItemSpriteExtra *>(sender);
            auto icon = static_cast<GJItemIcon *>(btn->getChildren()->objectAtIndex(0));
            auto label = static_cast<CCLabelBMFont *>(icon->getChildByIDRecursive("icon-price"));
            auto arrayID = parameters->p_iconID + parameters->p_iconType * 1000 + parameters->p_shopID * 100000;

            if (parameters->p_selected)
            {
                //  log::debug("Icon Deselected");

                if (parameters->p_shopID != 5)
                {
                    m_totalManaOrbs -= parameters->p_iconPrice;
                }
                else
                {
                    m_totalDiamonds -= parameters->p_iconPrice;
                }

                m_tagged[parameters->p_shopID - 1]--;

                label->setColor({255, 255, 255});

                //  std::vector<int>::iterator it = std::find(m_taggedItems.begin(), m_taggedItems.end(), arrayID);
                //  m_taggedItems.erase(it);

                std::vector<int>::iterator it;
                for (it = m_taggedItems.begin(); it != m_taggedItems.end() && *it != arrayID; it++)
                    ;

                if (it != m_taggedItems.end())
                {
                    m_taggedItems.erase(it);
                }
            }
            else
            {
                //  log::debug("Icon Selected");

                if (parameters->p_shopID != 5)
                {
                    m_totalManaOrbs += parameters->p_iconPrice;
                }
                else
                {
                    m_totalDiamonds += parameters->p_iconPrice;
                }

                m_tagged[parameters->p_shopID - 1]++;

                label->setColor({0, 255, 255});
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
        }
        else
        {
            FLAlertLayer::create("Nope", "You already bought this item.", "OK")->show();
        }

        //  log::debug("Set: {}", m_taggedItems);
    }
    else
    {
        ItemInfoPopup::create(parameters->p_iconID, iconType)->show();
        //  log::debug("Icon Popup");
    }
};

//	When the Info Button is pressed, gives a quick summary of the Player's stats in the Treasure Room.
void ShopRewardsListAlert::onInfoButton(CCObject *sender)
{
    std::string info =
        "<cr>The Shop:</c> " + std::to_string(m_itemCount[0]) + " out of " + std::to_string(m_itemTotal[0]) +
        "\n<cg>Secret Shop:</c> " + std::to_string(m_itemCount[1]) + " out of " + std::to_string(m_itemTotal[1]) +
        "\n<cy>Community Shop:</c> " + std::to_string(m_itemCount[2]) + " out of " + std::to_string(m_itemTotal[2]) +
        "\n<cp>The Mechanic: </c> " + std::to_string(m_itemCount[3]) + " out of " + std::to_string(m_itemTotal[3]) +
        "\n<cb>Diamond Shop:</c> " + std::to_string(m_itemCount[4]) + " out of " + std::to_string(m_itemTotal[4]) +
        "\n<co>Paths of Power:</c> " + std::to_string(m_itemCount[5]) + " out of " + std::to_string(m_itemTotal[5]);

    FLAlertLayer::create("Stats", info.c_str(), "OK")->show();
};

void ShopRewardsListAlert::secretDialogue(int shopType)
{
    auto dialogueStuff = ScratchDialog;
    CCArray *arr = CCArray::create();
    int colour = 2;

    for (auto ii = dialogueStuff.begin(); ii != dialogueStuff.end(); ii++)
    {
        //  log::debug("Test {} woop {}", ii->first, ii->second);
        auto dialog = DialogObject::create("Scratch", ii->first, ii->second, 1, false, {255, 255, 255});
        arr->addObject(dialog);
    }

    m_mainLayer->setVisible(false);

    auto dl = DialogLayer::createDialogLayer(nullptr, arr, colour);
    dl->animateInRandomSide();
    dl->setZOrder(2);

    this->addChild(dl, 3);
    m_mainLayer->setVisible(true);
};

ShopRewardsListAlert *ShopRewardsListAlert::create(int const &shopType)
{
    auto ret = new ShopRewardsListAlert();

    if (ret->initAnchored(450.f, 260.f, shopType))
    {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
};