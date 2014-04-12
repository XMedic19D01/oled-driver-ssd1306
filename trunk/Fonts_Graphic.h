#ifndef __FONTS_GRAPHIC
#define __FONTS_GRAPHIC
#include <stdint.h>
#include <string.h>

//for the version 0.3 ble watch

#define SIGNAL_80 0 // 80%
#define SIGNAL_60 1 // 60%
#define SIGNAL_40 2 // 40%
#define SIGNAL_20 3 // 20%
#define SIGNAL_00 4 // 00%

#define BATTERY_80 5 //battery 80
#define BATTERY_60 6 //battery 60
#define BATTERY_40 7 //battery 40
#define BATTERY_20 8 //battery 40

#define BULB_ON  9
#define BULB_OFF  10

#define LEFT 11
#define RIGHT 12

#define ALL_BLACK 13 //10
#define HALF_BLACK 14 //50
#define LOW_BLACK 15 //100

#define BUZZER_OFF 16
#define BUZZER_ON 17

#define VIBRATEICON 18

#define PAIR 19
#define UNPAIR 20

#define PULL_LEFT 21
#define PULL_MID  22
#define PULL_RIG  23
#define SELECT_LIGHT  24
#define SELECT_BUZZER 25
#define TOPAIR 26
#define TOFINDME 27
#define SPORTMODE 28

//-------------
#define MAIN_FRAME 0
#define SETTING_FRAME 1
#define SELECT_FRAME 2
#define ALARM_FRAME 3
#define NOTI_FRAME 4
#define NNOTI_FRAME 5
#define PAIR_FRAME  6
#define ADDRESS_FRAME 7
#define FINEME_FRAME 8
#define SPORT_FRAME 9
//define the font rename.
#define   FONT_MESSAGE   0//microsoftSansSerif_8ptFontInfo
#define   FONT_NOTINUM   1//consolas_16ptFontInfo
#define   FONT_OTHER     2//date_16ptFontInfo
#define   FONT_TIME      3//consolas_32ptFontInfo

// This structure describes a single character's display information
typedef struct
{
	const uint8_t widthBits;					// width, in bits (or pixels), of the character
	const uint8_t hightByte;					// hight, in bits (or pixels), of the character	
	const uint16_t offset;					// offset of the character's bitmap, in bytes, into the the FONT_INFO's data array
} FONT_CHAR_INFO;	

// Describes a single font
typedef struct
{
	const uint8_t 			heightPages;	// height, in pages (8 pixels), of the font's characters
	const uint8_t 			startChar;		// the first character in the font (e.g. in charInfo and data)
	const uint8_t 			endChar;		// the last character in the font
	const uint8_t			spacePixels;	// number of pixels that a space character takes up
	const FONT_CHAR_INFO*	charInfo;		// pointer to array of char information
	const uint8_t*			data;			// pointer to generated array of character visual representation
} FONT_INFO;	

// This structure describes a single character's display information
typedef struct
{
	const uint8_t widthBits;					// width, in bits (or pixels), of the character
	const uint8_t hightByte;					// hight, in bits (or pixels), of the character	
	const uint16_t offset;					// offset of the character's bitmap, in bytes, into the the FONT_INFO's data array
} GRAHPIC_PIC_INFO;	

// Describes a grahpic
typedef struct
{
	const uint8_t 			m_version;
	const uint8_t 			s_version;		// the first character in the font (e.g. in charInfo and data)
	const GRAHPIC_PIC_INFO*	grahInfo;		// pointer to array of char information
	const uint8_t*			data;			// pointer to generated array of character visual representation
} FIXGRAHPIC_INFO;	


// This structure describes a single character's display information
typedef struct
{
	const uint8_t widthBits;					// width, in bits (or pixels), of the character
	const uint8_t hightByte;					// hight, in bits (or pixels), of the character	
	const uint16_t offset;					// offset of the character's bitmap, in bytes, into the the FONT_INFO's data array
} FRAME_INFO;	
// Describes a frame stack 
typedef struct
{
	const uint8_t 			m_version;
	const uint8_t 			s_version;		// the first character in the font (e.g. in charInfo and data)
	const FRAME_INFO*	grahInfo;		// pointer to array of char information
	const uint8_t*			data;			// pointer to generated array of character visual representation
} FRAMEGRAHPIC_INFO;	

//for the font assert,for the ssd1306
extern const FONT_INFO fiveBySevenFontInfo;
extern const FONT_INFO microsoftSansSerif_8ptFontInfo;
extern const FONT_INFO consolas_16ptFontInfo;
extern const FONT_INFO date_16ptFontInfo ;
extern const FONT_INFO consolas_32ptFontInfo;

// for graphic assert
extern const FIXGRAHPIC_INFO watch_graphic ;
extern const FRAMEGRAHPIC_INFO frame_graphic;

#endif
