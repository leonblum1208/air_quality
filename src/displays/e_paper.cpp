#include "display.h"

uint16_t WIDTH_PX_E_PAPER = 264;
uint16_t HEIGHT_PX_E_PAPER = 176;


ThinkInk_270_Tricolor_C44 initEPaper(ThinkInk_270_Tricolor_C44 ePaper, uint8_t rotation, uint16_t backgroundColor, uint16_t textColor)
{
    ePaper.begin(THINKINK_TRICOLOR);
    ePaper.setRotation(rotation);
    ePaper.clearBuffer();
    ePaper.fillScreen(backgroundColor);
    ePaper.setTextColor(textColor);
    ePaper.setTextSize(2);
    ePaper.print("Waiting for first measurement...");
    ePaper.display();
    return ePaper;
}

void printAirQualityDataPointEPaper(ThinkInk_270_Tricolor_C44 ePaper, airQualityDataPoint tuple, uint16_t backgroundColor, uint16_t textColor)
{
    int16_t cur_cursor_x;
    int16_t line_height = 18;
    // Column 1
    int16_t cur_col_x = 0;

    ePaper.fillScreen(backgroundColor);
    ePaper.setTextColor(textColor);
    ePaper.setTextSize(2);

    ePaper.setCursor(cur_col_x, 0);
    ePaper.print(tuple.temperature, 1);
    ePaper.print("C");
    cur_cursor_x = ePaper.getCursorX();

    ePaper.setCursor(cur_col_x, line_height * 1);
    ePaper.print(tuple.humidity, 0);
    ePaper.print("%");
    cur_cursor_x = max(cur_cursor_x, ePaper.getCursorX());

    ePaper.setCursor(cur_col_x, line_height * 2);
    ePaper.print("VOC:");
    ePaper.print(tuple.vocIndex, 0);
    cur_cursor_x = max(cur_cursor_x, ePaper.getCursorX());

    // Column 2
    cur_col_x = cur_cursor_x + 10;
    ePaper.setCursor(cur_col_x, 0);
    ePaper.print("C02:");
    ePaper.print(tuple.co2);
    ePaper.print("ppm");
    cur_cursor_x = ePaper.getCursorX();

    ePaper.setCursor(cur_col_x, line_height * 1);
    ePaper.print("PM2,5:");
    ePaper.print(tuple.massConcentrationPm2p5, 1);
    cur_cursor_x = max(cur_cursor_x, ePaper.getCursorX());

    ePaper.setCursor(cur_col_x, line_height * 2);
    ePaper.print("NOx:");
    ePaper.print(tuple.noxIndex, 1);
    cur_cursor_x = max(cur_cursor_x, ePaper.getCursorX());
}

void helloWorldEPaper(ThinkInk_270_Tricolor_C44 ePaper)
{
    ePaper.print("hello world");
}

void printLastWaitingDurationEPaper(ThinkInk_270_Tricolor_C44 ePaper, float durationMilli, long nUpdates)
{
    ePaper.setTextSize(1);
    ePaper.setCursor(0, HEIGHT_PX_E_PAPER - 8);
    ePaper.print("Last update:");
    ePaper.print(durationMilli / (60.*1000.), 1);
    ePaper.print("m ");
    ePaper.print("n:");
    ePaper.print(nUpdates);
    ePaper.print(" Uptime:");
    ePaper.print(millis() / (60.*60.*1000.), 1);
    ePaper.print("h");
}

int8_t readButtonsEPaper(void)
{
    uint16_t reading = analogRead(A3);
    // Serial.println(reading);

    if (reading > 600)
    {
        return 0; // no buttons pressed
    }
    if (reading > 400)
    {
        return 4; // button D pressed
    }
    if (reading > 250)
    {
        return 3; // button C pressed
    }
    if (reading > 125)
    {
        return 2; // button B pressed
    }
    return 1; // Button A pressed
}