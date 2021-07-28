/**
 * @author:  7e12
 * @file:    stspin220.h
 * @date:    16 Jul 2021
 * @version: v1.0.0
 * @brief:   Arduino Uno Rev3 standard stepper motor controller library
**/

#ifndef _STSPIN220_H_
#define _STSPIN220_H_

#include "Arduino.h"
#include "math.h"

// For developing
// #define OUTPUT                          0

// User-defined macros
#define DEFAULT_STEPS_PER_REV           2048        // Default steps per revolution when driving the stepper motor in full-step mode
#define DEFAULT_STEP_PERIOD             1           // Period between 2 step clocks (ms)

// Developer-defined macros
#define CLOCKWISE                       0
#define COUNTER_CLOCKWISE               1
#define MODE_FULL_STEP                  0
#define MODE_1P2_STEP                   1
#define MODE_1P4_STEP                   2
#define MODE_1P8_STEP                   3
#define MODE_1P16_STEP                  4
#define MODE_1P32_STEP                  5
#define MODE_1P64_STEP                  6
#define MODE_1P128_STEP                 7
#define MODE_1P256_STEP                 8
                                                    //  MODE3   MODE4   MODE1   MODE2
                                                    //   STCK    DIR
static unsigned int StepModeMap[9]   = {0b0000,     //    0       0       0       0     MODE_FULL_STEP
                                        0b1010,     //    1       0       1       0     MODE_1P2_STEP
                                        0b0101,     //    0       1       0       1     MODE_1P4_STEP
                                        0b1011,     //    1       0       1       1     MODE_1P8_STEP
                                        0b1111,     //    1       1       1       1     MODE_1P16_STEP
                                        0b0001,     //    0       0       0       1     MODE_1P32_STEP
                                        0b0111,     //    1       1       1       1     MODE_1P64_STEP
                                        0b0010,     //    0       0       1       0     MODE_1P128_STEP
                                        0b0011};    //    0       0       1       1     MODE_1P256_STEP

// Struct type definitions for the following usage
typedef struct STSPIN220_Class          STSPIN220_ClassTypeDef;
typedef struct STSPIN220_APIs           STSPIN220_APIsTypeDef;
typedef struct STSPIN220_Pins           STSPIN220_PinsTypeDef;

// Motor driver module class
struct STSPIN220_Class {
    const STSPIN220_APIsTypeDef *api;
    STSPIN220_PinsTypeDef *pin;
    unsigned int stepMode;
    unsigned int stepPeriod;
    int isRunning;
};

// Motor driver module APIs
struct STSPIN220_APIs {
    void (*init)(STSPIN220_ClassTypeDef *self, STSPIN220_PinsTypeDef *pin, unsigned int stepMode);

    void (*setStepMode)(STSPIN220_ClassTypeDef *self, unsigned int stepMode);
    unsigned int (*getStepMode)(STSPIN220_ClassTypeDef *self);

    void (*runSteps)(STSPIN220_ClassTypeDef *self, unsigned long stepNumber, unsigned int runDirection);
    void (*runRevolutions)(STSPIN220_ClassTypeDef *self, unsigned int revNumber, unsigned int runDirection);
    void (*stop)(STSPIN220_ClassTypeDef *self);

    void (*setDirection)(STSPIN220_ClassTypeDef *self, unsigned int runDirection);
    unsigned int (*getDirection)(STSPIN220_ClassTypeDef *self);

    void (*setStepPeriod)(STSPIN220_ClassTypeDef *self, unsigned int stepPeriod);
    unsigned int (*getStepPeriod)(STSPIN220_ClassTypeDef *self);
};

// Motor driver module pins
struct STSPIN220_Pins {
    unsigned int pinStandbyReset;
    unsigned int pinMode1;
    unsigned int pinMode2;
    unsigned int pinMode3StepClock;
    unsigned int pinMode4Direction;
};

// For everywhere usage
extern STSPIN220_ClassTypeDef mySTSPIN220;

#endif /* _STSPIN220_H_ */
