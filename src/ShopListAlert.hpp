#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

class ShoppingListAlert : public Popup<> {
    protected:
        std::array<int, 5> m_itemTotal = {41, 38, 72, 40, 80};
        std::array<int, 5> m_itemCount = {0, 0, 0, 0, 0};

        bool m_selectMode = false;
        size_t m_currentPage = 1;
        int m_totalPrice = 0;

        bool setup() override;

        CCMenu * createPage(CCSize size, int ID, int index, bool isVisible);
        void createNavButton(CCMenu *, int, bool);
        void onSelectButton(CCObject *);
        void onInfoButton(CCObject *);
        void onPageButton(CCObject *);
        void onNavButton(CCObject *);

        void addItem(CCMenu *, int type, std::map<int, int>, bool);
    public:
        static ShoppingListAlert * create();
        void onIcon(CCObject *);
};