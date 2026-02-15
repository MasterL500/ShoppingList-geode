#include "ShopListAlert.hpp"

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

bool ShopRewardsListAlert::init(ShopType shopType)
{
    if (!Popup::init(450.f, 260.f))
        return false;

    auto layerSize = m_mainLayer->getContentSize();
    auto showPathsPage = Mod::get()->getSettingValue<bool>("paths-page");
    loadData();

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
        menu_selector(ShopRewardsListAlert::onInfo));

    infoButton->setID("info-button");
    infoMenu->addChildAtPosition(infoButton, Anchor::TopRight);
    m_mainLayer->addChildAtPosition(infoMenu, Anchor::Center);

    //  Navigation Menu
    auto navMenu = CCMenu::create();
    navMenu->setPosition({layerSize.width / 2 + 96.0f, layerSize.height / 2 - 104.0f});
    navMenu->setLayout(
        RowLayout::create()
            ->setGap(2.f));
    navMenu->setID("navigation-menu");
    m_mainLayer->addChild(navMenu);

    //  Settings Button (Itself)
    auto settingsButton = CCMenuItemSpriteExtra::create(
        IconSelectButtonSprite::createWithSprite("SL_SettingsIcon.png"_spr, 1.5F),
        this,
        menu_selector(ShopRewardsListAlert::onSettings));

    settingsButton->setID("settings-button");
    navMenu->addChild(settingsButton);

    //  Colors blue the button of the current Shop (If value is -1 it means it's from the Garage)
    auto enabledButton = (int)shopType != -1 ? (int)shopType + 1 : 1;

    //  Navigation Menu (Buttons)
    //  * I know this method to add the paths page is terrible
    for (int ii = 1; ii <= m_totalPages - !showPathsPage; ii++)
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
    iconMenu->setID("icon-menu");
    iconMenu->setScale(0.8f);
    iconMenu->setPosition({layerSize.width / 2, layerSize.height / 2 + 7.5f});
    m_mainLayer->addChild(iconMenu);

    auto aux_value = (int)shopType != -1 ? (int)shopType + 1 : 1;
    createIconPage(aux_value, 1);

    //  Select Mode (Menu)
    auto selectMenu = CCMenu::create();
    selectMenu->setContentSize({400.f, 212.f});
    selectMenu->ignoreAnchorPointForPosition(false);
    selectMenu->setID("select-menu");

    //  Button to enable Select Mode
    auto checkButton = CCMenuItemToggler::createWithStandardSprites(
        this,
        menu_selector(ShopRewardsListAlert::onSelectButton),
        0.75f);

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
    orbsPrice->setScale(0.4f);

    auto orbsIcon = CCSprite::createWithSpriteFrameName("currencyOrbIcon_001.png");
    orbsIcon->setPositionY(layerSize.height / 2 - 105.0f);
    orbsIcon->setPositionX(orbsPrice->getPositionX() - 12.0f);
    orbsIcon->setID("orbs-icon");

    //  Diamonds Pricing (When Select mode is Enabled)
    auto diamondsPrice = CCLabelBMFont::create(std::to_string(m_totalDiamonds).c_str(), "bigFont.fnt");
    diamondsPrice->setPosition({layerSize.width / 2 - 160.0f + orbsPrice->getContentWidth() + 25.0f, layerSize.height / 2 - 104.0f});
    diamondsPrice->setAnchorPoint({0.0f, 0.5f});
    diamondsPrice->setID("diamonds-label");
    diamondsPrice->setScale(0.4f);

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

    //  Triggers this function to get the title and name
    onNavButton(navMenu->getChildByTag(aux_value));

    //  Final features
    //  this->m_noElasticity = true;
    this->setID("shopping-list-popup");
    return true;
};

//  Saves up the amount of icons the player has unlocked so far
void ShopRewardsListAlert::loadData()
{
    auto gsm = GameStatsManager::sharedState();
    auto gm = GameManager::sharedState();

    for (auto storeItem : CCArrayExt<GJStoreItem *>(gsm->m_storeItemArray))
    {
        if (gsm->isItemUnlocked(UnlockType(storeItem->m_unlockType.value()), storeItem->m_typeID.value()))
            m_itemCount[int(storeItem->m_shopType)]++;

        m_itemTotal[int(storeItem->m_shopType)]++;
    }
};

bool isExtraType(UnlockType type)
{
    return type == UnlockType::Col1 ||
           type == UnlockType::Col2 ||
           type == UnlockType::Streak ||
           type == UnlockType::Death ||
           type == UnlockType::GJItem ||
           type == UnlockType::ShipFire;
};

void ShopRewardsListAlert::createIconPage(int ID, int index)
{
    auto gsm = GameStatsManager::sharedState();
    auto menu = this->getChildByIDRecursive("icon-menu");
    auto iconMenu = static_cast<CCMenu *>(menu);

    iconMenu->setContentSize({540.0f, 220.0f});
    iconMenu->removeAllChildren();
    iconMenu->updateLayout();
    iconMenu->setTag(ID);

    std::vector<GJStoreItem *> storeItems;
    for (auto storeItem : CCArrayExt<GJStoreItem *>(gsm->m_storeItemArray))
    {
        if (storeItem->m_shopType != ShopType(ID - 1))
            continue;

        storeItems.push_back(storeItem);
    }

    std::sort(storeItems.begin(), storeItems.end(), [](GJStoreItem *a, GJStoreItem *b)
              {
        auto aType = UnlockType(a->m_unlockType.value());
        auto bType = UnlockType(b->m_unlockType.value());
        auto aMisc = isExtraType(aType);
        auto bMisc = isExtraType(bType);
        auto aID = a->m_typeID.value();
        auto bID = b->m_typeID.value();
        return aMisc != bMisc ? aMisc < bMisc : aType != bType ? aType < bType : aID < bID; });

    for (int ii = 0; ii < storeItems.size(); ii++)
    {
        auto storeItem = storeItems[ii];
        auto extraType = isExtraType(UnlockType(storeItem->m_unlockType.value()));

        if ((index == 1 && (ii >= 40 || extraType)) ||
            (index == 2 && ii < 40 && !extraType))
            continue;

        createItem(iconMenu, storeItem->m_unlockType.value(), {{storeItem->m_typeID.value(), storeItem->m_price.value()}}, ID == 4);
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

    auto mark = CCSprite::create("SL_ExMark.png"_spr);
    mark->setPosition({27.f, 10.f});
    mark->setScale(0.75f);
    mark->setID("ex-mark");
    mark->setVisible(false);
    buttonSpr->addChild(mark);

    //  Button
    auto button = CCMenuItemSpriteExtra::create(
        buttonSpr,
        this,
        menu_selector(ShopRewardsListAlert::onNavButton));

    //  Adds ID and Tag
    button->setID("shop-button-" + std::to_string(tag));
    button->setTag(tag);

    //  Adds button to menu and updates layout.
    menu->addChild(button);
    menu->updateLayout();
};

//  When a Navigation button was pressed
void ShopRewardsListAlert::onNavButton(CCObject *sender)
{
    auto showPathsPage = Mod::get()->getSettingValue<bool>("paths-page");
    auto menu = this->getChildByIDRecursive("navigation-menu");
    auto navMenu = static_cast<CCMenu *>(menu);
    auto tag = sender->getTag();

    auto oldPage = m_currentPage;
    m_currentPage = tag;

    auto shopTitle = (tag == 1)   ? "The Shop"
                     : (tag == 2) ? "Scratch's Shop"
                     : (tag == 3) ? "Community Shop"
                     : (tag == 4) ? "The Mechanic"
                     : (tag == 5) ? "Diamond Shop"
                                  : "Paths of Power";
    this->setTitle(shopTitle);

    if (auto navButton = static_cast<CCMenuItemSpriteExtra *>(navMenu->getChildByTag(oldPage)))
    {
        static_cast<CCSprite *>(navButton->getNormalImage())->setDisplayFrame(CCSpriteFrameCache::get()->spriteFrameByName("geode.loader/baseIconSelect_Normal_Unselected.png"));
        navButton->updateSprite();
        navMenu->updateLayout();
        if (auto mark = static_cast<CCSprite *>(navButton->getChildByIDRecursive("ex-mark")))
            mark->setVisible(m_tagged[oldPage - 1] > 0 && m_selectMode);
    }

    if (auto navButton = static_cast<CCMenuItemSpriteExtra *>(sender))
    {
        static_cast<CCSprite *>(navButton->getNormalImage())->setDisplayFrame(CCSpriteFrameCache::get()->spriteFrameByName("geode.loader/baseIconSelect_Normal_Selected.png"));
        navButton->updateSprite();
        navMenu->updateLayout();
        if (auto mark = static_cast<CCSprite *>(navButton->getChildByIDRecursive("ex-mark")))
            mark->setVisible(false);
    }

    auto iconPage = this->getChildByIDRecursive("icon-menu");
    auto iconMenu = static_cast<CCMenu *>(iconPage);
    iconMenu->removeAllChildren();
    iconMenu->updateLayout();

    auto pageMenu = this->getChildByIDRecursive("page-menu");
    auto prevArrow = pageMenu->getChildByIDRecursive("arrow-prev-button");
    auto nextArrow = pageMenu->getChildByIDRecursive("arrow-next-button");

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

    createIconPage(m_currentPage, tag);
};

//  When the Select Button is pressed.
void ShopRewardsListAlert::onSelectButton(CCObject *sender)
{
    CCMenuItemToggler *toggler = static_cast<CCMenuItemToggler *>(sender);
    bool isToggled = !toggler->isToggled();
    m_selectMode = isToggled;

    auto selectOptionsMenu = static_cast<CCMenu *>(this->getChildByIDRecursive("select-options-menu"));
    auto diamondsPrice = static_cast<CCLabelBMFont *>(this->getChildByIDRecursive("diamonds-label"));
    auto diamondsIcon = static_cast<CCSprite *>(this->getChildByIDRecursive("diamonds-icon"));
    auto orbsPrice = static_cast<CCLabelBMFont *>(this->getChildByIDRecursive("orbs-label"));
    auto orbsIcon = static_cast<CCSprite *>(this->getChildByIDRecursive("orbs-icon"));

    selectOptionsMenu->setVisible(isToggled);
    diamondsPrice->setVisible(isToggled);
    diamondsIcon->setVisible(isToggled);
    orbsPrice->setVisible(isToggled);
    orbsIcon->setVisible(isToggled);

    for (auto iconButton : CCArrayExt<CCMenuItemSpriteExtra *>(this->getChildByIDRecursive("icon-menu")->getChildren()))
    {
        if (auto priceLabel = static_cast<CCLabelBMFont *>(iconButton->getChildByIDRecursive("icon-price")))
        {
            auto parameters = static_cast<IconParameters *>(iconButton->getUserObject("icon-parameters"_spr));
            priceLabel->setColor(isToggled && parameters->p_selected ? ccColor3B{0, 255, 255} : ccColor3B{255, 255, 255});
        }
    }

    for (auto navButton : CCArrayExt<CCMenuItemSpriteExtra *>(this->getChildByIDRecursive("navigation-menu")->getChildren()))
    {
        auto tag = navButton->getTag();
        if (auto mark = static_cast<CCSprite *>(navButton->getChildByIDRecursive("ex-mark")))
            mark->setVisible(isToggled && tag != m_currentPage && m_tagged[tag - 1] > 0);
    }

    //  log::debug("Select mode: {}", m_selectMode);
};

//  Selects all the icons found in the current Icon page
void ShopRewardsListAlert::onSelectAll(CCObject *sender)
{
    auto checkmark = Mod::get()->getSettingValue<bool>("display-checkmark");
    auto iconPage = this->getChildByIDRecursive("icon-menu");
    auto iconMenu = static_cast<CCMenu *>(iconPage);
    auto icons = iconMenu->getChildren();
    auto tag = iconMenu->getTag();

    //  log::debug("Tag {}", tag);

    for (unsigned int ii = 0; ii < iconMenu->getChildrenCount(); ii++)
    {
        auto node = icons->objectAtIndex(ii);
        auto parameters = static_cast<IconParameters *>(static_cast<CCNode *>(node)->getUserObject("icon-parameters"_spr));

        //  log::debug("Icon {} - Selected: {} - Unlocked: {}", parameters->p_iconID, parameters->p_selected, parameters->p_unlocked);

        if (!parameters->p_selected && (!parameters->p_unlocked || !checkmark))
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
    auto checkmark = Mod::get()->getSettingValue<bool>("display-checkmark");
    auto iconPage = this->getChildByIDRecursive("icon-menu");
    auto iconMenu = static_cast<CCMenu *>(iconPage);
    auto icons = iconMenu->getChildren();

    for (unsigned int ii = 0; ii < iconMenu->getChildrenCount(); ii++)
    {
        auto node = icons->objectAtIndex(ii);
        auto parameters = static_cast<IconParameters *>(static_cast<CCNode *>(node)->getUserObject("icon-parameters"_spr));

        if (parameters->p_selected && (!parameters->p_unlocked || !checkmark))
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
            {
                found = true;
                break;
            }
        }

        auto checkmark = Mod::get()->getSettingValue<bool>("display-checkmark");
        auto gsm = GameStatsManager::sharedState();
        auto gm = GameManager::sharedState();
        UnlockType iconType{type};
        IconType iconStatus{type - 1};

        auto iconSpr = GJItemIcon::createBrowserItem(
            iconType,
            iconID);

        //  log::debug("ITEM ID {} - UNLOCK STATE: {}", iconID, gm->isIconUnlocked(iconID, iconStatus));

        if (gsm->isItemUnlocked(iconType, iconID) && checkmark)
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

            if (found && m_selectMode)
                value->setColor({0, 255, 255});

            iconSpr->addChild(value);
        };

        //  Creates the Icon as a button itself.
        auto iconButton = CCMenuItemSpriteExtra::create(
            iconSpr,
            this,
            menu_selector(ShopRewardsListAlert::onIcon));

        //	Passes the user-defined IconParameters for its callback
        iconButton->setUserObject("icon-parameters"_spr, new IconParameters(type, iconID, price, menu->getTag(), gsm->isItemUnlocked(iconType, iconID), found));

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

    auto checkmark = Mod::get()->getSettingValue<bool>("display-checkmark");
    auto gsm = GameStatsManager::sharedState();

    iconMenu->setContentSize({380.0f, 140.0f});
    iconMenu->removeAllChildren();
    iconMenu->updateLayout();
    iconMenu->setTag(ID);

    iconMenu->setLayout(RowLayout::create()
                            ->setGap(45.0f)
                            ->setAutoScale(false)
                            ->setGrowCrossAxis(true)
                            ->setCrossAxisOverflow(false)
                            ->setAxisAlignment(AxisAlignment::Between)
                            ->setCrossAxisAlignment(AxisAlignment::Between)
                            ->setCrossAxisLineAlignment(AxisAlignment::Even));

    for (auto storeItem : CCArrayExt<GJStoreItem *>(gsm->m_storeItemArray))
    {
        if (storeItem->m_shopType != ShopType(ID - 1))
            continue;

        auto pathSprite = GJPathSprite::create(storeItem->m_typeID.value() - 5);
        pathSprite->addShardSprite();

        auto value = CCLabelBMFont::create(std::to_string(storeItem->m_price.value()).c_str(), "bigFont.fnt");
        value->setPosition({pathSprite->getScaledContentSize().width / 2, pathSprite->getContentSize().height / 2 - 22.5f});
        value->setID("icon-price");
        value->setScale(0.4f);

        pathSprite->addChild(value);

        auto unlocked = gsm->isItemUnlocked(UnlockType(storeItem->m_unlockType.value()), storeItem->m_typeID.value());
        if (unlocked && checkmark)
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
            if (kk == storeItem->m_typeID.value() + storeItem->m_unlockType.value() * 1000 + ID * 100000)
            {
                found = true;
                break;
            }
        }

        //  log::debug("Wow it's {}", jj->at(4));

        auto pathButton = CCMenuItemSpriteExtra::create(
            pathSprite,
            this,
            menu_selector(ShopRewardsListAlert::onPath));

        pathButton->setContentSize({40, 40});
        pathSprite->setPosition({20, 20});

        if (found && m_selectMode)
            value->setColor({0, 255, 255});

        pathButton->setUserObject("icon-parameters"_spr, new IconParameters(storeItem->m_unlockType.value(), storeItem->m_typeID.value(), storeItem->m_price.value(), ID, unlocked, found));
        iconMenu->addChild(pathButton);

        iconMenu->updateLayout();
    }
};

void ShopRewardsListAlert::onPath(CCObject *sender)
{
    auto parameters = static_cast<IconParameters *>(static_cast<CCNode *>(sender)->getUserObject("icon-parameters"_spr));
    auto checkmark = Mod::get()->getSettingValue<bool>("display-checkmark");
    auto gsm = GameStatsManager::sharedState();

    UnlockType unlockType{parameters->p_iconType};

    if (m_selectMode)
    {
        if (!gsm->isItemUnlocked(unlockType, parameters->p_iconID) || !checkmark)
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
        }
        else
        {
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
        if (auto nameLabel = static_cast<CCLabelBMFont *>(popup->getChildByIDRecursive("name-label")))
        {
            nameLabel->setString(iconName.c_str());
            nameLabel->setZOrder(20);
        }

        //  Changes the Description of the item
        if (auto descriptionArea = static_cast<TextArea *>(popup->getChildByIDRecursive("description-area")))
            descriptionArea->setString(descriptionString);

        //  Changes the label of the "achievement"
        if (auto achievementLabel = static_cast<CCLabelBMFont *>(popup->getChildByIDRecursive("achievement-label")))
            achievementLabel->setString("Paths of Power");

        popup->show();
    }
}

//	When a specific Icon Button is pressed
void ShopRewardsListAlert::onIcon(CCObject *sender)
{
    auto parameters = static_cast<IconParameters *>(static_cast<CCNode *>(sender)->getUserObject("icon-parameters"_spr));
    auto checkmark = Mod::get()->getSettingValue<bool>("display-checkmark");
    UnlockType iconType{parameters->p_iconType};
    auto gsm = GameStatsManager::sharedState();

    //  log::debug("Select mode: {}", m_selectMode);
    //  log::debug("Tag {}", parameters->p_shopID);

    log::info("ICON UNLOCKED: {}", gsm->isItemUnlocked(iconType, parameters->p_iconID));
    log::info("CHECKMARK ENABLED: {}", checkmark);
    log::info("SELECT MODE: {}", m_selectMode);

    if (m_selectMode)
    {
        if (!gsm->isItemUnlocked(iconType, parameters->p_iconID) || !checkmark)
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
void ShopRewardsListAlert::onInfo(CCObject *sender)
{
    std::string info =
        "<cr>The Shop:</c> " + std::to_string(m_itemCount[0]) + " out of " + std::to_string(m_itemTotal[0]) +
        "\n<cg>Secret Shop:</c> " + std::to_string(m_itemCount[1]) + " out of " + std::to_string(m_itemTotal[1]) +
        "\n<cy>Community Shop:</c> " + std::to_string(m_itemCount[2]) + " out of " + std::to_string(m_itemTotal[2]) +
        "\n<cp>The Mechanic: </c> " + std::to_string(m_itemCount[3]) + " out of " + std::to_string(m_itemTotal[3]) +
        "\n<cb>Diamond Shop:</c> " + std::to_string(m_itemCount[4]) + " out of " + std::to_string(m_itemTotal[4]);

    if (Mod::get()->getSettingValue<bool>("paths-page"))
        info += "\n<co>Paths of Power:</c> " + std::to_string(m_itemCount[5]) + " out of " + std::to_string(m_itemTotal[5]);

    FLAlertLayer::create("Stats", info.c_str(), "OK")->show();
};

//  Opens the settings of the mod
void ShopRewardsListAlert::onSettings(CCObject *sender)
{
    geode::openSettingsPopup(Mod::get());
}

ShopRewardsListAlert *ShopRewardsListAlert::create(ShopType shopType)
{
    auto ret = new ShopRewardsListAlert();

    if (ret->init(shopType))
    {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
};