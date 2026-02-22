#include "BongoCatSettings.hpp"

BongoCatSettings* BongoCatSettings::create() {
    auto ret = new BongoCatSettings();
    if (ret->init()) {
        ret->autorelease();
        return ret;
    }

    delete ret;
    return nullptr;
}

bool BongoCatSettings::init() {
    if (!Popup::init(320.f, 250.f)) return false;

    auto cat = OverlayManager::get()->getChildByType<BongoCat>(0);
    if (!cat) return true;

    m_cat = cat;

    this->setTitle("Bongo Cat Settings");

    auto list = BongoCatSettings::createScrollLayer();
    list->moveToTop();
    m_mainLayer->addChildAtPosition(list, Anchor::Center, { -list->getContentWidth() / 2, -list->getContentHeight() / 2 - 13 });

    auto borders = ListBorders::create();
    borders->setContentSize(list->getContentSize());
    m_mainLayer->addChildAtPosition(borders, Anchor::Center, ccp(0, -13));

    return true;
}

CCLabelBMFont* BongoCatSettings::createLabel(char const* txt, bool leftAlign) {
    auto label = CCLabelBMFont::create(txt, "bigFont.fnt");
    label->limitLabelWidth(100, 0.5, 0.1);
    if (leftAlign) label->setAnchorPoint(ccp(0, 0.5));
    return label;
}

Slider* BongoCatSettings::createSlider(SEL_MenuHandler menu, float val) {
    auto slider = Slider::create(this, menu);
    slider->setValue(val);
    slider->updateBar();
    return slider;
}

CCMenuItemSpriteExtra* BongoCatSettings::createResetButton() {
    auto resetSpr = CCSprite::createWithSpriteFrameName("GJ_getSongInfoBtn_001.png");
    resetSpr->setScale(0.75);
    auto resetBtn = CCMenuItemSpriteExtra::create(resetSpr, this, menu_selector(BongoCatSettings::onReset));
    return resetBtn;
}

TextInput* BongoCatSettings::createTextInput(char const* placeholder, std::function<void(const std::string&)> callback) {
    auto input = TextInput::create(75, placeholder);
    input->setCommonFilter(CommonFilter::Float);
    input->setMaxCharCount(5);
    input->getBGSprite()->setScale(0.35);
    input->setCallback(callback);
    input->getInputNode()->setAnchorPoint(ccp(0, 0));
    input->getInputNode()->setScale(0.83);
    return input;
}

ScrollLayer* BongoCatSettings::createScrollLayer() {
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    auto scrollLayer = ScrollLayer::create({ 300, 200 });

    scrollLayer->m_contentLayer->setLayout(
        ColumnLayout::create()
        ->setGap(5)
        ->setAxisReverse(true)
        ->setAxisAlignment(AxisAlignment::End)
        ->setAutoGrowAxis(scrollLayer->getContentHeight())
    );

    auto node = CCNode::create();
    node->setContentSize({ 300, 420 });

    auto bg = CCLayerColor::create(ccc4(0, 0, 0, 90), node->getContentWidth(), node->getContentHeight());
    bg->ignoreAnchorPointForPosition(false);
    node->addChildAtPosition(bg, Anchor::Center);

    scrollLayer->m_contentLayer->addChild(node);

    auto menu = CCMenu::create();
    menu->setContentSize({ 300, 290 });
    menu->setAnchorPoint(ccp(0.5, 1));
    node->addChildAtPosition(menu, Anchor::Top, ccp(0, -10));

    // --- Scale ---
    menu->addChildAtPosition(createLabel("Scale"), Anchor::Top, ccp(-20, -10));

    auto currentScale = m_cat->getScale();

    auto scaleSliderText = (currentScale - 0.25) / 4.75;
    m_scaleSlider = createSlider(menu_selector(BongoCatSettings::onScaleChange), scaleSliderText);
    menu->addChildAtPosition(m_scaleSlider, Anchor::Top, ccp(-20, -35));

    auto scaleText = std::to_string(currentScale).substr(0, 5);
    m_scaleInput = createTextInput(scaleText.c_str(), [this](const std::string& text) {
        auto scale = numFromString<float>(text).unwrapOr(0.01);
        if (scale < 0.01) scale = 0.01;
        if (scale > 5.f) scale = 5.f;

        m_scaleSlider->setValue((scale - 0.25) / 4.75);

        m_cat->setScale(scale);
    });
    menu->addChildAtPosition(m_scaleInput, Anchor::Top, ccp(35, -11));

    auto scaleReset = createResetButton();
    scaleReset->setTag(1);
    menu->addChildAtPosition(scaleReset, Anchor::Top, ccp(120, -35));

    // --- Pos X ---
    menu->addChildAtPosition(createLabel("Pos X", false), Anchor::Top, ccp(-20, -65));

    auto currentPosX = m_cat->getPositionX();
    auto currentPosXPercent = 100 * currentPosX / winSize.width;

    m_posXSlider = createSlider(menu_selector(BongoCatSettings::onPosXChange), currentPosXPercent / 100);
    menu->addChildAtPosition(m_posXSlider, Anchor::Top, ccp(-20, -90));

    int percentageX = static_cast<int>(currentPosXPercent + 0.5f);
    auto posXText = std::to_string(percentageX) + "%";

    m_posXInput = createTextInput(posXText.c_str(), [this, winSize](const std::string& text) {
        auto posX = numFromString<float>(text).unwrapOr(0.0);
        if (posX < 0.0) posX = 0.0;
        if (posX > 100) posX = 100;

        m_posXSlider->setValue(posX / 100);

        auto actualPos = posX * winSize.width / 100;
        m_cat->setPositionX(actualPos);
    });
    menu->addChildAtPosition(m_posXInput, Anchor::Top, ccp(35, -66));

    auto posXReset = createResetButton();
    posXReset->setTag(2);
    menu->addChildAtPosition(posXReset, Anchor::Top, ccp(120, -90));

    // --- Pos Y ---
    menu->addChildAtPosition(createLabel("Pos Y", false), Anchor::Top, ccp(-20, -120));

    auto currentPosY = m_cat->getPositionY();
    auto currentPosYPercent = 100 * currentPosY / winSize.height;

    m_posYSlider = createSlider(menu_selector(BongoCatSettings::onPosYChange), currentPosYPercent / 100);
    menu->addChildAtPosition(m_posYSlider, Anchor::Top, ccp(-20, -145));

    int percentageY = static_cast<int>(currentPosYPercent + 0.5f);
    auto posYText = std::to_string(percentageY) + "%";
    m_posYInput = createTextInput(posYText.c_str(), [this, winSize](const std::string& text) {
        auto posY = numFromString<float>(text).unwrapOr(0.0);
        if (posY < 0.0) posY = 0.0;
        if (posY > 100) posY = 100;

        m_posYSlider->setValue(posY / 100);

        auto actualPos = posY * winSize.height / 100;
        m_cat->setPositionY(actualPos);
    });
    menu->addChildAtPosition(m_posYInput, Anchor::Top, ccp(35, -121));

    auto posYReset = createResetButton();
    posYReset->setTag(3);
    menu->addChildAtPosition(posYReset, Anchor::Top, ccp(120, -145));

    // Flip X
    menu->addChildAtPosition(createLabel("Flip X", true), Anchor::Top, ccp(-105, -185));

    auto checkBoxSpriteOff = CCSprite::createWithSpriteFrameName("GJ_checkOff_001.png");
    checkBoxSpriteOff->setScale(0.6f);
    auto checkBoxSpriteOn = CCSprite::createWithSpriteFrameName("GJ_checkOn_001.png");
    checkBoxSpriteOn->setScale(0.6f);
    auto checkBoxButton = CCMenuItemToggler::create(checkBoxSpriteOff, checkBoxSpriteOn, this, menu_selector(BongoCatSettings::onFlipX));

    menu->addChildAtPosition(checkBoxButton, Anchor::Top, ccp(-120, -185));
    if(Mod::get()->getSavedValue<bool>("isFlipped")) checkBoxButton->toggle(true);

    // Hide in Level
    menu->addChildAtPosition(createLabel("Hide in level", true), Anchor::Top, ccp(30, -185));
    auto hideInLevelCheck = CCMenuItemToggler::create(checkBoxSpriteOff, checkBoxSpriteOn, this, menu_selector(BongoCatSettings::onHideLevel));

    menu->addChildAtPosition(hideInLevelCheck, Anchor::Top, ccp(15, -185));
    if (Mod::get()->getSavedValue<bool>("hideLevel")) hideInLevelCheck->toggle(true);

    // Paw selection
    auto pawMode = Mod::get()->getSavedValue<int>("pawMode", 0);
    std::string text;
    switch (pawMode) {
    case 1:
        text = "Only Left";
        break;
    case 2:
        text = "Only Right";
        break;
    default:
        text = "Both";
    }

    menu->addChildAtPosition(createLabel("Paws To Use", false), Anchor::Top, ccp(0, -220));

    m_pawLabel = CCLabelBMFont::create(text.c_str(), "goldFont.fnt");
    menu->addChildAtPosition(m_pawLabel, Anchor::Top, ccp(0, -245));

    auto nextSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    nextSpr->setFlipX(true);
    nextSpr->setScale(0.7);
    auto nextBtn = CCMenuItemSpriteExtra::create(nextSpr, this, menu_selector(BongoCatSettings::onSwitchPaws));
    nextBtn->setTag(1);
    menu->addChildAtPosition(nextBtn, Anchor::Top, ccp(100, -245));

    auto backSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    backSpr->setScale(0.7);
    auto backBtn = CCMenuItemSpriteExtra::create(backSpr, this, menu_selector(BongoCatSettings::onSwitchPaws));
    backBtn->setTag(-1);
    menu->addChildAtPosition(backBtn, Anchor::Top, ccp(-100, -245));

    // BG selection
    auto bgID = Mod::get()->getSavedValue<int>("bg", 1);
    auto texture = fmt::format("GJ_square{:02d}.png", bgID);
    menu->addChildAtPosition(createLabel("Background", false), Anchor::Top, ccp(0, -285));

    m_bg = CCScale9Sprite::create(texture.c_str());
    m_bg->setContentSize({ 160, 44 });
    m_bg->setScale(0.5);
    menu->addChildAtPosition(m_bg, Anchor::Top, ccp(0, -310));

    auto nextSprBG = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    nextSprBG->setFlipX(true);
    nextSprBG->setScale(0.7);
    auto nextBtnBG = CCMenuItemSpriteExtra::create(nextSprBG, this, menu_selector(BongoCatSettings::onSwitchBG));
    nextBtnBG->setTag(1);
    menu->addChildAtPosition(nextBtnBG, Anchor::Top, ccp(100, -310));

    auto backSprBG = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    backSprBG->setScale(0.7);
    auto backBtnBG = CCMenuItemSpriteExtra::create(backSprBG, this, menu_selector(BongoCatSettings::onSwitchBG));
    backBtnBG->setTag(-1);
    menu->addChildAtPosition(backBtnBG, Anchor::Top, ccp(-100, -310));

    // Font Selection
    auto font = Mod::get()->getSavedValue<int>("font", 0);
    std::string fntFile = "bigFont.fnt";
    if (font > 0 && font < 60) fntFile = fmt::format("gjFont{:02d}.fnt", font);
    if (font == 60) fntFile = "goldFont.fnt";
    if (font == 61) fntFile = "chatFont.fnt";

    menu->addChildAtPosition(createLabel("Font", false), Anchor::Top, ccp(0, -350));

    m_font = CCLabelBMFont::create(fmt::format("Font {}", font + 1).c_str(), fntFile.c_str());
    m_font->setScale(0.7);
    menu->addChildAtPosition(m_font, Anchor::Top, ccp(0, -375));

    auto nextSprFnt = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    nextSprFnt->setFlipX(true);
    nextSprFnt->setScale(0.7);
    auto nextBtnFnt = CCMenuItemSpriteExtra::create(nextSprFnt, this, menu_selector(BongoCatSettings::onSwitchFont));
    nextBtnFnt->setTag(1);
    menu->addChildAtPosition(nextBtnFnt, Anchor::Top, ccp(100, -375));

    auto backSprFnt = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
    backSprFnt->setScale(0.7);
    auto backBtnFnt = CCMenuItemSpriteExtra::create(backSprFnt, this, menu_selector(BongoCatSettings::onSwitchFont));
    backBtnFnt->setTag(-1);
    menu->addChildAtPosition(backBtnFnt, Anchor::Top, ccp(-100, -375));


    scrollLayer->m_contentLayer->updateLayout();

    return scrollLayer;
}

void BongoCatSettings::onScaleChange(CCObject* sender) {
    auto value = static_cast<SliderThumb*>(sender)->getValue();
    auto newScale = 0.25 + value * 4.75;

    m_cat->setScale(newScale);

    m_scaleInput->setString(std::to_string(newScale).substr(0, 5));

    Mod::get()->setSavedValue<float>("scale", newScale);
}

void BongoCatSettings::onPosXChange(CCObject* sender) {
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    float value = static_cast<SliderThumb*>(sender)->getValue();
    float newX = value * winSize.width;
    m_cat->setPositionX(newX);

    int percentage = static_cast<int>(value * 100.0f + 0.5f);

    m_posXInput->setString((std::to_string(percentage) + "%").c_str());

    Mod::get()->setSavedValue<float>("posX", newX);
}

void BongoCatSettings::onPosYChange(CCObject* sender) {
    auto winSize = CCDirector::sharedDirector()->getWinSize();

    float value = static_cast<SliderThumb*>(sender)->getValue();
    float newY = value * winSize.height;

    m_cat->setPositionY(newY);

    int percentage = static_cast<int>(value * 100.0f + 0.5f);

    m_posYInput->setString((std::to_string(percentage) + "%").c_str());

    Mod::get()->setSavedValue<float>("posY", newY);
}

void BongoCatSettings::onFlipX(CCObject* sender) {
    auto toggle = static_cast<CCMenuItemToggler*>(sender);
    bool isOn = !toggle->m_toggled;

    m_cat->getChildByType<CCSprite>(0)->setFlipX(isOn);
    m_cat->getChildByType<CCSprite>(1)->setFlipX(isOn);
    m_cat->getChildByType<CCSprite>(2)->setFlipX(isOn);

    Mod::get()->setSavedValue<bool>("isFlipped", isOn);
}

void BongoCatSettings::onHideLevel(CCObject* sender) {
    auto toggle = static_cast<CCMenuItemToggler*>(sender);
    bool isOn = !toggle->m_toggled;

    Mod::get()->setSavedValue<bool>("hideLevel", isOn);
}

void BongoCatSettings::onReset(CCObject* sender) {
    // 1: Scale, 2: PosX, 3: PosY
    auto id = sender->getTag();

    if (id == 1) {
        m_cat->setScale(1);
        m_scaleInput->setString("1.000");
        m_scaleSlider->setValue((1 - 0.25) / 4.75);
        Mod::get()->setSavedValue<float>("scale", 1);
    }

    if (id == 2) {
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto newPos = winSize.width - 45;

        m_cat->setPositionX(newPos);

        int percentage = static_cast<int>((newPos / winSize.width) * 100.0f + 0.5f);
        m_posXInput->setString((std::to_string(percentage) + "%").c_str());

        m_posXSlider->setValue(newPos / winSize.width);
        Mod::get()->setSavedValue<float>("posX", newPos);
    }

    if (id == 3) {
        auto winSize = CCDirector::sharedDirector()->getWinSize();
        auto newPos = 45;

        m_cat->setPositionY(newPos);

        int percentage = static_cast<int>((newPos / winSize.height) * 100.0f + 0.5f);
        m_posYInput->setString((std::to_string(percentage) + "%").c_str());

        m_posYSlider->setValue(newPos / winSize.height);
        Mod::get()->setSavedValue<float>("posY", newPos);
    }
}

void BongoCatSettings::onSwitchPaws(CCObject* sender) {
    // 0 = Both, 1 = Only Left, 2 = Only Right
    auto pawMode = Mod::get()->getSavedValue<int>("pawMode", 0);

    int delta = sender->getTag();
    pawMode = (pawMode + delta + 3) % 3;

    switch (pawMode) {
    case 1:
        m_pawLabel->setString("Only Left");
        break;
    case 2:
        m_pawLabel->setString("Only Right");
        break;
    default:
        m_pawLabel->setString("Both");
    }

    m_cat->m_pawMode = pawMode;

    Mod::get()->setSavedValue("pawMode", pawMode);
}

void BongoCatSettings::onSwitchBG(CCObject* sender) {
    auto bg = Mod::get()->getSavedValue<int>("bg", 1);

    int delta = sender->getTag();
    bg = ((bg - 1 + delta + 5) % 5) + 1;

    auto contentSize = m_cat->m_box->getContentSize();

    auto texture = fmt::format("GJ_square{:02d}.png", bg);
    auto sprFrame = CCSpriteFrame::create(texture.c_str(), {0, 0, 80, 80});
    m_bg->setSpriteFrame(sprFrame);
    m_bg->setContentSize(contentSize);

    m_cat->m_box->setSpriteFrame(sprFrame);
    m_cat->m_box->setContentSize(contentSize);

    Mod::get()->setSavedValue("bg", bg);
}

// 0 = Pusab, 60 = goldFont, 61 = chatFont
void BongoCatSettings::onSwitchFont(CCObject* sender) {
    auto font = Mod::get()->getSavedValue<int>("font", 0);

    int delta = sender->getTag();
    font = (font + delta + 62) % 62;

    m_font->setString(fmt::format("Font {}", font + 1).c_str());

    std::string fntFile = "bigFont.fnt";
    if (font > 0 && font < 60) fntFile = fmt::format("gjFont{:02d}.fnt", font);
    if (font == 60) fntFile = "goldFont.fnt";
    if (font == 61) fntFile = "chatFont.fnt";

    m_font->setFntFile(fntFile.c_str());
    m_cat->m_counter->setFntFile(fntFile.c_str());

    Mod::get()->setSavedValue("font", font);
}