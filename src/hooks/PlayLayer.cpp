#include <Geode/Geode.hpp>
#include "../BongoCat.hpp"
#include <Geode/modify/PlayLayer.hpp>

using namespace geode::prelude;

class $modify(PlayLayer) {
	bool init(GJGameLevel * level, bool p1, bool p2) {
		if (!PlayLayer::init(level, p1, p2)) return false;

		auto shouldHide = Mod::get()->getSavedValue<bool>("hideLevel");

		if (shouldHide) {
			auto cat = OverlayManager::get()->getChildByType<BongoCat>(0);
			if (!cat) return true;

			cat->setVisible(false);
		}

		return true;
	}
};