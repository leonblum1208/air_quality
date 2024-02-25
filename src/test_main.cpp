// #include "Adafruit_ThinkInk.h"

// #define EPD_DC 9     // can be any pin, but required!
// #define EPD_CS 10    // can be any pin, but required!
// #define EPD_BUSY -1  // can set to -1 to not use a pin (will wait a fixed delay)
// #define SRAM_CS 8    // can set to -1 to not use a pin (uses a lot of RAM!)
// #define EPD_RESET -1 // can set to -1 and share with chip Reset (can't deep sleep)


// // 2.7" Tricolor Featherwing or Breakout with IL91874 chipset
// ThinkInk_270_Tricolor_C44 ePaper(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY);

// void testdrawtext(char *text, uint16_t color)
// {
//     ePaper.setCursor(0, 0);
//     ePaper.setTextColor(color);
//     ePaper.setTextWrap(true);
//     ePaper.print(text);
// }

// int8_t readButtons(void)
// {
//     uint16_t reading = analogRead(A3);
//     // Serial.println(reading);

//     if (reading > 600)
//     {
//         return 0; // no buttons pressed
//     }
//     if (reading > 400)
//     {
//         return 4; // button D pressed
//     }
//     if (reading > 250)
//     {
//         return 3; // button C pressed
//     }
//     if (reading > 125)
//     {
//         return 2; // button B pressed
//     }
//     return 1; // Button A pressed
// }

// void setup()
// {
//     Serial.begin(115200);
//     while (!Serial)
//     {
//         delay(10);
//     }
//     Serial.println("Adafruit EPD full update test in red/black/white");
//     ePaper.begin(THINKINK_TRICOLOR);
//     ePaper.setRotation(2);

//     delay(5000);
//     Serial.println("Banner demo");
//     ePaper.clearBuffer();
//     ePaper.setTextSize(3);
//     ePaper.setTextColor(EPD_BLACK);
//     ePaper.print("hello lauras");
//     ePaper.print(readButtons());
//     ePaper.display();
// }

// void loop()
// {
// delay(1000);
// }
