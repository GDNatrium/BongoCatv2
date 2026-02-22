#include <Geode/Geode.hpp>
#include "../BongoCat.hpp"
#include <Geode/modify/CCTouchDispatcher.hpp>

using namespace geode::prelude;

class $modify(CCTouchDispatcher) {
    void touchesBegan(CCSet * touches, CCEvent * event) {
        CCTouchDispatcher::touchesBegan(touches, event);

        auto cat = OverlayManager::get()->getChildByType<BongoCat>(0);
        if (!cat) return;

        cat->bongo();
    }

    void touchesEnded(CCSet * touches, CCEvent * event) {
        CCTouchDispatcher::touchesEnded(touches, event);

        auto cat = OverlayManager::get()->getChildByType<BongoCat>(0);
        if (!cat) return;

        cat->stopBongo();
    }
};