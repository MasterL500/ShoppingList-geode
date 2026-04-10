#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

//  The Main Layer
class ShopListAlert : public Popup
{
protected:
	std::array<int, 6> m_itemTotal = {0, 0, 0, 0, 0, 0};
	std::array<int, 6> m_itemCount = {0, 0, 0, 0, 0, 0};
	std::array<int, 6> m_tagged = {0, 0, 0, 0, 0, 0};
	std::vector<int> m_taggedItems = {};

	CCMenu *m_iconMenu = nullptr;
	CCMenu *m_pageMenu = nullptr;
	CCMenu *m_navMenu = nullptr;
	CCMenu *m_selectMenu = nullptr;
	CCMenuItemSpriteExtra *m_prevBtn = nullptr;
	CCMenuItemSpriteExtra *m_nextBtn = nullptr;

	CCLabelBMFont *m_orbsPrice = nullptr;
	CCLabelBMFont *m_diamondsPrice = nullptr;
	CCSprite *m_selectPrices = nullptr;

	unsigned int m_totalPages = 6;
	int m_totalManaOrbs = 0;
	int m_totalDiamonds = 0;

	bool init(ShopType shopType);

	void createItem(UnlockType type, int id, int price);
	void createNavButton(int tag, bool isActive);
	void createIconPage(int id, int subPage);

	void onPageButton(CCObject *);
	void onSettings(CCObject *);
	void loadStats();

	int getOrder();
	int getSubPages();

	//  Selection Mode
	void onSelectButton(CCObject *);
	void onSelectItem(CCObject *);
	void onSelectAll(CCObject *);

public:
	unsigned int m_page = 0;
	unsigned int m_subPage = 0;
	bool m_selectMode = false;

	static ShopListAlert *create(ShopType shopType);
	void onNavButton(CCObject *);
	void onIcon(CCObject *);
	void onPath(CCObject *);
};

//  Structure for the Shop Item
struct ShopItemParameters : public CCObject
{
	UnlockType p_itemType;
	int p_itemID;
	int p_itemPrice;
	int p_shopID;
	bool p_unlocked;
	bool p_selected;

	ShopItemParameters(UnlockType type, int id, int price, int shop, bool unlocked, bool selected) : p_itemType(type), p_itemID(id), p_itemPrice(price), p_shopID(shop), p_unlocked(unlocked), p_selected(selected)
	{
		this->autorelease();
	}
};