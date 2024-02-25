#include <SensirionI2CScd4x.h>
#include <SensirionI2CSen5x.h>
#include "displays/display.h"

SensirionI2CScd4x scd4x;
SensirionI2CSen5x sen5x;
ThinkInk_270_Tricolor_C44 ePaper(EPD_DC, EPD_RESET, EPD_CS, SRAM_CS, EPD_BUSY);

float updateLatestAfterXMins = 15; // must be bigger then 3 min according to spec sheet
float updateAirQualityDiffPercent = 5;

long nUpdates = 0;
airQualityDataPoint currentlyDisplayedTuple = {0};
float durationSinceLastUpdateMilli = 0;
boolean updateEPaper = false;
boolean airQualityDiffBiggerThenPercent = false;
const uint16_t ArrayLength = 100;
size_t ArrayIndex = 0;
// uint16_t prevCo2ArrayIndex = (ArrayIndex - 1 + ArrayLength) % ArrayLength;
struct airQualityDataPoint df[ArrayLength] = {0};

float relativeChangeFloats(float oldF, float oldN, boolean makeAbsolute = true)
{
    float relChange = (oldN - oldF) / oldF;
    if (makeAbsolute && relChange < 0)
    {
        return relChange * (-1);
    };
    return relChange;
}
boolean airQualityDiffBiggerXPercent(float percent, airQualityDataPoint oldAqDp, airQualityDataPoint newAqDp)
{
    float maxDiff = 0;

    maxDiff = max(maxDiff, relativeChangeFloats(oldAqDp.temperature, newAqDp.temperature));
    maxDiff = max(maxDiff, relativeChangeFloats(oldAqDp.humidity, newAqDp.humidity));
    maxDiff = max(maxDiff, relativeChangeFloats(oldAqDp.co2, newAqDp.co2));
    // maxDiff = max(maxDiff, relativeChangeFloats(oldAqDp.vocIndex, newAqDp.vocIndex));
    // maxDiff = max(maxDiff, relativeChangeFloats(oldAqDp.noxIndex, newAqDp.noxIndex));
    // maxDiff = max(maxDiff, relativeChangeFloats(oldAqDp.massConcentrationPm2p5, newAqDp.massConcentrationPm2p5));
    // maxDiff = max(maxDiff, relativeChangeFloats(oldAqDp.massConcentrationPm1p0, newAqDp.massConcentrationPm1p0));
    // maxDiff = max(maxDiff, relativeChangeFloats(oldAqDp.massConcentrationPm4p0, newAqDp.massConcentrationPm4p0));
    // maxDiff = max(maxDiff, relativeChangeFloats(oldAqDp.massConcentrationPm10p0, newAqDp.massConcentrationPm10p0));
    return maxDiff > (percent / 100);
}

bool dataIsReady()
{
    bool isDataReadySCD41 = false;
    uint16_t errorSCD41;
    bool isDataReadySen55 = false;
    uint16_t errorSen55;
    errorSCD41 = scd4x.getDataReadyFlag(isDataReadySCD41);
    errorSen55 = sen5x.readDataReady(isDataReadySen55);
    Serial.print("SCD41 is ready: ");
    Serial.print(isDataReadySCD41);
    Serial.print(" SCD55 is ready: ");
    Serial.println(isDataReadySen55);

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
    tuple.timestampMilli = millis();

    return 0;
}

int updateAirQualityDataPoint(airQualityDataPoint &tuple)
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

    ePaper = initEPaper(ePaper, 2, EPD_WHITE, EPD_BLACK);

    while (!dataIsReady())
    {
        delay(5000);
    }
}

void loop()
{
    airQualityDataPoint tuple = df[ArrayIndex];
    uint16_t error;
    error = updateAirQualityDataPoint(tuple);
    if (error)
    {
        delay(5000);
    }
    df[ArrayIndex] = tuple;
    ArrayIndex = (ArrayIndex + 1) % ArrayLength;

    airQualityDiffBiggerThenPercent = airQualityDiffBiggerXPercent(updateAirQualityDiffPercent, currentlyDisplayedTuple, tuple);
    durationSinceLastUpdateMilli = tuple.timestampMilli - currentlyDisplayedTuple.timestampMilli;
    updateEPaper = airQualityDiffBiggerThenPercent || (durationSinceLastUpdateMilli > (updateLatestAfterXMins * 60 * 1000));

    Serial.print("airQualityDiffBiggerThenPercent:");
    Serial.print(airQualityDiffBiggerThenPercent);
    Serial.print("\t");
    Serial.print("durationSinceLastUpdateMilli:");
    Serial.print(durationSinceLastUpdateMilli);
    Serial.print("\t");
    Serial.print("updateEPaper:");
    Serial.println(updateEPaper);

    Serial.print("Co2:");
    Serial.print(tuple.co2);
    Serial.print("\t");
    Serial.print("Temperature:");
    Serial.print(tuple.temperature);
    Serial.print("\t");
    Serial.print("Humidity:");
    Serial.print(tuple.humidity);
    Serial.print("\t");
    Serial.print("voc:");
    Serial.print(tuple.vocIndex);
    Serial.print("\t");
    Serial.print("nox:");
    Serial.print(tuple.noxIndex);
    Serial.print("\t");
    Serial.print("PM2,5:");
    Serial.println(tuple.massConcentrationPm2p5);

    if (updateEPaper)
    {
        nUpdates++;
        ePaper.clearBuffer();
        printAirQualityDataPointEPaper(ePaper, tuple, EPD_WHITE, EPD_BLACK);
        printLastWaitingDurationEPaper(ePaper, durationSinceLastUpdateMilli, nUpdates);
        ePaper.display();
        Serial.print("Updated Screen after: ");
        Serial.println(durationSinceLastUpdateMilli);
        currentlyDisplayedTuple = tuple;
        updateEPaper = false;
    }

    Serial.println("");
    delay(60000);
}
