#include "BongoCat.hpp"
#include "CustomizeMenu.hpp"

BongoCat* BongoCat::create() {
	auto cat = OverlayManager::get()->getChildByID("Bongo Cat");
	if (cat) cat->removeFromParentAndCleanup(true);

	auto ret = new BongoCat();
	if (ret->init()) {
		ret->autorelease();
		return ret;
	}

	CC_SAFE_DELETE(ret);
	return nullptr;
}

void BongoCat::show() {
	auto winSize = CCDirector::sharedDirector()->getWinSize();

	OverlayManager::get()->addChild(this);
	this->setAnchorPoint(ccp(0, 0));
	this->setID("Bongo Cat");

	m_pawMode = Mod::get()->getSavedValue<int>("pawMode", 0);
	auto scale = Mod::get()->getSavedValue<float>("scale", 1);
	this->setScale(scale);

	auto posX = Mod::get()->getSavedValue<float>("posX", winSize.width - 45);
	auto posY = Mod::get()->getSavedValue<float>("posY", 45);
	this->setPosition(ccp(posX, posY));

	this->addEventListener(KeyboardInputEvent(), [this](KeyboardInputData& event) {
		if (event.action == KeyboardInputData::Action::Press) {
			bongo();
		}

		if (event.action == KeyboardInputData::Action::Release) {
			stopBongo();
		}
	});

	CCSize boxSize = { 160, 44 };

	auto bg = Mod::get()->getSavedValue<int>("bg", 1);
	auto texture = fmt::format("GJ_square{:02d}.png", bg);
	m_box = CCScale9Sprite::create(texture.c_str());
	m_box->setContentSize(boxSize);
	m_box->setScale(0.5);
	m_box->setPositionY(-23.5);
	this->addChild(m_box);

	auto shadow = NineSlice::create("square02_small.png");
	shadow->setOpacity(100);
	shadow->setContentSize({ boxSize.width - 44, boxSize.height - 14 });
	shadow->setPosition(ccp(-8.25, -23.5));
	shadow->setScale(0.5);
	this->addChild(shadow);

	auto optsMenu = CCMenu::create();
	optsMenu->giveMenuTouchPriority();
	optsMenu->setPosition(ccp(29.5, -23.5));
	optsMenu->setAnchorPoint(ccp(0, 0));
	optsMenu->setScale(0.5);
	this->addChild(optsMenu);

	auto optsSpr = CCSprite::createWithSpriteFrameName("accountBtn_settings_001.png");
	optsSpr->setScale(0.75);
	auto optsBtn = CCMenuItemSpriteExtra::create(optsSpr, this, menu_selector(BongoCat::onSettings));
	optsMenu->addChild(optsBtn);

	m_count = Mod::get()->getSavedValue<int>("count", 0);

	auto font = Mod::get()->getSavedValue<int>("font", 0);
	std::string fntFile = "bigFont.fnt";
	if (font > 0 && font < 60) fntFile = fmt::format("gjFont{:02d}.fnt", font);
	if (font == 60) fntFile = "goldFont.fnt";
	if (font == 61) fntFile = "chatFont.fnt";
	m_counter = CCLabelBMFont::create(std::to_string(m_count).c_str(), fntFile.c_str());
	m_counter->setAnchorPoint(ccp(0, 0.45));
	m_counter->limitLabelWidth(104.f, 1, 0.1);
	m_counter->setPosition(ccp(-36, -23.5));
	m_counter->setScale(0.5);
	this->addChild(m_counter);

	bool flipX = Mod::get()->getSavedValue<bool>("isFlipped", false);

	auto cat = Mod::get()->getSavedValue<int>("cat", 1);
	m_catSpr = CCSprite::createWithSpriteFrameName(fmt::format("cat{}_1.png"_spr, cat).c_str());
	if (!m_catSpr) {
		cat = 1;
		Mod::get()->setSavedValue("cat", 1);
		m_catSpr = CCSprite::createWithSpriteFrameName(fmt::format("cat1_1.png"_spr, cat).c_str());
	}
	m_catSpr->setFlipX(flipX);
	this->addChild(m_catSpr);

	auto hat = Mod::get()->getSavedValue<int>("hat", 1);
	m_hatSpr = CCSprite::createWithSpriteFrameName(fmt::format("hat{}.png"_spr, hat - 1).c_str());
	if (!m_hatSpr) {
		hat = 1;
		Mod::get()->setSavedValue("hat", 1);
		m_hatSpr = CCSprite::createWithSpriteFrameName(fmt::format("hat1.png"_spr, hat - 1).c_str());
	}
	m_hatSpr->setFlipX(flipX);
	this->addChild(m_hatSpr);

	if (hat == 1) {
		m_hatSpr->setVisible(false);
	}

	auto deco = Mod::get()->getSavedValue<int>("deco", 1);
	m_decoSpr = CCSprite::createWithSpriteFrameName(fmt::format("deco{}.png"_spr, deco - 1).c_str());
	if (!m_decoSpr) {
		deco = 1;
		Mod::get()->setSavedValue("deco", 1);
		m_decoSpr = CCSprite::createWithSpriteFrameName(fmt::format("deco1.png"_spr, deco - 1).c_str());
	}
	m_decoSpr->setFlipX(flipX);
	this->addChild(m_decoSpr);

	if (deco == 1) {
		m_decoSpr->setVisible(false);
	}
}

void BongoCat::onSettings(CCObject* sender) {
	if (CCDirector::sharedDirector()->getRunningScene()->getChildByType<CustomizeMenu>(0)) return;
	CustomizeMenu::create(this)->show();
}

void BongoCat::bongo() {
	m_count++;

	m_counter->setString(std::to_string(m_count).c_str());
	Mod::get()->setSavedValue<int>("count", m_count);

	auto cat = Mod::get()->getSavedValue<int>("cat", 1);
	auto spr = m_alternatePaw ? fmt::format("cat{}_2.png"_spr, cat) : fmt::format("cat{}_3.png"_spr, cat);
	if (m_pawMode == 1) spr = fmt::format("cat{}_3.png"_spr, cat);
	if (m_pawMode == 2) spr = fmt::format("cat{}_2.png"_spr, cat);

	auto frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(spr.c_str());
	m_catSpr->setDisplayFrame(frame);

	m_alternatePaw = !m_alternatePaw;
}

void BongoCat::stopBongo() {
	auto cat = Mod::get()->getSavedValue<int>("cat", 1);
	auto frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(fmt::format("cat{}_1.png"_spr, cat).c_str());
	m_catSpr->setDisplayFrame(frame);
}