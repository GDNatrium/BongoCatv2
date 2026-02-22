#include <Geode/Geode.hpp>
#include "BongoCat.hpp"
#include "ExtraSettings.hpp"

using namespace geode::prelude;

$on_game(TexturesLoaded) {
    BongoCat::create()->show();
}

$execute{
	(void)Mod::get()->registerCustomSettingType("open-menu", &BongoSettings::parse);
}