#include "nrf_error.h"
#include "hal_spi.h"
#include "nrf_gpio.h"
#include "hw_config.h"
#include "nrf_delay.h"
#include "app_util.h"
#include "SSD1306.h"
#include "Fonts_Graphic.h"

#ifdef USE_DOUBLE_BUFFER
uint8_t     display_buffer[1024];
#endif

uint8_t     Negative  = 0;
uint8_t     lcdbuffer = 0;//unset is write on buffer,set write on buffer and lcd
uint8_t     dispx     = 0;
uint8_t     dispPage  = 0;

//for the font assert
extern const FONT_INFO microsoftSansSerif_8ptFontInfo;
extern const FONT_INFO fiveBySevenFontInfo;
extern const FONT_INFO consolas_16ptFontInfo;
// for graphic assert
extern const FIXGRAHPIC_INFO watch_graphic ;
extern const FRAMEGRAHPIC_INFO frame_graphic;

/* Local functions prototypes */
void LcdDataWrite (unsigned char);
void LcdInstructionWrite (unsigned char);
void LcdWaitBusy (void);
/* General use definitions */


/* Global variables */
unsigned char u8CursorX, u8CursorY;

extern const FONT_INFO *pFontInfo ;//= &consolas_16ptFontInfo;

void (*lcd_write_low) (int , uint8_t );
void(* lcd_write_high) (int , uint8_t );
void(* lcd_write) (uint8_t );
void (*lcd_gotoxy) (uint8_t, uint8_t);
//you shoulde be Replace your gpio driver
static void setbits(nrf_gpio_port_select_t LCD_CTRL, uint32_t bits)
{
    nrf_gpio_pin_set(bits);
}

static void clrbits(nrf_gpio_port_select_t LCD_CTRL, uint32_t bits)
{
    nrf_gpio_pin_clear(bits);
}

//typedef LCD_RES 		PIN_SPI_RES			//for reset the lcd chip, pls ref datasheet,timming
//typedef LCD_CMD 		PIN_SPI_CMDDATA //set for data ;unset for command
//typedef LCD_SWITCH	OLED_ONOFF			//set for full; unset for half

static uint8_t GPIO_INIT()
{
    //change your designed interface
    hal_spi_init_t spi_params;
    GPIO_PIN_CONFIG(PIN_SPI_RES,
                    GPIO_PIN_CNF_DIR_Output,
                    GPIO_PIN_CNF_INPUT_Disconnect,
                    GPIO_PIN_CNF_PULL_Pullup,
                    GPIO_PIN_CNF_DRIVE_S0S1,
                    GPIO_PIN_CNF_SENSE_Disabled);
    GPIO_PIN_CONFIG(PIN_SPI_CMDDATA,
                    GPIO_PIN_CNF_DIR_Output,
                    GPIO_PIN_CNF_INPUT_Disconnect,
                    GPIO_PIN_CNF_PULL_Pullup,
                    GPIO_PIN_CNF_DRIVE_S0S1,
                    GPIO_PIN_CNF_SENSE_Disabled);
    // SPI initialization
    spi_params.module    = SPI0;
    spi_params.mode      = SPI_MODE0;
    spi_params.frequency = Freq_1Mbps;
    spi_params.lsb_first = false;
    spi_params.sck_pin   = PIN_SPI_SCLK;
    spi_params.mosi_pin  = PIN_SPI_SDIN;
    spi_params.miso_pin  = PIN_NOT_AVAILABLE;
    spi_params.ss_pin    = PIN_SPI_CS;
    s_spi_module = hal_spi_init(&spi_params);

    if (s_spi_module == 0)
    {
        return NRF_ERROR_INTERNAL;
    }

    return NRF_SUCCESS;
}

static void write_cmd(uint8_t u8Instruction)
{
    //change by your designe, interface
    unsigned char ucTxbuf[2];
    hal_spi_tx_rx(s_spi_module, 1, &u8Instruction, ucTxbuf);
}
static void write_data(uint8_t Data)
{
    unsigned char ucTxbuf[2];
    hal_spi_tx_rx(s_spi_module, 1, &Data, ucTxbuf);
}

void write_buffer_display(uint8_t data)
{
    display_buffer[((uint16_t)(dispPage << 7)) + (dispx++)] |= data;
}

void writeLow_buffer_display(int i, uint8_t data)
{
    if(	Negative)
    {
        data = ~data;
    }

    display_buffer[((uint16_t)(((dispPage + i) << 7))) + (dispx)] |= data;
}
void writeHight_buffer_display(int i, uint8_t data)
{
    if(	Negative)
    {
        data = ~data;
    }

    display_buffer[((uint16_t)((dispPage + i) << 7)) + (dispx++)] |= data;
}

/*-----------------------------------------------------------------------------
  LCD Initialization
  GLCD_LcdINIT()
  -----------------------------------------------------------------------------*/
void GLCD_LcdInit(void)
{
    uint32_t err_code;
    //init the select interface
    err_code = GPIO_INIT();

    if(err_code != NRF_SUCCESS)
    {
        return;
    }

    // Recommended power up sequence
    setbits(LCD_CTRL, LCD_RES);
    nrf_delay_ms(3);
    setbits(LCD_CTRL, LCD_RES);         // Reset Low for 3 uS
    nrf_delay_ms(3);
    // Recommended intialization sequence
    LcdInstructionWrite(LCD_DISP_OFF);
    LcdInstructionWrite(LCD_SET_RATIO_OSC);
    LcdInstructionWrite(0x80);
    LcdInstructionWrite(LCD_MULTIPLEX);
    LcdInstructionWrite(0x3F);
    LcdInstructionWrite(LCD_SET_OFFSET);
    LcdInstructionWrite(0x00);
    LcdInstructionWrite(LCD_SET_LINE);
    LcdInstructionWrite(LCD_CHARGE_PUMP);
    LcdInstructionWrite(LCD_PUMP_ON);
    LcdInstructionWrite(LCD_SET_SEG_REMAP0);
    LcdInstructionWrite(LCD_SET_SCAN_FLIP);
    LcdInstructionWrite(LCD_SET_PADS);
    LcdInstructionWrite(0x12);
    LcdInstructionWrite(LCD_SET_CONTRAST);
    LcdInstructionWrite(0xFF);
    LcdInstructionWrite(LCD_SET_CHARGE);
    LcdInstructionWrite(0xF1);
    LcdInstructionWrite(LCD_SET_VCOM);
    LcdInstructionWrite(0x40);
    LcdInstructionWrite(LCD_EON_OFF);
    LcdInstructionWrite(LCD_DISP_NOR);
    LcdInstructionWrite(LCD_MEM_ADDRESSING);
    LcdInstructionWrite(0x00);          // Horizontal Addressing mode
    LcdInstructionWrite(LCD_DISP_ON);
    lcd_write_low  = writeLow_buffer_display;
    lcd_write_high = writeHight_buffer_display;
    lcd_write      = write_buffer_display;
    lcd_gotoxy     = buffer_gotoxy ;
}

/*-------------------------------------------------------------------------------
  Send instruction to the LCD
  LcdDataWrite (uint8_t u8Instruction)
  u8Instruction = Instructino to send to the LCD
  -------------------------------------------------------------------------------*/
void LcdInstructionWrite (uint8_t u8Instruction)
{
    //select by the interface
    clrbits(LCD_CTRL, LCD_RS);
    write_cmd(u8Instruction);
}

/*-------------------------------------------------------------------------------

  Send datas to the LCD

  LcdDataWrite (uint8_t u8Data)

  u8Data = data to send to the LCD

  -------------------------------------------------------------------------------*/

void LcdDataWrite (uint8_t u8Data)
{
    //select by the interface
    setbits(LCD_CTRL, LCD_RS);
    write_data(u8Data);
}
//buffer and the lcd ram
void write_lcd_display(uint8_t data)
{
    /*display_buffer[((uint16_t)(dispPage<<7)) + (dispx++)] |= data;*/
    lcd_toxy(dispx++, dispPage);
    LcdDataWrite(data );
}

void writeLow_lcd_display(int i, uint8_t data)
{
    if(	Negative)
    {
        data = ~data;
    }

    /*display_buffer[((uint16_t)(((dispPage+i)<<7))) + (dispx)] |= data;*/
    lcd_toxy(dispx, dispPage + i);
    LcdDataWrite(data );
}
void writeHight_lcd_display(int i, uint8_t data)
{
    if(	Negative)
    {
        data = ~data;
    }

    /*display_buffer[((uint16_t)((dispPage+i)<<7)) + (dispx++)] |= data;*/
    lcd_toxy(dispx++, dispPage + i);
    LcdDataWrite(data );
}
/*-------------------------------------------------------------------------------

  Print a char on the LCD

  GLCD_Putchar (uint8_t u8Char)

  u8Char = char to display

  -------------------------------------------------------------------------------*/

void drv_DrawChar(char u8Char)
{
    uint16_t pointer;
    uint8_t *pData, data;
    uint8_t u8CharColumn = 0;
    int i;
    dispx = u8CursorX;

    if (u8Char == '\n')  	// New line
    {
        u8CursorX = 0;
        u8CursorY += pFontInfo->heightPages;
        return;
    }

    uint16_t offset = u8Char - (pFontInfo->startChar);
    const FONT_CHAR_INFO* pFontChar = (pFontInfo->charInfo);
    FONT_CHAR_INFO sCharInfo = pFontChar[offset];

    if(u8CursorX + sCharInfo.widthBits > 127)
    {
        dispx = u8CursorX = 0;
        u8CursorY += pFontInfo->heightPages;
    }

    pData = (uint8_t*) ((pFontInfo->data) + sCharInfo.offset);

    /* Draw a char */
    while (u8CharColumn < (sCharInfo.widthBits))
    {
        if(Negative)
        {
            *pData = ~*pData;
        }

        dispPage = u8CursorY;

        //for(i = 0; i < pFontChar->hightByte - 1; i++)
				for(i = 0; i < sCharInfo.hightByte - 1; i++)
        {
            lcd_write_low(i, *(pData + (i * sCharInfo.widthBits)));
        }

        //if(pFontChar->hightByte == 1)
				if(sCharInfo.hightByte == 1)
        {
            lcd_write(*pData);
        }
        else
        {
            lcd_write_high(i, *(pData + (i * sCharInfo.widthBits)));
        }

        pData++;
        u8CharColumn++;
        u8CursorX++;
    }

    if(u8Char == '/')
    {
        lcd_write(0x70);
        u8CursorX++;
    }
    else if(u8Char == '(')
    {
        lcd_write(0x70);
        u8CursorX += 2;
    }
    else if(u8Char == '&')
    {
        lcd_write(0x10);
        u8CursorX++;
    }
    else if(u8CursorX < 127)
    {
        dispPage = u8CursorY; // Select the page of the LCD
        dispx = u8CursorX;
        data = 0;

        // 			if(Negative) data = 255;
       // for(i = 0; i < pFontChar->hightByte - 1; i++)
				for(i = 0; i < sCharInfo.hightByte - 1; i++)
        {
            lcd_write_low(i, data);
        }

        //if(pFontChar->hightByte == 1)
				if(sCharInfo.hightByte == 1)
        {
            lcd_write(data);
        }
        else
        {
            lcd_write_high(i, data);
        }

        u8CursorX++;
    }

    if(u8CursorX >= 128)
    {
        u8CursorX = 0;
        u8CursorY += pFontInfo->heightPages;
    }
}

void drv_ClearChar(char u8Char)
//void GLCD_Clearchar(char u8Char)
{
    uint16_t pointer;
    uint8_t pData, data;
    uint8_t u8CharColumn = 0;
    int i;
    dispx = u8CursorX;
    uint16_t offset = u8Char - (pFontInfo->startChar);
    const FONT_CHAR_INFO* pFontChar = (pFontInfo->charInfo);
    FONT_CHAR_INFO sCharInfo = pFontChar[offset];

    if(u8CursorX + sCharInfo.widthBits > 127)
    {
        dispx = u8CursorX = 0;
        u8CursorY += pFontInfo->heightPages;
    }

    pData = 0x00;

    /* Draw a char */
    while (u8CharColumn < (sCharInfo.widthBits))
    {
        if(Negative)
        {
            pData = ~pData;
        }

        dispPage = u8CursorY;

        for(i = 0; i < pFontChar->hightByte - 1; i++)
        {
            lcd_write_low(i, pData);
        }

        if(pFontChar->hightByte == 1)
        {
            lcd_write(pData);
        }
        else
        {
            lcd_write_high(i, pData);
        }

//         pData++;
        u8CharColumn++;
        u8CursorX++;
    }

    if(u8CursorX < 127)
    {
        dispPage = u8CursorY; // Select the page of the LCD
        dispx = u8CursorX;
        data = 0;

        // 			if(Negative) data = 255;
        for(i = 0; i < pFontChar->hightByte - 1; i++)
        {
            lcd_write_low(i, data);
        }

        if(pFontChar->hightByte == 1)
        {
            lcd_write(pData);
        }
        else
        {
            lcd_write_high(i, data);
        }

        u8CursorX++;
    }
}

//#ifdef USE_DOUBLE_BUFFER
// Clear display buffer
void clr_display(void)
{
    uint16_t i;

    for(i = 0; i < 1024; i++)
    {
        display_buffer[i] = 0;
    }
}

void full_display(void)
{
    uint16_t i;

    for(i = 0; i < 1024; i++)
    {
        display_buffer[i] = 0xFF;
    }
}

void LcdXset(uint8_t x)
{
    LcdInstructionWrite(LCD_SET_PAGE + (x & 0x07));
}

void LcdYset(uint8_t y)
{
    y += 2;
    LcdInstructionWrite(LCD_SET_COL_HI | (y >> 4));	// Set column at 2
    LcdInstructionWrite(LCD_SET_COL_LO | (y & 0x0F));
}
void lcd_toxy(uint8_t x, uint8_t y)
{
// 		u8CursorX= x;
// 		u8CursorY= y;
    LcdYset(x);
    //LcdXset(y>>3);
    LcdXset(y);
}

// Set pixel on display buffer
void set_pixel(uint8_t x, uint8_t y)
{
    display_buffer[((uint16_t)(y << 4) & 0xFF80) + x] |= (uint8_t)(0x01 << (y & 0x07));
}

uint8_t LcdDataRead(void)
{
    uint8_t data;
    return data;		// return the data read
}

// Transfer display buffer to LCD
void show_display(void)
{
    unsigned char u8Page, u8Column;
    uint8_t *p;
    LcdInstructionWrite(LCD_SET_PAGE);
    LcdInstructionWrite(LCD_SET_COL_HI);	// Set column at 0
    LcdInstructionWrite(LCD_SET_COL_LO);
    p = display_buffer;

    // process the 8 pages of the LCD
    for(u8Page = 0; u8Page < 8; u8Page++)
    {
        for(u8Column = 0; u8Column < 128; u8Column++)
        {
            LcdDataWrite(*p++);
        }
    }
}

static void BufferXset(uint8_t x)
{
    u8CursorX = x;
}
static void BufferYset(uint8_t y)
{
    u8CursorY = y;
}

void buffer_gotoxy(uint8_t x, uint8_t y)
{
    BufferXset(x);
    BufferYset(y);
}

void drv_DrawString (const char *ptr)
{
    while(*ptr)
    {
        drv_DrawChar (*ptr++);
    }
}

void drv_ClearString(const char *ptr)
{
    while(*ptr)
    {
        drv_ClearChar (*ptr++);
    }
}

void lcd_clear_graphics(void)
{
    unsigned char u8Page, u8Column;
    LcdInstructionWrite(LCD_SET_PAGE);
    LcdInstructionWrite(LCD_SET_COL_HI);	// Set column at 0
    LcdInstructionWrite(LCD_SET_COL_LO);

    /* process the 8 pages of the LCD */
    for(u8Page = 0; u8Page < 8; u8Page++)
    {
        for(u8Column = 0; u8Column < 128; u8Column++)
        {
            LcdDataWrite(0);
        }
    }
}

//assign the x,y and the pic(referenc the index from list)
void lcd_load_Block_graphics(int x, int y, int index)
{
    int i, u8PicColumn = 0;
    const FIXGRAHPIC_INFO *pGrahpicInfo = &watch_graphic;
    const GRAHPIC_PIC_INFO* pGraphic = (pGrahpicInfo->grahInfo);
    uint8_t *pData;
    GRAHPIC_PIC_INFO sPicInfo = pGraphic[index];
    //move the current
    BufferXset(x);
    BufferYset(y);
    lcd_gotoxy(x, y);
    dispx = u8CursorX;
    dispPage = u8CursorY;
    //to draw the fix graphic
    pData = (uint8_t*) ((pGrahpicInfo->data) + sPicInfo.offset);

    while (u8PicColumn < (sPicInfo.widthBits))
    {
        if(Negative)
        {
            *pData = ~*pData;
        }

        dispPage = u8CursorY;

        for(i = 0; i < sPicInfo.hightByte - 1; i++)
        {
            lcd_write_low(i, *(pData + (i * sPicInfo.widthBits)));
        }

        if(sPicInfo.hightByte == 1)
        {
            lcd_write(*pData);
        }
        else
        {
            lcd_write_high(i, *(pData + (i * sPicInfo.widthBits)));
        }

        pData++;
        u8PicColumn++;
        u8CursorX++;
    }
}

void lcd_clear_Block_graphics(int x, int y, int index)
{
    int i, u8PicColumn = 0;
    const FIXGRAHPIC_INFO *pGrahpicInfo = &watch_graphic;
    const GRAHPIC_PIC_INFO* pGraphic = (pGrahpicInfo->grahInfo);
    uint8_t data = 0x00;
    GRAHPIC_PIC_INFO sPicInfo = pGraphic[index];
    //move the current
    BufferXset(x);
    BufferYset(y);
    lcd_gotoxy(x, y);
    dispx = u8CursorX;
    dispPage = u8CursorY;

    while (u8PicColumn < (sPicInfo.widthBits))
    {
        if(Negative)
        {
            data = ~data;
        }

        dispPage = u8CursorY;

        for(i = 0; i < sPicInfo.hightByte - 1; i++)
        {
            lcd_write_low(i, data);
        }

        if(sPicInfo.hightByte == 1)
        {
            lcd_write(data);
        }
        else
        {
            lcd_write_high(i, data);
        }

        u8PicColumn++;
        u8CursorX++;
    }
}

void switch_frame(uint8_t index)
{
    unsigned char u8Page, u8Column;
    /* process the 8 pages of the LCD */
    const FRAMEGRAHPIC_INFO *pFrameInfo = &frame_graphic;
    const FRAME_INFO* pGraphic = (pFrameInfo->grahInfo);
    uint8_t *pData;
    FRAME_INFO pFrame = pGraphic[index];
    pData = (uint8_t*) ((pFrameInfo->data) + pFrame.offset);
    uint16_t i;

    for(i = 0; i < 1024; i++)
    {
        display_buffer[i] = *(pData + i);
    }
}

void SetInvert(uint8_t isOn)
{
    Negative = isOn;
    return;
}

/*void (*lcd_write_low) (int ,uint8_t );
  void(* lcd_write_high) (int ,uint8_t );
  void(* lcd_write) (uint8_t );
  void (*lcd_gotoxy) (uint8_t, uint8_t);*/
void SetWriteTarget(uint8_t isOnlcd)
{
    lcdbuffer = isOnlcd;

    if(isOnlcd == 0)
    {
        lcd_write_low = writeLow_buffer_display;
        lcd_write_high = writeHight_buffer_display;
        lcd_write = write_buffer_display;
        lcd_gotoxy = buffer_gotoxy;
    }
    else
    {
        lcd_write_low = writeLow_lcd_display;
        lcd_write_high = writeHight_lcd_display;
        lcd_write = write_lcd_display;
        lcd_gotoxy = lcd_toxy;
    }

    return;
}

//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//  Sleep Mode
//
//    "0x00" Enter Sleep Mode
//    "0x01" Exit Sleep Mode
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
void Sleep(unsigned char a)
{
    switch(a)
    {
    case 0:
        LcdInstructionWrite(LCD_DISP_OFF);//off
        LcdInstructionWrite(LCD_PUMP_OFF);
        nrf_delay_ms(50);
        break;

    case 1:
        LcdInstructionWrite(LCD_PUMP_ON);
        nrf_delay_ms(50);
        LcdInstructionWrite(LCD_EON_OFF);
        LcdInstructionWrite(LCD_DISP_ON);
        break;

    default:
        break;
    }
}
int8_t drv_LcdWidth(void)
{
    return LCD_WIDTH;
}

int8_t drv_LcdHeight(void)
{
    return LCD_HIGHT;
}

int8_t drv_LcdPixelBits(void)
{
    return PIXELBITS;
}

void drv_LcdSetForegroundVRamBase(uint32_t addr)
{
    return;
}
uint32_t drv_LcdGetForegroundVRamBase(void)
{
    return FOREGROUNDVRAMBASE;
}
void drv_LcdSetBackgroundVRamBase(uint32_t addr)
{
    return;
}
uint32_t drv_LcdGetBackgroundVRamBase(void)
{
    return BACKGROUNDVRAMBASE;
}
//#endif
