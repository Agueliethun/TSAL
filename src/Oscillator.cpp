#include "Oscillator.h"
#include "MidiNotes.h"
#include <iostream>

namespace tsal {

Oscillator::Oscillator() {
  setMode(SINE);
  setGain(0.5);
  setNote(tsal::C4);
}

// Helpful implementation of ployBLEP to reduce aliasing
// http://metafunction.co.uk/all-about-digital-oscillators-part-2-blits-bleps/
double Oscillator::getOutput() {
  // If not active, just play any sound
  if (!mActive) {
    return 0.0;
  }
 
  double value = 0.0;
  double t = mPhase / (M_PI * 2);

  switch (mMode) {
    case SINE:
      value = mSine.getWaveformSample(mPhase);
      break;
    case SAW:
      value = mSaw.getWaveformSample(mPhase);
      value -= polyBLEP(t); // Layer output of Poly BLEP on top
      break;
    case SQUARE:
      value = mSquare.getWaveformSample(mPhase);
      value += polyBLEP(t); // Layer output of Poly BLEP on top (flip)
      value -= polyBLEP(fmod(t + 0.5, 1.0)); // Layer output of Poly BLEP on top (flop)
      break;
    case CUSTOM:
      value = mCustomWaveform.getWaveformSample(mPhase);
      break;
  }

  mPhase += mPhaseStep;  
  if (mPhase >= mPI2)
    mPhase = 0;
  return value * SCALE * mGain;
}

/**
 * @brief Get the note from a corresponding frequency
 * 
 * @param frequency 
 * @return unsigned 
 */
unsigned Oscillator::getNoteFromFrequency(double frequency) {
  return (12/log(2)) * log(frequency/27.5) + 21;
}

/**
 * @brief Get the frequency from a corresponding note
 * 
 * @param note 
 * @return double 
 */
double Oscillator::getFrequencyFromNote(unsigned note) {
  return 27.5 * pow(2.0, (note - 21) / 12.0);
};

double Oscillator::polyBLEP(double t)
{
  double dt = mPhaseStep / mPI2;

  // t-t^2/2 +1/2
  // 0 < t <= 1
  // discontinuities between 0 & 1
  if (t < dt) {
   t /= dt;
   return t + t - t * t - 1.0;
  }

  // t^2/2 +t +1/2
  // -1 <= t <= 0
  // discontinuities between -1 & 0
  else if (t > 1.0 - dt) {
   t = (t - 1.0) / dt;
   return t * t + t + t + 1.0;
  }

  // no discontinuities 
  // 0 otherwise
  else return 0.0;
}

/** 
 * @brief Set a custom waveform for the oscillator
 * 
 * Given a Waveform class, the oscillator will automatically switch to CUSTOM mode and 
 * start sampling from the new waveform.
 * 
 * @param waveform a Waveform class that implements the getWaveformSample method
 */ 
void Oscillator::setWaveform(Waveform waveform) {
  mCustomWaveform = waveform;
  setMode(CUSTOM);
}

/**
 * @brief Set the note 
 * 
 * @param note (midi format)
 */
void Oscillator::setNote(unsigned note) {
  setFrequency(getFrequencyFromNote(note));
}

/**
 * @brief Set the frequncy
 * 
 * @param frequency 
 */
void Oscillator::setFrequency(double frequency) {
  mFrequency = frequency;
  mPhaseStep = mFrequency * mPI2 / ((double) Mixer::getSampleRate());
}

/**
 * @brief Set the gain
 * 
 * @param gain 
 */
void Oscillator::setGain(double gain) {
  mGain = 0.5 * gain;
}

/**
 * @brief Set the mode
 * 
 * @param mode 
 */
void Oscillator::setMode(OscillatorMode mode) {
  mMode = mode;
}

/**
 * @brief Get the frequency
 * 
 * @return double 
 */
double Oscillator::getFrequency() const {
  return mFrequency;
}

/**
 * @brief Get the note
 * 
 * @return unsigned (midi)
 */
unsigned Oscillator::getNote() const {
  return getNoteFromFrequency(mFrequency);
}

/**
 * @brief Get the gain
 * 
 * @return double 
 */
double Oscillator::getGain() const {
  return mGain;
}

const double Oscillator::mPI2 = M_PI * 2;

}
