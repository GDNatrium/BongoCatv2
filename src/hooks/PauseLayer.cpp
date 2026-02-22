#include <Geode/Geode.hpp>
#include "../BongoCat.hpp"
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;

class $modify(PauseLayer) {
	void onEdit(CCObject * sender) {
		PauseLayer::onEdit(sender);

		auto cat = OverlayManager::get()->getChildByType<BongoCat>(0);
		if (!cat) return;

		cat->setVisible(true);
	}

	void onQuit(CCObject* sender) {
		PauseLayer::onQuit(sender);

		auto cat = OverlayManager::get()->getChildByType<BongoCat>(0);
		if (!cat) return;

		cat->setVisible(true);
	}
};