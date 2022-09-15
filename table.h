#include "common.h"

/*  Typedefs  */

typedef struct {
    char        label[11];
    IR_TYPE_T   type;
    uint32_t    data;
} SIGNAL_T;

typedef struct {
    char            label[9];
    const SIGNAL_T  *pTable;
    uint8_t         tableSize;
} TARGET_T;

/*  Tables  */

PROGMEM static const SIGNAL_T signalTableCLRL2Ch1[] = {
    { "POWER/LAMP", IR_TYPE_IRISOHYAMA_103, 0xA00028    },
    { "LIGHTEN",    IR_TYPE_IRISOHYAMA_103, 0x800408    },
    { "DARKEN",     IR_TYPE_IRISOHYAMA_103, 0x800808    },
    { "WHITE COL",  IR_TYPE_IRISOHYAMA_103, 0x800208    },
    { "YELLOW COL", IR_TYPE_IRISOHYAMA_103, 0x800108    },
    { "OFF TIMER",  IR_TYPE_IRISOHYAMA_103, 0x000088    },
};

PROGMEM static const SIGNAL_T signalTableCLRL2Ch2[] = {
    { "POWER/LAMP", IR_TYPE_IRISOHYAMA_103, 0xA00020    },
    { "LIGHTEN",    IR_TYPE_IRISOHYAMA_103, 0x800400    },
    { "DARKEN",     IR_TYPE_IRISOHYAMA_103, 0x800800    },
    { "WHITE COL",  IR_TYPE_IRISOHYAMA_103, 0x800200    },
    { "YELLOW COL", IR_TYPE_IRISOHYAMA_103, 0x800100    },
    { "OFF TIMER",  IR_TYPE_IRISOHYAMA_103, 0x000080    },
};

PROGMEM static const SIGNAL_T signalTableRE2028Ch1[] = {
    { "POWER",      IR_TYPE_NEC_6D82,   0xAB    },
    { "MAX LIGHT",  IR_TYPE_NEC_6D82,   0xA6    },
    { "LIGHTEN",    IR_TYPE_NEC_6D82,   0xBA    },
    { "DARKEN",     IR_TYPE_NEC_6D82,   0xBB    },
    { "NIGHT LAMP", IR_TYPE_NEC_6D82,   0xBC    },
    { "OFF TIMER",  IR_TYPE_NEC_6D82,   0xAF    },
    { "RUSUBAN",    IR_TYPE_NEC_6D82,   0x1971  },
};

PROGMEM static const SIGNAL_T signalTableRE2028Ch2[] = {
    { "POWER",      IR_TYPE_NEC_6D82,   0x2B    },
    { "MAX LIGHT",  IR_TYPE_NEC_6D82,   0x26    },
    { "LIGHTEN",    IR_TYPE_NEC_6D82,   0x3A    },
    { "DARKEN",     IR_TYPE_NEC_6D82,   0x3B    },
    { "NIGHT LAMP", IR_TYPE_NEC_6D82,   0x3C    },
    { "OFF TIMER",  IR_TYPE_NEC_6D82,   0x2F    },
    { "RUSUBAN",    IR_TYPE_NEC_6D82,   0x1972  },
};

PROGMEM static const SIGNAL_T signalTableHK9327KCh1[] = {
    { "MAX LIGHT",  IR_TYPE_KASEIKYO_S_522C,    0x2C0   },
    { "PREFERENCE", IR_TYPE_KASEIKYO_S_522C,    0x2D0   },
    { "LIGHTEN",    IR_TYPE_KASEIKYO_S_522C,    0x2A0   },
    { "DARKEN",     IR_TYPE_KASEIKYO_S_522C,    0x2B0   },
    { "NIGHT LAMP", IR_TYPE_KASEIKYO_S_522C,    0x2E0   },
    { "OFF",        IR_TYPE_KASEIKYO_S_522C,    0x2F0   },
};

PROGMEM static const SIGNAL_T signalTableHK9327KCh2[] = {
    { "MAX LIGHT",  IR_TYPE_KASEIKYO_S_522C,    0x340   },
    { "PREFERENCE", IR_TYPE_KASEIKYO_S_522C,    0x350   },
    { "LIGHTEN",    IR_TYPE_KASEIKYO_S_522C,    0x320   },
    { "DARKEN",     IR_TYPE_KASEIKYO_S_522C,    0x330   },
    { "NIGHT LAMP", IR_TYPE_KASEIKYO_S_522C,    0x360   },
    { "OFF",        IR_TYPE_KASEIKYO_S_522C,    0x370   },
};

PROGMEM static const SIGNAL_T signalTableHK9327KCh3[] = {
    { "MAX LIGHT",  IR_TYPE_KASEIKYO_S_522C,    0x3C0   },
    { "PREFERENCE", IR_TYPE_KASEIKYO_S_522C,    0x3D0   },
    { "LIGHTEN",    IR_TYPE_KASEIKYO_S_522C,    0x3A0   },
    { "DARKEN",     IR_TYPE_KASEIKYO_S_522C,    0x3B0   },
    { "NIGHT LAMP", IR_TYPE_KASEIKYO_S_522C,    0x3E0   },
    { "OFF",        IR_TYPE_KASEIKYO_S_522C,    0x3F0   },
};

PROGMEM static const SIGNAL_T signalTableRMJD022[] = {
    { "POWER",      IR_TYPE_SONY_1,     0x15    },
    { "SOURCE",     IR_TYPE_SONY_1,     0x25    },
    { "HOME",       IR_TYPE_SONY_1,     0x60    },
    { "UP",         IR_TYPE_SONY_1,     0x74    },
    { "DOWN",       IR_TYPE_SONY_1,     0x75    },
    { "LEFT",       IR_TYPE_SONY_1,     0x34    },
    { "RIGHT",      IR_TYPE_SONY_1,     0x33    },
    { "ENTER",      IR_TYPE_SONY_1,     0x65    },
    { "BACK",       IR_TYPE_SONY_97,    0x23    },
    { "OPTION",     IR_TYPE_SONY_97,    0x36    },
    { "DISPLAY",    IR_TYPE_SONY_1,     0x3A    },
    { "INFO",       IR_TYPE_SONY_97,    0x16    },
    { "RECORD",     IR_TYPE_SONY_97,    0x20    },
    { "PROGRAM",    IR_TYPE_SONY_A4,    0x5B    },
    { "RAKU2START", IR_TYPE_SONY_77,    0x6A    },
    { "MY PROGRAM", IR_TYPE_SONY_77,    0x6B    },
    { "LINK MENU",  IR_TYPE_SONY_1A,    0x58    },
    { "SCENE SEL",  IR_TYPE_SONY_1A,    0x78    },
    { "THEATER",    IR_TYPE_SONY_77,    0x60    },
    { "WIDESCREEN", IR_TYPE_SONY_A4,    0x3D    },
    { "3D",         IR_TYPE_SONY_77,    0x4D    },
    { "---------",  IR_TYPE_UNKNOWN,    0x00    },
    { "BROADCAST",  IR_TYPE_SONY_97,    0x32    },
    { "BS",         IR_TYPE_SONY_97,    0x2C    },
    { "CS",         IR_TYPE_SONY_97,    0x2B    },
    { "#1",         IR_TYPE_SONY_1,     0x00    },
    { "#2",         IR_TYPE_SONY_1,     0x01    },
    { "#3",         IR_TYPE_SONY_1,     0x02    },
    { "#4",         IR_TYPE_SONY_1,     0x03    },
    { "#5",         IR_TYPE_SONY_1,     0x04    },
    { "#6",         IR_TYPE_SONY_1,     0x05    },
    { "#7",         IR_TYPE_SONY_1,     0x06    },
    { "#8",         IR_TYPE_SONY_1,     0x07    },
    { "#9",         IR_TYPE_SONY_1,     0x08    },
    { "#10/#0",     IR_TYPE_SONY_1,     0x09    },
    { "#11",        IR_TYPE_SONY_1,     0x0A    },
    { "#12",        IR_TYPE_SONY_1,     0x0B    },
    { "10KEY",      IR_TYPE_SONY_97,    0x0C    },
    { "---------",  IR_TYPE_UNKNOWN,    0x00    },
    { "PLAY",       IR_TYPE_SONY_97,    0x1A    },
    { "PAUSE",      IR_TYPE_SONY_97,    0x19    },
    { "STOP",       IR_TYPE_SONY_97,    0x18    },
    { "PREVIOUS",   IR_TYPE_SONY_97,    0x3C    },
    { "NEXT",       IR_TYPE_SONY_97,    0x3D    },
    { "F.REWIND",   IR_TYPE_SONY_97,    0x1B    },
    { "F.FORWARD",  IR_TYPE_SONY_97,    0x1C    },
    { "10S RWD",    IR_TYPE_SONY_97,    0x79    },
    { "15S FWD",    IR_TYPE_SONY_97,    0x78    },
    { "SET CHAP",   IR_TYPE_SONY_77,    0x6C    },
    { "DELETE",     IR_TYPE_SONY_77,    0x1F    },
    { "POPUP MENU", IR_TYPE_SONY_1A,    0x61    },
    { "CC SEL",     IR_TYPE_SONY_97,    0x28    },
    { "VOICE SEL",  IR_TYPE_SONY_1,     0x17    },
    { "EJECT",      IR_TYPE_SONY_97,    0x48    },
    { "---------",  IR_TYPE_UNKNOWN,    0x00    },
    { "BLUE",       IR_TYPE_SONY_97,    0x24    },
    { "RED",        IR_TYPE_SONY_97,    0x25    },
    { "GREEN",      IR_TYPE_SONY_97,    0x26    },
    { "YELLOW",     IR_TYPE_SONY_97,    0x27    },
    { "VOLUME+",    IR_TYPE_SONY_1,     0x12    },
    { "VOLUME-",    IR_TYPE_SONY_1,     0x13    },
    { "MUTE",       IR_TYPE_SONY_1,     0x14    },
    { "CHANNEL+",   IR_TYPE_SONY_1,     0x10    },
    { "CHANNEL-",   IR_TYPE_SONY_1,     0x11    },
    { "D BUTTON",   IR_TYPE_SONY_97,    0x15    },
};

PROGMEM static const SIGNAL_T signalTableRMTVR502J[] = {
    { "POWER",      IR_TYPE_SONY_1E5A,  0x15    },
    { "POWER(TV)",  IR_TYPE_SONY_1,     0x15    },
    { "SOURCE(TV)", IR_TYPE_SONY_1,     0x25    },
    { "OPEN/CLOSE", IR_TYPE_SONY_1E5A,  0x16    },
    { "HOME",       IR_TYPE_SONY_1E5A,  0x42    },
    { "UP",         IR_TYPE_SONY_1E5A,  0x39    },
    { "DOWN",       IR_TYPE_SONY_1E5A,  0x3A    },
    { "LEFT",       IR_TYPE_SONY_1E5A,  0x3B    },
    { "RIGHT",      IR_TYPE_SONY_1E5A,  0x3C    },
    { "ENTER",      IR_TYPE_SONY_1E5A,  0x3D    },
    { "BACK",       IR_TYPE_SONY_1E5A,  0x43    },
    { "OPTION",     IR_TYPE_SONY_1E5A,  0x3F    },
    { "DISPLAY",    IR_TYPE_SONY_1E5A,  0x41    },
    { "INFO",       IR_TYPE_SONY_1E5A,  0x44    },
    { "PROGRAM",    IR_TYPE_SONY_1E5A,  0x2A    },
    { "RECORD",     IR_TYPE_SONY_1E5A,  0x1D    },
    { "VIDEO LIST", IR_TYPE_SONY_1E5A,  0x47    },
    { "---------",  IR_TYPE_UNKNOWN,    0x00    },
    { "BROADCAST",  IR_TYPE_SONY_1E5A,  0x6B    },
    { "BS",         IR_TYPE_SONY_1E5A,  0x6A    },
    { "CS",         IR_TYPE_SONY_1E5A,  0x6E    },
    { "#1",         IR_TYPE_SONY_1E5A,  0x00    },
    { "#2",         IR_TYPE_SONY_1E5A,  0x01    },
    { "#3",         IR_TYPE_SONY_1E5A,  0x02    },
    { "#4",         IR_TYPE_SONY_1E5A,  0x03    },
    { "#5",         IR_TYPE_SONY_1E5A,  0x04    },
    { "#6",         IR_TYPE_SONY_1E5A,  0x05    },
    { "#7",         IR_TYPE_SONY_1E5A,  0x06    },
    { "#8",         IR_TYPE_SONY_1E5A,  0x07    },
    { "#9",         IR_TYPE_SONY_1E5A,  0x08    },
    { "#10/#0",     IR_TYPE_SONY_1E5A,  0x09    },
    { "#11",        IR_TYPE_SONY_1E5A,  0x0A    },
    { "#12",        IR_TYPE_SONY_1E5A,  0x0B    },
    { "10KEY",      IR_TYPE_SONY_1E5A,  0x61    },
    { "---------",  IR_TYPE_UNKNOWN,    0x00    },
    { "PLAY",       IR_TYPE_SONY_1E5A,  0x1A    },
    { "PAUSE",      IR_TYPE_SONY_1E5A,  0x19    },
    { "STOP",       IR_TYPE_SONY_1E5A,  0x18    },
    { "PREVIOUS",   IR_TYPE_SONY_1E5A,  0x57    },
    { "NEXT",       IR_TYPE_SONY_1E5A,  0x56    },
    { "F.REWIND",   IR_TYPE_SONY_1E5A,  0x1B    },
    { "F.FORWARD",  IR_TYPE_SONY_1E5A,  0x1C    },
    { "10S RWD",    IR_TYPE_SONY_1E5A,  0x76    },
    { "15S FWD",    IR_TYPE_SONY_1E5A,  0x75    },
    { "FAST PLAY",  IR_TYPE_SONY_1E5A,  0x22    },
    { "SET CHAP",   IR_TYPE_SONY_1E5A,  0x48    },
    { "DELETE",     IR_TYPE_SONY_1E5A,  0x0F    },
    { "POPUP MENU", IR_TYPE_SONY_1E5A,  0x29    },
    { "CC SEL",     IR_TYPE_SONY_1E5A,  0x63    },
    { "VOICE SEL",  IR_TYPE_SONY_1E5A,  0x64    },
    { "---------",  IR_TYPE_UNKNOWN,    0x00    },
    { "BLUE",       IR_TYPE_SONY_1E5A,  0x66    },
    { "RED",        IR_TYPE_SONY_1E5A,  0x67    },
    { "GREEN",      IR_TYPE_SONY_1E5A,  0x68    },
    { "YELLOW",     IR_TYPE_SONY_1E5A,  0x69    },
    { "VOLUME+",    IR_TYPE_SONY_1,     0x12    },
    { "VOLUME-",    IR_TYPE_SONY_1,     0x13    },
    { "MUTE",       IR_TYPE_SONY_1,     0x14    },
    { "CHANNEL+",   IR_TYPE_SONY_1E5A,  0x10    },
    { "CHANNEL-",   IR_TYPE_SONY_1E5A,  0x11    },
    { "D BUTTON",   IR_TYPE_SONY_1E5A,  0x62    },
};

PROGMEM static const SIGNAL_T signalTableRMTVB101J[] = {
    { "POWER",      IR_TYPE_SONY_1C5A,  0x15    },
    { "POWER(TV)",  IR_TYPE_SONY_1,     0x15    },
    { "SOURCE(TV)", IR_TYPE_SONY_1,     0x25    },
    { "OPEN/CLOSE", IR_TYPE_SONY_1C5A,  0x16    },
    { "HOME",       IR_TYPE_SONY_1C5A,  0x42    },
    { "UP",         IR_TYPE_SONY_1C5A,  0x39    },
    { "DOWN",       IR_TYPE_SONY_1C5A,  0x3A    },
    { "LEFT",       IR_TYPE_SONY_1C5A,  0x3B    },
    { "RIGHT",      IR_TYPE_SONY_1C5A,  0x3C    },
    { "ENTER",      IR_TYPE_SONY_1C5A,  0x3D    },
    { "BACK",       IR_TYPE_SONY_1C5A,  0x43    },
    { "OPTION",     IR_TYPE_SONY_1C5A,  0x3F    },
    { "DISPLAY",    IR_TYPE_SONY_1C5A,  0x41    },
    { "FAVORITE",   IR_TYPE_SONY_1C5A,  0x5E    },
    { "NETFLIX",    IR_TYPE_SONY_1C5A,  0x4B    },
    { "---------",  IR_TYPE_UNKNOWN,    0x00    },
    { "PLAY",       IR_TYPE_SONY_1C5A,  0x1A    },
    { "PAUSE",      IR_TYPE_SONY_1C5A,  0x19    },
    { "STOP",       IR_TYPE_SONY_1C5A,  0x18    },
    { "PREVIOUS",   IR_TYPE_SONY_1C5A,  0x57    },
    { "NEXT",       IR_TYPE_SONY_1C5A,  0x56    },
    { "F.REWIND",   IR_TYPE_SONY_1C5A,  0x1B    },
    { "F.FORWARD",  IR_TYPE_SONY_1C5A,  0x1C    },
    { "TOP MENU",   IR_TYPE_SONY_1C5A,  0x2C    },
    { "POPUP MENU", IR_TYPE_SONY_1C5A,  0x29    },
    { "CC SEL",     IR_TYPE_SONY_1C5A,  0x63    },
    { "VOICE SEL",  IR_TYPE_SONY_1C5A,  0x64    },
    { "---------",  IR_TYPE_UNKNOWN,    0x00    },
    { "BLUE",       IR_TYPE_SONY_1C5A,  0x66    },
    { "RED",        IR_TYPE_SONY_1C5A,  0x67    },
    { "GREEN",      IR_TYPE_SONY_1C5A,  0x68    },
    { "YELLOW",     IR_TYPE_SONY_1C5A,  0x69    },
    { "VOLUME+",    IR_TYPE_SONY_1,     0x12    },
    { "VOLUME-",    IR_TYPE_SONY_1,     0x13    },
    { "MUTE",       IR_TYPE_SONY_1,     0x14    },
};

PROGMEM static const SIGNAL_T signalTableXQSX610[] = {
    { "POWER",      IR_TYPE_PLUSMINUS0_76,  0x80    },
    { "FLOW+",      IR_TYPE_PLUSMINUS0_76,  0x40    },
    { "FLOW-",      IR_TYPE_PLUSMINUS0_76,  0x62    },
    { "FLUCTUATE",  IR_TYPE_PLUSMINUS0_76,  0x10    },
    { "SWING",      IR_TYPE_PLUSMINUS0_76,  0xC2    },
    { "AMPLITUDE",  IR_TYPE_PLUSMINUS0_76,  0x20    },
    { "ON TIMER",   IR_TYPE_PLUSMINUS0_76,  0x08    },
    { "OFF TIMER",  IR_TYPE_PLUSMINUS0_76,  0x04    },
};

#define tableSize(p)    (sizeof(p) / sizeof(SIGNAL_T))

PROGMEM static const TARGET_T targetTable[] = {
    { "ROOM 1",     signalTableCLRL2Ch1,    tableSize(signalTableCLRL2Ch1)      },
    { "ROOM 2",     signalTableRE2028Ch1,   tableSize(signalTableRE2028Ch1)     },
    { "ROOM 3",     signalTableHK9327KCh3,  tableSize(signalTableHK9327KCh3)    },
    { "LIVING",     signalTableHK9327KCh2,  tableSize(signalTableHK9327KCh2)    },
    { "BED ROOM",   signalTableHK9327KCh1,  tableSize(signalTableHK9327KCh1)    },
    { "TV",         signalTableRMJD022,     tableSize(signalTableRMJD022)       },
    { "BD(REC)",    signalTableRMTVR502J,   tableSize(signalTableRMTVR502J)     },
    { "BD(PLAY)",   signalTableRMTVB101J,   tableSize(signalTableRMTVB101J)     },
    { "FUN(+-0)",   signalTableXQSX610,     tableSize(signalTableXQSX610)       },
};

#define TARGET_TABLE_SIZE   (sizeof(targetTable) / sizeof(TARGET_T))
