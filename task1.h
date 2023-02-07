#include <Arduino.h>
#include <Bounce2.h>

#define red 27
#define yellow 26
#define green 25
#define builtin 5
#define button 33
TaskHandle_t task_a = NULL;
TaskHandle_t task_b = NULL;
Bounce debouncer = Bounce();

void dim_LED(void* param)
{
  int i=0;
  int status = 0;
  while (1)
  {
    if (i ==0) status =0;
    else if (i == 255) status = 1;

    if (!status) ++i;
    else if (status) --i;

    ledcWrite(0,i);
    vTaskDelay(5/portTICK_PERIOD_MS);
  }
}
void count_btn(void* param)
{
  int i = 0;
  while (1)
  {
    debouncer.update();
    if (debouncer.fell())
    {
      ++i;
      Serial.println(i);
    }
  }
}
void setup() {

  Serial.begin(115200);
  ledcSetup(0,5000,8);
  ledcAttachPin(builtin,0);

  debouncer.attach(button, INPUT_PULLUP);
  debouncer.interval(25);

  xTaskCreatePinnedToCore(dim_LED,"Built-in LED",1000, NULL, 1, &task_a, 0);
  xTaskCreatePinnedToCore(count_btn,"Count Button Pressed",1000, NULL, 1, &task_b, 1);
}


void loop() {
  // put your main code here, to run repeatedly:
}