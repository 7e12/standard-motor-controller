/**
 * @author:  7e12
 * @file:    stspin220.c
 * @date:    16 Jul 2021
 * @version: v1.0.0
 * @brief:   Arduino Uno Rev3 standard stepper motor controller library
**/

#include "stspin220.h"

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
