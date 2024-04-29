#ifndef TONE_H
#define TONE_H
#include <Arduino.h>

enum WaveForm {
  SQUARE = 0,
  SAW,
  TRIANGLE,
  SINE,
};

#define SAMPLE_FREQUENCY 40000
#define WAVE_BUFFER_SIZE 200

int8_t waveBufferSquare[WAVE_BUFFER_SIZE];
int8_t waveBufferSine[WAVE_BUFFER_SIZE];
int8_t waveBufferSaw[WAVE_BUFFER_SIZE];
int8_t waveBufferTriangle[WAVE_BUFFER_SIZE];


class Tone {

  public:
    WaveForm waveForm; 
    int attackTime;
    int attackLevel;
    int decayTime;
    int decayLevel;
    int sustainTime;
    int sustainLevel;
    int relaseTime;

    double frequency = 466.16;
    volatile uint32_t sampleCounter;

    void init();
    void start();
    int8_t sampleValue(int millisInto);
};

void Tone::init() {
  for(int i = 0; i < WAVE_BUFFER_SIZE; i++) {
    waveBufferSquare[i] = i <= WAVE_BUFFER_SIZE / 2 ? -128 : 127;
  }

  for(int i = 0; i < WAVE_BUFFER_SIZE; i++) {
    waveBufferSine[i] = 127 * sin((2*PI*i)/WAVE_BUFFER_SIZE);
  }

  for(int i = 0; i < WAVE_BUFFER_SIZE; i++) {
    waveBufferSaw[i] = 127 * (1.0 - (float)i * (2.0/WAVE_BUFFER_SIZE));
  }

  for(int i = 0; i < WAVE_BUFFER_SIZE; i++) {
    if(i <= WAVE_BUFFER_SIZE/2)
      // f1 = (-4/T)*t + 1
      waveBufferTriangle[i] = 127 * ((float)i * (-4.0/WAVE_BUFFER_SIZE) + 1);
    else
      // f2 = (4/T)*t - 3
      waveBufferTriangle[i] = 127 * ((float)i * (4.0/WAVE_BUFFER_SIZE) - 3);
  }
  
}

void Tone::start() {
  waveForm = TRIANGLE;
  sampleCounter = 0;
}

int8_t Tone::sampleValue(int millisInto) {

  //Number of samples for one period
  int samplesPerPeriod = SAMPLE_FREQUENCY / frequency;
  int currentSample = sampleCounter % samplesPerPeriod;
  int ix = (currentSample * WAVE_BUFFER_SIZE) / samplesPerPeriod;
  
  if(waveForm == SQUARE) {
      sampleCounter++;
      return waveBufferSquare[ix];
  }

  if(waveForm == SAW) {
      sampleCounter++;
      return waveBufferSaw[ix];
  }

  if(waveForm == SINE) {
      sampleCounter++;
      return waveBufferSine[ix];
  }

  if(waveForm == TRIANGLE) {
      sampleCounter++;
      return waveBufferTriangle[ix];
  }

  return 0;
}

#endif