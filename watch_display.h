#ifndef __WATCH_DISPLAY
#ifndef __WATCH_DISPLAY
#define __WATCH_DISPLAY
#include "ble_cts_date_time.h"
#include "Fonts_Graphic.h"
#include "SSD1306.h"

//rename the font name (for message, date,calendar,time..)
// typedef  FONT_INFO microsoftSansSerif_8ptFontInfo Font_Message;
// typedef FONT_INFO consolas_16ptFontInfo Font_NotiNum;
// typedef FONT_INFO date_16ptFontInfo Font_other;
// typedef FONT_INFO consolas_32ptFontInfo Font_Time;
//define the data struct for ble watch
//

//define the macro
//define the font rename.
#define   FONT_MESSAGE   0//microsoftSansSerif_8ptFontInfo
#define   FONT_NOTINUM   1//consolas_16ptFontInfo
#define   FONT_OTHER     2//date_16ptFontInfo
#define   FONT_TIME      3//consolas_32ptFontInfo

//define the origin point
#define   ORIGIN_POINTX         0
#define   ORIGIN_POINTY         0
#define   ORIGIN                (ORIGIN_POINTX,ORIGIN_POINTY)

#define   MSINGLE_POINTX        65
#define   MSINGLE_POINTY        0
#define   MAIN_SINGLE           (MSINGLE_POINTX,MSINGLE_POINTY)

#define   MBATTERY_POINTX       105
#define   MBATTERY_POINTY       0
#define   MAIN_BATTERY          (MBATTERY_POINTX,MBATTERY_POINTY)

#define   MHOUR_POINTX          3
#define   MHOUR_POINTY          3
#define   MAIN_HOUR             (MBATTERY_POINTX,MBATTERY_POINTY)

#define   MMINUTE_POINTX        55
#define   MMINUTE_POINTY        3
#define   MAIN_MINUTE           (MMINUTE_POINTX,MMINUTE_POINTY)

#define   MAMPM_POINTX          100
#define   MAMPM_POINTY          4
#define   MAIN_MAMPM            (MAMPM_POINTX,MAMPM_POINTY)

#define   MDATE_POINTX          0
#define   MDATE_POINTY          6
#define   MAIN_DATE             (MDATE_POINTX,MDATE_POINTY)

#define   SINGLE_POINTX         2
#define   SINGLE_POINTY         0
#define   ET_SINGLE             (SSINGLE_POINTX,SSINGLE_POINTY)

#define   BATTERY_POINTX        103
#define   BATTERY_POINTY        0
#define   ET_BATTERY            (MBATTERY_POINTX,MBATTERY_POINTY)

#define   HOUR_POINTX           41
#define   HOUR_POINTY           0
#define   ET_HOUR               (SHOUR_POINTX_POINTX,SHOUR_POINTX_POINTY)

#define   MINUTE_POINTX         70
#define   MINUTE_POINTY         0
#define   ET_MINUTE             (SMINUTE_POINTX,SMINUTE_POINTY)

#define   SSETITEM_POINTX       55
#define   SSETITEM_POINTY       3
#define   SET_ITEM              (SMINUTE_POINTX,SMINUTE_POINTY)

#define   SSELECT_POINTX        0
#define   SSELECT_POINTY        3
#define   SELECT_ITEM           (SSELECT_POINTX,SSELECT_POINTY)

#define   PAIR_POINTX        15
#define   PAIR_POINTY        2
#define   PAIR_FRAM          (PAIR_POINTX,PAIR_POINTY)


#define   DIRECT_POINTX        90
#define   DIRECT_POINTY        7
#define   DIRECT_ITEM           (SSELECT_POINTX,SSELECT_POINTY)


#define   ALARM_HOUR_POINTX     41
#define   ALARM_HOUR_POINTY     6
#define   ALARM_HOUR            (ALARM_HOUR_POINTX,ALARM_HOUR_POINTY)

#define   ALARM_MINUTE_POINTX   70
#define   ALARM_MINUTE_POINTY   6
#define   ALARM_MINUTE          (ALARM_MINUTE_POINTX,ALARM_MINUTE_POINTY)

#define   NOTI_MAIL_POINTX      9
#define   NOTI_MAIL_POINTY      6
#define   NOTI_MAIL             (NOTI_MAIL_POINTX,NOTI_MAIL_POINTY)

#define   NOTI_CALL_POINTX      54
#define   NOTI_CALL_POINTY      6
#define   NOTI_CALL             (NOTI_CALL_POINTX,NOTI_CALL_POINTY)

#define   NOTI_SMS_POINTX       95
#define   NOTI_SMS_POINTY       6
#define   NOTI_SMS              (NOTI_SMS_POINTX,NOTI_SMS_POINTY)

#define   ALARM_POINTX       55
#define   ALARM_POINTY       3
#define   ALARM_ICON              (SMINUTE_POINTX,SMINUTE_POINTY)

#define  ANCS_ITEM1_X           0
#define  ANCS_ITEM1_Y           3
#define  ANCS_ITEM2_X           0
#define  ANCS_ITEM2_Y           5
#define  ANCS_ITEM3_X           0
#define  ANCS_ITEM3_Y           5

#define  ADDBOOK_TITLE_X           50
#define  ADDBOOK_TITLE_Y           0

#define  ADDBOOK_CONTENT_X           0
#define  ADDBOOK_CONTENT_Y           3

#define PERIOD_VALUE 12
#define WRITE_BUFFER 0
#define WRITE_LCD    1

extern const FONT_INFO *pFontInfo;// = &consolas_16ptFontInfo;
//function for watch
uint32_t DispalyInt(void );
void Switch_Page(uint8_t index, const ble_cts_date_time_t a_sTime,unsigned char toSubMode);
void SetDisplayInvert(uint8_t isTurnOn);
//draw the single
//need assign the x/y point and the rf single.
void DrawSingle(uint8_t x, uint8_t y, int value);
void UpdateRSSIStatus(unsigned char WorkMode, unsigned char SubMode,int value);
//draw the battery
//need assign the x/y point and the barry value.
void UpdateBatteryStatus(unsigned char WorkMode, unsigned char SubMode,int value);
void DrawBattery(uint8_t x, uint8_t y, int value);

void SetFont(uint8_t font_type);
//draw information display 
void SetCurrent_period(const ble_cts_date_time_t a_sTime);
void SetCurrent_time(const ble_cts_date_time_t a_sTime);
void SetCalendar(const ble_cts_date_time_t a_sTime);
void SetSmall_time(const ble_cts_date_time_t a_sTime);
void DrawSettinnItem(uint8_t index);
void ClearSettinnItem(uint8_t index);
void DrawDirectBar(uint8_t index);
void SetNotiNum(int8_t index);
int8_t GetNotiNum(void);
void DisplayAncsData(uint8_t select,int8_t);
void ShowAlarm_time(const ble_cts_date_time_t a_sTime);
void ShowAlarmIcon(void);
void ClearAlarmIcon(void);


//work mode
uint8_t isEntryIdle(void);
void m_oled_goto_idle(void);
void m_oled_go(void);
#endif
#define __WATCH_DISPLAY
#include "ble_cts_date_time.h"
#include "Fonts_Graphic.h"
#include "SSD1306.h"

//rename the font name (for message, date,calendar,time..)
// typedef  FONT_INFO microsoftSansSerif_8ptFontInfo Font_Message;
// typedef FONT_INFO consolas_16ptFontInfo Font_NotiNum;
// typedef FONT_INFO date_16ptFontInfo Font_other;
// typedef FONT_INFO consolas_32ptFontInfo Font_Time;
//define the data struct for ble watch
//

//define the macro
//define the font rename.
#define   FONT_MESSAGE   0//microsoftSansSerif_8ptFontInfo
#define   FONT_NOTINUM   1//consolas_16ptFontInfo
#define   FONT_OTHER     2//date_16ptFontInfo
#define   FONT_TIME      3//consolas_32ptFontInfo

//define the origin point
#define   ORIGIN_POINTX         0
#define   ORIGIN_POINTY         0
#define   ORIGIN                (ORIGIN_POINTX,ORIGIN_POINTY)

#define   MSINGLE_POINTX        65
#define   MSINGLE_POINTY        0
#define   MAIN_SINGLE           (MSINGLE_POINTX,MSINGLE_POINTY)

#define   MBATTERY_POINTX       105
#define   MBATTERY_POINTY       0
#define   MAIN_BATTERY          (MBATTERY_POINTX,MBATTERY_POINTY)

#define   MHOUR_POINTX          3
#define   MHOUR_POINTY          3
#define   MAIN_HOUR             (MBATTERY_POINTX,MBATTERY_POINTY)

#define   MMINUTE_POINTX        55
#define   MMINUTE_POINTY        3
#define   MAIN_MINUTE           (MMINUTE_POINTX,MMINUTE_POINTY)

#define   MAMPM_POINTX          100
#define   MAMPM_POINTY          4
#define   MAIN_MAMPM            (MAMPM_POINTX,MAMPM_POINTY)

#define   MDATE_POINTX          0
#define   MDATE_POINTY          6
#define   MAIN_DATE             (MDATE_POINTX,MDATE_POINTY)

#define   SINGLE_POINTX         2
#define   SINGLE_POINTY         0
#define   ET_SINGLE             (SSINGLE_POINTX,SSINGLE_POINTY)

#define   BATTERY_POINTX        103
#define   BATTERY_POINTY        0
#define   ET_BATTERY            (MBATTERY_POINTX,MBATTERY_POINTY)

#define   ANCS_NOTI_POINTX        90
#define   ANCS_NOTI_POINTY        0
#define   ANCS_NOTI            (MBATTERY_POINTX,MBATTERY_POINTY)

#define   HOUR_POINTX           41
#define   HOUR_POINTY           0
#define   ET_HOUR               (SHOUR_POINTX_POINTX,SHOUR_POINTX_POINTY)

#define   MINUTE_POINTX         70
#define   MINUTE_POINTY         0
#define   ET_MINUTE             (SMINUTE_POINTX,SMINUTE_POINTY)

#define   SSETITEM_POINTX       55
#define   SSETITEM_POINTY       3
#define   SET_ITEM              (SMINUTE_POINTX,SMINUTE_POINTY)

#define   SSELECT_POINTX        0
#define   SSELECT_POINTY        3
#define   SELECT_ITEM           (SSELECT_POINTX,SSELECT_POINTY)

#define   PAIR_POINTX        15
#define   PAIR_POINTY        2
#define   PAIR_FRAM          (PAIR_POINTX,PAIR_POINTY)


#define   DIRECT_POINTX        90
#define   DIRECT_POINTY        7
#define   DIRECT_ITEM           (SSELECT_POINTX,SSELECT_POINTY)


#define   ALARM_HOUR_POINTX     41
#define   ALARM_HOUR_POINTY     6
#define   ALARM_HOUR            (ALARM_HOUR_POINTX,ALARM_HOUR_POINTY)

#define   ALARM_MINUTE_POINTX   70
#define   ALARM_MINUTE_POINTY   6
#define   ALARM_MINUTE          (ALARM_MINUTE_POINTX,ALARM_MINUTE_POINTY)

#define   NOTI_MAIL_POINTX      9
#define   NOTI_MAIL_POINTY      6
#define   NOTI_MAIL             (NOTI_MAIL_POINTX,NOTI_MAIL_POINTY)

#define   NOTI_CALL_POINTX      54
#define   NOTI_CALL_POINTY      6
#define   NOTI_CALL             (NOTI_CALL_POINTX,NOTI_CALL_POINTY)

#define   NOTI_SMS_POINTX       95
#define   NOTI_SMS_POINTY       6
#define   NOTI_SMS              (NOTI_SMS_POINTX,NOTI_SMS_POINTY)

#define   ALARM_POINTX       55
#define   ALARM_POINTY       3
#define   ALARM_ICON              (SMINUTE_POINTX,SMINUTE_POINTY)

#define  ANCS_ITEM1_X           0
#define  ANCS_ITEM1_Y           3
#define  ANCS_ITEM2_X           0
#define  ANCS_ITEM2_Y           5
#define  ANCS_ITEM3_X           0
#define  ANCS_ITEM3_Y           5

#define  GPS_DIS_POINTX         52
#define  GPS_DIS_POINTY         2

#define  FILENAME_POINTX        38
#define  FILENAME_POINTY        6

#define PERIOD_VALUE 12
#define WRITE_BUFFER 0
#define WRITE_LCD    1

extern const FONT_INFO *pFontInfo;// = &consolas_16ptFontInfo;
//function for watch
uint32_t DispalyInt(void );
void Switch_Page(uint8_t index, const ble_cts_date_time_t a_sTime,unsigned char toSubMode);
void SetDisplayInvert(uint8_t isTurnOn);
//draw the single
//need assign the x/y point and the rf single.
void DrawSingle(uint8_t x, uint8_t y, int value);
void UpdateRSSIStatus(unsigned char WorkMode, unsigned char SubMode,int value);
//draw the battery
//need assign the x/y point and the barry value.
void UpdateBatteryStatus(unsigned char WorkMode, unsigned char SubMode,int value);
void DrawBattery(uint8_t x, uint8_t y, int value);

void SetFont(uint8_t font_type);
//draw information display 
void SetCurrent_period(const ble_cts_date_time_t a_sTime);
void SetCurrent_time(const ble_cts_date_time_t a_sTime);
void SetCalendar(const ble_cts_date_time_t a_sTime);
void SetSmall_time(const ble_cts_date_time_t a_sTime);
void DrawSettinnItem(uint8_t index);
void ClearSettinnItem(uint8_t index);
void DrawDirectBar(uint8_t index);
void SetNotiNum(int8_t index);
int8_t GetNotiNum(void);
void DisplayAncsData(uint8_t select,int8_t);
void ShowAlarm_time(const ble_cts_date_time_t a_sTime);
void ShowAlarmIcon(void);
void ClearAlarmIcon(void);
void ClearNotiIcon(void);
void DrawNotiIcon(void);
void ClearNotiIcon(void);
void ClearSingle(uint8_t x, uint8_t y);
void DisplayAddressBook(uint8_t *);
uint8_t* GetAddressBookData(uint8_t select, int8_t index);
void ShowGPS_Distance(const uint8_t dis);
void ShowFileName(const uint8_t* filename);
//work mode
uint8_t isEntryIdle(void);
void m_oled_goto_idle(void);
void m_oled_go(void);
#endif
