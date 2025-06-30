#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>
#include <Geode/ui/GeodeUI.hpp>

using namespace geode::prelude;

class ShopRewardsListAlert : public Popup<ShopType>
{
public:
    unsigned int m_currentPage = 1;

protected:
    unsigned int m_totalPages = 6;

    std::array<int, 6> m_itemTotal = {0, 0, 0, 0, 0, 0};
    std::array<int, 6> m_itemCount = {0, 0, 0, 0, 0, 0};
    std::array<int, 6> m_tagged = {0, 0, 0, 0, 0, 0};

    //  std::set<int> m_taggedItems = {};
    std::vector<int> m_taggedItems = {};

    bool m_selectMode = false;
    int m_totalManaOrbs = 0;
    int m_totalDiamonds = 0;

    bool setup(ShopType) override;

    void createItem(CCMenu *, int, std::map<int, int>, bool);
    void createPath(CCMenu *, int);
    void createPathPage(int);

    void createNavButton(CCMenu *, int, bool);
    void createIconPage(int, int);

    void onPageButton(CCObject *);
    void onSettings(CCObject *);
    void onInfo(CCObject *);
    void onPath(CCObject *);

    void onSelectButton(CCObject *);
    void onSelectAll(CCObject *);
    void onDeselectAll(CCObject *);

    void loadData();

public:
    static ShopRewardsListAlert *create(ShopType);
    void onNavButton(CCObject *);
    void onIcon(CCObject *);
};