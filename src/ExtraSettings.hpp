#pragma once

#include <Geode/Geode.hpp>
#include "BongoCatSettings.hpp"

using namespace geode::prelude;

class BongoSettings : public SettingBaseValue<int> {
public:
	static Result<std::shared_ptr<SettingV3>> parse(std::string const&, std::string const&, matjson::Value const&) {
		return Ok(std::make_shared<BongoSettings>());
	};
	SettingNode* createNode(float width) override;
};

class ExtraBongoSettingNode : public SettingValueNode<BongoSettings> {
protected:
	bool init(std::shared_ptr<BongoSettings>& setting, float width) {
		if (!SettingValueNodeV3::init(setting, width)) return false;

		this->setContentSize({ width, 40.f });

		auto sprite = ButtonSprite::create("Open Settings", 0, false, "bigFont.fnt", "GJ_button_04.png", 24.5f, 0.4f);
		auto btn = CCMenuItemSpriteExtra::create(sprite, this, menu_selector(ExtraBongoSettingNode::onOpen));
		auto menu = CCMenu::create();
		menu->setPosition({ width / 2, 20.f });
		menu->addChild(btn);
		this->addChild(menu);

		return true;
	}

	void onOpen(CCObject*) {
		BongoCatSettings::create()->show();
	}

public:
	static ExtraBongoSettingNode* create(std::shared_ptr<BongoSettings> value, float width) {
		auto ret = new ExtraBongoSettingNode();
		if (ret && ret->init(value, width)) {
			ret->autorelease();
			return ret;
		}
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
};

SettingNode* BongoSettings::createNode(float width) {
	return ExtraBongoSettingNode::create(std::static_pointer_cast<BongoSettings>(shared_from_this()), width);
}