/**
 * @author:  7e12
 * @file:    arduino-uno-rev3.ino
 * @date:    16 Jul 2021
 * @version: v1.0.0
 * @brief:   Arduino Uno Rev3 standard stepper motor controller
**/

/* <<<<<<< STSPIN220.H >>>>>>> */
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

/* <<<<<<< STSPIN220.C >>>>>>>*/
// APIs function declarations
void init(STSPIN220_ClassTypeDef *self, STSPIN220_PinsTypeDef *pin, unsigned int stepMode);

void setStepMode(STSPIN220_ClassTypeDef *self, unsigned int stepMode);
unsigned int getStepMode(STSPIN220_ClassTypeDef *self);

void runSteps(STSPIN220_ClassTypeDef *self, unsigned long stepNumber, unsigned int runDirection);
void runRevolutions(STSPIN220_ClassTypeDef *self, unsigned int revNumber, unsigned int runDirection);
void stop(STSPIN220_ClassTypeDef *self);

void setDirection(STSPIN220_ClassTypeDef *self, unsigned int runDirection);
unsigned int getDirection(STSPIN220_ClassTypeDef *self);

void setStepPeriod(STSPIN220_ClassTypeDef *self, unsigned int stepPeriod);
unsigned int getStepPeriod(STSPIN220_ClassTypeDef *self);

// Motor driver module APIs declaration
static STSPIN220_APIsTypeDef STSPIN220_APIs = {
    .init           = init,

    .setStepMode    = setStepMode,
    .getStepMode    = getStepMode,

    .runSteps       = runSteps,
    .runRevolutions = runRevolutions,
    .stop           = stop,

    .setDirection   = setDirection,
    .getDirection   = getDirection,

    .setStepPeriod  = setStepPeriod,
    .getStepPeriod  = getStepPeriod,
};

// Motor driver module class declaration
STSPIN220_ClassTypeDef mySTSPIN220 = {
    .api = &STSPIN220_APIs,
};

// Initialize the motor driver module
void init(STSPIN220_ClassTypeDef *self, STSPIN220_PinsTypeDef *pin, unsigned int stepMode) {
    self->pin = pin;

    pinMode(pin->pinStandbyReset, OUTPUT);
    pinMode(pin->pinMode1, OUTPUT);
    pinMode(pin->pinMode2, OUTPUT);
    pinMode(pin->pinMode3StepClock, OUTPUT);
    pinMode(pin->pinMode4Direction, OUTPUT);

    self->stepPeriod = DEFAULT_STEP_PERIOD;

    self->api->setStepMode(self, stepMode);
};

// Set step mode of the motor driver module
void setStepMode(STSPIN220_ClassTypeDef *self, unsigned int stepMode) {
    self->stepMode = stepMode;
    stepMode = StepModeMap[stepMode];

    digitalWrite(self->pin->pinStandbyReset, 0);
    delay(100);

    digitalWrite(self->pin->pinMode3StepClock, stepMode >> 3 & 0b1);
    digitalWrite(self->pin->pinMode4Direction, stepMode >> 2 & 0b1);
    digitalWrite(self->pin->pinMode1, stepMode >> 1 & 0b1);
    digitalWrite(self->pin->pinMode2, stepMode & 0b1);
    delay(100);
};

// Get step mode of the motor driver module
unsigned int getStepMode(STSPIN220_ClassTypeDef *self) {
    return self->stepMode;
};

// Run the motor driver module in step mode
void runSteps(STSPIN220_ClassTypeDef *self, unsigned long stepNumber, unsigned int runDirection) {
    self->isRunning = 1;

    digitalWrite(self->pin->pinStandbyReset, 1);
    delay(100);
    digitalWrite(self->pin->pinMode4Direction, runDirection);

    for (unsigned long i = 0; i < stepNumber; ++i) {
        digitalWrite(self->pin->pinMode3StepClock, 1);
        delay(self->stepPeriod);

        // For testing
        if (i == DEFAULT_STEPS_PER_REV) {
            // Stop running
            // self->api->stop(self);

            // Set new step period
            // self->api->setStepPeriod(self, 1000);
        }

        digitalWrite(self->pin->pinMode3StepClock, 0);
        delay(self->stepPeriod);
    }

    self->isRunning = 0;
};

// Run the motor driver module in revolution mode
void runRevolutions(STSPIN220_ClassTypeDef *self, unsigned int revNumber, unsigned int runDirection) {
    self->isRunning = 1;
    unsigned long stepNumber = revNumber * DEFAULT_STEPS_PER_REV * pow(2, self->stepMode);

    digitalWrite(self->pin->pinStandbyReset, 1);
    delay(100);
    digitalWrite(self->pin->pinMode4Direction, runDirection);

    for (unsigned long i = 0; i < stepNumber; ++i) {
        digitalWrite(self->pin->pinMode3StepClock, 1);
        delay(self->stepPeriod);

        // For testing
        if (i == DEFAULT_STEPS_PER_REV) {
            // Stop running
            // self->api->stop(self);

            // Set new step period
            // self->api->setStepPeriod(self, 1000);
        }

        digitalWrite(self->pin->pinMode3StepClock, 0);
        delay(self->stepPeriod);
    }

    self->isRunning = 0;
};

// Stop the motor driver module
void stop(STSPIN220_ClassTypeDef *self) {
    self->api->setStepMode(self, self->stepMode);
    self->isRunning = 0;
};

// Set direction of the motor driver module
void setDirection(STSPIN220_ClassTypeDef *self, unsigned int runDirection) {
    digitalWrite(self->pin->pinMode4Direction, runDirection);
    delay(10);
};

// Get direction of the motor driver module
unsigned int getDirection(STSPIN220_ClassTypeDef *self) {
    return digitalRead(self->pin->pinMode4Direction);
};

// Set step period of the motor driver module
void setStepPeriod(STSPIN220_ClassTypeDef *self, unsigned int stepPeriod) {
    self->stepPeriod = stepPeriod / 2;
};

// Get step period of the motor driver module
unsigned int getStepPeriod(STSPIN220_ClassTypeDef *self) {
    return self->stepPeriod;
};

/* <<<<<<< MAIN >>>>>>> */
void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);

    // Initial pins declarations
    STSPIN220_PinsTypeDef myPin = {
        .pinStandbyReset    = 8,
        .pinMode1           = 5,
        .pinMode2           = 10,
        .pinMode3StepClock  = 3,
        .pinMode4Direction  = 7,
    };

    // Initialize the motor driver module
    mySTSPIN220.api->init(&mySTSPIN220, &myPin, MODE_1P2_STEP);

    // Check step mode and step period
    Serial.print("Step Mode: ");
    Serial.print(mySTSPIN220.api->getStepMode(&mySTSPIN220));
    Serial.print("\n");

    Serial.print("Step Period: ");
    Serial.print(mySTSPIN220.api->getStepPeriod(&mySTSPIN220));
    Serial.print("\n");
}

void loop() {
    // put your main code here, to run repeatedly:

    // Run 4096 steps clockwise
    mySTSPIN220.api->runSteps(&mySTSPIN220, DEFAULT_STEPS_PER_REV * 2, CLOCKWISE);

    // Change running direction
    mySTSPIN220.api->setDirection(&mySTSPIN220, ~(mySTSPIN220.api->getDirection(&mySTSPIN220)));

    // Run 4096 steps counter-clockwise
    mySTSPIN220.api->runSteps(&mySTSPIN220, DEFAULT_STEPS_PER_REV * 2, mySTSPIN220.api->getDirection(&mySTSPIN220));

    // Set new step mode
    mySTSPIN220.api->setStepMode(&mySTSPIN220, MODE_FULL_STEP);

    // Run 7 revolutions
    mySTSPIN220.api->runRevolutions(&mySTSPIN220, 7, mySTSPIN220.api->getDirection(&mySTSPIN220));

    // Do nothing infinitely
    while (1) {
        delay(1000);
    }
}
