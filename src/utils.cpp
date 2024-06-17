#include "utils.hpp"

//	Icon Parameters (to create the info pop-up when pressed
struct IconParameters : public CCObject{
	UnlockType p_iconType;
	int p_iconId;

	IconParameters(UnlockType icon, int id) : p_iconType(icon), p_iconId(id){
		this->autorelease();
	}
};

//	Creates the menu where every Icon is added as a button (That when pressed it displays the Info Pop-up of said Icon)
CCMenu * MLUtils::createIconPage(CCSize size, int ID, bool isVisible){
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto menu = CCMenu::create();

    menu->setContentSize(size);
    menu->setID("icon-page-" + std::to_string(ID));
    menu->setLayout(RowLayout::create()
        ->setGap(20.0f)
        ->setGrowCrossAxis(true)
        ->setCrossAxisOverflow(false)
        ->setCrossAxisLineAlignment(AxisAlignment::Even)
    );

    menu->setPositionY(winSize.height / 2 + 8.f);
    menu->setVisible(isVisible);
    menu->setScale(0.75f);

    return menu;
};

//  Function to add Icons (Base case version)
template <typename T>
void MLUtils::addIcons(CCMenu * menu, cocos2d::SEL_MenuHandler handler, int type, const T &iconID){
    auto noCheckmark = Mod::get()->getSettingValue<bool>("disable-checkmark");
    auto gm = GameStatsManager::sharedState();

    UnlockType iconType[type];

    auto iconSpr = GJItemIcon::createBrowserItem(
        iconType,
        iconID
    );

    if(gm->isItemUnlocked(iconType, iconID) && !noCheckmark){
        auto checkSpr = CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png");
        checkSpr->setPosition(iconSpr->getContentSize() / 2);
        checkSpr->setScale(0.75f);

        iconSpr->darkenStoreItem({ 30, 30, 30 });
        iconSpr->addChild(checkSpr);
    } else if(iconType == UnlockType::Col1 || iconType == UnlockType::Col2){
        auto text = (iconType == UnlockType::Col1) ? CCLabelBMFont::create("1", "bigFont.fnt") : CCLabelBMFont::create("2", "bigFont.fnt");

        text->setPosition(iconSpr->getContentSize() / 2);
        text->setScale(0.5f);

        iconSpr->addChild(text);
    };

    //  Creates the Icon as a button itself.
    auto iconButton = CCMenuItemSpriteExtra::create(
        iconSpr,
        this,
        handler
    );

        //	Passes the user-defined IconParameters for its callback
    iconButton->setUserObject(new IconParameters(iconType, iconID));

    menu->addChild(iconButton);
    menu->updateLayout();
};

//  Function to add Icons (Recursive version)
template <typename T, typename... ARGS>
void MLUtils::addIcons(CCMenu * menu, cocos2d::SEL_MenuHandler handler, int type, const T &iconID, const ARGS &...args){
    auto noCheckmark = Mod::get()->getSettingValue<bool>("disable-checkmark");
    auto gm = GameStatsManager::sharedState();

    UnlockType iconType[type];

    auto iconSpr = GJItemIcon::createBrowserItem(
        iconType,
        iconID
    );

    if(gm->isItemUnlocked(iconType, iconID) && !noCheckmark){
        auto checkSpr = CCSprite::createWithSpriteFrameName("GJ_completesIcon_001.png");
        checkSpr->setPosition(iconSpr->getContentSize() / 2);
        checkSpr->setScale(0.75f);

        iconSpr->darkenStoreItem({ 30, 30, 30 });
        iconSpr->addChild(checkSpr);
    } else if(iconType == UnlockType::Col1 || iconType == UnlockType::Col2){
        auto text = (iconType == UnlockType::Col1) ? CCLabelBMFont::create("1", "bigFont.fnt") : CCLabelBMFont::create("2", "bigFont.fnt");

        text->setPosition(iconSpr->getContentSize() / 2);
        text->setScale(0.5f);

        iconSpr->addChild(text);
    };

    //  Creates the Icon as a button itself.
    auto iconButton = CCMenuItemSpriteExtra::create(
        iconSpr,
        this,
        handler
    );

        //	Passes the user-defined IconParameters for its callback
    iconButton->setUserObject(new IconParameters(iconType, iconID));

    menu->addChild(iconButton);
    menu->updateLayout();

    addIcons(menu, handler, type, args...);
};