#ifndef SSD1306_H
#define SSD1306_H

#include "gpio_def_iwatch_alford1224.h"
#include "hal_spi.h"
// Remove comment to use double buffer, requires 1k of RAM
#define USE_DOUBLE_BUFFER
//#include <stdint.h>
#define LCD_RES      	PIN_SPI_RES                 // RESET
#define LCD_RS           PIN_SPI_CMDDATA             // Data / Instruction
#define LCD_RW           PIN_NOT_AVAILABLE           // Read / Write
#define LCD_E        	PIN_SPI_CS                  // Enable
#define LCD_CTRL         NRF_GPIO_PORT_SELECT_PORT3
#define LCD_DATA_OUT     SPI0
#define LCD_DATA_IN      SPI0
#define LCD_DDR          SPI0

#define LCD_SET_COL_HI           	0x10
#define LCD_SET_COL_LO           	0x00
#define LCD_SET_LINE             	0x40
#define LCD_SET_CONTRAST         	0x81
#define LCD_SET_SEG_REMAP0          0xA0
#define LCD_SET_SEG_REMAP1	        0xA1
#define LCD_EON_OFF              	0xA4
#define LCD_EON_ON               	0xA5
#define LCD_DISP_NOR             	0xA6
#define LCD_DISP_REV             	0xA7
#define LCD_MULTIPLEX               0xA8
#define LCD_CHARGE_PUMP          	0x8D
#define LCD_PUMP_OFF             	0x10
#define LCD_PUMP_ON              	0x14
#define LCD_DISP_OFF             	0xAE
#define LCD_DISP_ON              	0xAF
#define LCD_SET_PAGE             	0xB0
#define LCD_SET_SCAN_FLIP        	0xC0
#define LCD_SET_SCAN_NOR         	0xC8
#define LCD_SET_OFFSET           	0xD3
#define LCD_SET_RATIO_OSC        	0xD5
#define LCD_SET_CHARGE           	0xD9
#define LCD_SET_PADS             	0xDA
#define LCD_SET_VCOM             	0xDB
#define LCD_NOP                  	0xE3
#define LCD_SCROLL_RIGHT         	0x26
#define LCD_SCROLL_LEFT          	0x27
#define LCD_SCROLL_VR            	0x29
#define LCD_SCROLL_VL            	0x2A
#define LCD_SCROLL_OFF           	0x2E
#define LCD_SCROLL_ON            	0x2F
#define LCD_SCROLL_ON            	0x2F
#define LCD_VERT_SCROLL_A        	0xA3
#define LCD_MEM_ADDRESSING       	0x20
#define LCD_SET_COL_ADDR         	0x21
#define LCD_SET_PAGE_ADDR        	0x22

#define LCD_WIDTH       128
#define LCD_HIGHT       64
#define PIXELBITS       1
#define FOREGROUNDVRAMBASE 0x00
#define BACKGROUNDVRAMBASE 0x00

extern      uint16_t    pointer;
extern      uint8_t     Underline;
extern      uint8_t     Negative;
extern      uint8_t     Sub;

#define lcd_goto(x,y) { u8CursorX=(x); u8CursorY=(y); }
/* EXTERN Function Prototype(s) */
void GLCD_LcdInit (void);
void lcd_clear_graphics (void);
void GLCD_DisplayPicture (const uint8_t *pointer);
//void GLCD_Locate (uint8_t, uint8_t);
void GLCD_Print (const char *);
//uint8_t lcd_putsp(const char *);
//void lcd_puts (const char *);
//void lcd_clrs(char *);
//void GLCD_Clearchar(char);
void clr_display(void);
void show_display(void);
void set_pixel(uint8_t x, uint8_t y);
void lcd_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);

void write_buffer_display(uint8_t data);
void writeLow_buffer_display(int i, uint8_t data);
void writeHight_buffer_display(int i, uint8_t data);

void write_lcd_display(uint8_t data);
void writeLow_lcd_display(int i, uint8_t data);
void writeHight_lcd_display(int i, uint8_t data);

void zero_display(void);
void printF(uint8_t x, uint8_t y, int32_t Data);
uint8_t tiny_printp(uint8_t x, uint8_t y, const char *ptr);
uint8_t LcdDataRead(void);
void full_display(void);
void buffer_gotoxy(uint8_t x, uint8_t y);
void lcd_toxy(uint8_t x, uint8_t y);
void lcd_clear_graphics(void);
void lcd_load_Block_graphics(int x, int y, int index);
void lcd_clear_Block_graphics(int x, int y, int index);
void switch_frame(uint8_t index);
void SetInvert(uint8_t isOn);
void SetFont(uint8_t font_type);
void lcd_write_Block_graphics(int x, int y, int index);
void SetWriteTarget(uint8_t isOverWrite);
void Sleep(unsigned char a);

int8_t drv_LcdWidth(void);
int8_t drv_LcdHeight(void);
int8_t drv_LcdPixelBits(void);
//void drv_LcdBackLightOn(void);
//uint8_t drv_LcdBackLightStatus(void);
//drv_LcdSetContrast(..);
//drv_LcdGetContrast(void);
//drv_SetLcdDrawType
//drv_GetLcdDrawType
void drv_LcdSetForegroundVRamBase(uint32_t);
uint32_t drv_LcdGetForegroundVRamBase(void);
void drv_LcdSetBackgroundVRamBase(uint32_t);
uint32_t drv_LcdGetBackgroundVRamBase(void);
//drv_LcdUpdate(void);
//drv_LcdUpdateArea(..);
//drv_DrawPixel
//drv_DrawVLine(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2)
//drv_DrawHLine(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2)
//drv_DrawLine(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2)
//drv_DrawRect(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2)
//drv_GetImage(..);
//drv_PutImage
//void drv_ClearScreen(void)
void drv_DrawChar(char);
void drv_ClearChar(char);
void drv_DrawString(const char *);
void drv_ClearString(const char *);

extern void (*lcd_gotoxy) (uint8_t, uint8_t);
/* Global variables */
extern unsigned char u8CursorX, u8CursorY;
extern unsigned char dispPage, dispx;
extern uint32_t* s_spi_module;

#endif

