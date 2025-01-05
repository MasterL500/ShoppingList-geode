#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

class ShopRewardsListAlert : public Popup<ShopType> {
    protected:
        size_t m_currentPage = 1;
        size_t m_totalPages = 6;

        std::array<int, 6> m_itemTotal = {0, 0, 0, 0, 0, 0};
        std::array<int, 6> m_itemCount = {0, 0, 0, 0, 0, 0};
        std::array<int, 6> m_tagged = {0, 0, 0, 0, 0, 0};
        
        //  std::set<int> m_taggedItems = {};
        std::vector<int> m_taggedItems = {};

        bool m_selectMode = false;
        int m_totalManaOrbs = 0;
        int m_totalDiamonds = 0;

        bool setup(ShopType) override;

        void createNavButton(CCMenu *, int, bool);
        void createIconPage(int, int);

        void onNavButton(CCObject *);
        void onPageButton(CCObject *);
        void onInfoButton(CCObject *);

        void createItem(CCMenu *, int, std::map<int, int>, bool);
        
        void createPath(CCMenu *, int);
        void createPathPage(int);
        void onPath(CCObject *);

        void onSelectButton(CCObject *);
        void onSelectAll(CCObject *);
        void onDeselectAll(CCObject *);

        void loadData();
    public:
        static ShopRewardsListAlert * create(ShopType);
        void secretDialogue(ShopType);
        void onIcon(CCObject *);
};