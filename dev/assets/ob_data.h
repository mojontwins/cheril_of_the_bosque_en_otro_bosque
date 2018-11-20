// This file contains where characters are in the map.

	const unsigned char ob_char_n_pants [] = {
		15, 5, 30, 4, 29, 20
	};

	const unsigned char ob_char_yx [] = {
		0x4c, 0xa5, 0x44, 0x2d, 0x1c, 0x48
	};
	
// Scripts

// These texts are not referenced as they are used
// directly:

// Indexes 0-5: character names.
// Index 6 "THIS IS..."
// Index 7, "YOU MUST TALK TO..."
// Index 8, "YOU ARE DONE WITH US!"
// Index 9, "NOW COLLECT THE NUTS!"

// Script for Johnny Limite

	const unsigned char script_0 [] = {
		0,
		10, 11, 0, 0,
		0,
		12, 13, 14, 15,
		0,
		16, 17, 18, 19,
		0xfe
	};

	// Script for Ramire the Vampire

	const unsigned char script_1 [] = {
		1,
		20, 21, 22, 0,
		83,
		23, 24, 25, 0,
		1,
		26, 27, 28, 29,
		83,
		30, 31, 0, 0,
		1,
		32, 33, 0, 0,
		0xfe
	};

	// Script for Amador
	 
	const unsigned char script_2 [] = {
		2, 
		34, 35, 36, 37,
		83,
		38, 39, 0, 0,
		2,
		40, 41, 42, 0,
		83,
		43, 44, 45, 46,
		0xfe
	};

	// Script for Yun

	const unsigned char script_3 [] = {
		3,
		47, 48, 0, 0,
		83,
		49, 50, 0, 0,
		3,
		51, 52, 53, 54,
		83,
		55, 56, 0, 0,
		3,
		57, 58, 59, 0,
		0xfe
	};

	// Script for Sir Ababol

	const unsigned char script_4 [] = {
		4, 
		60, 61, 62, 0,
		83,
		63, 0, 0, 0,
		4,
		64, 65, 66, 67,
		83,
		68, 0, 0, 0,
		4,
		69, 70, 0, 0,
		83,
		71, 0, 0, 0,
		0xfe
	};

	// Script for Lala

	const unsigned char script_5 [] = {
		5,
		72, 73, 0, 0,
		83,
		74, 75, 76, 0,
		5,
		77, 78, 79, 80,
		83,
		81, 82, 0, 0,
		0xfe
	};

// Scripts index

	const unsigned char *scripts [] = {
		script_0, script_1, script_2, script_3, script_4, script_5
	};

// To draw the text box

const unsigned char box_buff [] = {
	23,24,24,24,24,24,24,24,24,24,24,25,
	26,27,27,27,27,27,27,27,27,27,27,28,
	26,27,27,27,27,27,27,27,27,27,27,28,
	29,30,30,30,30,30,30,30,30,30,30,31
};
