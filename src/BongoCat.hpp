#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class BongoCat : public CCLayer {
public:
	static BongoCat* create();
	void show();

	void bongo();
	void stopBongo();

	CCSprite* m_catSpr;
	CCSprite* m_hatSpr;
	CCSprite* m_decoSpr;

	CCScale9Sprite* m_box;
	CCLabelBMFont* m_counter;

	int m_pawMode;

private:
	void onSettings(CCObject* sender);

	int m_count;

	bool m_alternatePaw;
};