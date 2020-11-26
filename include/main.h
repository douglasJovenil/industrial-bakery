#include "pic18f4550_config.h"
#include "defines.h"
#include "xlcd.h"
#include "ADC.h"

void definitions();
void blinkLed();
void removeBread(uint8_t *bread_qnt, uint32_t *desired_temp, uint8_t step_temp);
void addBread(uint8_t *bread_qnt, uint8_t *desired_temp, uint8_t step_temp);
void changeStepTemp(uint8_t *step_temp);
void writeInformationToLCD(uint32_t desired_temp_int, uint8_t step_temp_int, uint8_t bread_qnt_int);
uint8_t getStepTemp();
uint8_t getDesiredTemp();
void intToArray(uint32_t num, uint8_t array[]);
int32_t getValueADC(int8_t pin_adc, uint8_t number_part);
uint8_t arrayToInt(uint8_t array[]);
void keyboardScan(uint8_t array[]);
uint32_t abs(int32_t num);
void writeCharLCD(uint8_t character, uint8_t horizontal_increment, uint8_t vertical_position);
void writeArrayLCD(const int8_t string[], uint8_t horizontal_increment, int8_t vertical_position);
void clearLCD();
void startLCD();
void DelayFor18TCY(void);
void DelayPORXLCD(void);
void DelayXLCD(void);
void delay(uint32_t time);
void clearArray(uint8_t array[], uint8_t size);
void fillBlankArray(uint8_t array[], uint8_t size);
uint8_t sizeArray(const uint8_t array[]);