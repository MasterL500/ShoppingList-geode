#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

class ShoppingListAlert : public Popup<> {
    protected:
        size_t m_currentPage = 1;
        std::array<int, 6> m_itemTotal = {42, 38, 72, 40, 80, 10};
        std::array<int, 6> m_itemCount = {0, 0, 0, 0, 0, 0};
        std::array<int, 6> m_tagged = {0, 0, 0, 0, 0, 0};
        
        //  std::set<int> m_taggedItems = {};
        std::vector<int> m_taggedItems = {};

        bool m_selectMode = false;
        int m_totalManaOrbs = 0;
        int m_totalDiamonds = 0;

        bool setup() override;

        void createItem(CCMenu *, int, std::map<int, int>, bool);
        //  void createPath(CCMenu *, int);

        void createNavButton(CCMenu *, int, bool);
        void createIconPage(int, int);

        void onNavButton(CCObject *);
        void onPageButton(CCObject *);
        void onInfoButton(CCObject *);
        void onSelectButton(CCObject *);
        void onSelectAll(CCObject *);
        void onDeselectAll(CCObject *);

        void loadData();
    public:
        static ShoppingListAlert * create();
        void onIcon(CCObject *);
};