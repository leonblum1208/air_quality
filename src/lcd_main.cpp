// #include <Arduino.h>
// #include <SensirionI2CScd4x.h>
// #include <Wire.h>
// #include <LCDWIKI_GUI.h> //Core graphics library
// #include <LCDWIKI_KBV.h> //Hardware-specific library

// SensirionI2CScd4x scd4x;
// LCDWIKI_KBV lcd(ILI9486,A3,A2,A1,A0,A4); //model,cs,cd,wr,rd,reset

// void setup_scd4x_sensor() {
//     while (!Serial) {
//         delay(100);
//     }
//     Wire.begin();

//     uint16_t error;
//     char errorMessage[256];

//     scd4x.begin(Wire);

//     // stop potentially previously started measurement
//     error = scd4x.stopPeriodicMeasurement();
//     if (error) {
//         Serial.print("Error trying to execute stopPeriodicMeasurement(): ");
//         errorToString(error, errorMessage, 256);
//         Serial.println(errorMessage);
//     }

//     // Start Measurement
//     error = scd4x.startPeriodicMeasurement();
//     if (error) {
//         Serial.print("Error trying to execute startPeriodicMeasurement(): ");
//         errorToString(error, errorMessage, 256);
//         Serial.println(errorMessage);
//     }
// }

// void print_results_to_lcd(uint16_t co2, float temperature, float humidity) {
//     lcd.Fill_Screen(255, 255, 255);
//     lcd.Set_Text_colour(0, 0, 0);
//     lcd.Set_Text_Size(2);
//     lcd.Set_Text_Mode(1);
//     lcd.Print_String("Hello World!", 0, 0);
//     lcd.Print_String("Hello World!", 0, 15);
// }


// void setup() 
// {
//     Serial.begin(57600);
//     setup_scd4x_sensor();
//     lcd.Init_LCD();
//     lcd.Set_Rotation(1);
//     Serial.println("Waiting for first measurement... (10 sec)");
//     delay(10000);
// }

// void loop() 
// {
//     uint16_t error;
//     char errorMessage[256];
//     // Read Measurement
//     uint16_t co2 = 0;
//     float temperature = 0.0f;
//     float humidity = 0.0f;
//     print_results_to_lcd(0, 0.0, 0.0);
//     error = scd4x.readMeasurement(co2, temperature, humidity);

//     if (error) {
//         Serial.print("Error trying to execute readMeasurement(): ");
//         errorToString(error, errorMessage, 256);
//         Serial.println(errorMessage);
//     } else if (co2 == 0) {
//         Serial.println("Invalid sample detected, skipping.");
//     } else {
//         Serial.print("Co2:");
//         Serial.print(co2);
//         Serial.print("\t");
//         Serial.print("Temperature:");
//         Serial.print(temperature);
//         Serial.print("\t");
//         Serial.print("Humidity:");
//         Serial.println(humidity);
//         print_results_to_lcd(co2, temperature, humidity);
//     }

//     delay(10000);
// }