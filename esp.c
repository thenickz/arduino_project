#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
//----------------------------------------Host & httpsPort
const char *host = "script.google.com";
const int httpsPort = 443;
//----------------------------------------
WiFiClientSecure client;
// id do script no google planilhas
String GAS_ID = "1R84CMVBQcQy2iNMFVgLcduxqTEesFAf8ukH2vDRDd"; //--> spreadsheet script ID
#define ON_Board_LED 2
const char *ssid = "nickM32";          // seu wifi
const char *password = "senhanicolau"; // sua senha
WiFiServer server(80);
#include <SoftwareSerial.h>
SoftwareSerial mySerial(3, 1); // RX, TX
String dado;
void setup()
{
    Serial.begin(115200);
    pinMode(ON_Board_LED, OUTPUT);
    digitalWrite(ON_Board_LED, HIGH);
    // conectar no wifi
    while (!Serial)
    {
    }
    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    Serial.print("Connecting");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        digitalWrite(ON_Board_LED, LOW);
        delay(250);
        digitalWrite(ON_Board_LED, HIGH);
        delay(250);
    }
    Serial.println("");
    Serial.println("WiFi connected");
    server.begin();
    Serial.println("Server started");
    Serial.println(WiFi.localIP());
    delay(10);
    Serial.println("ok");
    client.setInsecure();
}
void loop()
{
    // recebe o dado pelo url mandado do arduino uno
    digitalWrite(ON_Board_LED, HIGH);
    if (Serial.available() > 0)
    {
        dado = Serial.readString();
        dado.trim();
        String url = "/macros/s/" + GAS_ID + dado;
        Serial.println(url);
        if (!client.connect(host, httpsPort))
        {
            Serial.println("connection failed");
            return;
        }
        Serial.print("request ing URL: ");
        client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                     "Host: " + host + "\r\n" +
                     "User-Agent: BuildFailureDetectorESP8266\r\n" +
                     "Connection: close\r\n\r\n");
        digitalWrite(ON_Board_LED, LOW);
        Serial.println("request sent");
    }
}
