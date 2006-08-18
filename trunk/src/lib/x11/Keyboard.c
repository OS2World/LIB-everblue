#include "X11.h"
#include <X11/keysym.h>

#ifndef XK_EuroSign
#define XK_EuroSign 0x20ac
#endif

struct KeyNames_st {
	KeySym	keysym;
	KeyCode	keycode;
	char	col;
	char	*keyname, *keyvalue;
} KeyNames[] = {
	{ XK_space,		0xb9,0,			"space",	" " },
	{ XK_nobreakspace,	0,0,			"nobreakspace",	"†" },
	{ XK_exclam,		'!',0,			"exclam",	"!" },
	{ XK_quotedbl,		'"',0,			"quotedbl",	"\"" },
	{ XK_numbersign,	'#',0,			"numbersign",	"#" },
	{ XK_dollar,		'$',0,			"dollar",	"$" },
	{ XK_percent,		'%',0,			"percent",	"%" },
	{ XK_ampersand,		'&',0,			"ampersand",	"&" },
	{ XK_apostrophe,	39,0,			"apostrophe",	"'" },
	{ XK_quoteright,	39,0,			"quoteright",	"'" },
	{ XK_parenleft,		'(',0,			"parenleft",	"(" },
	{ XK_parenright,	')',0,			"parenright",	")" },
	{ XK_asterisk,		'*',0,			"asterisk",	"*" },
	{ XK_plus,		'+',0,			"plus",		"+" },
	{ XK_comma,		',',0,			"comma",	"," },
	{ XK_minus,		'-',0,			"minus",	"-" },
	{ XK_period,		'.',0,			"period",	"." },
	{ XK_slash,		'/',0,			"slash",	"/" },
	{ XK_0,			'0',0,			"0",		"0" },
	{ XK_1,			'1',0,			"1",		"1" },
	{ XK_2,			'2',0,			"2",		"2" },
	{ XK_3,			'3',0,			"3",		"3" },
	{ XK_4,			'4',0,			"4",		"4" },
	{ XK_5,			'5',0,			"5",		"5" },
	{ XK_6,			'6',0,			"6",		"6" },
	{ XK_7,			'7',0,			"7",		"7" },
	{ XK_8,			'8',0,			"8",		"8" },
	{ XK_9,			'9',0,			"9",		"9" },	
	{ XK_colon,		':',0,			"colon",	":" },
	{ XK_semicolon,		';',0,			"semicolon",	";" },
	{ XK_less,		'<',0,			"less",		"<" },
	{ XK_equal,		'=',0,			"equal",	"=" },
	{ XK_greater,		'>',0,			"greater",	">" },
	{ XK_question,		'?',0,			"question",	"?" },
	{ XK_at,		'@',0,			"at",		"@" },
	{ XK_A,			'A',0,			"A",		"A" },
	{ XK_A,			0x9e,0,			"A",		"A" },
	{ XK_B,			'B',0,			"B",		"B" },
	{ XK_B,			0xb0,0,			"B",		"B" },
	{ XK_C,			'C',0,			"C",		"C" },
	{ XK_C,			0xae,0,			"C",		"C" },
	{ XK_D,			'D',0,			"D",		"D" },
	{ XK_D,			0xa0,0,			"D",		"D" },
	{ XK_E,			'E',0,			"E",		"E" },
	{ XK_E,			0x92,0,			"E",		"E" },
	{ XK_F,			'F',0,			"F",		"F" },
	{ XK_F,			0xa1,0,			"F",		"F" },
	{ XK_G,			'G',0,			"G",		"G" },
	{ XK_G,			0xa2,0,			"G",		"G" },
	{ XK_H,			'H',0,			"H",		"H" },
	{ XK_H,			0xa3,0,			"H",		"H" },
	{ XK_I,			'I',0,			"I",		"I" },
	{ XK_I,			0x97,0,			"I",		"I" },
	{ XK_J,			'J',0,			"J",		"J" },
	{ XK_J,			0xa4,0,			"J",		"J" },
	{ XK_K,			'K',0,			"K",		"K" },
	{ XK_K,			0xa5,0,			"K",		"K" },
	{ XK_L,			'L',0, 			"L",		"L" },
	{ XK_L,			0xa6,0,			"L",		"L" },
	{ XK_M,			'M',0,			"M",		"M" },
	{ XK_M,			0xb2,0,			"M",		"M" },
	{ XK_N,			'N',0,			"N",		"N" },
	{ XK_N,			0xb1,0,			"N",		"N" },
	{ XK_O,			'O',0,			"O",		"O" },
	{ XK_O,			0x98,0,			"O",		"O" },
	{ XK_P,			'P',0,			"P",		"P" },
	{ XK_P,			0x99,0,			"P",		"P" },
	{ XK_Q,			'Q',0,			"Q",		"Q" },
	{ XK_Q,			0x90,0,			"Q",		"Q" },
	{ XK_R,			'R',0,			"R",		"R" },
	{ XK_R,			0x93,0,			"R",		"R" },
	{ XK_S,			'S',0,			"S",		"S" },
	{ XK_S,			0x9f,0,			"S",		"S" },
	{ XK_T,			'T',0,			"T",		"T" },
	{ XK_T,			0x94,0,			"T",		"T" },
	{ XK_U,			'U',0,			"U",		"U" },
	{ XK_U,			0x96,0,			"U",		"U" },
	{ XK_V,			'V',0,			"V",		"V" },
	{ XK_V,			0xaf,0,			"V",		"V" },
	{ XK_W,			'W',0,			"W",		"W" },
	{ XK_W,			0x91,0,			"W",		"W" },
	{ XK_X,			'X',0,			"X",		"X" },
	{ XK_X,			0xad,0,			"X",		"X" },
	{ XK_Y,			'Y',0,			"Y",		"Y" },
	{ XK_Y,			0x95,0,			"Y",		"Y" },
	{ XK_Z,			'Z',0,			"Z",		"Z" },
	{ XK_Z,			0xac,0,			"Z",		"Z" },
	{ XK_bracketleft,	'[',0,			"bracketleft",	"[" },
	{ XK_backslash,		'\\',0,			"backslash",	"\\" },
	{ XK_bracketright,	']',0,			"bracketright",	"]" },
	{ XK_asciicircum,	'^',0,			"asciicircum",	"^" },
	{ XK_underscore,	'_',0,			"underscore",	"_" },
	{ XK_grave,		'`',0,			"grave",	"`" },
	{ XK_quoteleft,		'`',0,			"quoteleft",	"`" },
	{ XK_a,			'a',0,			"a",		"a" },
	{ XK_b,			'b',0,			"b",		"b" },
	{ XK_c,			'c',0,			"c",		"c" },
	{ XK_d,			'd',0,			"d",		"d" },
	{ XK_e,			'e',0,			"e",		"e" },
	{ XK_f,			'f',0,			"f",		"f" },
	{ XK_g,			'g',0,			"g",		"g" },
	{ XK_h,			'h',0,			"h",		"h" },
	{ XK_i,			'i',0,			"i",		"i" },
	{ XK_j,			'j',0,			"j",		"j" },
	{ XK_k,			'k',0,			"k",		"k" },
	{ XK_l,			'l',0,			"l",		"l" },
	{ XK_m,			'm',0,			"m",		"m" },
	{ XK_n,			'n',0,			"n",		"n" },
	{ XK_o,			'o',0,			"o",		"o" },
	{ XK_p,			'p',0,			"p",		"p" },
	{ XK_q,			'q',0,			"q",		"q" },
	{ XK_r,			'r',0,			"r",		"r" },
	{ XK_s,			's',0,			"s",		"s" },
	{ XK_t,			't',0,			"t",		"t" },
	{ XK_u,			'u',0,			"u",		"u" },
	{ XK_v,			'v',0,			"v",		"v" },
	{ XK_w,			'w',0,			"w",		"w" },
	{ XK_x,			'x',0,			"x",		"x" },
	{ XK_y,			'y',0,			"y",		"y" },
	{ XK_z,			'z',0,			"z",		"z" },
	{ XK_braceleft,		'{',0,			"braceleft",	"{" },
	{ XK_bar,		'|',0,			"bar",		"|" },
	{ XK_braceright,	'}',0,			"braceright",	"}" },
	{ XK_asciitilde,	'~',0,			"asciitilde",	"~" },
	{ XK_Shift_L,		0xaa,0,			"Shift_L" },
	{ XK_Shift_R,		0xb6,0,			"Shift_R" },
	{ XK_Control_L,		0x9d,0,			"Control_L" },
	{ XK_Control_R,		0xdb,0,			"Control_R" },
	{ XK_Alt_L,		0xb8,0,			"Alt_L" },
	{ XK_Alt_R,		0xde,0,			"Alt_R" },
	{ XK_Caps_Lock,		0xba,0,			"Caps_Lock" },
	{ XK_BackSpace, 	0x08,0,			"BackSpace" },
	{ XK_Tab,		0x09,0,			"Tab" },
	{ XK_Tab,		0x8f,0,			"Tab" },
	{ XK_KP_Tab,		0x09,0,			"KP_Tab" },
	{ XK_Linefeed,		0x0a,0,			"Linefeed" },
	{ XK_Clear,		0,0,			"Clear" },
	{ XK_Return,		0x0d,0,			"Return" },
	{ XK_Return,		0x9c,0,			"Return" },
	{ XK_Select,		0,0,			"Select" },
	{ XK_Pause,		0xdf,0,			"Pause" },
	{ XK_Scroll_Lock,	0xc6,0,			"Scroll_Lock" },
	{ XK_Sys_Req,		0xd4,0,			"Sys_Req" },
	{ XK_Escape,		0x81,0,			"Escape" },
	{ XK_Delete,		0xe9,0,			"Delete" },
	{ XK_Home,		0xe0,0,			"Home" },
	{ XK_Left,		0xe3,0,			"Left" },
	{ XK_Up,		0xe1,0,			"Up" },
	{ XK_Right,		0xe4,0,			"Right" },
	{ XK_Down,		0xe6,0,			"Down" },
	{ XK_Page_Up,		0xe2,0,			"Page_Up" },
	{ XK_Prior,		0xe2,0,			"Prior" },
	{ XK_Page_Down,		0xe7,0,			"Page_Down" },
	{ XK_Next,		0xe7,0,			"Next" },
	{ XK_End,		0xe5,0,			"End" },
	{ XK_Print,		0xdd,0,			"Print" },
	{ XK_Insert,		0xe8,0,			"Insert" },
	{ XK_Menu,		0,0,			"Menu" },
	{ XK_Break,		0xee,0,			"Break" },
	{ XK_Num_Lock,		0xc5,0,			"Num_Lock" },
	{ XK_KP_Decimal,	0xd3,0,			"KP_Decimal",	"." },
	{ XK_KP_0,		0xd2,0,			"KP_0",		"0" },
	{ XK_KP_1,		0xcf,0,			"KP_1",		"1" },
	{ XK_KP_2,		0xd0,0,			"KP_2",		"2" },
	{ XK_KP_3,		0xd1,0,			"KP_3",		"3" },
	{ XK_KP_4,		0xcb,0,			"KP_4",		"4" },
	{ XK_KP_5,		0xcc,0,			"KP_5",		"5" },
	{ XK_KP_6,		0xcd,0,			"KP_6",		"6" },
	{ XK_KP_7,		0xc7,0,			"KP_7",		"7" },
	{ XK_KP_8,		0xc8,0,			"KP_8",		"8" },
	{ XK_KP_9,		0xc9,0,			"KP_9",		"9" },
	{ XK_KP_Delete,		0x53,2,			"KP_Delete" },
	{ XK_KP_Insert,		0x52,2,			"KP_Insert" },
	{ XK_KP_End,		0x4f,2,			"KP_End" },
	{ XK_KP_Down,		0x50,2,			"KP_Down" },
	{ XK_KP_Page_Down,	0x51,2,			"KP_Page_Down" },
	{ XK_KP_Next,		0x51,2,			"KP_Next" },
	{ XK_KP_Left,		0x4b,2,			"KP_Left" },
	{ XK_KP_Right,		0x4d,2,			"KP_Right" },
	{ XK_KP_Home,		0x47,2,			"KP_Home" },
	{ XK_KP_Up,		0x48,2,			"KP_Up" },
	{ XK_KP_Page_Up,	0x49,2,			"KP_Page_Up" },
	{ XK_KP_Prior,		0x49,2,			"KP_Prior" },
	{ XK_KP_Divide,		0xdc,0,			"KP_Divide",	"/" },
	{ XK_KP_Multiply,	0xb7,0,			"KP_Multiply",	"*" },
	{ XK_KP_Subtract,	0xca,0,			"KP_Subtract",	"-" },
	{ XK_KP_Add,		0xce,0,			"KP_Add",	"+" },
	{ XK_KP_Enter,		0xda,0,			"KP_Enter" },
	{ XK_KP_Equal,		0xda,0,			"KP_Equal" },
	{ XK_F1,		0xbb,0,			"F1" },
	{ XK_F2,		0xbc,0,			"F2" },
	{ XK_F3,		0xbd,0,			"F3" },
	{ XK_F4,		0xbe,0,			"F4" },
	{ XK_F5,		0xbf,0,			"F5" },
	{ XK_F6,		0xc0,0,			"F6" },
	{ XK_F7,		0xc1,0,			"F7" },
	{ XK_F8,		0xc2,0,			"F8" },
	{ XK_F9,		0xc3,0,			"F9" },
	{ XK_F10,		0xc4,0,			"F10" },
	{ XK_F11,		0xd7,0,			"F11" },
	{ XK_F12,		0xd8,0,			"F12" },
	/* High ASCII KeyCodes */
	{ XK_EuroSign,		0xd5,2,			"EuroSign",	"’" },	
	{ XK_exclamdown,	'≠',2,			"exclamdown",	"°" },
	{ XK_cent,		'Ω',2,			"cent",		"¢" },
	{ XK_sterling,		'ú',2,			"sterling",	"£" },
	{ XK_currency,		'œ',2,			"currency",	"§" },
	{ XK_yen,		'æ',2,			"yen",		"•" },
	{ XK_brokenbar,		'›',2,			"brokenbar",	"¶" },
	{ XK_section,		'ı',2,			"section",	"ß" },
	{ XK_diaeresis,		'˘',2,			"diaeresis",	"®" },
	{ XK_copyright,		'∏',2,			"copyright",	"©" },
	{ XK_ordfeminine,	'¶',2,			"ordfemine",	"™" },
	{ XK_guillemotleft,	'Æ',2,			"guillemotleft","´" },
	{ XK_notsign,		'™',2,			"notsign",	"¨" },
	{ XK_hyphen,		'',2,			"hyphen",	"≠" },
	{ XK_registered,	'©',2,			"registered",	"Æ" },
	{ XK_macron,		0,0,			"macron",	"Ø" },
	{ XK_degree,		'¯',2,			"degree",	"∞" },
	{ XK_plusminus,		'Ò',2,			"plusminus",	"±" },
	{ XK_twosuperior,	'˝',2,			"twosuperior",	"≤" },
	{ XK_threesuperior,	'¸',2,			"threesuperior","≥" },
	{ XK_acute,		'Ô',2,			"acute",	"¥" },
	{ XK_mu,		'Ê',2,			"mu",		"µ" },
	{ XK_paragraph,		'Ù',2,			"paragraph",	"∂" },
	{ XK_periodcentered,	'˙',2,			"periodcentered","∑" },
	{ XK_cedilla,		'˜',2,			"cedilla",	"∏" },
	{ XK_onesuperior,	'˚',2,			"onesuperior",	"π" },
	{ XK_masculine,		0,0,			"masculine",	"∫" },
	{ XK_guillemotright,	'Ø',2,			"guillemotright","ª" },
	{ XK_onequarter,	'¨',2,			"onequarter",	"º" },
	{ XK_onehalf,		'´',2,			"onehalf",	"Ω" },
	{ XK_threequarters,	'Û',2,			"threequarters","æ" },
	{ XK_questiondown,	'®',2,			"questiondown",	"ø" },
	{ XK_Agrave,		'∑',2,			"Agrave",	"¿" },
	{ XK_Aacute,		'µ',2,			"Aacute",	"¡" },
	{ XK_Acircumflex,	'∂',2,			"Acircumflex",	"¬" },
	{ XK_Atilde,		'«',2,			"Atilde",	"√" },
	{ XK_Adiaeresis,	'é',2,			"Adiaeresis",	"ƒ" },
	{ XK_Aring,		'è',2,			"Aring",	"≈" },
	{ XK_AE,		'í',2,			"AE",		"∆" },
	{ XK_Ccedilla,		'Ä',2,			"Ccedilla",	"«" },
	{ XK_Egrave,		'‘',2,			"Egrave",	"»" },
	{ XK_Eacute,		'ê',2,			"Eacute",	"…" },
	{ XK_Ecircumflex,	'“',2,			"Ecircumflex",	" " },
	{ XK_Ediaeresis,	'”',2,			"Ediaeresis",	"À" },
	{ XK_Igrave,		'ﬁ',2,			"Igrave",	"Ã" },
	{ XK_Iacute,		'÷',2,			"Iacute",	"Õ" },
	{ XK_Icircumflex,	'◊',2,			"Icircumflex",	"Œ" },
	{ XK_Idiaeresis,	'ÿ',2,			"Idiaeresis",	"œ" },
	{ XK_ETH,		0,0,			"ETH",		"–" },
	{ XK_Ntilde,		'•',2,			"Ntilde",	"—" },
	{ XK_Ograve,		'„',2,			"Ograve",	"“" },
	{ XK_Oacute,		'‡',2,			"Oacute",	"”" },
	{ XK_Ocircumflex,	'‚',2,			"Ocircumflex",	"‘" },
	{ XK_Otilde,		'Â',2,			"Otilde",	"’" },
	{ XK_Odiaeresis,	'ô',2,			"Odiaeresis",	"÷" },
	{ XK_multiply,		'û',2,			"multiply",	"◊" },
	{ XK_Ooblique,		'ù',2,			"Ooblique",	"ÿ" },
	{ XK_Ugrave,		'Î',2,			"Ugrave",	"Ÿ" },
	{ XK_Uacute,		'È',2,			"Uacute",	"⁄" },
	{ XK_Ucircumflex,	'Í',2,			"Ucircumflex",	"€" },
	{ XK_Udiaeresis,	'ö',2,			"Udiaeresis",	"‹" },
	{ XK_Yacute,		'Ì',2,			"Yacute",	"›" },
	{ XK_THORN,		0,0,			"THORN",	"ﬁ" },
	{ XK_ssharp,		0,0,			"ssharp",	"ﬂ" },
	{ XK_agrave,		'Ö',2,			"agrave",	"‡" },
	{ XK_aacute,		'†',2,			"aacute",	"·" },
	{ XK_acircumflex,	'É',2,			"acircumflex",	"‚" },
	{ XK_atilde,		'∆',2,			"atilde",	"„" },
	{ XK_adiaeresis,	'Ñ',2,			"adiaeresis",	"‰" },
	{ XK_aring,		'Ü',2,			"aring",	"Â" },
	{ XK_ae,		'ë',2,			"ae",		"Ê" },
	{ XK_ccedilla,		'á',2,			"ccedilla",	"Á" },
	{ XK_egrave,		'ä',2,			"egrave",	"Ë" },
	{ XK_eacute,		'Ç',2,			"eacute",	"È" },
	{ XK_ecircumflex,	'à',2,			"ecircumflex",	"Í" },
	{ XK_ediaeresis,	'â',2,			"ediaeresis",	"Î" },
	{ XK_igrave,		'ç',2,			"igrave",	"Ï" },
	{ XK_iacute,		'°',2,			"iacute",	"Ì" },
	{ XK_icircumflex,	'å',2,			"icircumflex",	"Ó" },
	{ XK_idiaeresis,	'ã',2,			"idiaeresis",	"Ô" },
	{ XK_eth,		0,0,			"eth",		"" },
	{ XK_ntilde,		'§',2,			"ntilde",	"Ò" },
	{ XK_ograve,		'ï',2,			"ograve",	"Ú" },
	{ XK_oacute,		'¢',2,			"oacute",	"Û" },
	{ XK_ocircumflex,	'ì',2,			"ocircumflex",	"Ù" },
	{ XK_otilde,		'‰',2,			"otilde",	"ı" },
	{ XK_odiaeresis,	'î',2,			"odiaeresis",	"ˆ" },
	{ XK_division,		'ˆ',2,			"division",	"˜" },
	{ XK_oslash,		'õ',2,			"oslash",	"¯" },
	{ XK_ugrave,		'ó',2,			"ugrave",	"˘" },
	{ XK_uacute,		'£',2,			"uacute",	"˙" },
	{ XK_ucircumflex,	'ñ',2,			"ucircumflex",	"˚" },
	{ XK_udiaeresis,	'Å',2,			"udiaeresis",	"¸" },
	{ XK_yacute,		'Ï',2,			"yacute",	"˝" },
	{ XK_thorn,		0,0,			"thorn",	"˛" },
	{ XK_ydiaeresis,	'ò',2,			"ydiaeresis",	"ˇ" },
	{ XK_Kanji,		0,0,			"Kanji" },
	/* Special KeySyms */
#if 0
	{ XK_blank,		' ',0			"blank",	" " },
	{ XK_soliddiamond,	4,0,			"soliddiamond",	"\004" },
	{ XK_checkerboard,	177,2,			"checkerboard" },
	{ XK_ht,		9,0,			"ht",		"\011" },
	{ XK_ff,		12,0,			"ff",		"\014" },
	{ XK_cr,		13,0,			"cr",		"\015" },
	{ XK_lf,		10,0,			"lf",		"\012" },
	{ XK_vt,		11,0,			"vt",		"\013" },
#endif
	/* Special codes */
	{ XK_Mode_switch,	17,0,			"Mode_switch" },
	{ 0,			0,0,			NULL },
};


#define KEYSYM_PER_CODE 4
KeySym *_XkeyMap = NULL, *_XkeyMap_end = NULL;
extern unsigned int _XmaxKeyCode, _XminKeyCode;

void _XInitKeyMapping(void)
{
	DBUG_ENTER("_XInitKeyMapping")
	int idx;
	unsigned int keycode, maxcode = 0, mincode = 0xfffffff, size;

	if (_XkeyMap) DBUG_VOID_RETURN;

	for (idx = 0; KeyNames[idx].keyname; idx++) 
	if ((keycode = KeyNames[idx].keycode)) {
		if (maxcode < keycode) maxcode = keycode;
		if (mincode > keycode) mincode = keycode;
	}

	_XmaxKeyCode = maxcode;
	_XminKeyCode = mincode;

	size = (maxcode-mincode + 1) * KEYSYM_PER_CODE;
	if (!(_XkeyMap = calloc( size ,sizeof(KeySym)))) {
		fprintf(stderr,"Xlib_Keyboard: Failed to create keymap!\n");
		abort();
	}

	_XkeyMap_end = _XkeyMap + size;

	for (idx = 0; KeyNames[idx].keyname; idx++)
	if ((keycode = KeyNames[idx].keycode)) {
		KeySym *code = &_XkeyMap[(keycode - mincode) * KEYSYM_PER_CODE];
		int col = (KeyNames[idx].col == KC_VIRTUALKEY)? 2 :
			  (KeyNames[idx].col == KC_SCANCODE)? 3 : 0;
		if (code[col]) continue;
		code[col] = KeyNames[idx].keysym;
	}

	DBUG_VOID_RETURN;
}

KeySym *XGetKeyboardMapping(Display* display, unsigned int first_keycode, 
	int keycode_count, int* keysyms_per_keycode_return)
{
	DBUG_ENTER("XGetKeyboardMapping")
	long size, count;
	KeySym *map = NULL, *src, *dst, *end;

	if (!display) DBUG_RETURN(NULL);
	if (!keycode_count) goto finish;

	count = keycode_count * KEYSYM_PER_CODE;
	size = count + 1;

	if (!(dst = (KeySym *)Xcalloc( size, sizeof(KeySym) ))) {
		goto finish;
	}
	map = dst;

	if (first_keycode < display->min_keycode) {
		unsigned int diff = (display->min_keycode - first_keycode) * KEYSYM_PER_CODE;
		dst += diff; count -= diff;
		first_keycode = display->min_keycode;
	}
	src = display->keysyms + 
		(first_keycode - display->min_keycode) * KEYSYM_PER_CODE;
	end = display->keysyms + 
		(display->max_keycode - display->min_keycode) * KEYSYM_PER_CODE;
	
	while (count && src < end) {
		*(dst++) = *(src++);
		count--;
	}

	finish:
	if (keysyms_per_keycode_return)
		*keysyms_per_keycode_return = KEYSYM_PER_CODE;
	DBUG_RETURN(map);
}

XModifierKeymap *XGetModifierMapping(register Display *dpy) {       
    DBUG_ENTER("XGetModifierMapping")
    XModifierKeymap *res, *rc;

    res = XNewModifiermap(8);
    if (!res) DBUG_RETURN(NULL);

    DBUG_POINT("ShiftMapIndex");
    if ((rc = XInsertModifiermapEntry(res, 0xaa, ShiftMapIndex))) res=rc;
    if ((rc = XInsertModifiermapEntry(res, 0xb6, ShiftMapIndex))) res=rc;
    DBUG_POINT("LockMapIndex");
	if ((rc = XInsertModifiermapEntry(res, 0xba, LockMapIndex))) res=rc;
	DBUG_POINT("ControlMapIndex");
	if ((rc = XInsertModifiermapEntry(res, 0x9d, ControlMapIndex))) res=rc;
	if ((rc = XInsertModifiermapEntry(res, 0xdb, ControlMapIndex))) res=rc;
	DBUG_POINT("Mod1MapIndex");
	if ((rc = XInsertModifiermapEntry(res, 0x11, Mod1MapIndex))) res=rc;
	DBUG_POINT("Mod2MapIndex");
	if ((rc = XInsertModifiermapEntry(res, 0xb8, Mod2MapIndex))) res=rc;
	if ((rc = XInsertModifiermapEntry(res, 0xde, Mod2MapIndex))) res=rc;

	DBUG_POINT("SyncHandle()");
	SyncHandle();
	DBUG_RETURN(res);
}

XModifierKeymap *XNewModifiermap(int keyspermodifier) {
	DBUG_ENTER("XNewModifiermap")
	XModifierKeymap *res = (XModifierKeymap *) Xmalloc((sizeof (XModifierKeymap)));
	if (res) {
		res->max_keypermod = keyspermodifier;
		res->modifiermap = (keyspermodifier > 0 ?
				(KeyCode *) Xmalloc((unsigned) (8 * keyspermodifier))
				: (KeyCode *) NULL);
		if (keyspermodifier && (res->modifiermap == NULL)) {
			Xfree((char *) res);
			DBUG_RETURN((XModifierKeymap *) NULL);
		}
	}
	DBUG_RETURN(res);
}


int XFreeModifiermap(XModifierKeymap *map) {
	DBUG_ENTER("XFreeModifiermap")
	if (map) {
		if (map->modifiermap)
			Xfree((char *) map->modifiermap);
		Xfree((char *) map);
	}
	DBUG_RETURN(1);
}

XModifierKeymap *XInsertModifiermapEntry(XModifierKeymap *map, unsigned int
		keycode, int modifier) {
	DBUG_ENTER("XInsertModifiermapEntry")
	XModifierKeymap *newmap;
	int i, row = modifier * map->max_keypermod, newrow, lastrow;

	for (i=0; i<map->max_keypermod; i++) {
		if (map->modifiermap[ row+i ] == keycode)
			DBUG_RETURN(map); /* already in the map */
		if (map->modifiermap[ row+i ] == 0) {
			map->modifiermap[ row+i ] = keycode;
			DBUG_RETURN(map); /* we added it without stretching the map */
		}
	}   

	/* stretch the map */
	if ((newmap = XNewModifiermap(map->max_keypermod+1)) == NULL)
		DBUG_RETURN((XModifierKeymap *) NULL);
	newrow = row = 0;
	lastrow = newmap->max_keypermod * 8;
	while (newrow < lastrow) {
		for (i=0; i<map->max_keypermod; i++)
			newmap->modifiermap[ newrow+i ] = map->modifiermap[ row+i ];
		newmap->modifiermap[ newrow+i ] = 0;
		row += map->max_keypermod;
		newrow += newmap->max_keypermod;
	}
	(void) XFreeModifiermap(map);
	newrow = newmap->max_keypermod * modifier + newmap->max_keypermod - 1;
	newmap->modifiermap[ newrow ] = keycode;
	DBUG_RETURN(newmap);
}

XModifierKeymap *XDeleteModifiermapEntry(XModifierKeymap *map, unsigned int
		keycode, int modifier) {
	DBUG_ENTER("XDeleteModifiermapEntry")
	int i,
	row = modifier * map->max_keypermod;

	for (i=0; i<map->max_keypermod; i++) {
		if (map->modifiermap[ row+i ] == keycode)
			map->modifiermap[ row+i ] = 0;
	}
	/* should we shrink the map?? */
	DBUG_RETURN(map);
}

char *XKeysymToString(KeySym keysym) {
	struct KeyNames_st *key = KeyNames;
	while(key->keysym && key->keysym != keysym)
		key++;
	return key->keyname;
}

KeySym XStringToKeysym(_Xconst char* string) {
	struct KeyNames_st *key = KeyNames;
	while(key->keysym && strcmp(key->keyname,string))
		key++;
	return key->keysym;
}
