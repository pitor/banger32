#include <Arduino.h>
#include "esp32-hal-cpu.h"
#include <driver/dac.h>
#include "tone.h"
#include <M5StickCPlus.h>

volatile int interruptCounter;
volatile bool mute = true;
int totalInterruptCounter;

#define SAMPLE_FREQUENCY 40000;
#define TONE_FREQUENCY 466.16;


void initTimer();
void IRAM_ATTR onTimer();

Tone tone2;
//#define TESTING
 
void setup() {
  M5.begin(true, false, true);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(YELLOW);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setCursor(65, 10);
  M5.Lcd.println("ESP32 Synth");
  M5.Lcd.print("CPUFreq:");
  M5.Lcd.print(getCpuFrequencyMhz());
  Serial.begin(115200);
  tone2.init();
  #ifndef TESTING
  initTimer();
  #endif
  tone2.start();
}

portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

#ifdef TESTING
void loop() {
  int8_t ix = tone.sampleValue(0);
  Serial.println(ix);
  delay(100);
}
#else
void loop() {
    portENTER_CRITICAL(&timerMux);
    int localInterruptCounter = interruptCounter;
    interruptCounter = 0;
    portEXIT_CRITICAL(&timerMux);

    M5.update();
    if(M5.BtnA.wasReleased()) {
      mute = !mute;
    }
    if(M5.BtnB.wasReleased()) {
      tone2.waveForm = (WaveForm)(((int)tone2.waveForm + 1) % 4);
    }
    Serial.println(localInterruptCounter);
    delay(100);
    
}
#endif

hw_timer_t * timer = NULL;


volatile uint8_t output = 0;

void IRAM_ATTR onTimer() {
  portENTER_CRITICAL_ISR(&timerMux);
  interruptCounter++;

  int16_t output16 = tone2.sampleValue(0) + 128;
  dac_output_voltage(DAC_CHANNEL_1, mute ? 0 : output16 & 0xff);
  
  portEXIT_CRITICAL_ISR(&timerMux);
 
}

void initTimer() {
  dac_output_enable(DAC_CHANNEL_1);
  dac_output_enable(DAC_CHANNEL_2);

  //Divide timer by 80 to get 1 million ticks pr second
  timer = timerBegin(1, 80, true);
  
  timerAttachInterrupt(timer, &onTimer, true);

  int ticksPrSecond=1000000;
  int alarmTicks = ticksPrSecond/SAMPLE_FREQUENCY;
  
  timerAlarmWrite(timer, alarmTicks, true);
  
  timerAlarmEnable(timer);
}
