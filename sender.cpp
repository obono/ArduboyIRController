#include "common.h"
#include "table.h"

/*  Defines  */

#define     DISPLAY_ITEMS   9

/*  Local Functions  */

static void setTarget(uint8_t idx);

/*  Local Functions (macros)  */

#define circulate(n, v, m)  (((n) + (v) + (m)) % (m))

/*  Local Variables  */

SIGNAL_T    *pTable;
uint8_t     targetIdx, signalIdx, tablePos, tableSize;
bool        isFirst = true;

/*---------------------------------------------------------------------------*/
/*                              Main Functions                               */
/*---------------------------------------------------------------------------*/

void initSender(void)
{
    beginIRSender();
    if (isFirst) {
        setTarget(0);
        isFirst = false;
    }
    isInvalid = true;
}

MODE_T updateSender(void)
{
    MODE_T ret = MODE_SENDER;
    handleDPad();
    if (padX != 0) {
        setTarget(circulate(targetIdx, padX, TARGET_TABLE_SIZE));
        isInvalid = true;
    }
    if (padY != 0) {
        signalIdx = circulate(signalIdx, padY, tableSize);
        if (tablePos + 2 > signalIdx) {
            tablePos = (signalIdx > 2) ? signalIdx - 2 : 0;
        }
        if (tableSize > DISPLAY_ITEMS && tablePos + DISPLAY_ITEMS - 3 < signalIdx) {
            tablePos = ((signalIdx < tableSize - 3) ? signalIdx + 3 : tableSize) - DISPLAY_ITEMS;
        }
        isInvalid = true;
    }
    if (arduboy.buttonDown(B_BUTTON)) {
        IR_TYPE_T type = (IR_TYPE_T)pgm_read_byte(&pTable[signalIdx].type);
        if (type != IR_TYPE_UNKNOWN) {
            uint32_t data = pgm_read_dword(&pTable[signalIdx].data);
            sendIR(type, data);
        }
    }
    if (arduboy.buttonDown(A_BUTTON)) {
        ret = MODE_RECEIVER;
    }
    return ret;
}

void drawSender(void)
{
    drawButtonIcon(0, 0, ICON_BUTTON_A);
    arduboy.printEx(8, 0, F("[SENDER]  "));
    arduboy.print((const __FlashStringHelper *)&targetTable[targetIdx].label);
    drawButtonIcon(60, 0, ICON_BUTTON_LR);
    arduboy.drawLine(8, 6, WIDTH - 8, 6);

    int iMax = tableSize - tablePos;
    if (iMax > DISPLAY_ITEMS) {
        iMax = DISPLAY_ITEMS;
    }
    for (int i = 0; i < iMax; i++) {
        uint8_t idx = tablePos + i;
        int16_t x = 10;
        int16_t y = 10 + i * 6;
        if (idx == signalIdx) {
            x -= 4;
            drawButtonIcon(x - 6, y - 1, ICON_BUTTON_UD);
        }
        arduboy.printEx(x, y, (const __FlashStringHelper *)&pTable[idx].label);
    }
    drawButtonIcon(96, 57, ICON_BUTTON_B);
    arduboy.printEx(104, 58, F("SEND"));
}

/*---------------------------------------------------------------------------*/

static void setTarget(uint8_t idx)
{
    targetIdx = idx;
    pTable = (SIGNAL_T *)pgm_read_ptr(&targetTable[idx].pTable);
    signalIdx = 0;
    tablePos = 0;
    tableSize = pgm_read_byte(&targetTable[idx].tableSize);
}
