#include "common.h"

/*  Defines  */

#define PAD_REPEAT_DELAY    (FPS / 4)
#define PAD_REPEAT_INTERVAL (FPS / 12)

#define IMG_BUTTONS_W       7
#define IMG_BUTTONS_H       7

/*  Global Variables  */

MyArduboy2  arduboy;
uint8_t     counter;
int8_t      padX, padY, padRepeatCount;
bool        isInvalid;

/*  Local Variables  */

PROGMEM static const uint8_t imgButtons[][7] = { // 7x7 x3
    { 0x3E, 0x47, 0x6B, 0x6D, 0x6D, 0x41, 0x3E },
    { 0x3E, 0x41, 0x55, 0x55, 0x51, 0x65, 0x3E },
    { 0x14, 0x36, 0x77, 0x36, 0x14, 0x00, 0x00 },
    { 0x04, 0x0E, 0x1F, 0x00, 0x1F, 0x0E, 0x04 },
};

/*---------------------------------------------------------------------------*/
/*                             Common Functions                              */
/*---------------------------------------------------------------------------*/

void handleDPad(void)
{
    padX = padY = 0;
    if (arduboy.buttonPressed(LEFT_BUTTON | RIGHT_BUTTON | UP_BUTTON | DOWN_BUTTON)) {
        if (++padRepeatCount >= (PAD_REPEAT_DELAY + PAD_REPEAT_INTERVAL)) {
            padRepeatCount = PAD_REPEAT_DELAY;
        }
        if (padRepeatCount == 1 || padRepeatCount == PAD_REPEAT_DELAY) {
            if (arduboy.buttonPressed(LEFT_BUTTON))  padX--;
            if (arduboy.buttonPressed(RIGHT_BUTTON)) padX++;
            if (arduboy.buttonPressed(UP_BUTTON))    padY--;
            if (arduboy.buttonPressed(DOWN_BUTTON))  padY++;
        }
    } else {
        padRepeatCount = 0;
    }
}

void drawButtonIcon(int16_t x, int16_t y, ICON_T icon)
{
    arduboy.drawBitmap(x, y, imgButtons[icon],
            IMG_BUTTONS_W, IMG_BUTTONS_H, arduboy.getTextColor());
}
