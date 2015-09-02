#ifndef __CONFIG_H__
#define __CONFIG_H__


// 背景画像
#define PNG_SPLASH_BACKGROUND "background/default.png"
#define PNG_HOME_BACKGROUND "background/home.png"
#define PNG_PUZZLE_BACKGROUND "background/puzzle.png"
#define PNG_PUZZLE_FIELD "background/puzzle_field.png"
#define PNG_PREFS_BACKGROUND "background/prefs.png"

// ボタン
#define PNG_PLUS_BTN "menu/btnPlus.png"
#define PNG_PLUS_HL_BTN "menu/btnPlus_hl.png"
#define PNG_MINUS_BTN "menu/btnMinus.png"
#define PNG_MINUS_HL_BTN "menu/btnMinus_hl.png"


// ドロップフィールドの設定
#define MAX_DROP_X 6
#define MAX_DROP_Y 6
#define IS_SETTING_HEX 1


// アニメーションにかける時間
#define TIME_START_GAME 1

#define TIME_DROP_MOVE 0.1f
#define TIME_DROP_CREATE 0.1f

#define FONT_NUMBER "fonts/number.fnt"
#define FONT_WHITE "fonts/whiteFont.fnt"

// ドロップの管理
enum kDrop
{
    kDropRed,
    kDropBlue,
    kDropOrange,
    kDropGreen,
    kDropPurple,
	kDropGray,
    kDropCount,
};

// ドロップの隣接管理
enum kDropDirection
{
	kRight,
	kLeft,
	kTop,
	kBottom,
	kRightTop,
	kRightBottom,
	kLeftTop,
	kLeftBottom,
	kDropDirectionSize,
};

#define ccsf(...) CCString::createWithFormat(__VA_ARGS__)->getCString()

#endif //__CONFIG_H__