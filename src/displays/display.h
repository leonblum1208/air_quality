#ifndef DISPLAY_H // To make sure you don't declare the function more than once by including the header multiple times.
#define DISPLAY_H

#include <Arduino.h>
#include <MCUFRIEND_kbv.h>
#include "Adafruit_ThinkInk.h"
#include <Wire.h>

#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define GRAY 0x8410

// EPD_BLACK
// EPD_WHITE
// EPD_RED

#define EPD_DC 9     // can be any pin, but required!
#define EPD_CS 10    // can be any pin, but required!
#define EPD_BUSY -1  // can set to -1 to not use a pin (will wait a fixed delay)
#define SRAM_CS 8    // can set to -1 to not use a pin (uses a lot of RAM!)
#define EPD_RESET -1 // can set to -1 and share with chip Reset (can't deep sleep)

struct airQualityDataPoint
{
    uint16_t co2;
    float temperature;
    float humidity;
    float vocIndex;
    float noxIndex;
    float massConcentrationPm1p0;
    float massConcentrationPm2p5;
    float massConcentrationPm4p0;
    float massConcentrationPm10p0;
    long timestampMilli;
};

void helloWorld(MCUFRIEND_kbv tft);
MCUFRIEND_kbv initLCD(MCUFRIEND_kbv tft, uint8_t rotation, uint16_t backgroundColor, uint16_t textColor);
MCUFRIEND_kbv clearLCD(MCUFRIEND_kbv tft, uint16_t backgroundColor, uint16_t textColor);
void printAirQualityDataPoint(MCUFRIEND_kbv tft, airQualityDataPoint tuple, uint16_t backgroundColor, uint16_t textColor);

void printLastWaitingDurationEPaper(ThinkInk_270_Tricolor_C44 ePaper, float durationMilli, long nUpdates);
void helloWorldEPaper(ThinkInk_270_Tricolor_C44 tft);
ThinkInk_270_Tricolor_C44 initEPaper(ThinkInk_270_Tricolor_C44 tft, uint8_t rotation, uint16_t backgroundColor, uint16_t textColor);
void printAirQualityDataPointEPaper(ThinkInk_270_Tricolor_C44 tft, airQualityDataPoint tuple, uint16_t backgroundColor, uint16_t textColor);
int8_t readButtonsEPaper(void);

#endif
