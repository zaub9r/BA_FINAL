//
//  Beatdetector.hpp
//  BA_FIX
//
//  Created by Maximilian Deistler on 06.12.17.
//
//

#ifndef Beatdetector_hpp
#define Beatdetector_hpp


#include "ofMain.h"
#include "fft.h"

#define FFT_BINS 512
#define FFT_SUBBANDS 64
#define ENERGY_HISTORY 40


#endif /* BeatDetector_hpp */

class Beatdetector {
    fft FFT;
public:
    
    float fftSmoothed[FFT_BINS];
    float averageEnergy[FFT_SUBBANDS];
    float fftVariance[FFT_SUBBANDS];
    float fftSubbands[FFT_SUBBANDS];
    float beatConstant[FFT_SUBBANDS];
    float energyHistory[FFT_SUBBANDS][ENERGY_HISTORY];
    float mappedFftValues[FFT_SUBBANDS];
    
    
    int buffersize;
    int fftsize;
    int historyPos;
    int samplesCounted;
    
    float smoothedVolume;
    float mappedVolume;
    
    
    bool usefft;
    bool detectbeat;
    
    
    vector<float>channel;
    vector<float>channelHistory;
    
    
    
    Beatdetector();
    void enableBeatDetect(){detectbeat = true;}
    void disableBeatDetect(){detectbeat = false;}
    void audioReceived(float * input, int bufferSize, int nChannels);
    
    void updateRMS();
    void updateFft();
    void updateMappedFftValues();
    void updateMicIn(float damping);
    void update();
    void setGain(float damping, int subband);
    void findPeak(int binMin, int binMax);
    void mapFftData(int subband);
    
    
    bool checkBeat(int subband);
    
    int getBufferSize();
    
    float getBandValue(int subband);
    float getMagnitude();
    float getInFft();
    float getAverageEnergy(int subband);
    float getSmoothedVolume();
    float getOnsetValue(int subband);
    float getMappedFftValue(int subband);
    
    
    
    float * in_fft;
    float * magnitude;
    float * phase;
    float * power;
    float * audio_input;
    float * magnitude_average;
    
    ofParameterGroup beatParams;
    ofParameter<float>sensitivity;
    ofParameter<float>threshhold;
    ofParameter<float>gain;
    ofParameter<float>blocker;
    //ofParameter<float>
};