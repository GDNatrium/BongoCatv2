#pragma once

#include <Geode/Geode.hpp>
#include "BongoCat.hpp"

using namespace geode::prelude;

class BongoCatSettings : public Popup {
public:
	static BongoCatSettings* create();

protected:
	bool init();

private:
	BongoCat* m_cat;
	CCLabelBMFont* m_pawLabel;
	CCScale9Sprite* m_bg;
	CCLabelBMFont* m_font;

	TextInput* m_scaleInput;
	TextInput* m_posXInput;
	TextInput* m_posYInput;

	Slider* m_scaleSlider;
	Slider* m_posXSlider;
	Slider* m_posYSlider;

	ScrollLayer* createScrollLayer();

	CCLabelBMFont* createLabel(char const* txt, bool leftAlign = false);
	Slider* createSlider(SEL_MenuHandler menu, float val);
	CCMenuItemSpriteExtra* createResetButton();
	TextInput* createTextInput(char const* placeholder, std::function<void(const std::string&)> callback);

	void onScaleChange(CCObject* sender);
	void onPosXChange(CCObject* sender);
	void onPosYChange(CCObject* sender);
	void onFlipX(CCObject* sender);
	void onHideLevel(CCObject* sender);

	void onSwitchPaws(CCObject* sender);
	void onSwitchBG(CCObject* sender);
	void onSwitchFont(CCObject* sender);

	void onReset(CCObject* sender);
};