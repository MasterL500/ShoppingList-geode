#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

class ShopListAlert : public Popup<> {
    protected:
        bool setup() override;
        void createShopButton(CCMenu *, int, bool);

        void addShopIcon(CCMenu *, int, int, int, bool);

        template <typename T>
        void addIcons(CCMenu*, int, const T &);
        template <typename T, typename... A>
        void addIcons(CCMenu*, int, const T &, const A &...);

        void onShopButton(CCObject *);
    public:
        static ShopListAlert * create();
        void onIconButton(CCObject *);
};