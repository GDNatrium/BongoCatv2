#include "CustomizeMenu.hpp"
#include "Customization.hpp"
#include "BongoCatSettings.hpp"

CustomizeMenu* CustomizeMenu::create(BongoCat* cat) {
    auto ret = new CustomizeMenu();
    ret->m_cat = cat;
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool CustomizeMenu::init() {
    if (!Popup::init(320.f, 250.f)) return false;

    this->setTitle("Bongo Cat Customization");

    m_scrollLayer = CustomizeMenu::createScrollLayer();
    m_scrollLayer->moveToTop();
    m_mainLayer->addChildAtPosition(m_scrollLayer, Anchor::Center, { -m_scrollLayer->getContentWidth() / 2 + 10, -m_scrollLayer->getContentHeight() / 2 - 13 });

    auto borders = ListBorders::create();
    borders->setContentSize(m_scrollLayer->getContentSize());
    borders->setZOrder(1);
    m_mainLayer->addChildAtPosition(borders, Anchor::Center, ccp(0, -13));

    auto prevPageSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    auto prevPageBtn = CCMenuItemSpriteExtra::create(prevPageSpr, this, menu_selector(CustomizeMenu::onChangePage));
    prevPageBtn->setTag(-1);
    m_mainLayer->getChildByType<CCMenu>(0)->addChildAtPosition(prevPageBtn, Anchor::Left, ccp(-30, 0));

    auto nextPageSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    nextPageSpr->setFlipX(true);
    auto nextPageBtn = CCMenuItemSpriteExtra::create(nextPageSpr, this, menu_selector(CustomizeMenu::onChangePage));
    nextPageBtn->setTag(1);
    m_mainLayer->getChildByType<CCMenu>(0)->addChildAtPosition(nextPageBtn, Anchor::Right, ccp(30, 0));

    auto settingsSpr = CCSprite::createWithSpriteFrameName("GJ_optionsBtn02_001.png");
    auto settingsBtn = CCMenuItemSpriteExtra::create(settingsSpr, this, menu_selector(CustomizeMenu::onSettings));
    m_mainLayer->getChildByType<CCMenu>(0)->addChildAtPosition(settingsBtn, Anchor::TopRight, ccp(-5, -5));

    return true;
}

ScrollLayer* CustomizeMenu::createScrollLayer() {
    std::vector<Customization::Customs> items;
    std::string titleTxt;

    switch (m_page) {
    case 1:
        items = Customization::HATS;
        titleTxt = "Hats";
        break;
    case 2:
        items = Customization::DECOS;
        titleTxt = "Decos";
        break;
    default:
        items = Customization::CATS;
        titleTxt = "Cats";
    };

    auto scrollLayer = ScrollLayer::create({ 300, 200 });

    scrollLayer->m_contentLayer->setLayout(
        ColumnLayout::create()
        ->setGap(5)
        ->setAxisReverse(true)
        ->setAxisAlignment(AxisAlignment::End)
        ->setAutoGrowAxis(scrollLayer->getContentHeight())
    );

    RowLayout* layout = RowLayout::create();
    layout->setGap(3);
    layout->setAutoScale(false);
    layout->setGrowCrossAxis(true);
    layout->setCrossAxisOverflow(true);

    auto offSetNodeStart = CCScale9Sprite::create("square02_small.png");
    offSetNodeStart->setOpacity(0);
    offSetNodeStart->setContentSize({ 280, 5 });
    scrollLayer->m_contentLayer->addChildAtPosition(offSetNodeStart, Anchor::Center);

    int rowAmount = (items.size() + 3 - 1) / 3;
    float nodeHeight = rowAmount * 90 + 25;

    auto catSettingsNode = CCScale9Sprite::create("square02_small.png");
    catSettingsNode->setOpacity(50);
    catSettingsNode->setContentSize({ 280, nodeHeight });
    scrollLayer->m_contentLayer->addChildAtPosition(catSettingsNode, Anchor::Center);

    auto title = CCLabelBMFont::create(titleTxt.c_str(), "bigFont.fnt");
    title->setScale(0.6);
    catSettingsNode->addChildAtPosition(title, Anchor::Top, ccp(0, -15));

    RowLayout* mainLayout = RowLayout::create();
    mainLayout->setGap(15);
    mainLayout->setAutoScale(false);
    mainLayout->setGrowCrossAxis(true);
    mainLayout->setCrossAxisOverflow(true);

    auto catMenu = CCMenu::create();
    catMenu->setContentSize({ 280, catSettingsNode->getContentHeight() - 30 });
    catMenu->setLayout(mainLayout);
    catMenu->setAnchorPoint(ccp(0.5, 1));
    catSettingsNode->addChildAtPosition(catMenu, Anchor::Top, ccp(0, -30));

    for (int i = 0; i < items.size(); ++i) {
        auto bg = CCScale9Sprite::create("square02_small.png");
        bg->setOpacity(50);
        bg->setContentSize({ 75, 75 });
        catMenu->addChild(bg);

        auto item = CCSprite::createWithSpriteFrameName(items[i].spriteName);

        auto itemBtn = CCMenuItemSpriteExtra::create(item, this, menu_selector(CustomizeMenu::onChangeItem));
        itemBtn->setTag(i + 1);

        if (m_page != 0) {
            auto defaultSpr = CCSprite::createWithSpriteFrameName("locked.png"_spr);
            defaultSpr->setZOrder(-1);
            itemBtn->addChildAtPosition(defaultSpr, Anchor::Center);
        }

        auto name = CCLabelBMFont::create(items[i].name.c_str(), "goldFont.fnt");
        name->limitLabelWidth(55, 0.5, 0.1);
        bg->addChildAtPosition(name, Anchor::Bottom, ccp(0, 15));

        auto itemMenu = CCMenu::create();
        itemMenu->setContentSize(bg->getContentSize());
        bg->addChildAtPosition(itemMenu, Anchor::Center);
        itemMenu->addChildAtPosition(itemBtn, Anchor::Center);
    }

    catMenu->updateLayout();

    auto offSetNodeEnd = CCScale9Sprite::create("square02_small.png");
    offSetNodeEnd->setOpacity(0);
    offSetNodeEnd->setContentSize({ 280, 5 });
    scrollLayer->m_contentLayer->addChildAtPosition(offSetNodeEnd, Anchor::Center);

    scrollLayer->m_contentLayer->updateLayout();

    return scrollLayer;
}

void CustomizeMenu::onChangeItem(CCObject* sender) {
    auto tag = sender->getTag() - 1;

    std::vector<Customization::Customs> items;
    CCSprite* catPart;

    switch (m_page) {
    case 1:
        items = Customization::HATS;
        catPart = m_cat->m_hatSpr;
        Mod::get()->setSavedValue<int>("hat", tag + 1);
        break;
    case 2:
        items = Customization::DECOS;
        catPart = m_cat->m_decoSpr;
        Mod::get()->setSavedValue<int>("deco", tag + 1);
        break;
    default:
        items = Customization::CATS;
        catPart = m_cat->m_catSpr;
        Mod::get()->setSavedValue<int>("cat", tag + 1);
    };

    if (tag == 0 && m_page != 0) {
        catPart->setVisible(false);
    }
    else {
        catPart->setVisible(true);
        auto frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(items[tag].spriteName);
        catPart->setDisplayFrame(frame);
    }
}

void CustomizeMenu::onChangePage(CCObject* sender) {
    m_page = (m_page + sender->getTag() + 3) % 3;

    m_scrollLayer->removeFromParentAndCleanup(true);

    m_scrollLayer = createScrollLayer();
    m_scrollLayer->moveToTop();
    m_mainLayer->addChildAtPosition(m_scrollLayer, Anchor::Center, { -m_scrollLayer->getContentWidth() / 2 + 10, -m_scrollLayer->getContentHeight() / 2 - 13 });
}

void CustomizeMenu::onSettings(CCObject* sender) {
    BongoCatSettings::create()->show();
}