#ifndef ESP32SYNTH_OSCILLATOR
#define ESP32SYNTH_OSCILLATOR

#include <Arduino.h>

enum WaveForm {
  SQUARE = 0,
  SAW,
  TRIANGLE,
  SINE,
};

#define SAMPLE_FREQUENCY 40000
#define WAVE_BUFFER_SIZE 200

int8_t owaveBufferSquare[WAVE_BUFFER_SIZE];
int8_t owaveBufferSine[WAVE_BUFFER_SIZE];
int8_t owaveBufferSaw[WAVE_BUFFER_SIZE];
int8_t owaveBufferTriangle[WAVE_BUFFER_SIZE];


class Oscillator {

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
#endif
