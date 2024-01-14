#include <MCUFRIEND_kbv.h>
#include <Arduino.h>
#include <SensirionI2CScd4x.h>
#include <SensirionI2CSen5x.h>
#include <Wire.h>
#include "displays/display.h"

SensirionI2CScd4x scd4x;
SensirionI2CSen5x sen5x;
MCUFRIEND_kbv tft;


uint16_t version = MCUFRIEND_KBV_H_;
const uint16_t ArrayLength = 100;
size_t ArrayIndex = 0;
// uint16_t prevCo2ArrayIndex = (ArrayIndex - 1 + ArrayLength) % ArrayLength;
struct airQualityDataPoint df[ArrayLength] = {0};

bool dataIsReady()
{
    bool isDataReadySCD41 = false;
    uint16_t errorSCD41;
    bool isDataReadySen55 = false;
    uint16_t errorSen55;
    errorSCD41 = scd4x.getDataReadyFlag(isDataReadySCD41);
    errorSen55 = sen5x.readDataReady(isDataReadySen55);
    if (errorSCD41 || errorSen55)
    {
        Serial.print("Error trying to retrieve data ready flag: ");
        Serial.print("errorSCD41: ");
        Serial.print(errorSCD41);
        Serial.print(" errorSen55: ");
        Serial.println(errorSen55);
        return false;
    }
    return isDataReadySen55 && isDataReadySCD41;
}

int read_data(airQualityDataPoint &tuple)
{
    if (!dataIsReady())
    {
        return 1;
    }
    // SCD41 variables
    uint16_t co2;
    float temperatureSCD41;
    float humiditySCD41;
    uint16_t errorSCD41;

    // Sen55 variables
    float massConcentrationPm1p0;
    float massConcentrationPm2p5;
    float massConcentrationPm4p0;
    float massConcentrationPm10p0;
    float humiditySen55;
    float temperatureSen55;
    float vocIndex;
    float noxIndex;
    uint16_t errorSen55;

    errorSen55 = sen5x.readMeasuredValues(
        massConcentrationPm1p0, massConcentrationPm2p5, massConcentrationPm4p0,
        massConcentrationPm10p0, humiditySen55, temperatureSen55, vocIndex,
        noxIndex);
    errorSCD41 = scd4x.readMeasurement(co2, temperatureSCD41, humiditySCD41);
    if (errorSCD41 || errorSen55)
    {
        Serial.print("Error trying to read measurements from sensors: ");
        Serial.print("errorSCD41: ");
        Serial.print(errorSCD41);
        Serial.println("errorSen55: ");
        Serial.print(errorSen55);
        return 2;
    }

    tuple.co2 = co2;
    tuple.temperature = (temperatureSCD41 + temperatureSen55) / 2;
    tuple.humidity = (humiditySCD41 + humiditySen55) / 2;
    tuple.massConcentrationPm1p0 = massConcentrationPm1p0;
    tuple.massConcentrationPm2p5 = massConcentrationPm2p5;
    tuple.massConcentrationPm4p0 = massConcentrationPm4p0;
    tuple.massConcentrationPm10p0 = massConcentrationPm10p0;
    tuple.vocIndex = vocIndex;
    tuple.noxIndex = noxIndex;

    return 0;
}

int updateairQualityDataPoint(airQualityDataPoint &tuple)
{
    uint16_t error;
    error = read_data(tuple);
    if (error)
    {
        return 1;
    }
    return 0;
}

void setup()
{
    Serial.begin(115200);
    while (!Serial)
    {
        delay(100);
    }
    Wire.begin();

    scd4x.begin(Wire);
    scd4x.startPeriodicMeasurement();

    sen5x.begin(Wire);
    sen5x.deviceReset();
    sen5x.startMeasurement();

    tft = initLCD(tft, 3, BLACK, WHITE);

    while (!dataIsReady())
    {
        delay(5000);
    }
}

void loop()
{
    airQualityDataPoint tuple = df[ArrayIndex];
    uint16_t error;
    error = updateairQualityDataPoint(tuple);
    if (error){
        delay(5000);
    }
    df[ArrayIndex] = tuple;
    ArrayIndex = (ArrayIndex + 1) % ArrayLength;

    Serial.print("Co2:");
    Serial.print(tuple.co2);
    Serial.print("\t");
    Serial.print("Temperature:");
    Serial.print(tuple.temperature);
    Serial.print("\t");
    Serial.print("Humidity:");
    Serial.println(tuple.humidity);

    printAirQualityDataPoint(tft, tuple, BLACK, WHITE);

    delay(20000);
}
