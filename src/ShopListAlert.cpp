#include "ShopListAlert.hpp"

bool ShopListAlert::init(ShopType shopType)
{
	if (!Popup::init(450.f, 260.f))
		return false;

	loadStats();

	//  Background
	auto background = NineSlice::create("square02b_001.png", {0, 0, 80, 80});
	background->setContentSize({430, 180});
	background->setColor({133, 68, 41});
	m_mainLayer->addChildAtPosition(background, Anchor::Center, ccp(0, 2), false);

	//  Info Stats
	std::string info =
		"<cr>The Shop:</c> " + std::to_string(m_itemCount[0]) + " out of " + std::to_string(m_itemTotal[0]) +
		"\n<cg>Secret Shop:</c> " + std::to_string(m_itemCount[1]) + " out of " + std::to_string(m_itemTotal[1]) +
		"\n<cs>Community Shop:</c> " + std::to_string(m_itemCount[2]) + " out of " + std::to_string(m_itemTotal[2]) +
		"\n<ca>The Mechanic: </c> " + std::to_string(m_itemCount[3]) + " out of " + std::to_string(m_itemTotal[3]) +
		"\n<cl>Diamond Shop:</c> " + std::to_string(m_itemCount[4]) + " out of " + std::to_string(m_itemTotal[4]);

	if (Mod::get()->getSettingValue<bool>("paths-page"))
	{
		info += "\n<co>Paths of Power:</c> " + std::to_string(m_itemCount[5]) + " out of " + std::to_string(m_itemTotal[5]);
	}

	//  Info Button
	auto infoButton = InfoAlertButton::create("Unlocked Items", info.c_str(), 1);
	infoButton->setID("stats-button");
	m_buttonMenu->addChildAtPosition(infoButton, Anchor::TopRight, ccp(-20, -20), false);

	//  Navigation Menu
	m_navMenu = CCMenu::create();
	m_navMenu->setScale(0.9f);
	m_navMenu->setID("navigation-menu");
	m_navMenu->setLayout(RowLayout::create()
							 ->setGap(2.f));
	m_mainLayer->addChildAtPosition(m_navMenu, Anchor::Bottom, ccp(100, 24), false);

	//  Settings Button
	auto settingsButton = CCMenuItemSpriteExtra::create(
		IconSelectButtonSprite::createWithSprite("SettingsIcon.png"_spr, 1.5F),
		this,
		menu_selector(ShopListAlert::onSettings));
	settingsButton->setID("settings-button");
	m_navMenu->addChild(settingsButton);
	m_navMenu->updateLayout();

	//  Colors the button of the current shop (if value is -1, it means it's outside of any)
	auto shopAux = (int)shopType != -1 ? (int)shopType : 0;

	//  Navigation Menu Buttons
	for (int ii = 0; ii < m_totalPages; ii++)
		createNavButton(ii, ii == shopAux);

	//  Page Navigation Menu (within the Icon Menu)
	m_pageMenu = CCMenu::create();
	m_pageMenu->setContentSize(m_mainLayer->getContentSize());
	m_pageMenu->setID("page-menu");
	m_mainLayer->addChildAtPosition(m_pageMenu, Anchor::BottomLeft, ccp(0, 0), false);

	//  Page Navigation Buttons
	m_prevBtn = CCMenuItemSpriteExtra::create(
		CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png"),
		this,
		menu_selector(ShopListAlert::onPageButton));
	m_prevBtn->setID("prev-page-button");
	m_prevBtn->setVisible(false);
	m_prevBtn->setTag(-1);

	auto nextSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
	nextSpr->setFlipX(true);

	m_nextBtn = CCMenuItemSpriteExtra::create(
		nextSpr,
		this,
		menu_selector(ShopListAlert::onPageButton));
	m_nextBtn->setID("next-page-button");
	m_nextBtn->setTag(1);

	m_pageMenu->addChildAtPosition(m_prevBtn, Anchor::Left, ccp(-20, 0), false);
	m_pageMenu->addChildAtPosition(m_nextBtn, Anchor::Right, ccp(20, 0), false);

	//  Creates the Menu for the Icons
	m_iconMenu = CCMenu::create();
	m_iconMenu->setContentSize({530.0f, 220.0f});

	m_iconMenu->setID("icon-menu");
	m_iconMenu->setScale(0.8f);
	m_mainLayer->addChildAtPosition(m_iconMenu, Anchor::Center, ccp(0, 2), false);

	//  Select Mode
	auto checkButton = CCMenuItemToggler::createWithStandardSprites(
		this,
		menu_selector(ShopListAlert::onSelectButton),
		0.75f);
	checkButton->setID("select-mode-button");
	m_buttonMenu->addChildAtPosition(checkButton, Anchor::BottomLeft, ccp(24, 24), false);

	//  Select Options Menu
	m_selectMenu = CCMenu::create();
	m_selectMenu->setLayout(RowLayout::create()->setGap(6.f));
	m_selectMenu->setID("select-options-menu");
	m_selectMenu->setVisible(false);
	m_mainLayer->addChildAtPosition(m_selectMenu, Anchor::Bottom, ccp(0, -15), false);

	//  Select All (Option)
	auto selectAllSpr = ButtonSprite::create("Select All", 100, false, "bigFont.fnt", "GJ_button_04.png", 24, 0.25f);
	auto selectAllBtn = CCMenuItemSpriteExtra::create(
		selectAllSpr,
		this,
		menu_selector(ShopListAlert::onSelectAll));
	selectAllBtn->setTag(1);

	//  Deselect All (Option)
	auto deselectAllSpr = ButtonSprite::create("Deselect All", 100, false, "bigFont.fnt", "GJ_button_04.png", 24, 0.25f);
	auto deselectAllBtn = CCMenuItemSpriteExtra::create(
		deselectAllSpr,
		this,
		menu_selector(ShopListAlert::onSelectAll));
	deselectAllBtn->setTag(0);

	m_selectMenu->addChild(selectAllBtn);
	m_selectMenu->addChild(deselectAllBtn);
	m_selectMenu->updateLayout();

	//  Select Mode cost count
	m_selectPrices = CCSprite::create();
	m_selectPrices->setAnchorPoint({0, 0.5});
	m_selectPrices->setContentSize({170, 20});
	m_selectPrices->setVisible(false);

	//  Orbs Pricing (When Select Mode is enabled)
	m_orbsPrice = CCLabelBMFont::create(std::to_string(m_totalManaOrbs).c_str(), "bigFont.fnt");
	m_orbsPrice->limitLabelWidth(80, 0.4f, 0.3f);
	m_orbsPrice->setAnchorPoint({0, 0.5});
	m_orbsPrice->setID("orbs-label");

	auto m_orbsIcon = CCSprite::createWithSpriteFrameName("currencyOrbIcon_001.png");
	m_orbsIcon->setID("orbs-icon");

	//  Diamonds Pricing (When Select mode is Enabled)
	m_diamondsPrice = CCLabelBMFont::create(std::to_string(m_totalDiamonds).c_str(), "bigFont.fnt");
	m_diamondsPrice->limitLabelWidth(60, 0.4f, 0.3f);
	m_diamondsPrice->setAnchorPoint({0, 0.5});
	m_diamondsPrice->setID("diamonds-label");

	auto m_diamondsIcon = CCSprite::createWithSpriteFrameName("currencyDiamondIcon_001.png");
	m_diamondsIcon->setID("diamonds-icon");

	m_selectPrices->addChildAtPosition(m_orbsIcon, Anchor::Left, ccp(10, 0), false);
	m_selectPrices->addChildAtPosition(m_orbsPrice, Anchor::Left, ccp(25, 0), false);
	m_selectPrices->addChildAtPosition(m_diamondsIcon, Anchor::Center, ccp(15, 0), false);
	m_selectPrices->addChildAtPosition(m_diamondsPrice, Anchor::Center, ccp(30, 0), false);
	m_mainLayer->addChildAtPosition(m_selectPrices, Anchor::BottomLeft, ccp(40, 24), false);

	//  To activate when starting
	onNavButton(m_navMenu->getChildByTag(shopAux));

	this->setID("Shopping-List-Popup");
	return true;
}

//  Saves up the amount of icons the player has unlocked so far
void ShopListAlert::loadStats()
{
	auto GSM = GameStatsManager::sharedState();
	auto GM = GameManager::sharedState();

	for (auto storeItem : CCArrayExt<GJStoreItem *>(GSM->m_storeItemArray))
	{
		if (GSM->isItemUnlocked(UnlockType(storeItem->m_unlockType.value()), storeItem->m_typeID.value()))
			m_itemCount[int(storeItem->m_shopType)]++;

		m_itemTotal[int(storeItem->m_shopType)]++;
	}
}

//  Whenever the current item type is an extra
bool isShopItemExtra(UnlockType type)
{
	return type == UnlockType::Col1 ||
		   type == UnlockType::Col2 ||
		   type == UnlockType::Streak ||
		   type == UnlockType::Death ||
		   type == UnlockType::GJItem ||
		   type == UnlockType::ShipFire;
};

//  Whenever the current item is a Path of Power
bool isShopItemPath(UnlockType type, int id)
{
	return type == UnlockType::GJItem && (id >= 6 && id <= 15);
}

//  Creates a page of the Icons
void ShopListAlert::createIconPage(int id, int subPage)
{
	auto GSM = GameStatsManager::sharedState();

	//  Removes all current icons of the menu and updates the tag
	m_iconMenu->setContentSize({540.0f, 220.0f});
	m_iconMenu->removeAllChildren();
	m_iconMenu->updateLayout();
	m_iconMenu->setTag(id);

	//  Default Layout
	m_iconMenu->setLayout(RowLayout::create()
							  ->setGap(24.0f)
							  ->setAutoScale(false)
							  ->setGrowCrossAxis(true)
							  ->setCrossAxisOverflow(false)
							  ->setCrossAxisLineAlignment(AxisAlignment::Even));

	//  It's not the default Layout
	if (getOrder() != 0)
	{
		m_iconMenu->setContentSize({520.0f, 215.0f});
		m_iconMenu->setLayout(RowLayout::create()
								  ->setGap(24.0f)
								  ->setAutoScale(false)
								  ->setGrowCrossAxis(true)
								  ->setCrossAxisOverflow(false)
								  ->setCrossAxisLineAlignment(AxisAlignment::Even));
	}

	//  Paths Layout
	if (id == 5)
	{
		m_iconMenu->setContentSize({380.0f, 140.0f});
		m_iconMenu->setLayout(RowLayout::create()
								  ->setGap(55.0f)
								  ->setAutoScale(false)
								  ->setGrowCrossAxis(true)
								  ->setCrossAxisOverflow(false)
								  ->setAxisAlignment(AxisAlignment::Between)
								  ->setCrossAxisAlignment(AxisAlignment::Between)
								  ->setCrossAxisLineAlignment(AxisAlignment::Even));
	}

	//  Only saves the items of the current store
	std::vector<GJStoreItem *> storeItemsAux;
	for (auto storeItem : CCArrayExt<GJStoreItem *>(GSM->m_storeItemArray))
	{
		if (storeItem->m_shopType != ShopType(id))
			continue;

		storeItemsAux.push_back(storeItem);
	}

	/// Sorting the Icons
	if (getOrder() != 1)
	{
		std::sort(storeItemsAux.begin(), storeItemsAux.end(), [this](GJStoreItem *a, GJStoreItem *b)
				  {
			auto aIndex = a->m_index.value();
			auto bIndex = b->m_index.value();
			auto aID = a->m_typeID.value();
			auto bID = b->m_typeID.value();
			auto aType = UnlockType(a->m_unlockType.value());
			auto bType = UnlockType(b->m_unlockType.value());
			auto aMisc = isShopItemExtra(aType);
			auto bMisc = isShopItemExtra(bType);
			auto aPrice = a->m_price.value();
			auto bPrice = b->m_price.value();

			//  Ascending Price
			if(getOrder() == 2)
				return aMisc != bMisc ? aMisc < bMisc : aPrice != bPrice ? aPrice < bPrice : aType != bType ? aType < bType : aID < bID;
					
			//  Descending Price
			if(getOrder() == 3)
				return aMisc != bMisc ? aMisc < bMisc : aPrice != bPrice ? aPrice > bPrice : aType != bType ? aType > bType : aID > bID;
			
			//  Mod Default
			return aMisc != bMisc ? aMisc < bMisc : aType != bType ? aType < bType : aID < bID; });

		//  Creates the item within the page
		for (int ii = 0; ii < storeItemsAux.size(); ii++)
		{
			auto storeItem = storeItemsAux[ii];
			auto isExtra = isShopItemExtra(UnlockType(storeItem->m_unlockType.value()));
			auto isPath = isShopItemPath(UnlockType(storeItem->m_unlockType.value()), storeItem->m_typeID.value());

			if ((subPage == 0 && (ii >= 40 || (isExtra && !isPath))) || (subPage == 1 && (ii < 40 && !isExtra)))
				continue;

			//  Creates the Item
			createItem(UnlockType(storeItem->m_unlockType.value()), storeItem->m_typeID.value(), storeItem->m_price.value());
		}
	}
	else
	{
		//  Actual order from the shops
		for (int ii = 0; ii < storeItemsAux.size(); ii++)
		{
			auto storeItem = storeItemsAux[ii];

			if ((subPage == 0 && ii >= 30) || (subPage == 1 && (ii < 30 || ii >= 60)) || (subPage == 2 && ii < 60))
				continue;

			//  Creates the Item
			createItem(UnlockType(storeItem->m_unlockType.value()), storeItem->m_typeID.value(), storeItem->m_price.value());
		}
	}

	/*
		//  Sorts the items (OLD VERSION)
		std::sort(storeItemsAux.begin(), storeItemsAux.end(), [](GJStoreItem *a, GJStoreItem *b)
				  {
		auto aType = UnlockType(a->m_unlockType.value());
		auto bType = UnlockType(b->m_unlockType.value());
		auto aMisc = isShopItemExtra(aType);
		auto bMisc = isShopItemExtra(bType);
		auto aID = a->m_typeID.value();
		auto bID = b->m_typeID.value();
		return aMisc != bMisc ? aMisc < bMisc : aType != bType ? aType < bType : aID < bID; });

		//  Creates the item within the page
		for (int ii = 0; ii < storeItemsAux.size(); ii++)
		{
			auto storeItem = storeItemsAux[ii];
			auto isExtra = isShopItemExtra(UnlockType(storeItem->m_unlockType.value()));
			auto isPath = isShopItemPath(UnlockType(storeItem->m_unlockType.value()), storeItem->m_typeID.value());

			if ((subPage == 0 && (ii >= 40 || (isExtra && !isPath))) || (subPage == 1 && (ii < 40 && !isExtra)))
				continue;

			//  Creates the Item
			createItem(UnlockType(storeItem->m_unlockType.value()), storeItem->m_typeID.value(), storeItem->m_price.value());
		}
	*/
}

//  Creates the Item
void ShopListAlert::createItem(UnlockType type, int id, int price)
{
	auto checkmark = Mod::get()->getSettingValue<bool>("display-checkmark");
	auto GSM = GameStatsManager::sharedState();
	auto GM = GameManager::sharedState();

	//  Verifies if selected item is included in the Tagged vector
	auto isTagged = false;
	for (int item : m_taggedItems)
	{
		if (item == id + (int)type * 1000 + m_iconMenu->getTag() * 100000)
		{
			isTagged = true;
			break;
		}
	}

	auto iconSpr = GJItemIcon::createBrowserItem(
		type,
		id);
	iconSpr->setID("item-icon");

	if (GSM->isItemUnlocked(type, id) && checkmark)
	{
		auto checkmarkSpr = CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png");
		checkmarkSpr->setPosition(iconSpr->getContentSize() / 2);
		checkmarkSpr->setScale(0.75f);

		iconSpr->darkenStoreItem({30, 30, 30});
		iconSpr->addChild(checkmarkSpr);
	}
	else
	{
		if (type == UnlockType::Col1 || type == UnlockType::Col2)
		{
			auto colorNum = (type == UnlockType::Col1) ? "1" : "2";
			auto colorLabel = CCLabelBMFont::create(colorNum, "bigFont.fnt");
			colorLabel->setPosition(iconSpr->getContentSize() / 2);
			colorLabel->setScale(0.5f);

			iconSpr->addChild(colorLabel);
		}

		auto priceLabel = CCLabelBMFont::create(std::to_string(price).c_str(), "bigFont.fnt");
		priceLabel->setID("item-price");
		priceLabel->setScale(0.4f);

		if (isTagged && m_selectMode)
			priceLabel->setColor({0, 255, 255});

		iconSpr->addChildAtPosition(priceLabel, Anchor::Center, ccp(0, -22.5f), false);
	}

	//  Creates the Icon as a button itself.
	auto iconButton = CCMenuItemSpriteExtra::create(
		iconSpr,
		this,
		menu_selector(ShopListAlert::onIcon));

	//  Passes the user-defined Parameters for its callback
	iconButton->setUserObject("icon-parameters"_spr, new ShopItemParameters(type, id, price, m_iconMenu->getTag(), GSM->isItemUnlocked(type, id), isTagged));

	m_iconMenu->addChild(iconButton);
	m_iconMenu->updateLayout();
}

//  Creates a button for the Navigation Menu
void ShopListAlert::createNavButton(int tag, bool isActive)
{
	//  Sprite List
	std::vector<const char *> sprites = {
		"ShopKeeper01.png"_spr,
		"ShopKeeper02.png"_spr,
		"ShopKeeper03.png"_spr,
		"ShopKeeper04.png"_spr,
		"ShopKeeper05.png"_spr,
		"PathIcon.png"_spr};

	//  Base color based on whenever the current page is on
	auto buttonColor = (isActive) ? IconSelectBaseColor::Selected : IconSelectBaseColor::Unselected;
	auto buttonSpr = IconSelectButtonSprite::createWithSprite(sprites[tag], 1.25f, buttonColor);

	//  Whenever there's a selected item in the page
	auto markSpr = CCSprite::create("ExclamationMark.png"_spr);
	markSpr->setScale(0.75f);
	markSpr->setID("ex-mark");
	markSpr->setVisible(false);
	buttonSpr->addChildAtPosition(markSpr, Anchor::Center, ccp(10, 10), false);

	//  Button
	auto button = CCMenuItemSpriteExtra::create(
		buttonSpr,
		this,
		menu_selector(ShopListAlert::onNavButton));
	button->setID(fmt::format("navigation-button-{:02}", tag + 1));
	button->setTag(tag);

	//  Adds button to menu and updates layout.
	m_navMenu->addChild(button);
	m_navMenu->updateLayout();
}

//  When a Navigation Button is pressed
void ShopListAlert::onNavButton(CCObject *sender)
{
	auto showPaths = Mod::get()->getSettingValue<bool>("paths-page");
	auto tag = sender->getTag();

	auto oldPage = m_page;
	m_page = tag;
	m_subPage = 0;

	log::debug("NAV BUTTON -- Old: {}, New: {}", oldPage, m_page);

	//  Title List
	std::vector<const char *> pageTitles = {
		"The Shop",
		"Secret Shop",
		"Community Shop",
		"The Mechanic",
		"Diamond Shop",
		"Paths of Power"};
	this->setTitle(pageTitles[tag]);

	//  Updates the sprite of the button of the previous page
	if (auto oldNavButton = static_cast<CCMenuItemSpriteExtra *>(m_navMenu->getChildByTag(oldPage)))
	{
		static_cast<CCSprite *>(oldNavButton->getNormalImage())->setDisplayFrame(CCSpriteFrameCache::get()->spriteFrameByName("geode.loader/baseIconSelect_Normal_Unselected.png"));
		oldNavButton->updateSprite();
		m_navMenu->updateLayout();

		if (auto markSpr = static_cast<CCSprite *>(oldNavButton->getChildByIDRecursive("ex-mark")))
			markSpr->setVisible(m_tagged[oldPage] && m_selectMode);
	}

	//  Updates the sprite of the button for the current page
	if (auto navButton = static_cast<CCMenuItemSpriteExtra *>(sender))
	{
		static_cast<CCSprite *>(navButton->getNormalImage())->setDisplayFrame(CCSpriteFrameCache::get()->spriteFrameByName("geode.loader/baseIconSelect_Normal_Selected.png"));
		navButton->updateSprite();
		m_navMenu->updateLayout();

		if (auto markSpr = static_cast<CCSprite *>(navButton->getChildByIDRecursive("ex-mark")))
			markSpr->setVisible(false);
	}

	m_pageMenu->setVisible(getSubPages() != 0);
	m_prevBtn->setVisible(false);
	m_nextBtn->setVisible(true);
	createIconPage(tag, 0);
}

//  When the Arrow buttons are pressed
void ShopListAlert::onPageButton(CCObject *sender)
{
	log::debug("PAGE BUTTON -- Tag: {}, SubPage: {}", sender->getTag(), m_subPage);

	m_subPage += sender->getTag();

	m_prevBtn->setVisible(m_subPage > 0);
	m_nextBtn->setVisible(m_subPage < getSubPages());

	createIconPage(m_page, m_subPage);
}

//  When the Select Mode button is pressed
void ShopListAlert::onSelectButton(CCObject *sender)
{
	CCMenuItemToggler *toggler = static_cast<CCMenuItemToggler *>(sender);
	m_selectMode = !toggler->isToggled();

	m_selectPrices->setVisible(m_selectMode);
	m_selectMenu->setVisible(m_selectMode);

	for (auto iconButton : CCArrayExt<CCMenuItemSpriteExtra *>(m_iconMenu->getChildren()))
	{
		if (auto priceLabel = static_cast<CCLabelBMFont *>(iconButton->getChildByIDRecursive("item-price")))
		{
			auto parameters = static_cast<ShopItemParameters *>(iconButton->getUserObject("icon-parameters"_spr));
			priceLabel->setColor(m_selectMode && parameters->p_selected ? ccColor3B{0, 255, 255} : ccColor3B{255, 255, 255});
		}
	}

	for (auto navButton : CCArrayExt<CCMenuItemSpriteExtra *>(m_navMenu->getChildren()))
	{
		auto tag = navButton->getTag();
		if (auto mark = static_cast<CCSprite *>(navButton->getChildByIDRecursive("ex-mark")))
			mark->setVisible(m_selectMode && tag != m_page && m_tagged[tag] > 0);
	}
}

//  When any of the Select All buttons is pressed
void ShopListAlert::onSelectAll(CCObject *sender)
{
	auto checkmark = Mod::get()->getSettingValue<bool>("display-checkmark");
	auto icons = m_iconMenu->getChildren();
	auto tag = m_iconMenu->getTag();

	for (auto ii = 0; ii < m_iconMenu->getChildrenCount(); ii++)
	{
		auto node = icons->objectAtIndex(ii);
		auto parameters = static_cast<ShopItemParameters *>(static_cast<CCNode *>(node)->getUserObject("icon-parameters"_spr));
		bool option = sender->getTag() ? !parameters->p_selected : parameters->p_selected;

		if (option && (!parameters->p_unlocked || !checkmark))
			onIcon(node);
	}
}

//  When an Icon is pressed
void ShopListAlert::onIcon(CCObject *sender)
{
	auto parameters = static_cast<ShopItemParameters *>(static_cast<CCNode *>(sender)->getUserObject("icon-parameters"_spr));
	auto checkmark = Mod::get()->getSettingValue<bool>("display-checkmark");
	auto GSM = GameStatsManager::sharedState();

	log::info("*** Item Type: {} -- ID: {}", (int)parameters->p_itemType, parameters->p_itemID);
	// log::debug("* Is Item Unlocked? -- {}", GSM->isItemUnlocked(parameters->p_itemType, parameters->p_itemID));
	// log::debug("* Checkmark Enabled? -- {}", checkmark);
	// log::debug("* In Select Mode? -- {}", m_selectMode);

	if (m_selectMode)
	{
		onSelectItem(sender);
	}
	else
	{
		auto popup = ItemInfoPopup::create(parameters->p_itemID, parameters->p_itemType);

		if (isShopItemPath(parameters->p_itemType, parameters->p_itemID))
		{
			std::string iconName = ItemInfoPopup::nameForUnlockType(parameters->p_itemID, parameters->p_itemType);
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
		}

		popup->show();
	}
}

//  When an Item is selected
void ShopListAlert::onSelectItem(CCObject *sender)
{
	auto parameters = static_cast<ShopItemParameters *>(static_cast<CCNode *>(sender)->getUserObject("icon-parameters"_spr));
	auto checkmark = Mod::get()->getSettingValue<bool>("display-checkmark");
	auto GSM = GameStatsManager::sharedState();

	if (!GSM->isItemUnlocked(parameters->p_itemType, parameters->p_itemID) || !checkmark)
	{
		auto btn = static_cast<CCMenuItemSpriteExtra *>(sender);
		auto icon = static_cast<GJItemIcon *>(btn->getChildByID("item-icon"));
		auto label = static_cast<CCLabelBMFont *>(icon->getChildByIDRecursive("item-price"));
		auto arrayID = parameters->p_itemID + (int)parameters->p_itemType * 1000 + parameters->p_shopID * 100000;

		if (parameters->p_selected)
		{
			if (parameters->p_shopID != 4)
			{
				m_totalManaOrbs -= parameters->p_itemPrice;
			}
			else
			{
				m_totalDiamonds -= parameters->p_itemPrice;
			}

			m_tagged[parameters->p_shopID]--;

			std::vector<int>::iterator it;
			for (it = m_taggedItems.begin(); it != m_taggedItems.end() && *it != arrayID; it++)
				;

			if (it != m_taggedItems.end())
			{
				m_taggedItems.erase(it);
			}

			label->setColor({255, 255, 255});
		}
		else
		{
			if (parameters->p_shopID != 4)
			{
				m_totalManaOrbs += parameters->p_itemPrice;
			}
			else
			{
				m_totalDiamonds += parameters->p_itemPrice;
			}

			m_tagged[parameters->p_shopID]++;
			m_taggedItems.push_back(arrayID);

			label->setColor({0, 255, 255});
		}

		parameters->p_selected = !parameters->p_selected;

		m_orbsPrice->setString(std::to_string(m_totalManaOrbs).c_str());
		m_orbsPrice->limitLabelWidth(80, 0.4f, 0.3f);

		m_diamondsPrice->setString(std::to_string(m_totalDiamonds).c_str());
		m_diamondsPrice->limitLabelWidth(80, 0.4f, 0.3f);
	}
	else
	{
		FLAlertLayer::create("Nope", "You already bought this item.", "OK")->show();
	}
}

//  Gets the amount of Subpages a shop has
int ShopListAlert::getSubPages()
{
	switch (m_page)
	{
	case 2:
	case 4:
		if (getOrder() == 1)
			return 2;
		break;

	case 5:
		return 0;
	}

	return 1;
};

//  Gets the order based on the index of the options
int ShopListAlert::getOrder()
{
	auto order = Mod::get()->getSettingValue<std::string>("sorting-order");

	std::vector<std::string> orders = {
		"Mod Default",
		"Shop Order",
		"Price (Ascending)",
		"Price (Descending)"};

	for (int ii = 0; ii < orders.size(); ii++)
	{
		if (utils::string::equalsIgnoreCase(order, orders[ii]))
			return ii;
	}

	return -1;
}

//  Opens the settings of the Mod
void ShopListAlert::onSettings(CCObject *)
{
	geode::openSettingsPopup(Mod::get());
}

//  Creating the Popup
ShopListAlert *ShopListAlert::create(ShopType shop)
{
	auto ret = new ShopListAlert;

	if (ret->init(shop))
	{
		ret->autorelease();
		return ret;
	};

	delete ret;
	return nullptr;
}