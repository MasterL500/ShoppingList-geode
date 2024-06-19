#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

class ShoppingListAlert : public Popup<> {
    protected:
        size_t m_currentPage = 1;
        bool m_selectMode = false;
        int m_totalPrice = 0;

        bool setup() override;

        CCMenu * createPage(CCSize size, int ID, int index, bool isVisible);
        void createNavButton(CCMenu *, int, bool);
        void onSelectButton(CCObject *);
        void onPageButton(CCObject *);
        void onNavButton(CCObject *);
        
        void addItem(CCMenu *, int type, std::map<int, int>, bool);
    public:
        static ShoppingListAlert * create();
        void onIcon(CCObject *);
};