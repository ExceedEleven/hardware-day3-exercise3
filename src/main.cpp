#include <Arduino.h>
#include <Bounce2.h>
#include <HTTPClient.h>
#include "connect.h"

#define red 27
#define yellow 26
#define green 25
#define builtin 5
#define button 33
TaskHandle_t task_1 = NULL;
TaskHandle_t task_2 = NULL;
TaskHandle_t task_3 = NULL;
Bounce debouncer = Bounce();
int btn_status = 0; //0: ปิดไฟ 1: เปิดไฟ

void PLS(void* param)
{
  while (1)
  {
    vTaskDelay(5000/portTICK_PERIOD_MS);
    if (btn_status)
    {
      digitalWrite(red,LOW);
      btn_status =0;
      vTaskDelete(task_2);
    }
  }
}
void dim_LED(void* param)
{
  int i=0;
  int builtin_status = 0;
  while (1)
  {
    debouncer.update();
    if (debouncer.fell())
    {
      debouncer.update();
      if (!btn_status)
      {
        btn_status = 1;
        digitalWrite(red,HIGH);
        xTaskCreatePinnedToCore(PLS,"Count Button Pressed",10000, NULL, 1, &task_2, 1);
      }
      else if(btn_status)
      {
        btn_status = 0;
        digitalWrite(red,LOW);
        vTaskDelete(task_2);
      }
    }
    if (i ==0) builtin_status =0;
    else if (i == 255) builtin_status = 1;

    if (!builtin_status) ++i;
    else if (builtin_status) --i;

    ledcWrite(0,i);
    vTaskDelay(5/portTICK_PERIOD_MS);
  }
}
void problems(void* param)
{
  int a,b,res;
  String op,QID, message;
  while (1)
  {
    get_question(a,b,res,op,QID);
    post_answer(a,b,res,op,QID,message);
    Serial.print("a:");
    Serial.println(a);
    Serial.print("b:");
    Serial.println(b);
    Serial.print("op:");
    Serial.println(op);
    Serial.print("result:");
    Serial.println(res);
    Serial.print("questionId:");
    Serial.println(QID);
    Serial.println(message);
    vTaskDelay(5000/portTICK_PERIOD_MS);
  }
}
void setup() {

  Serial.begin(115200);
  ledcSetup(0,5000,8);
  ledcAttachPin(builtin,0);
  pinMode(red,OUTPUT);
  debouncer.attach(button, INPUT_PULLUP);
  debouncer.interval(25);
  Connect_Wifi();
  xTaskCreatePinnedToCore(dim_LED,"Built-in LED",10000, NULL, 1, &task_1, 0);
  xTaskCreatePinnedToCore(problems,"calculate the problem ",30000,NULL, 1, &task_3,1);

}


void loop() {
  // put your main code here, to run repeatedly:
}