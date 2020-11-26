#include "main.h"

void main(){
  definitions();
  startLCD();
  uint32_t desired_temp = getDesiredTemp();
  uint8_t step_temp = getStepTemp();
  uint8_t bread_qnt = 0;

  while(TRUE){
    writeInformationToLCD(desired_temp, step_temp, bread_qnt);
    addBread(&bread_qnt, &desired_temp, step_temp);
    removeBread(&bread_qnt, &desired_temp, step_temp);
    changeStepTemp(&step_temp);
    blinkLed();
  }
}

void definitions(){
  TRISD = 0b11111000;
  TRISA = 0b00001111;
  CMCON = 0b00000001;
  TRISB = OUT;
  TRISC = OUT;
  OpenADC(ADC_FOSC_16 & ADC_RIGHT_JUST & ADC_4_TAD, ADC_CH0 & ADC_INT_OFF & ADC_REF_VDD_VSS, ADC_6ANA);
}

// ############################### STATE FUNCTIONS ###############################

void blinkLed(){
  static uint8_t flag = 0;

  if (flag == 0 && getValueADC(ADC_CH2, FULL_NUMBER) > 100){
    for (uint8_t i = 0; i < 6; i++){
      CMCON = 0b00000001;
      delay(300);
      CMCON = 0b00010001;
      delay(300);
    }
    CMCON = 0b00000001;
    flag++;
  }
  if (getValueADC(ADC_CH2, FULL_NUMBER) < 100) flag = 0;
}

void removeBread(uint8_t *bread_qnt, uint32_t *desired_temp, uint8_t step_temp){
  if (REMOVE_BREAD_BUTTON && *bread_qnt > 0){
    (*bread_qnt)--;
    *desired_temp -= step_temp;
    while(REMOVE_BREAD_BUTTON);
  }
}

void addBread(uint8_t *bread_qnt, uint8_t *desired_temp, uint8_t step_temp){
  if (BREAD_BUTTON && *bread_qnt < MAX_QNT_BREAD){
    (*bread_qnt)++;
    *desired_temp += step_temp;
    while(BREAD_BUTTON);
  }
}

void changeStepTemp(uint8_t *step_temp){
  if (STEP_TEMP_BUTTON){
    clearLCD();
    *step_temp = getStepTemp();
  }
}

void writeInformationToLCD(uint32_t desired_temp_int, uint8_t step_temp_int, uint8_t bread_qnt_int){
  uint8_t real_temp[LCD_SIZE];
  uint8_t desired_temp[LCD_SIZE];
  uint8_t step_temp[LCD_SIZE];
  uint8_t bread_qnt[LCD_SIZE];
  uint8_t diff_temp[LCD_SIZE];

  fillBlankArray(step_temp, LCD_SIZE);
  fillBlankArray(desired_temp, LCD_SIZE);
  fillBlankArray(real_temp, LCD_SIZE);
  fillBlankArray(diff_temp, LCD_SIZE);

  intToArray(getValueADC(ADC_CH2, FULL_NUMBER), real_temp);
  intToArray(abs(getValueADC(ADC_CH2, FULL_NUMBER) - desired_temp_int), diff_temp);
  intToArray(desired_temp_int, desired_temp);
  intToArray(step_temp_int, step_temp);
  intToArray(bread_qnt_int, bread_qnt);

  writeArrayLCD("RL: ", 0, UP);
  writeArrayLCD(real_temp, 4, UP);
  writeArrayLCD("DSJ: ", 8, UP);
  writeArrayLCD(desired_temp, 13, UP);

  writeArrayLCD("IC:", 0, DOWN);
  writeArrayLCD(step_temp, 3, DOWN);
  writeArrayLCD("Q:", 6, DOWN);
  writeArrayLCD(bread_qnt, 8, DOWN);
  writeArrayLCD("DF:", 10, DOWN);
  writeArrayLCD(diff_temp, 13, DOWN);
}

uint8_t getStepTemp(){
  uint8_t array_temp[LCD_SIZE];
  uint8_t int_temp = -1;

  while (int_temp < MIN_STEP_TEMP || int_temp > MAX_STEP_TEMP){
    writeArrayLCD("Temp passo:\0", 0, UP);
    keyboardScan(array_temp);
    int_temp = arrayToInt(array_temp);
    
    if(int_temp < MIN_STEP_TEMP || int_temp > MAX_STEP_TEMP){
      clearLCD();
      writeArrayLCD("Temp invalida!\0", 0, UP);
      delay(ERROR_TIME);
    }
    clearLCD();
  }
  return arrayToInt(array_temp);
}

uint8_t getDesiredTemp(){
  uint8_t array_temp[LCD_SIZE];
  writeArrayLCD("Temp desejada:\0", 0, UP);
  keyboardScan(array_temp);
  clearLCD();
  return arrayToInt(array_temp);
}

// ############################## PROGRAM FUNCTIONS ##############################

void intToArray(uint32_t num, uint8_t array[]){
  uint8_t scale = 1;
  uint8_t index = 0;

  for (uint32_t i = num; i > 9; i /= 10) scale *= 10; // OVERFLOW NESSA CARALHA
  while(num > 0){
    array[index] = (int)(num / scale) + ZERO_ASCII;
    num %= scale;
    scale /= 10;
    index++;
  }

  while (scale != 0){
    array[index] = ZERO_ASCII;
    scale /= 10;
    index++;
  }

}

int32_t getValueADC(int8_t pin_adc, uint8_t number_part){
  SetChanADC(pin_adc); 
  ConvertADC();          
  while(BusyADC());
  switch (number_part){
    case DECIMAL: return (int)(ReadADC()*0.49) / 10;
    case UNITARY: return (int)(ReadADC()*0.49) % 10;
    case FULL_NUMBER: return (int)(ReadADC()*0.49);
    default: return -1;
  }
}

uint8_t arrayToInt(uint8_t array[]){
  uint8_t num_int = 0;
  uint8_t scale = 1;

  for (int i = sizeArray(array) - 1; i >= 0; i--){
    num_int += (array[i] - ZERO_ASCII) * scale;
    scale *= 10;
  }
  return num_int;
}

void keyboardScan(uint8_t array[]){
  uint8_t dictionary[] = {_ZERO, ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, ERASE, ENTER};
  uint8_t pressed_enter = FALSE;
  uint8_t index_array = 0;

  while (!pressed_enter){
    for (LATD = 1; LATD <= NUM_COLUMNS; LATD <<= 1){
      for (uint8_t j = 0; j <= NUM_BUTTONS; j++){
        if (PORTD == dictionary[j]){
          if (dictionary[j] == ENTER){
            array[index_array] = '\0';
            pressed_enter = TRUE;
          } else if(dictionary[j] == ERASE && index_array > 0){
            index_array--;
            array[index_array] = '\0';
            writeCharLCD(array[index_array], index_array, DOWN);
          } else {
            array[index_array] = ZERO_ASCII + j;
            writeCharLCD(array[index_array], index_array, DOWN);
            index_array++;
          }
          while (RD4 | RD5 | RD6 | RD7);
        }
      }
    }
  }
}

uint32_t abs(int32_t num){
  return (num < 0) ? (num * -1) : num; 
}

// ############################## LCD FUNCTIONS ##############################

void writeCharLCD(uint8_t character, uint8_t horizontal_increment, uint8_t vertical_position){
  SetDDRamAddr((vertical_position == UP) ? START_LCD + horizontal_increment : END_LCD + horizontal_increment);
  while(BusyXLCD());
  putcXLCD(character);
  while(BusyXLCD());
}

void writeArrayLCD(const int8_t string[], uint8_t horizontal_increment, int8_t vertical_position){
  uint8_t ram_position = (vertical_position == UP) ? START_LCD + horizontal_increment  : END_LCD + horizontal_increment;
  
  for (uint8_t i = 0; i < sizeArray(string); i++){
    while(BusyXLCD());
    SetDDRamAddr(ram_position);
    while(BusyXLCD());
    putcXLCD(string[i]);
    while(BusyXLCD());
    ram_position++;
  }
}

void clearLCD(){	
   WriteCmdXLCD(CMD_CLEAR);
   while(BusyXLCD());
   delay(DELAY_CLEAR);
}

void startLCD(){
  OpenXLCD(FOUR_BIT&LINES_5X7);
  while(BusyXLCD());      
  WriteCmdXLCD(0x06); 
  while(BusyXLCD()); 
}

void DelayFor18TCY(void){
  __delay_us((8*18.0*4000000.0)/_XTAL_FREQ); 
}

void DelayPORXLCD(void){
  __delay_ms(60);
}

void DelayXLCD(void){
  __delay_ms(20);
}

// ############################## OTHER FUNCTIONS ##############################

void delay(uint32_t time){
  for (uint32_t i = 0; i < time; i++) __delay_ms(1);
}

void clearArray(uint8_t array[], uint8_t size){
  for (uint8_t i = 1; i <= size; i++) array[i] = '\0';
}

void fillBlankArray(uint8_t array[], uint8_t size){
  for (uint8_t i = 0; i < size; i++) array[i] = ' ';
  array[size] = '\0';
}

uint8_t sizeArray(const uint8_t array[]){
  uint8_t i = 0;
  while (array[i] != '\0') i++;
  return i;
}