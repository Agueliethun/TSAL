#ifndef MIDISEQUENCER_H
#define MIDISEQUENCER_H

#include "Util.hpp"
#include <condition_variable>
#include <functional>
#include <vector>
#include <algorithm>

namespace tsal {

/** @class MidiSequencer
 * @brief A sequencer that handles the scheduling of events
 * 
 * MidiSequencer handles the real-time scheduling of the process
 * based upon BPM and PPQ parameters
 */
class MidiSequencer {
  public:
    void tick();
    void start();
    void stop();
    void setTick(unsigned tick);
    void setBPM(unsigned bpm);
    void setPPQ(unsigned ppq);
    unsigned getTick() const;
    void waitForTick(unsigned tick);
  private:
    std::mutex mMutex; 
    std::condition_variable mCondition;
    std::vector<unsigned> mTickEvents;
    void setSamplesPerTick();
    unsigned mSampleTime = 0;
    unsigned mTick;


    unsigned mPPQ = 240;
    unsigned mBPM = 100;
    double mSamplesPerTick;

    static Util::ParameterRange<unsigned> mBPMRange;
    static Util::ParameterRange<unsigned> mPPQRange;
};

}
#endif
