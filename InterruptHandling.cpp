#include "InterruptHandling.h"

void InterruptHandler::init() {
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