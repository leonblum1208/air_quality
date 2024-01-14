#ifndef DISPLAY_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define DISPLAY_H

#include <Arduino.h>
#include <MCUFRIEND_kbv.h>

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GRAY    0x8410

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
    long timestampMicro;
};


void helloWorld(MCUFRIEND_kbv tft);
MCUFRIEND_kbv initLCD(MCUFRIEND_kbv tft, uint8_t rotation, uint16_t backgroundColor, uint16_t textColor);
MCUFRIEND_kbv clearLCD(MCUFRIEND_kbv tft, uint16_t backgroundColor, uint16_t textColor);
void printAirQualityDataPoint(MCUFRIEND_kbv tft, airQualityDataPoint tuple, uint16_t backgroundColor, uint16_t textColor);

#endif