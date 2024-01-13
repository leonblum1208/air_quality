#include <MCUFRIEND_kbv.h>
#include <Arduino.h>
#include <SensirionI2CScd4x.h>
#include <Wire.h>


MCUFRIEND_kbv tft;
SensirionI2CScd4x scd4x;

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GRAY    0x8410

struct dataTuple {
    uint16_t co2;
    float temperature;
    float humidity;
    long timestampMicro;
};

uint16_t version = MCUFRIEND_KBV_H_;
const uint16_t ArrayLength = 100;
size_t ArrayIndex = 0;
//uint16_t prevCo2ArrayIndex = (ArrayIndex - 1 + ArrayLength) % ArrayLength;
struct dataTuple df[ArrayLength] = {0};


bool data_is_ready(){
    bool isDataReady = false;
    uint16_t error;
    error = scd4x.getDataReadyFlag(isDataReady);
    if (error) {
        Serial.print("Error trying to execute getDataReadyFlag()");
        return false;
    }
    if (!isDataReady) {
        Serial.print("Data not ready yet");
    }
    return isDataReady;
}

int read_data(uint16_t &co2, float &temperature, float &humidity){
    if (data_is_ready()){
        scd4x.readMeasurement(co2, temperature, humidity);
        return 0;
    }
    return 1;
}

int update_dataframe(){
    uint16_t co2 = 0;
    float temperature = 0.0f;
    float humidity = 0.0f;
    uint16_t error;
    Serial.println("Trying to read data.");
    error = read_data(co2, temperature, humidity);
    if (error) {
        return 1;
    }

    ArrayIndex = (ArrayIndex + 1) % ArrayLength;
    df[ArrayIndex].co2 = co2;
    df[ArrayIndex].temperature = temperature;
    df[ArrayIndex].humidity = humidity;
    return 0;
}

void setup()
{
    Serial.begin(115200);
        while (!Serial) {
        delay(100);
    }
    Wire.begin();
    scd4x.begin(Wire);
    scd4x.startPeriodicMeasurement();
    uint16_t ID = tft.readID();
    tft.reset();
    tft.begin(ID);
    tft.setRotation(3);  // "PORTRAIT", "LANDSCAPE", "PORTRAIT_REV", "LANDSCAPE_REV"
    tft.fillScreen(BLACK);
    tft.print("Waiting for first measurement...");
    while (!data_is_ready()){
        delay(5000);
    }
}


void loop()
{
    update_dataframe();

    Serial.print("Co2:");
    Serial.print(df[ArrayIndex].co2);
    Serial.print("\t");
    Serial.print("Temperature:");
    Serial.print(df[ArrayIndex].temperature);
    Serial.print("\t");
    Serial.print("Humidity:");
    Serial.println(df[ArrayIndex].humidity);

    tft.fillScreen(BLACK);
    tft.setTextSize(2);
    tft.setTextColor(WHITE);
    tft.setCursor(40, 40);
    tft.print("C02: ");
    tft.print(df[ArrayIndex].co2);
    tft.println("ppm");

    delay(20000);
}
