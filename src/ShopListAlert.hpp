#pragma once
#include <Geode/Geode.hpp>
#include <Geode/ui/Popup.hpp>

using namespace geode::prelude;

class ShopListAlert : public Popup<> {
    protected:
        bool setup() override;

        CCMenu * createIconPage(int, bool);

    public:
        static ShopListAlert * create();
};