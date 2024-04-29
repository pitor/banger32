#include "Oscillator.h"

void Oscillator::init() {
  for(int i = 0; i < WAVE_BUFFER_SIZE; i++) {
    owaveBufferSquare[i] = i <= WAVE_BUFFER_SIZE / 2 ? -128 : 127;
  }

  for(int i = 0; i < WAVE_BUFFER_SIZE; i++) {
    owaveBufferSine[i] = 127 * sin((2*PI*i)/WAVE_BUFFER_SIZE);
  }

  for(int i = 0; i < WAVE_BUFFER_SIZE; i++) {
    owaveBufferSaw[i] = 127 * (1.0 - (float)i * (2.0/WAVE_BUFFER_SIZE));
  }

  for(int i = 0; i < WAVE_BUFFER_SIZE; i++) {
    if(i <= WAVE_BUFFER_SIZE/2)
      // f1 = (-4/T)*t + 1
      owaveBufferTriangle[i] = 127 * ((float)i * (-4.0/WAVE_BUFFER_SIZE) + 1);
    else
      // f2 = (4/T)*t - 3
      owaveBufferTriangle[i] = 127 * ((float)i * (4.0/WAVE_BUFFER_SIZE) - 3);
  }
  
}

void Oscillator::start() {
  waveForm = TRIANGLE;
  sampleCounter = 0;
}

int8_t Oscillator::sampleValue(int millisInto) {

  //Number of samples for one period
  int samplesPerPeriod = SAMPLE_FREQUENCY / frequency;
  int currentSample = sampleCounter % samplesPerPeriod;
  int ix = (currentSample * WAVE_BUFFER_SIZE) / samplesPerPeriod;
  
  if(waveForm == SQUARE) {
      sampleCounter++;
      return owaveBufferSquare[ix];
  }

  if(waveForm == SAW) {
      sampleCounter++;
      return owaveBufferSaw[ix];
  }

  if(waveForm == SINE) {
      sampleCounter++;
      return owaveBufferSine[ix];
  }

  if(waveForm == TRIANGLE) {
      sampleCounter++;
      return owaveBufferTriangle[ix];
  }

  return 0;
}