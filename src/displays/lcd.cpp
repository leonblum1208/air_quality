#include "display.h"

MCUFRIEND_kbv initLCD(MCUFRIEND_kbv tft, uint8_t rotation, uint16_t backgroundColor, uint16_t textColor)
{
    uint16_t ID = tft.readID();
    tft.reset();
    tft.begin(ID);
    tft.setRotation(rotation); // "PORTRAIT", "LANDSCAPE", "PORTRAIT_REV", "LANDSCAPE_REV"
    tft = clearLCD(tft, backgroundColor, textColor);
    tft.print("Waiting for first measurement...");
    return tft;
}

MCUFRIEND_kbv clearLCD(MCUFRIEND_kbv tft, uint16_t backgroundColor, uint16_t textColor)
{
    tft.fillScreen(backgroundColor);
    tft.setTextColor(textColor);
    tft.setTextSize(2);
    tft.setCursor(0, 0);
    return tft;
}

void printAirQualityDataPoint(MCUFRIEND_kbv tft, airQualityDataPoint tuple, uint16_t backgroundColor, uint16_t textColor)
{
    tft = clearLCD(tft, backgroundColor, textColor);

    int16_t cur_cursor_x;

    // Column 1
    int16_t cur_col_x = 0;
    tft.setCursor(cur_col_x, 0);
    tft.print(tuple.temperature, 1);
    tft.print("C");
    cur_cursor_x = tft.getCursorX();

    tft.setCursor(cur_col_x, 20);
    tft.print(tuple.humidity, 0);
    tft.print("%");

    // Column 2
    cur_col_x = max(cur_cursor_x, tft.getCursorX()) + 10;
    tft.setCursor(cur_col_x, 0);
    tft.print("C02:");
    tft.print(tuple.co2);
    tft.print("ppm");
    cur_cursor_x = tft.getCursorX();

    tft.setCursor(cur_col_x, 20);
    tft.print("PM2,5:");
    tft.print(tuple.massConcentrationPm2p5, 1);

    // Column 3
    cur_col_x =  max(cur_cursor_x, tft.getCursorX()) + 10;
    tft.setCursor(cur_col_x, 0);
    tft.print("VOC:");
    tft.println(tuple.vocIndex, 0);

    tft.setCursor(cur_col_x, 20);
    tft.print("NOx:");
    tft.println(tuple.noxIndex, 1);
}

void helloWorld(MCUFRIEND_kbv tft)
{
    tft.print("hello world");
}
