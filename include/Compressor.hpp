#include "Effect.h"
#include "Buffer.h"
#include "Util.h"


#ifndef COMPRESSOR_H
#define COMPRESSOR_H

/* I still don't know if buffer size should be sample rate dependent
 * If it turns out to be the case then dynamically allocate things 
 * Similar to what is done in Delay
 */
#define COMPRESSOR_MAX_BUFFER 512

namespace tsal {

/** @class Compressor
 * @brief An audio compressor
 * 
 * A compressor used to reduce clipping. Essentially, if the sound routed through the 
 * compressor is louder than the compressors threshold, it will reduce the loudness.
 * Implementation adapted from https://christianfloisand.wordpress.com/2014/06/09/dynamics-processing-compressorlimiter-part-1/
 */
class Compressor : public Effect {
  public:
    Compressor();
    virtual double getOutput() override;
    void setAttackTime(double attackTime);
    void setReleaseTime(double releaseTime);
    void setThreshold(double threshold);
    void setRatio(double ratio);
    void setPreGain(double preGain);
    void setPostGain(double postGain);

    static double ampToDb(double amplitude);
    static double dbToAmp(double db);

  private:
    void filterAudio();
    void getEnvelope();
    void calculateSlope();
    double mEnvelopeSample = 0.0;

    // Figure what an optimal buffer size would be 
    Buffer<double> mBuffer;
    Buffer<double> mEnvelope;
    unsigned mCurrentSample = COMPRESSOR_MAX_BUFFER;
      
    double mSlope = 0.0;
    double mGain = 0.0;
    double mAttackGain = 0.0;
    double mReleaseGain = 0.0;

    double mThreshold = 80.0;
    double mRatio = 2.0;
    double mPreGain = 0.0;
    double mPostGain = 0.0;

    static ParameterRange<double> mAttackTimeRange;
    static ParameterRange<double> mReleaseTimeRange;
    static ParameterRange<double> mThresholdRange;
    static ParameterRange<double> mRatioRange;
    static ParameterRange<double> mGainRange;
};

}
#endif