#include <Wire.h>
#include <Adafruit_INA219.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(6, 7); // RX, TX
#include <SPI.h>
#include <LiquidCrystal_I2C.h>
#include "EmonLib.h"
// desenho do quadrado no m²
byte dois_q[8] = {
    0b01100,
    0b10010,
    0b00100,
    0b01000,
    0b11110,
    0b00000,
    0b00000,
    0b00000};
LiquidCrystal_I2C lcd(0x27, 16, 2);
String co;
String in;
unsigned long looptime = 0;
unsigned long time1 = 0;
unsigned long time2 = 0;
int minutos = 0;
Adafruit_INA219 ina219;
EnergyMonitor emon1;
int pino_sct = A0;
float current_mA = 0;
float solar_wm2 = 0;
float temp = 0;
void setup(void)
{
    Serial.begin(9600);
    while (!Serial)
    {
    }
    Wire.begin();
    lcd.init();
    // lcd.begin();
    lcd.createChar(2, dois_q);
    lcd.clear();
    lcd.backlight();
    mySerial.begin(115200);
    uint32_t currentFrequency;
    if (!ina219.begin())
    {
        Serial.println("Failed to find INA219 chip");
        while (1)
        {
            delay(10);
        }
    }
    ina219.setCalibration_16V_400mA();
    emon1.current(0, 10.6); // calibração do sensor de corrente
}
void loop(void)
{
    // começa a contar o tempo e fazer os calculos
    looptime = millis();
    double Irms = emon1.calcIrms(1480);
    current_mA = ina219.getCurrent_mA();
    solar_wm2 = 6.923130768114198 * current_mA + 19.12791886444639;
    co = String(Irms, 2);
    in = String(solar_wm2);
    co.replace(".", ",");
    in.replace(".", ",");
    Serial.print(" ");
    Serial.print(co);
    Serial.println(" A");
    Serial.print(" ");
    Serial.print(in);
    Serial.println("W/m2");
    Serial.print(" ");
    Serial.print(current_mA);
    Serial.println("mA");
    // manda para o lcd
    if (looptime - time1 >= 2000)
    {
        lcd.clear();
        lcd.setBacklight(HIGH);
        time1 = looptime;
        lcd.setCursor(0, 0);
        lcd.print(in);
        lcd.setCursor(7, 0);
        lcd.print("W/m");
        lcd.setCursor(10, 0);
        lcd.write((byte)2);
        lcd.setCursor(0, 1);
        lcd.print(co);
        lcd.setCursor(6, 1);
        lcd.print("A");
    }
    // manda para o esp na serial
    if (looptime - time2 >= 60000)
    {
        time2 = looptime;
        mySerial.print("/exec?corrente=" + co + "&irradiancia=" + in);
    }
}