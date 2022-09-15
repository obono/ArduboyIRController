#define DECODE_KASEIKYO
#define DECODE_NEC
#define DECODE_SONY
#define FEEDBACK_LED_IS_ACTIVE_LOW
#include <IRremote.hpp>
#include "common.h"

/*  Defines  */

#define PIN_RECEIVE_IR          2
#define PIN_SEND_IR             3
#define PIN_FEEDBACK_LED        9

#define NEC_REPEAT_TIMES        0

#define KASEIKYO_DATA_BITS      12
#define KASEIKYO_SHORT_BITS     (KASEIKYO_VENDOR_ID_BITS + KASEIKYO_VENDOR_ID_PARITY_BITS + \
                                 KASEIKYO_DATA_BITS + KASEIKYO_PARITY_BITS)
#define KASEIKYO_REPEAT_TIMES   0

#define SONY_REPEAT_TIMES       2

#define IRISOHYAMA_KHZ          38
#define IRISOHYAMA_HEADER1_MARK 2200
#define IRISOHYAMA_HEADER2_MARK 5500
#define IRISOHYAMA_HEADER_SPACE 900
#define IRISOHYAMA_ONE_MARK     1650
#define IRISOHYAMA_ZERO_MARK    550
#define IRISOHYAMA_BIT_SPACE    450
#define IRISOHYAMA_ADDRESS_BITS 12
#define IRISOHYAMA_DATA_BITS    24
#define IRISOHYAMA_PARITY_BITS  4
#define IRISOHYAMA_BITS         (IRISOHYAMA_ADDRESS_BITS + IRISOHYAMA_DATA_BITS + \
                                 IRISOHYAMA_PARITY_BITS)

#define PLUSMINUS0_KHZ          37
#define PLUSMINUS0_HEADER_MARK  2540
#define PLUSMINUS0_HEADER_SPACE 3292
#define PLUSMINUS0_ONE_MARK     1268
#define PLUSMINUS0_ONE_SPACE    408
#define PLUSMINUS0_ZERO_MARK    432
#define PLUSMINUS0_ZERO_SPACE   1232
#define PLUSMINUS0_REPEAT_PERIOD 40000
#define PLUSMINUS0_BITS         16
#define PLUSMINUS0_REPEAT_TIMES 2

/*  Local Functions  */

static void sendIrisOhyama(uint16_t address, uint32_t command);
static void sendKaseikyoShort(uint16_t data, uint_fast8_t repeat, uint16_t vendor);
static void sendPlusMinus0(uint8_t address, uint8_t command, uint_fast8_t repeat);

static bool decodeKaseikyoShort(IR_INFO_T &info);
static bool decodeIrisOhyama(IR_INFO_T &info);
static bool decodePlusMinus0(IR_INFO_T &info);
static bool decodePulseDistanceWidthData(uint8_t aNumberOfBits, uint8_t aStartOffset,
        uint16_t aOneMarkMicros, uint16_t aOneSpaceMicros, uint16_t aZeroMarkMicros,
        uint16_t aZeroSpaceMicros, bool aMSBfirst);

static void convertIRData(IR_INFO_T &info, const __FlashStringHelper *protocol);
static void convertKaseikyoIRData(IR_INFO_T &info, uint16_t vendor);

/*---------------------------------------------------------------------------*/
/*                            IR Sender Functions                            */
/*---------------------------------------------------------------------------*/

void beginIRSender(void)
{
    IrSender.begin(PIN_SEND_IR, DISABLE_LED_FEEDBACK);
}

void sendIR(IR_TYPE_T type, uint32_t data)
{
    arduboy.setRGBled(GREEN_LED, 31);
    switch (type) {
        case IR_TYPE_IRISOHYAMA_103:
            sendIrisOhyama(0x103, data);
            break;
        case IR_TYPE_NEC_6D82:
            IrSender.sendNEC(0x6D82, data, NEC_REPEAT_TIMES);
            break;
        case IR_TYPE_KASEIKYO_S_522C:
            sendKaseikyoShort(data, KASEIKYO_REPEAT_TIMES, 0x522C);
            break;
        case IR_TYPE_SONY_1:
            IrSender.sendSony(0x1, data, SONY_REPEAT_TIMES, SONY_BITS_MIN);
            break;
        case IR_TYPE_SONY_1A:
            IrSender.sendSony(0x1A, data, SONY_REPEAT_TIMES, SONY_BITS_15);
            break;
        case IR_TYPE_SONY_77:
            IrSender.sendSony(0x77, data, SONY_REPEAT_TIMES, SONY_BITS_15);
            break;
        case IR_TYPE_SONY_97:
            IrSender.sendSony(0x97, data, SONY_REPEAT_TIMES, SONY_BITS_15);
            break;
        case IR_TYPE_SONY_A4:
            IrSender.sendSony(0xA4, data, SONY_REPEAT_TIMES, SONY_BITS_15);
            break;
        case IR_TYPE_SONY_1C5A:
            IrSender.sendSony(0x1C5A, data, SONY_REPEAT_TIMES, SONY_BITS_MAX);
            break;
        case IR_TYPE_SONY_1E5A:
            IrSender.sendSony(0x1E5A, data, SONY_REPEAT_TIMES, SONY_BITS_MAX);
            break;
        case IR_TYPE_PLUSMINUS0_76:
            sendPlusMinus0(0x76, data, PLUSMINUS0_REPEAT_TIMES);
            break;
        default:
            break;
    }
    arduboy.setRGBled(GREEN_LED, 0);
}

/*---------------------------------------------------------------------------*/

static void sendIrisOhyama(uint16_t address, uint32_t command)
{
    // Vendor Parity
    uint8_t tVendorParity = (address ^ (address >> 4) ^ (address >> 8)) & 0xF;

    // Data
    LongUnion tSendValue;
    tSendValue.ULong = command & 0xFFFFFF;
    tVendorParity ^= tSendValue.UByte.LowByte ^ tSendValue.UByte.MidLowByte ^ tSendValue.UByte.MidHighByte;
    tVendorParity = (tVendorParity ^ (tVendorParity >> 4)) & 0xF;
    tSendValue.UByte.HighByte = tVendorParity ^ 0xF;

    // Set IR carrier frequency
    IrSender.enableIROut(IRISOHYAMA_KHZ); // 38 kHZ

    // Header
    IrSender.mark(IRISOHYAMA_HEADER1_MARK);
    IrSender.space(IRISOHYAMA_HEADER_SPACE);
    IrSender.mark(IRISOHYAMA_HEADER2_MARK);
    IrSender.space(IRISOHYAMA_HEADER_SPACE);

    // Address
    IrSender.sendPulseDistanceWidthData(IRISOHYAMA_ONE_MARK, IRISOHYAMA_BIT_SPACE,
            IRISOHYAMA_ZERO_MARK, IRISOHYAMA_BIT_SPACE, address, IRISOHYAMA_ADDRESS_BITS,
            PROTOCOL_IS_LSB_FIRST);

    // Send command + parity + stop bit
    IrSender.sendPulseDistanceWidthData(IRISOHYAMA_ONE_MARK, IRISOHYAMA_BIT_SPACE,
            IRISOHYAMA_ZERO_MARK, IRISOHYAMA_BIT_SPACE, tSendValue.ULong,
            IRISOHYAMA_BITS - IRISOHYAMA_ADDRESS_BITS, PROTOCOL_IS_LSB_FIRST, SEND_STOP_BIT);
}

static void sendKaseikyoShort(uint16_t data, uint_fast8_t repeat, uint16_t vendor)
{
    // Vendor Parity
    uint8_t tVendorParity = vendor ^ (vendor >> 8);
    tVendorParity = (tVendorParity ^ (tVendorParity >> 4)) & 0xF;

    // Data
    LongUnion tSendValue;
    tSendValue.UWord.LowWord = data << KASEIKYO_VENDOR_ID_PARITY_BITS;
    tSendValue.UByte.LowByte |= tVendorParity; // set low nibble to parity
    tSendValue.UByte.MidHighByte = tSendValue.UByte.LowByte ^ tSendValue.UByte.MidLowByte; // Parity

    // Set IR carrier frequency
    IrSender.enableIROut(KASEIKYO_KHZ); // 37 kHz

    repeat++;
    while (repeat > 0) {

        // Header
        IrSender.mark(KASEIKYO_HEADER_MARK);
        IrSender.space(KASEIKYO_HEADER_SPACE);

        // Vendor ID
        IrSender.sendPulseDistanceWidthData(KASEIKYO_BIT_MARK, KASEIKYO_ONE_SPACE,
                KASEIKYO_BIT_MARK, KASEIKYO_ZERO_SPACE, vendor, KASEIKYO_VENDOR_ID_BITS,
                PROTOCOL_IS_LSB_FIRST);

        // Send data + parity + stop bit
        IrSender.sendPulseDistanceWidthData(KASEIKYO_BIT_MARK, KASEIKYO_ONE_SPACE,
                KASEIKYO_BIT_MARK, KASEIKYO_ZERO_SPACE, tSendValue.ULong,
                KASEIKYO_SHORT_BITS - KASEIKYO_VENDOR_ID_BITS, PROTOCOL_IS_LSB_FIRST,
                SEND_STOP_BIT);

        // skip last delay!
        if (--repeat > 0) {
            // send repeated command in a fixed raster
            _delay_us(KASEIKYO_REPEAT_SPACE);
        }
    }
}

static void sendPlusMinus0(uint8_t address, uint8_t command, uint_fast8_t repeat)
{
    uint32_t tData = address | command << 8;
    IrSender.sendPulseDistanceWidth(PLUSMINUS0_KHZ, PLUSMINUS0_HEADER_MARK, PLUSMINUS0_HEADER_SPACE,
            PLUSMINUS0_ONE_MARK, PLUSMINUS0_ONE_SPACE, PLUSMINUS0_ZERO_MARK, PLUSMINUS0_ZERO_SPACE,
            tData, PLUSMINUS0_BITS, PROTOCOL_IS_LSB_FIRST, SEND_STOP_BIT,
            PLUSMINUS0_REPEAT_PERIOD / MICROS_IN_ONE_MILLI, repeat);
}

/*---------------------------------------------------------------------------*/
/*                           IR Receiver Functions                           */
/*---------------------------------------------------------------------------*/

void beginIRReceiver(void)
{
    IrReceiver.begin(PIN_RECEIVE_IR, ENABLE_LED_FEEDBACK, PIN_FEEDBACK_LED);
}

bool receiveIR(IR_INFO_T &info)
{
    bool ret = false;
    if (irparams.StateForISR == IR_REC_STATE_STOP &&
            (decodeKaseikyoShort(info) || decodeIrisOhyama(info) || decodePlusMinus0(info))) {
        ret = true;
    } else if (IrReceiver.decode()) {
        switch (IrReceiver.decodedIRData.protocol) {
            case APPLE:
                IrReceiver.decodedIRData.command |= IrReceiver.decodedIRData.address << 8; 
                IrReceiver.decodedIRData.address = APPLE_ADDRESS;
                // Continue to following...
            case NEC:
            case ONKYO:
                convertIRData(info, F("NEC"));
                break;
            case PANASONIC:
                convertKaseikyoIRData(info, PANASONIC_VENDOR_ID_CODE);
                break;
            case KASEIKYO:
                convertKaseikyoIRData(info, IrReceiver.decodedIRData.extra);
                break;
            case KASEIKYO_JVC:
                convertKaseikyoIRData(info, JVC_VENDOR_ID_CODE);
                break;
            case KASEIKYO_DENON:
                convertKaseikyoIRData(info, DENON_VENDOR_ID_CODE);
                break;
            case KASEIKYO_SHARP:
                convertKaseikyoIRData(info, SHARP_VENDOR_ID_CODE);
                break;
            case KASEIKYO_MITSUBISHI:
                convertKaseikyoIRData(info, MITSUBISHI_VENDOR_ID_CODE);
                break;
            case SONY:
                convertIRData(info, F("SONY"));
                break;
            case UNKNOWN:
            default:
                convertIRData(info, F("UNKNOWN"));
                break;
        }
        ret = true;
    }

    if (ret) {
        uint8_t len = IrReceiver.decodedIRData.rawDataPtr->rawlen;
        memcpy(info.pBuffer, IrReceiver.decodedIRData.rawDataPtr->rawbuf, len * sizeof(uint16_t));
        if (len < RAW_BUFFER_LENGTH) {
            info.pBuffer[len] = 0;
        }
        IrReceiver.resume();
    }
    return ret;
}

void endIRReceiver(void)
{
    IrReceiver.end();
}

/*---------------------------------------------------------------------------*/

static bool decodeKaseikyoShort(IR_INFO_T &info)
{
    // Check length and header
    if (IrReceiver.decodedIRData.rawDataPtr->rawlen != ((2 * KASEIKYO_SHORT_BITS) + 4) ||
            !matchMark(IrReceiver.decodedIRData.rawDataPtr->rawbuf[1], KASEIKYO_HEADER_MARK) ||
            !matchMark(IrReceiver.decodedIRData.rawDataPtr->rawbuf[2], KASEIKYO_HEADER_SPACE)) {
        return false;
    }

    // Decode Vendor ID
    if (!IrReceiver.decodePulseDistanceData(KASEIKYO_VENDOR_ID_BITS, 3, KASEIKYO_BIT_MARK,
            KASEIKYO_ONE_SPACE, KASEIKYO_ZERO_SPACE, PROTOCOL_IS_LSB_FIRST)) {
        return false;
    }
    uint16_t tVendorId = IrReceiver.decodedIRData.decodedRawData;
    uint8_t tVendorParity = tVendorId ^ (tVendorId >> 8);
    tVendorParity = (tVendorParity ^ (tVendorParity >> 4)) & 0xF;

    // Decode data + parity
    if (!IrReceiver.decodePulseDistanceData(KASEIKYO_SHORT_BITS - KASEIKYO_VENDOR_ID_BITS,
            3 + (2 * KASEIKYO_VENDOR_ID_BITS), KASEIKYO_BIT_MARK, KASEIKYO_ONE_SPACE,
            KASEIKYO_ZERO_SPACE, PROTOCOL_IS_LSB_FIRST)) {
        return false;
    }
    LongUnion tValue;
    tValue.ULong = IrReceiver.decodedIRData.decodedRawData;

    info.protocol = F("KASEIKYO-S");
    info.vendor = tVendorId;
    info.address = 0;
    info.command = tValue.UWord.LowWord >> KASEIKYO_VENDOR_ID_PARITY_BITS;
    info.nbits = KASEIKYO_SHORT_BITS;
    info.data = (uint64_t)tValue.ULong << KASEIKYO_VENDOR_ID_BITS | tVendorId;
    info.flags = 0;

    // Check parity
    uint8_t tParity = tValue.UByte.LowByte ^ tValue.UByte.MidLowByte;
    if (tVendorParity != (tValue.UByte.LowByte & 0xF) || tValue.UByte.MidHighByte != tParity) {
        info.flags |= IRDATA_FLAGS_PARITY_FAILED;
    }

    // Check for repeat
    if (IrReceiver.decodedIRData.rawDataPtr->rawbuf[0] < (KASEIKYO_REPEAT_PERIOD / MICROS_PER_TICK)) {
        info.flags |= IRDATA_FLAGS_IS_REPEAT;
    }

    return true;
}

static bool decodeIrisOhyama(IR_INFO_T &info)
{
    // Check length and header
    if (IrReceiver.decodedIRData.rawDataPtr->rawlen != ((2 * IRISOHYAMA_BITS) + 6) ||
            !matchMark(IrReceiver.decodedIRData.rawDataPtr->rawbuf[1], IRISOHYAMA_HEADER1_MARK) ||
            !matchMark(IrReceiver.decodedIRData.rawDataPtr->rawbuf[2], IRISOHYAMA_HEADER_SPACE) ||
            !matchMark(IrReceiver.decodedIRData.rawDataPtr->rawbuf[3], IRISOHYAMA_HEADER2_MARK) ||
            !matchMark(IrReceiver.decodedIRData.rawDataPtr->rawbuf[4], IRISOHYAMA_HEADER_SPACE)) {
        return false;
    }

    // Decode address
    if (!IrReceiver.decodePulseWidthData(IRISOHYAMA_ADDRESS_BITS, 5, IRISOHYAMA_ONE_MARK,
            IRISOHYAMA_ZERO_MARK, IRISOHYAMA_BIT_SPACE, PROTOCOL_IS_LSB_FIRST)) {
        return false;
    }
    uint16_t tAddress = IrReceiver.decodedIRData.decodedRawData;
    uint8_t tParity = (tAddress ^ (tAddress >> 4) ^ (tAddress >> 8)) & 0xF;

    // Decode data + parity
    if (!IrReceiver.decodePulseWidthData(IRISOHYAMA_BITS - IRISOHYAMA_ADDRESS_BITS,
            5 + (2 * IRISOHYAMA_ADDRESS_BITS), IRISOHYAMA_ONE_MARK, IRISOHYAMA_ZERO_MARK,
            IRISOHYAMA_BIT_SPACE, PROTOCOL_IS_LSB_FIRST)) {
        return false;
    }
    LongUnion tValue;
    tValue.ULong = IrReceiver.decodedIRData.decodedRawData;
    tParity ^= (tValue.UByte.LowByte ^ tValue.UByte.MidLowByte ^ tValue.UByte.MidHighByte);
    tParity = (tParity ^ (tParity >> 4)) & 0xF;

    info.protocol = F("IRISOHYAMA");
    info.vendor = 0;
    info.address = tAddress;
    info.command = tValue.ULong & 0xFFFFFF;
    info.nbits = IRISOHYAMA_BITS;
    info.data = (uint64_t)tValue.ULong << IRISOHYAMA_ADDRESS_BITS | tAddress;
    info.flags = 0;

    // Check parity
    if ((tParity ^ tValue.UByte.HighByte) != 0xF) {
        info.flags |= IRDATA_FLAGS_PARITY_FAILED;
    }

    return true;
}

static bool decodePlusMinus0(IR_INFO_T &info)
{
    // Check length and header
    if (IrReceiver.decodedIRData.rawDataPtr->rawlen != ((2 * PLUSMINUS0_BITS) + 4) ||
            !matchMark(IrReceiver.decodedIRData.rawDataPtr->rawbuf[1], PLUSMINUS0_HEADER_MARK) ||
            !matchMark(IrReceiver.decodedIRData.rawDataPtr->rawbuf[2], PLUSMINUS0_HEADER_SPACE)) {
        return false;
    }

    // Decode data
    if (!decodePulseDistanceWidthData(PLUSMINUS0_BITS, 3, PLUSMINUS0_ONE_MARK,
            PLUSMINUS0_ONE_SPACE, PLUSMINUS0_ZERO_MARK, PLUSMINUS0_ZERO_SPACE,
            PROTOCOL_IS_LSB_FIRST)) {
        return false;
    }

    LongUnion tValue;
    tValue.ULong = IrReceiver.decodedIRData.decodedRawData;

    info.protocol = F("PLUSMINUS0");
    info.vendor = 0;
    info.address = tValue.UByte.LowByte;
    info.command = tValue.UByte.MidLowByte;
    info.nbits = PLUSMINUS0_BITS;
    info.data = info.command;
    info.flags = 0;

    // Check for repeat
    if (IrReceiver.decodedIRData.rawDataPtr->rawbuf[0] < (PLUSMINUS0_REPEAT_PERIOD / MICROS_PER_TICK)) {
        info.flags |= IRDATA_FLAGS_IS_REPEAT;
    }

    return true;
}

static bool decodePulseDistanceWidthData(uint8_t aNumberOfBits, uint8_t aStartOffset,
        uint16_t aOneMarkMicros, uint16_t aOneSpaceMicros, uint16_t aZeroMarkMicros,
        uint16_t aZeroSpaceMicros, bool aMSBfirst)
{
    unsigned int *tRawBufPointer = &IrReceiver.decodedIRData.rawDataPtr->rawbuf[aStartOffset];
    uint32_t tDecodedData = 0;

    if (aMSBfirst) {
        for (uint_fast8_t i = 0; i < aNumberOfBits; i++) {
            bool isOne;
            // Check for variable length mark
            if (matchMark(*tRawBufPointer, aOneMarkMicros)) {
                isOne = true;
            } else if (matchMark(*tRawBufPointer, aZeroMarkMicros)) {
                isOne = false;
            } else {
                return false;
            }
            tRawBufPointer++;

            // Check for variable length space indicating a 0 or 1
            if (isOne && matchSpace(*tRawBufPointer, aOneSpaceMicros)) {
                tDecodedData = (tDecodedData << 1) | 1;
            } else if (!isOne && matchSpace(*tRawBufPointer, aZeroSpaceMicros)) {
                tDecodedData = (tDecodedData << 1) | 0;
            } else {
                return false;
            }
            tRawBufPointer++;
        }
    } else {
        for (uint32_t tMask = 1UL; aNumberOfBits > 0; tMask <<= 1, aNumberOfBits--) {
            bool isOne;
            // Check for variable length mark
            if (matchMark(*tRawBufPointer, aOneMarkMicros)) {
                isOne = true;
            } else if (matchMark(*tRawBufPointer, aZeroMarkMicros)) {
                isOne = false;
            } else {
                return false;
            }
            tRawBufPointer++;

            // Check for variable length space indicating a 0 or 1
            if (isOne && matchSpace(*tRawBufPointer, aOneSpaceMicros)) {
                tDecodedData |= tMask; // set the bit
            } else if (!isOne && matchSpace(*tRawBufPointer, aZeroSpaceMicros)) {
                // do not set the bit
            } else {
                return false;
            }
            tRawBufPointer++;
        }
    }
    IrReceiver.decodedIRData.decodedRawData = tDecodedData;
    return true;
}

static void convertIRData(IR_INFO_T &info, const __FlashStringHelper *protocol)
{
    info.protocol = protocol;
    info.vendor = 0;
    info.address = IrReceiver.decodedIRData.address;
    info.command = IrReceiver.decodedIRData.command;
    info.nbits = IrReceiver.decodedIRData.numberOfBits;
    info.flags = IrReceiver.decodedIRData.flags;
    info.data = IrReceiver.decodedIRData.decodedRawData;
}

static void convertKaseikyoIRData(IR_INFO_T &info, uint16_t vendor)
{
    info.protocol = F("KASEIKYO");
    info.vendor = vendor;
    info.address = IrReceiver.decodedIRData.address;
    info.command = IrReceiver.decodedIRData.command;
    info.nbits = IrReceiver.decodedIRData.numberOfBits;
    info.flags = IrReceiver.decodedIRData.flags;
    info.data = IrReceiver.decodedIRData.decodedRawData;
    info.data = info.data << KASEIKYO_VENDOR_ID_BITS | info.vendor;
}
