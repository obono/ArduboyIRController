#include "common.h"

/*  Defines  */

#define SIGNAL_BUFFER_LEN           100
#define SIGNAL_MICROS_PER_TICK      50
#define SIGNAL_TICKS_PER_PIXEL      20

#define IR_INFO_FLAGS_IS_REPEAT     0x01
#define IR_INFO_FLAGS_PARITY_FAILED 0x04
#define IR_INFO_FLAGS_WAS_OVERFLOW  0x40
#define IR_INFO_FLAGS_IS_MSB_FIRST  0x80

/*  Typedefs  */

/*  Local Functions  */

static void printHex(uint64_t value);
static char getHexChar(uint8_t c);

/*  Local Functions (macros)  */

/*  Local Variables  */

IR_INFO_T   lastIRInfo;
uint16_t    signalBuffer[SIGNAL_BUFFER_LEN];
uint8_t     signalPos, repeatTimes;
bool        isUnknown;

/*---------------------------------------------------------------------------*/
/*                              Main Functions                               */
/*---------------------------------------------------------------------------*/

void initReceiver(void)
{
    beginIRReceiver();
    lastIRInfo.protocol = NULL;
    isInvalid = true;
}

MODE_T updateReceiver(void)
{
    MODE_T ret = MODE_RECEIVER;
    IR_INFO_T info;
    info.pBuffer = signalBuffer;
    if (receiveIR(info)) {
        bool isRepeat = info.flags & IR_INFO_FLAGS_IS_REPEAT;
        if (isRepeat && info.protocol == lastIRInfo.protocol && info.vendor == lastIRInfo.vendor &&
                info.address == lastIRInfo.address && info.command == lastIRInfo.command) {
            repeatTimes++;
        } else {
            repeatTimes = 0;
        }
        lastIRInfo = info;
        signalPos = 1;
        isUnknown = (info.command == 0 && info.nbits == 0 && !isRepeat);
        isInvalid = true;
    }
    if (isUnknown) {
        handleDPad();
        if (padX < 0 && signalPos > 1 || padX > 0 && signalBuffer[signalPos * 2] > 0) {
            signalPos += padX;
            isInvalid = true;
        }
    }
    if (arduboy.buttonDown(A_BUTTON)) {
        endIRReceiver();
        ret = MODE_SENDER;
    }
    return ret;
}

void drawReceiver(void)
{
    drawButtonIcon(0, 0, ICON_BUTTON_A);
    arduboy.printEx(8, 0, F("[RECEIVER]"));
    arduboy.drawLine(8, 6, WIDTH - 8, 6);

    int16_t y = 10;
    if (lastIRInfo.protocol) {
        /*  Protocol & Vendor  */
        arduboy.printEx(0, y, F("PROTOCOL:"));
        arduboy.print(lastIRInfo.protocol);
        if (lastIRInfo.vendor > 0) {
            y += 6;
            arduboy.printEx(0, y, F("VENDOR ID:"));
            printHex(lastIRInfo.vendor);
        }
        y += 8;

        if (isUnknown) {
            /*  Signal Detail  */
            drawButtonIcon(0, y, ICON_BUTTON_LR);
            arduboy.printEx(8, y, '[');
            arduboy.print(signalPos);
            arduboy.print(F("]:+"));
            arduboy.print(signalBuffer[signalPos * 2 - 1] * SIGNAL_MICROS_PER_TICK);
            uint16_t ticks = signalBuffer[signalPos * 2];
            if (ticks == 0) {
                arduboy.print(F(" (STOP)"));
            } else {
                arduboy.print(F("/-"));
                arduboy.print(ticks * SIGNAL_MICROS_PER_TICK);
            }
        } else {
            /*  Address & Command  */
            if (lastIRInfo.address > 0) {
                arduboy.printEx(0, y, F("ADDRESS:"));
                printHex(lastIRInfo.address);
                y += 6;
            }
            arduboy.printEx(0, y, F("COMMAND:"));
            printHex(lastIRInfo.command);
            if (lastIRInfo.flags & IR_INFO_FLAGS_PARITY_FAILED) {
                arduboy.print('?');
            }
            y += 8;

            /*  Raw Data  */
            arduboy.printEx(0, y, F("# OF BITS:"));
            arduboy.print(lastIRInfo.nbits);
            if (lastIRInfo.nbits > 0) {
                arduboy.print('(');
                arduboy.print((lastIRInfo.flags & IR_INFO_FLAGS_IS_MSB_FIRST) ? 'M' : 'L');
                arduboy.print(F("SB)"));
                y += 6;
                arduboy.printEx(0, y, F("RAW:"));
                printHex(lastIRInfo.data);
            }
            if (repeatTimes > 0) {
                y += 6;
                arduboy.printEx(0, y, F("REPEAT:"));
                arduboy.print(repeatTimes);
                arduboy.print(F(" (-"));
                arduboy.print((uint32_t)signalBuffer[0] * SIGNAL_MICROS_PER_TICK);
                arduboy.print(')');
            }
            y += 8;
        }

        /*  Graphical Signals  */
        int16_t x = 0;
        for (int i = 1; i < SIGNAL_BUFFER_LEN && signalBuffer[i]; i++) {
            uint8_t w = (signalBuffer[i] + SIGNAL_TICKS_PER_PIXEL - 1) / SIGNAL_TICKS_PER_PIXEL;
            uint8_t h = (i & 1) * 5 + 1;
            arduboy.fillRect(x, HEIGHT - h, w, h, WHITE);
            if (isUnknown && (i + 1) / 2 == signalPos) {
                arduboy.drawFastHLine(x, HEIGHT - 8, w, WHITE);
            }
            x += w;
        }
    } else {
        arduboy.printEx(0, y, F("WAITING FOR SIGNAL..."));
    }
}

/*---------------------------------------------------------------------------*/

static void printHex(uint64_t value)
{
    int i = 0;
    char buf[16];
    while (i == 0 || value > 0) {
        buf[i++] = getHexChar(value & 0xF);
        value >>= 4;
    }
    arduboy.print(F("0X"));
    while (i > 0) {
        arduboy.print(buf[--i]);
    }
}

static char getHexChar(uint8_t a)
{
    return (a < 10) ? '0' + a : 'A' + a - 10;
}
