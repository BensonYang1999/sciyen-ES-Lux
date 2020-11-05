#include "config.h"
#include "modes.h"
#include "communication.h"

TaskHandle_t LED_UPDATE;
TaskHandle_t WIFI_HANDLE;

Effects effect = Effects();
Communication comm = Communication();
void setup(){
    Serial.begin(115200);
    Serial.println("Start up");

    comm.init();

    xTaskCreatePinnedToCore(
        WIFI_HANDLE_CODE,
        "wifi_handle",
        10000,
        NULL,
        1,
        &WIFI_HANDLE,
        0);
    delay(100);
    
    xTaskCreatePinnedToCore(
        LED_UPDATE_CODE, // Function to implement the task
        "led_update",    // Name of the task
        10000,           // Stack size in words
        NULL,            // Task input parameter
        1,               // Priority of the task
        &LED_UPDATE,     // Task handle.
        1);              // Core where the task should run
    delay(100);
}

void LED_UPDATE_CODE(void *pvParameters)
{
    ValueParam p1 = (ValueParam){.func=FuncRamp, .range=100, .lower=0, .p1=250, .p2=0};
    ValueParam p2 = (ValueParam){.func=FuncConst, .range=1, .lower=0, .p1=100, .p2=0};
    ValueParam p3 = (ValueParam){.func=FuncConst, .range=1, .lower=0, .p1=0, .p2=0};
    ValueParam p4 = (ValueParam){.func=FuncConst, .range=1, .lower=0, .p1=255, .p2=0};
    ValueParam p5 = (ValueParam){.func=FuncTri, .range=100, .lower=0, .p1=100, .p2=0};
    ValueParam p6 = (ValueParam){.func=FuncPulse, .range=100, .lower=0, .p1=5, .p2=0};
    Mode m;
    m.XH = p1;
    m.XS = p4;
    m.XV = p2;
    m.YH = p3;
    m.YS = p3;
    m.YV = p3;
    m.param[0] = 1;
    m.param[1] = 10;
    while (1){
        effect.square(&m);
    }
}

void WIFI_HANDLE_CODE(void *pvParameters)
{
    Serial.print("Task1 running on core ");
    Serial.println(xPortGetCoreID());
    while (1)
    {
        /*if (Serial.available())
        {
            light_stat = Serial.parseInt();
        }
        delay(10);*/
        //get_data();
        delay(10);
    }
}

void loop()
{
    comm.updateOTA();
    effect.update();
}
