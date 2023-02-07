#include <Arduino.h>
#include <ArduinoJson.h>
#include <HTTPClient.h>

extern int a,b, res;
extern String op, QID;

const char* ssid = "Jtb";
const char* password = "zlsq1912";
const String baseUrl = "https://exceed-hardware-stamp465.koyeb.app";
void Connect_Wifi(){
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi");
        while (WiFi.status() != WL_CONNECTED) {
            delay(500);
            Serial.print(".");
        }
        Serial.print("OK! IP=");
        Serial.println(WiFi.localIP());
        Serial.println("---------------------");
}

void get_question(int &a, int &b, int &res, String &op, String &QID)
{
    DynamicJsonDocument doc(65536);
    HTTPClient http;
    String QUrl = baseUrl + "/question";
    http.begin(QUrl);
    int httpResponseCode = http.GET();
    
    if (httpResponseCode >= 200 && httpResponseCode < 300)
    {
        String payload = http.getString();
        deserializeJson(doc, payload);
        a = doc["a"].as<int>();
        b = doc["b"].as<int>();
        op = doc["op"].as<String>();
        QID = doc["questionId"].as<String>();
        if (op == "+")
            res = a+b;
        else if (op == "-")
            res = a-b;
        else if (op == "*")
            res = a*b;
    }
    
}
void post_answer(int a, int b, int res, String op, String QID, String &message)
{
    DynamicJsonDocument doc(65536);
    String QUrl = baseUrl + "/answer";
    HTTPClient http;
    http.begin(QUrl);
    http.addHeader("Content-Type", "application/json");
    
    String json;
    
    doc["questionId"] = QID;
    doc["result"] =  res;
    serializeJson(doc,json);
    int httpResponseCode = http.POST(json);
    if (httpResponseCode == 200)
    {
        Serial.println("Done");
        Serial.println("");
    }
    else
    {
        Serial.print("Error ");
        Serial.println(httpResponseCode);
    }
    message = http.getString();
}