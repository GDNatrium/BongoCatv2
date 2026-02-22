#pragma once

namespace Customization {
	struct Customs {
		std::string name;
		const char* spriteName;
	};

	inline std::vector<Customs> CATS {
		{ "Bongo", "cat1_2.png"_spr },
		{ "Tabby", "cat2_2.png"_spr },
		{ "Orange", "cat3_2.png"_spr },
		{ "Gray", "cat4_2.png"_spr },
		{ "Luna", "cat5_2.png"_spr },
		{ "Fox", "cat6_2.png"_spr },
		{ "Panda", "cat7_2.png"_spr },
	};

	inline std::vector<Customs> HATS {
		{ "None", "hat0.png"_spr },
		{ "Party Hat", "hat1.png"_spr },
		{ "Beanie", "hat2.png"_spr },
		{ "Chef Hat", "hat3.png"_spr },
		{ "Sombrero", "hat4.png"_spr },
		{ "Top Hat", "hat5.png"_spr },
		{ "Witch", "hat6.png"_spr },
		{ "Crown", "hat7.png"_spr },
		{ "Leaf", "hat8.png"_spr },
		{ "Bongo", "hat9.png"_spr },
		{ "Bread", "hat10.png"_spr },
		{ "Strawberry", "hat11.png"_spr },
		{ "UFO", "hat12.png"_spr },
		{ "Orange", "hat13.png"_spr },
		{ "Snowman", "hat14.png"_spr },
		{ "Santa", "hat15.png"_spr },
		{ "Cherry", "hat16.png"_spr },
		{ "Player", "hat17.png"_spr },
		{ "Monster", "hat18.png"_spr },
		{ "Unicorn", "hat19.png"_spr },
	};

	inline std::vector<Customs> DECOS {
		{ "None", "deco0.png"_spr },
		{ "Bowtie", "deco1.png"_spr },
		{ "CAT", "deco2.png"_spr },
		{ "Moustache", "deco3.png"_spr },
		{ "Drink", "deco4.png"_spr },
		{ "Bubble Gum", "deco5.png"_spr },
		{ "Monocle", "deco6.png"_spr },
		{ "Angry", "deco7.png"_spr },
		{ "Tear", "deco8.png"_spr },
		{ "Annoyed", "deco9.png"_spr },
		{ "Name Plate", "deco10.png"_spr },
	};
};
