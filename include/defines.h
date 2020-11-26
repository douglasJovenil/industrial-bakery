#define HIGH    1
#define LOW     0
#define FALSE   0
#define TRUE    1
#define OFF     0
#define ON      1
#define OUT     0
#define IN      1
#define DOWN   'd'
#define UP     'u'

#define ONE     17
#define TWO     18
#define THREE   20
#define FOUR    33
#define FIVE    34
#define SIX     36
#define SEVEN   65
#define EIGHT   66
#define NINE    68
#define ERASE   129
#define _ZERO   130
#define ENTER   132

#define NUM_BUTTONS 11
#define NUM_COLUMNS 0b100
#define ZERO_ASCII  48
#define END_LCD     0x40
#define START_LCD   0x00
#define CMD_CLEAR   0x01
#define DELAY_CLEAR 80
#define LCD_SIZE 18
#define BREAD_BUTTON RD3
#define REMOVE_BREAD_BUTTON RC1
#define STEP_TEMP_BUTTON RC0

#define ERROR_TIME 1000
#define MAX_STEP_TEMP 15
#define MIN_STEP_TEMP 1
#define DECIMAL 1
#define UNITARY 0 
#define FULL_NUMBER 2
#define BREAD_RELEASE_DELAY 500
#define MAX_QNT_BREAD 5

typedef unsigned char uint8_t;
typedef unsigned int uint32_t;
typedef int int32_t;
typedef char int8_t;