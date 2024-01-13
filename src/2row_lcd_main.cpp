// #include <Arduino.h>
// #include <SensirionI2CScd4x.h>
// #include <SensirionI2CSen5x.h>
// #include <Wire.h>
// #include <LiquidCrystal.h>

// SensirionI2CScd4x scd4x;
// SensirionI2CSen5x sen5x;
// const int rs = 7, en = 8, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
// LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// void setup()
// {
//     Serial.begin(115200);
//         while (!Serial) {
//         delay(100);
//     }
//     lcd.begin(16, 2);
//     lcd.print("Loading...");

//     Wire.begin();
//     scd4x.begin(Wire);
//     scd4x.startPeriodicMeasurement();

//     sen5x.begin(Wire);
//     sen5x.deviceReset();
//     sen5x.startMeasurement();

//     delay(10000);
// }

// void loop()
// {
//     // SCD41 variables
//     uint16_t co2 = 0;
//     float temperatureSCD41 = 0.0f;
//     float humiditySCD41 = 0.0f;
//     bool isDataReadySCD41 = false;
//     uint16_t errorSCD41;

//     // Sen55 variables
//     float massConcentrationPm1p0;
//     float massConcentrationPm2p5;
//     float massConcentrationPm4p0;
//     float massConcentrationPm10p0;
//     float humiditySen55;
//     float temperatureSen55;
//     float vocIndex;
//     float noxIndex;
//     bool isDataReadySen55 = false;
//     uint16_t errorSen55;

//     errorSCD41 = scd4x.getDataReadyFlag(isDataReadySCD41);
//     errorSen55 = sen5x.readDataReady(isDataReadySen55);
//     if (errorSCD41 || errorSen55) {
//         Serial.print("Error trying to retrieve data ready flag");
//         delay(1000);
//         return;
//     }
//     if (!isDataReadySen55 || !isDataReadySCD41) {
//         delay(5000);
//         return;
//     }
//     errorSen55 = sen5x.readMeasuredValues(
//         massConcentrationPm1p0, massConcentrationPm2p5, massConcentrationPm4p0,
//         massConcentrationPm10p0, humiditySen55, temperatureSen55, vocIndex,
//         noxIndex);
//     errorSCD41 = scd4x.readMeasurement(co2, temperatureSCD41, humiditySCD41);

//     Serial.print("SEN55:");
//     Serial.print("\t");
//     Serial.print("Temp:");
//     Serial.print(temperatureSen55);
//     Serial.print("\t");
//     Serial.print("Humidity:");
//     Serial.print(humiditySen55);
//     Serial.print("\t");
//     Serial.print("VocIndex:");
//     Serial.print(vocIndex);
//     Serial.print("\t");
//     Serial.print("NoxIndex:");
//     Serial.print(noxIndex);
//     Serial.print("\t");
//     Serial.print("MassConcentration: Pm1p0:");
//     Serial.print(massConcentrationPm1p0);
//     Serial.print("\t");
//     Serial.print("Pm2p5:");
//     Serial.print(massConcentrationPm2p5);
//     Serial.print("\t");
//     Serial.print("Pm4p0:");
//     Serial.print(massConcentrationPm4p0);
//     Serial.print("\t");
//     Serial.print("Pm10p0:");
//     Serial.print(massConcentrationPm10p0);
//     Serial.print("\n");

//     Serial.print("SCD41:");
//     Serial.print("\t");
//     Serial.print("Temp:");
//     Serial.print(temperatureSCD41);
//     Serial.print("\t");
//     Serial.print("Humidity:");
//     Serial.print(humiditySCD41);
//     Serial.print("\t");
//     Serial.print("Co2:");
//     Serial.println(co2);

//     float temperatureAverage = (temperatureSCD41 + temperatureSen55) / 2;
//     float humidityAverage = (humiditySCD41 + humiditySen55) / 2;

//     lcd.clear();
//     lcd.setCursor(0, 0);
//     lcd.print(temperatureAverage, 1);
//     lcd.print("C");
//     lcd.print(" ");
//     lcd.print(humidityAverage, 0);
//     lcd.print("%");
//     lcd.print(" ");
//     lcd.print(noxIndex, 1);
//     lcd.print("NOx");
//     lcd.print("    ");

//     lcd.setCursor(0, 1);
//     lcd.print("C02:");
//     lcd.print(co2);
//     lcd.print(" ");
//     lcd.print("PM2:");
//     lcd.print(massConcentrationPm2p5, 1);

//     delay(10000);
//     lcd.setCursor(10, 0);
//     lcd.print(vocIndex, 0);
//     lcd.print("VOC");
//     lcd.print("    ");

//     delay(10000);
// }
