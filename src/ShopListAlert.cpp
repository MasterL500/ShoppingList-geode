#include "ShopListAlert.hpp"

bool ShopListAlert::setup(){
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    this->setTitle("Shop List");

    this->m_noElasticity = true;
    return true;
};

CCMenu * ShopListAlert::createIconPage(int ID, bool isVisible){
    auto winSize = CCDirector::sharedDirector()->getWinSize();
    auto menu = CCMenu::create();

    menu->setContentSize({520.f, 220.f});
    menu->setID("icon-page-" + std::to_string(ID));
    menu->setLayout(RowLayout::create()
        ->setGap(13.0f)
        ->setGrowCrossAxis(true)
        ->setCrossAxisOverflow(false)
        ->setCrossAxisLineAlignment(AxisAlignment::Even)
	);

    menu->setVisible(isVisible);
    menu->setPositionY(winSize.height / 2 + 7.5f);
    menu->setScale(0.75f);
    
    return menu;
};

ShopListAlert * ShopListAlert::create() {
    auto ret = new ShopListAlert();

    if (ret && ret->init(420.f, 260.f)){
        ret->autorelease();
        return ret;
    };

    CC_SAFE_DELETE(ret);
    return nullptr;
};