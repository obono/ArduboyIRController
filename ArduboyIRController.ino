#include "common.h"

/*  Defines  */

#define IDLE_TIMEOUT        (FPS * 5)

#define callInitFunc(idx)   ((void (*)(void)) pgm_read_ptr((void *) &moduleTable[idx].initFunc))()
#define callUpdateFunc(idx) ((MODE_T (*)(void)) pgm_read_ptr((void *) &moduleTable[idx].updateFunc))()
#define callDrawFunc(idx)   ((void (*)(void)) pgm_read_ptr((void *) &moduleTable[idx].drawFunc))()

/*  Typedefs  */

typedef struct
{
    void(*initFunc)(void);
    MODE_T(*updateFunc)(void);
    void(*drawFunc)(void);
} MODULE_FUNCS;

/*  Local Variables  */

PROGMEM static const MODULE_FUNCS moduleTable[] = {
    { initSender,   updateSender,   drawSender   },
    { initReceiver, updateReceiver, drawReceiver },
};

static MODE_T   mode;
static uint8_t  idleCount;

/*---------------------------------------------------------------------------*/

void setup()
{
    arduboy.beginNoLogo();
    arduboy.setFrameRate(FPS);
    arduboy.setRGBled(0, 0, 0);
    idleCount = 0;
    mode = MODE_SENDER;
    callInitFunc(mode);
}

void loop()
{
    if (!(arduboy.nextFrame())) return;
    MODE_T nextMode = callUpdateFunc(mode);
    bool isActed = (arduboy.buttonsState() != 0 || isInvalid);
    if (isInvalid) {
        arduboy.clear();
        callDrawFunc(mode);
        arduboy.display();
        isInvalid = false;
    }
    if (isActed) {
        if (idleCount == IDLE_TIMEOUT) {
            arduboy.sendLCDCommand(0x81);
            arduboy.sendLCDCommand(0xCF);
        }
        idleCount = 0;
    } else {
        if (idleCount < IDLE_TIMEOUT && ++idleCount == IDLE_TIMEOUT) {
            arduboy.sendLCDCommand(0x81);
            arduboy.sendLCDCommand(0x00);
        }
    }
    if (mode != nextMode) {
        mode = nextMode;
        callInitFunc(mode);
    }
}
