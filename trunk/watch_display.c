//watch_dispaly.c
//
/*****************************
 * File name:watch_dispaly.c
 * Directory:board/module/
 * Project name:none
 * Author:steven
 * Create Time:2014/2/24 下午 06:17:34
 * Program discrptor:

 Version:
 Copyright(C) steven.,Ltd.
 All Right Reserved

 History:
 2014/2/24 下午 06:17:43: version 0.2
 2014/2/25 下午 03:03:10: base the string/graphic address
 2014/3/4 上午 10:45:13 : add new function to draw the direct bar
 add new function -
void SetDisplayInvert(uint8_t isTurnOn):
invert display
assigned the fonts
*****************************/

/*****************************
 * INCLUDE FILE
*****************************/
#include <stdio.h>
#include "nrf_error.h"
#include "hal_spi.h"
#include "nrf_gpio.h"
#include "hw_config.h"
#include "watch_display.h"
#include "g_workmode.h"
#include "ANCS.h"
#include "m_rtcc.h"
/*****************************
 * CONSTANT Definition
*****************************/
//常數名稱,需全大寫表示,它所作用的範圍只有在此*.c
//常數的用途
//

const FONT_INFO *pFontInfo = &consolas_16ptFontInfo;
const static uint8_t g_month_string[][4] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
const static uint8_t g_week_string[][4] = {"", "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
char temp_hour[2] = {'0'}, temp_min[2] = {'0'};
uint8_t temp_month[3], temp_week[3];
static  int8_t index = 0;
extern sRtcc  sRTCC_CURRENT;

/*****************************
 * 資料結構與資料型別定義
*****************************/
//一般資料結構定義在*.h,除非這個結構只有在此*.c中使用
/*****************************
 * Internal Function Declaration
*****************************/
static uint8_t isIdleStatu = false;

typedef enum
{
    TURN_OFF_INVERT = 0,
    TURN_ON_INVERT
} Invert_type;


//宣告只有這個程式內部會用到的function( static function不會放在*.h中)

static void setIdleStatus(uint8_t set)
{
    isIdleStatu = set;
}
/*****************************
 * Function name: DispalyInt
 * Description: to init the ssd1306 driver
 * Parameters:
 * return value : none
 * Algorithm:
 * Note:
*****************************/
uint32_t DispalyInt(void )
{
    uint32_t err_code = NRF_SUCCESS;
    GLCD_LcdInit();
    clr_display	();
    show_display();
    return err_code;
}

/*****************************
 * Function name: Switch_Page
 * Description: to init the ssd1306 driver
 * Parameters:
 * return value : none
 * Algorithm:
 * Note:
 * only load the page into buffer,but not show(write into
 * ssd1306,should be write all information then show
*****************************/
void Switch_Page(uint8_t index, const ble_cts_date_time_t a_sTime, unsigned char toSubMode)
{
    SetWriteTarget(WRITE_BUFFER);
    //when every switch the page ,the buffer be clear
    clr_display();
    SetDisplayInvert(TURN_OFF_INVERT);
    //reset the u8CursorX/Y
//     buffer_gotoxy(ORIGIN_POINTX,ORIGIN_POINTY);
    lcd_gotoxy(ORIGIN_POINTX, ORIGIN_POINTY);
    switch_frame(index);

    //for init the single /battery and call other function to init
    //to draw the battery
    switch(index)
    {
    case MAIN_FRAME:
				if(toSubMode & SUB_MBT)
					DrawSingle(MSINGLE_POINTX, MSINGLE_POINTY, -30);
				
        DrawBattery(MBATTERY_POINTX, MBATTERY_POINTY, 55);

        if(isNeedNoti)
        {
            DrawNotiIcon();
        }

        SetCurrent_time(a_sTime);
        SetCurrent_period(a_sTime);
        SetCalendar(a_sTime);
        break;

    case SETTING_FRAME:
				if(toSubMode & SUB_MBT)
					DrawSingle(SINGLE_POINTX, SINGLE_POINTY, -30);

        DrawBattery(BATTERY_POINTX, BATTERY_POINTY, 55);
        SetSmall_time(a_sTime);
        //load the icon
        lcd_load_Block_graphics(SSETITEM_POINTX, SSETITEM_POINTY, PAIR);
        break;

    case SELECT_FRAME:
				if(toSubMode & SUB_MBT)
					DrawSingle(SINGLE_POINTX, SINGLE_POINTY, -30);

        DrawBattery(BATTERY_POINTX, BATTERY_POINTY, 55);
        SetSmall_time(a_sTime);

        //show the select item
        if(toSubMode == SUB_BRIGH)
        {
            lcd_load_Block_graphics(SSELECT_POINTX, SSELECT_POINTY, SELECT_LIGHT);
            lcd_load_Block_graphics(DIRECT_POINTX, DIRECT_POINTY, PULL_LEFT);
        }
//         else if(toSubMode == SUB_BRIGH_1)
//         {
//             lcd_load_Block_graphics(SSELECT_POINTX, SSELECT_POINTY, SELECT_LIGHT);
//             lcd_load_Block_graphics(DIRECT_POINTX, DIRECT_POINTY, PULL_MID);
//         }
//         else if(toSubMode == SUB_BRIGH_2)
//         {
//             lcd_load_Block_graphics(SSELECT_POINTX, SSELECT_POINTY, SELECT_LIGHT);
//             //lcd_load_Block_graphics(DIRECT_POINTX,DIRECT_POINTY,PULL_RIG);
//             lcd_load_Block_graphics(DIRECT_POINTX, DIRECT_POINTY, PULL_MID);
//         }
        else if(toSubMode == SUB_BUZZER)
        {
            lcd_load_Block_graphics(SSELECT_POINTX, SSELECT_POINTY, SELECT_BUZZER);
            lcd_load_Block_graphics(DIRECT_POINTX, DIRECT_POINTY, PULL_LEFT);
        }
//         else if(toSubMode == SUB_BUZZER_OFF)
//         {
//             lcd_load_Block_graphics(SSELECT_POINTX, SSELECT_POINTY, SELECT_BUZZER);
//             lcd_load_Block_graphics(DIRECT_POINTX, DIRECT_POINTY, PULL_RIG);
//         }
        else
            ;

        break;

    case ALARM_FRAME:
				if(toSubMode & SUB_MBT)
					DrawSingle(SINGLE_POINTX, SINGLE_POINTY, -30);

        DrawBattery(BATTERY_POINTX, BATTERY_POINTY, 55);
        SetSmall_time(a_sTime);
        ShowAlarm_time(sRTCC_CURRENT.sDateAlarm);
        break;

    case NOTI_FRAME:
//GetNotifNum(ANS_TYPE_EMAIL), GetNotifNum(ANS_TYPE_MISSEDCALL),GetNotifNum(ANS_TYPE_SOCIAL)
        ClearNotiIcon();
				if(toSubMode & SUB_MBT)
					DrawSingle(SINGLE_POINTX, SINGLE_POINTY, -30);

        DrawBattery(BATTERY_POINTX, BATTERY_POINTY, 55);
        SetSmall_time(a_sTime);
        SetNotiNum(0);
        break;

    case NNOTI_FRAME:
				if(toSubMode & SUB_MBT)
					DrawSingle(SINGLE_POINTX, SINGLE_POINTY, -30);

        DrawBattery(BATTERY_POINTX, BATTERY_POINTY, 55);
        SetSmall_time(a_sTime);
        break;
				
    case ADDRESS_FRAME:
					;//show the title
        break;
				
    case PAIR_FRAME:
        if(toSubMode == SUB_PAIR)
        {
            lcd_load_Block_graphics(PAIR_POINTX, PAIR_POINTY, TOPAIR);
        }

        break;
    
		case SPORT_FRAME:
				//show the file name ,but do't show now
				//the gps distance show 00
				//show the shake ,no now
				ShowGPS_Distance(00);
				//ShowFileName("abcmp3");
			break;
				
    default:
        break;
    }

    show_display();
    SetWriteTarget(WRITE_LCD);
}

/*****************************
 * Function name: SetDisplayInvert
 * Description: the display data invert 0 to 1
 * Parameters: isTurnOn ,set invert ; unset ,no change
 * return value : none
 * Algorithm:
 * Note:
 * for the SSD1306 driver, maybe you need pregrame data.
*****************************/
void SetDisplayInvert(uint8_t isTurnOn)
{
    if(isTurnOn)
    {
        SetInvert(TURN_ON_INVERT);
    }
    else
    {
        SetInvert(TURN_OFF_INVERT);
    }

    return;
}

/*****************************
 * Function name: DrawSingle
 * Description: draw the single on frame,this is write by
 * Parameters: uint8_t x,uint8_t y, int value
 * return value : none
 * Algorithm:
 * Note:
 * write byte by OR logic
*****************************/
void DrawSingle(uint8_t x, uint8_t y, int value)
{
    uint8_t index = 0;

    if(value > -20)
    {
        index = SIGNAL_80;
    }
    else if(value > -40)
    {
        index = SIGNAL_60;
    }
    else if(value > -60)
    {
        index = SIGNAL_40;
    }
    else if(value < -80)
    {
        index = SIGNAL_20;
    }
    else
        ;

    lcd_load_Block_graphics(x, y, index);
    return;
}

/*****************************
 * Function name: ClearSingle
 * Description: clear the single on frame,this is write by
 * Parameters: uint8_t x,uint8_t y
 * return value : none
 * Algorithm:
 * Note:
 * clear the single icon
*****************************/
void ClearSingle(uint8_t x, uint8_t y)
{
	lcd_clear_Block_graphics(x,y,SIGNAL_80);
}
/*****************************
 * Function name: DrawBattery
 * Description: draw the battery value on frame,this is write by
 * Parameters: uint8_t x,uint8_t y, int value
 * return value : none
 * Algorithm:
 * Note:
 * write byte by OR logic
*****************************/
void DrawBattery(uint8_t x, uint8_t y, int value)
{
    uint8_t index = 0;

    if(value > 75)
    {
        index = BATTERY_80;
    }
    else if(value > 49)
    {
        index = BATTERY_60;
    }
    else if(value > 24)
    {
        index = BATTERY_40;
    }
    else if(value > 24)
    {
        index = BATTERY_20;
    }
    else
        ;

    lcd_load_Block_graphics(x, y, index);
    return;
}

/*****************************
 * Function name: DrawSettinnItem
 * Description: draw the setting item by parameter
 * Parameters: uint8_t index
 * return value : none
 * Algorithm:
 * Note:
 * shoule be check in function workmode
 * parameter has
*****************************/
void DrawSettinnItem(uint8_t index)
{
    lcd_load_Block_graphics(SSETITEM_POINTX, SSETITEM_POINTY, index);
    return;
}

/*****************************
 * Function name: ClearSettinnItem
 * Description: clear the setting item by parameter
 * Parameters: uint8_t index
 * return value : none
 * Algorithm:
 * Note:
 * shoule be check in function workmode
 * parameter has
*****************************/
void ClearSettinnItem(uint8_t index)
{
    lcd_clear_Block_graphics(SSETITEM_POINTX, SSETITEM_POINTY, index);
    return;
}
/*****************************
 * Function name: SetFont
 * Description: set the font by define
 * Parameters: font_type
 * return value : none
 * Algorithm:
 * Note:
 * reference the define fontinfo from Fonts_Graphic
*****************************/
void SetFont(uint8_t font_type)
{
    if(font_type == FONT_MESSAGE)
    {
        pFontInfo = &microsoftSansSerif_8ptFontInfo;
    }
    else if(font_type == FONT_NOTINUM)
    {
        pFontInfo = &consolas_16ptFontInfo;
    }
    else if(font_type == FONT_OTHER)
    {
        pFontInfo = &date_16ptFontInfo;
    }
    else if(font_type == FONT_TIME)
    {
        pFontInfo = &consolas_32ptFontInfo;
    }
    else
        ;

    return;
}

/*****************************
 * Function name: SetCurrent_period
 * Description: write the current period to display
 * Parameters: ble_cts_date_time_t
 * return value : none
 * Algorithm:
 * Note:
 * should be clear and write
 * 1.set Font, 2. paser the time struct and format string
 * 3.set the current x/y 4.clear the buffer and write
*****************************/
void SetCurrent_period(const ble_cts_date_time_t a_sTime)
{
    SetFont(FONT_OTHER);
    //SetOverWrite(0);
    buffer_gotoxy(MAMPM_POINTX, MAMPM_POINTY);
    lcd_gotoxy(MAMPM_POINTX, MAMPM_POINTY);

    if(a_sTime.hours > PERIOD_VALUE)
    {
        drv_DrawString("PM");
    }
    else
    {
        drv_DrawString("AM");
    }

    return;
}

/*****************************
 * Function name: SetCurrent_time
 * Description: write the current time to display
 * Parameters: ble_cts_date_time_t
 * return value : none
 * Algorithm:
 * Note:
 * should be clear and write
 * 1.set Font, 2. paser the time struct and format string
 * 3.set the current x/y 4.clear the buffer and write
*****************************/
void SetCurrent_time(const ble_cts_date_time_t a_sTime)
{
    char hour[2], min[2];

    if ((a_sTime.hours >= 24) || (a_sTime.minutes >= 60) )
    {
        return;
    }

    SetFont(FONT_TIME);
    //SetOverWrite(0);//overwrite display
    sprintf(hour, "%02d", a_sTime.hours);
    sprintf(min, "%02d", a_sTime.seconds);
// 		buffer_gotoxy(MHOUR_POINTX,MHOUR_POINTY);
//     lcd_gotoxy(MHOUR_POINTX,MHOUR_POINTY);
// 		lcd_clrs(temp_hour);
    buffer_gotoxy(MHOUR_POINTX, MHOUR_POINTY);
    lcd_gotoxy(MHOUR_POINTX, MHOUR_POINTY);
    drv_DrawString(hour);
// 		memcpy(temp_hour,hour,2);
// 		buffer_gotoxy(MMINUTE_POINTX,MMINUTE_POINTY);
//     lcd_gotoxy(MMINUTE_POINTX,MMINUTE_POINTY);
// 		lcd_clrs(temp_min);
    buffer_gotoxy(MMINUTE_POINTX, MMINUTE_POINTY);
    lcd_gotoxy(MMINUTE_POINTX, MMINUTE_POINTY);
    drv_DrawString(min);
// 		memcpy(temp_min,min,2);
    return;
}


/*****************************
 * Function name: SetSmall_time
 * Description: write the current time to display on other page
 * Parameters: ble_cts_date_time_t
 * return value : none
 * Algorithm:
 * Note:
 * should be clear and write
 * 1.set Font, 2. paser the time struct and format string
 * 3.set the current x/y 4.clear the buffer and write
*****************************/
void SetSmall_time(const ble_cts_date_time_t a_sTime)
{
    char hour[2], min[2];

    if ((a_sTime.hours >= 24) || (a_sTime.minutes >= 60) )
    {
        return;
    }

    SetFont(FONT_OTHER);
    sprintf(hour, "%02d", a_sTime.hours);
    sprintf(min, "%02d", a_sTime.seconds);
    buffer_gotoxy(HOUR_POINTX, HOUR_POINTY);
    lcd_gotoxy(HOUR_POINTX, HOUR_POINTY);
    drv_DrawString(hour);
    buffer_gotoxy(MINUTE_POINTX, MINUTE_POINTY);
    lcd_gotoxy(MINUTE_POINTX, MINUTE_POINTY);
    drv_DrawString(min);
    return;
}
/*****************************
 * Function name: isEntryIdle
 * Description: return the oled is into sleep mode
 * Parameters: none
 * return value : uint8_t
 * Algorithm:
 * Note:
 * set is into sleep mode
 * unset is....normal mode
*****************************/
uint8_t isEntryIdle()
{
    return isIdleStatu;
}

/*****************************
 * Function name: m_oled_goto_idle
 * Description: call when the timeout,switch to write into buffer and close the oled
 * Parameters: none, the power manage callback must none
 * return value : none
 * Algorithm:
 * Note:
 * call when the power manager idle timeout,call
*****************************/
void m_oled_goto_idle(void)
{
    if(! isEntryIdle())
    {
        Sleep(0x0);   //else display the oled
        setIdleStatus(true);
        //close the i2c
    }
}


/*****************************
 * Function name: m_oled_go
 * Description: wakeup call when device into sleep mode,
 * Parameters: none
 * return value : none
 * Algorithm:
 * Note:
 * switch oled write to direct lcd and write back buffer to led ram.
*****************************/
void m_oled_go(void)
{
    setIdleStatus(false);
    Sleep(0x1);   //else display the oled
}

/*****************************
 * Function name: SetCurrent_time
 * Description: write the current time to display
 * Parameters: ble_cts_date_time_t
 * return value : none
 * Algorithm:
 * Note:
 * should be clear and write
 * 1.set Font, 2. paser the time struct and format string
 * 3.set the current x/y 4.clear the buffer and write
*****************************/
void SetCalendar(const ble_cts_date_time_t a_sTime)
{
    char puctimeString[16];

    if ( (0 == a_sTime.month) || (0 == a_sTime.day_of_week))
    {
        return;
    }

    SetFont(FONT_OTHER);
    buffer_gotoxy(MDATE_POINTX, MDATE_POINTY);
    lcd_gotoxy(MDATE_POINTX, MDATE_POINTY);
    sprintf(puctimeString, "%s", g_month_string[a_sTime.month - 1]);
    //sprintf(puctimeString, "%s,%02d,%04d,%s", g_month_string[a_sTime.month - 1]);
    //, a_sTime.day, a_sTime.year,
    //        g_week_string[a_sTime.day_of_week]);
    //drv_DrawString(puctimeString);
    return;
}
/*****************************
 * Function name: UpdateBatteryStatus
 * Description: update the battery status and draw into display.
 * Parameters: the work mode and the battery
 * return value : none
 * Algorithm:
 * Note:
*****************************/
void UpdateBatteryStatus(unsigned char WorkMode, unsigned char SubMode, int value)
{
    if(WorkMode == MAIN_PAGE)
    {
        //draw the battery by the sub mode,sometime should not show the battery state
        if(SubMode == SUB_PAIR)
        {
            return;
        }
        else //other sub mode draw the battery
        {
            DrawBattery(BATTERY_POINTX, BATTERY_POINTY, value);
        }
    }
}

/*****************************
 * Function name: UpdateRSSIStatus
 * Description: update the rssi status and draw into display.
 * Parameters: the work mode and the rssi
 * return value : none
 * Algorithm:
 * Note:
*****************************/
void UpdateRSSIStatus(unsigned char WorkMode, unsigned char SubMode, int value)
{
        //draw the battery by the sub mode,sometime should not show the battery state
        if(SubMode == SUB_PAIR||SubMode == SUB_MESSGE)
        {
            return;
        }
        else if(WorkMode == MAIN_PAGE)//other sub mode draw the battery
        {
            lcd_gotoxy(MSINGLE_POINTX, MSINGLE_POINTY);
            DrawSingle(MSINGLE_POINTX, MSINGLE_POINTY, value);
        }
        else//other sub mode draw the battery
        {
            lcd_gotoxy(SINGLE_POINTX, SINGLE_POINTY);
            DrawSingle(SINGLE_POINTX, SINGLE_POINTY, value);
        }				
    return;
}

/*****************************
 * Function name: DrawDirectBar
 * Description: draw the direct bar(have left,mid and right)
 * Parameters: uint8_t index
 * return value : none
 * Algorithm:
 * Note:
 * index PULL_LEFT,PULL_MID and PULL_RIG
*****************************/
void DrawDirectBar(uint8_t index)
{
    if(index > 24 || index < 20) //should be have three status
    {
        return;
    }

    lcd_load_Block_graphics(DIRECT_POINTX, DIRECT_POINTY, index);
    return;
}

/*****************************
 * Function name: SetNotiNum
 * Description: print the number of the A.N.C.S notification number
 * Parameters: int8_t Email,int8_t MissCall,int8_t Social,int index
 * return value : none
 * Algorithm:
 * Note:
 * 1.set the font, 2.have set font invert by the index 3.unset 4.printf the number of the number of notification
 * count
*****************************/
void SetNotiNum(int8_t offset)
{
    int8_t i = 0;
    char ENum[2];
    //set the font
    SetFont(FONT_NOTINUM);
    index += offset;

    //dobule check
    if( index > 2)
    {
        index = 0;
    }
    else if(index < 0)
    {
        index = 2;
    }

    //GetNotifNum(ANS_TYPE_EMAIL), GetNotifNum(ANS_TYPE_MISSEDCALL),
    //            GetNotifNum(ANS_TYPE_SOCIAL)
    for(i = 0 ; i < 3; i++)
    {
        if(i == index)//which one is select item
        {
            SetDisplayInvert(TURN_ON_INVERT);
        }
        else
        {
            SetDisplayInvert(TURN_OFF_INVERT);
        }

        if(i == 0) //mail
        {
            buffer_gotoxy(NOTI_MAIL_POINTX, NOTI_MAIL_POINTY);
            lcd_gotoxy(NOTI_MAIL_POINTX, NOTI_MAIL_POINTY);
            sprintf(ENum, "%01d", GetNotifNum(ANS_TYPE_EMAIL));
        }
        else if(i == 1) //miss call
        {
            buffer_gotoxy(NOTI_CALL_POINTX, NOTI_CALL_POINTY);
            lcd_gotoxy(NOTI_CALL_POINTX, NOTI_CALL_POINTY);
            sprintf(ENum, "%01d", GetNotifNum(ANS_TYPE_MISSEDCALL));
        }
        else //sms
        {
            buffer_gotoxy(NOTI_SMS_POINTX, NOTI_SMS_POINTY);
            lcd_gotoxy(NOTI_SMS_POINTX, NOTI_SMS_POINTY);
            sprintf(ENum, "%01d", GetNotifNum(ANS_TYPE_SOCIAL));
        }

        drv_DrawString(ENum);
    }

    SetDisplayInvert(TURN_OFF_INVERT);
    return;
}

/*****************************
 * Function name: GetNotiNum
 * Description: get the index for the get ancs message
 * Parameters: none
 * return value : int8_t ,a index
 * Algorithm:
 * Note:
 * should be define the returnl value for Explanation which one,reference the ancs define
*****************************/
int8_t GetNotiNum(void)
{
    if(index == 0)
    {
        return ANS_TYPE_EMAIL;
    }
    else if(index == 1)
    {
        return ANS_TYPE_MISSEDCALL;
    }
    else
    {
        return ANS_TYPE_SOCIAL;
    }
}
/*****************************
 * Function name: ShowAlarm_time
 * Description: show the current time to display on other page
 * Parameters: ble_cts_date_time_t
 * return value : none
 * Algorithm:
 * Note:
 * should be clear and write
 * 1.set Font, 2. paser the time struct and format string
 * 3.set the current x/y 4.clear the buffer and write
*****************************/
void ShowAlarm_time(const ble_cts_date_time_t a_sTime)
{
    char hour[2], min[2];

    if ((a_sTime.hours >= 24) || (a_sTime.minutes >= 60) )
    {
        return;
    }

    SetFont(FONT_OTHER);
    sprintf(hour, "%02d", a_sTime.hours);
    sprintf(min, "%02d", a_sTime.seconds);
    buffer_gotoxy(ALARM_HOUR_POINTX, ALARM_HOUR_POINTY);
    lcd_gotoxy(ALARM_HOUR_POINTX, ALARM_HOUR_POINTY);
    drv_DrawString(hour);
    buffer_gotoxy(ALARM_MINUTE_POINTX, ALARM_MINUTE_POINTY);
    lcd_gotoxy(ALARM_MINUTE_POINTX, ALARM_MINUTE_POINTY);
    drv_DrawString(min);
    return;
}

/*****************************
 * Function name: displayAncsData
 * Description: dispaly the ances data by the select(reference the ancs category
 * Parameters: uint8_t select
 * return value : none
 * Algorithm:
 * Note:
 * should be clear and write
 * 1.set Font* 2.set the current x/y
*****************************/
void DisplayAncsData(uint8_t select, int8_t index)
{
    char pmsgString[16];
    uint8_t *u8NotiData, u8Time;
    int8_t account = 0, CoordinateY = ANCS_ITEM1_Y;
    SetFont(FONT_OTHER);
    u8NotiData = getUnMailNum(select);

    for(u8Time = 0; u8Time < u8NotiData[0]; u8Time++)
    {
        memset(pmsgString, '\0', sizeof(char) * 16);
        memcpy(pmsgString, u8NotiData + 1 + 10 * u8Time, sizeof(uint8_t) * 10);


        if(account == index)
        {
            SetInvert(1);
        }
        else
        {
            SetInvert(0);
        }

        buffer_gotoxy(ANCS_ITEM1_X, CoordinateY);
        lcd_gotoxy(ANCS_ITEM1_X, CoordinateY);
        CoordinateY += 2;
        drv_DrawString(pmsgString);

        if(++account > 2)
        {
            break;
        }
    }
    SetInvert(0);
}

/*****************************
 * Function name: GetAncsData
 * Description: get the ances data by the select(reference the ancs category
 * Parameters: uint8_t select
 * return value : none
 * Algorithm:
 * Note:
 * should be clear and write
 * 1.set Font* 2.set the current x/y
*****************************/
uint8_t AddBookString[16];

uint8_t* GetAddressBookData(uint8_t select, int8_t index)
{
    uint8_t *u8NotiData, u8Time;
    u8NotiData = getUnMailNum(select);
    memset(AddBookString, '\0', sizeof(char) * 16);
    memcpy(AddBookString, u8NotiData + 1 + 10 * index, sizeof(uint8_t) * 10);
		return AddBookString;
}


/*****************************
 * Function name: DisplayAddressBook
 * Description: dispaly address book infor by the select(reference the ancs category
 * Parameters: 
 * return value : none
 * Algorithm:
 * Note:
 * should be clear and write
 * 1.set Font* 2.set the current x/y
*****************************/
void DisplayAddressBook(uint8_t* u8Title)
{
    SetFont(FONT_OTHER);
    buffer_gotoxy(ADDBOOK_TITLE_X, ADDBOOK_TITLE_Y);
    lcd_gotoxy(ADDBOOK_TITLE_X, ADDBOOK_TITLE_Y);	
		drv_DrawString(u8Title);
    buffer_gotoxy(ADDBOOK_CONTENT_X, ADDBOOK_CONTENT_Y);
    lcd_gotoxy(ADDBOOK_CONTENT_X, ADDBOOK_CONTENT_Y);	
}


/*****************************
 * Function name: ShowAlarmIcon
 * Description: dispaly the the alarm icon
 * Parameters: none
 * return value : none
 * Algorithm:
 * Note:
*****************************/
void ShowAlarmIcon(void)
{
    lcd_load_Block_graphics(ALARM_POINTX, ALARM_POINTY, VIBRATEICON);
    return;
}

/*****************************
 * Function name: ClearAlarmIcon
 * Description: clear the the alarm icon
 * Parameters: none
 * return value : none
 * Algorithm:
 * Note:
*****************************/
void ClearAlarmIcon(void)
{
    lcd_clear_Block_graphics(ALARM_POINTX, ALARM_POINTY, VIBRATEICON);
    return;
}

/*****************************
 * Function name: DrawNotiIcon
 * Description: draw notification icon
 * Parameters: none
 * return value : none
 * Algorithm:
 * Note:
 * draw the Notification icon when switch difference page
 * and check the isNeedNoti varible
*****************************/
void DrawNotiIcon(void)
{
    lcd_load_Block_graphics(ANCS_NOTI_POINTX, ANCS_NOTI_POINTY, BULB_ON);
    return;
}

/*****************************
 * Function name: ClearNotiIcon
 * Description: clear the the notification icon,when you entry
 *              the Noti page,unset the isNeedNoti varible and clear
 *              notification icon
 * Parameters: none
 * return value : none
 * Algorithm:
 * Note:
*****************************/
void ClearNotiIcon(void)
{
    isNeedNoti = 0;
    lcd_clear_Block_graphics(ANCS_NOTI_POINTX, ANCS_NOTI_POINTY, BULB_ON);
    return;
}

/*****************************
 * Function name: ShowGPS_Distance
 * Description: show the gps distance form central to display 
 * Parameters: uint8_t u8GpsDistance
 * return value : none
 * Algorithm:
 * Note:
 * should be clear and write
 * 1.set Font,  2.set the current x/y 3.clear the buffer and write
*****************************/
void ShowGPS_Distance(const uint8_t dis)
{
    char distance[4];
    SetFont(FONT_OTHER);
    sprintf(distance, "%02d", dis);
    buffer_gotoxy(GPS_DIS_POINTX, GPS_DIS_POINTY);
    lcd_gotoxy(GPS_DIS_POINTX, GPS_DIS_POINTY);
    drv_DrawString(distance);
    return;
}

/*****************************
 * Function name: ShowFileName
 * Description: show the music file form central to display 
 * Parameters: uint8_t *filename
 * return value : none
 * Algorithm:
 * Note:
 * should be clear and write
 * 1.set Font, 
 * 2.set the current x/y 3.clear the buffer and write
*****************************/
void ShowFileName(const uint8_t* filename)
{
    SetFont(FONT_OTHER);
    buffer_gotoxy(FILENAME_POINTX, FILENAME_POINTY);
    lcd_gotoxy(FILENAME_POINTX, FILENAME_POINTY);
    drv_DrawString(filename);
    return;
}
