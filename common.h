#pragma once

#include "MyArduboy2.h"

/*  Defines  */

#define FPS     25

enum MODE_T : uint8_t {
    MODE_SENDER = 0,
    MODE_RECEIVER,
};

enum ICON_T : uint8_t {
    ICON_BUTTON_A = 0,
    ICON_BUTTON_B,
    ICON_BUTTON_UD,
    ICON_BUTTON_LR,
};

enum IR_TYPE_T : uint8_t {
    IR_TYPE_UNKNOWN = 0,
    IR_TYPE_IRISOHYAMA_103,
    IR_TYPE_NEC_6D82,
    IR_TYPE_KASEIKYO_S_522C,
    IR_TYPE_SONY_1,
    IR_TYPE_SONY_1A,
    IR_TYPE_SONY_77,
    IR_TYPE_SONY_97,
    IR_TYPE_SONY_A4,
    IR_TYPE_SONY_1C5A,
    IR_TYPE_SONY_1E5A,
    IR_TYPE_PLUSMINUS0_76,
};

/*  Typedefs  */

typedef struct {
    const __FlashStringHelper *protocol;
    uint16_t    vendor;
    uint16_t    address;
    uint32_t    command;
    uint8_t     nbits;
    uint8_t     flags;
    uint64_t    data;
    uint16_t    *pBuffer;
} IR_INFO_T;

/*  Global Functions (Common)  */

void    handleDPad(void);
void    drawButtonIcon(int16_t x, int16_t y, ICON_T icon);

/*  Global Functions (IR control)  */

void    beginIRSender(void);
void    sendIR(IR_TYPE_T type, uint32_t data);

void    beginIRReceiver(void);
bool    receiveIR(IR_INFO_T &info);
void    endIRReceiver(void);

/*  Global Functions (Each Mode)  */

void    initSender(void);
MODE_T  updateSender(void);
void    drawSender(void);

void    initReceiver(void);
MODE_T  updateReceiver(void);
void    drawReceiver(void);

/*  Global Variables  */

extern MyArduboy2   arduboy;

extern uint8_t  counter;
extern int8_t   padX, padY, padRepeatCount;
extern bool     isInvalid;
