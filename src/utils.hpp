#pragma once
#include <Geode/Geode.hpp>

using namespace geode::prelude;

namespace MLUtils {
    CCMenu * createIconPage(CCSize, int, bool);

    template <typename T>
    void addIcons(CCMenu*, cocos2d::SEL_MenuHandler, int, const T &);

    template <typename T, typename... ARGS>
    void addIcons(CCMenu*, cocos2d::SEL_MenuHandler, int, const T &, const ARGS &...);
}