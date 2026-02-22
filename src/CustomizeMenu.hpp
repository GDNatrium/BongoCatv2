#pragma once

#include <Geode/Geode.hpp>
#include "BongoCat.hpp"

using namespace geode::prelude;

class CustomizeMenu : public Popup {
public:
	static CustomizeMenu* create(BongoCat* cat);

protected:
	bool init();

	ScrollLayer* createScrollLayer();

	void onChangeItem(CCObject* sender);
	void onSettings(CCObject* sender);
	void onChangePage(CCObject* sender);

private:
	BongoCat* m_cat;

	int m_page = 0;

	ScrollLayer* m_scrollLayer;
};