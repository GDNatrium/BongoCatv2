#include <Geode/Geode.hpp>
#include "../BongoCat.hpp"
#include <Geode/modify/GJBaseGameLayer.hpp>

using namespace geode::prelude;

// Have to modify now that playing inputs take priority over the bongo KeyboardInputEvent
class $modify(GJBaseGameLayer) {
	void handleButton(bool down, int button, bool isPLayer1) {
		GJBaseGameLayer::handleButton(down, button, isPLayer1);

		auto cat = OverlayManager::get()->getChildByType<BongoCat>(0);
		if (!cat) return;

		down ? cat->bongo() : cat->stopBongo();
	}
};