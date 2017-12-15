//
//  Beatdetector.cpp
//  BA_FIX
//
//  Created by Maximilian Deistler on 06.12.17.
//
//

#include "Beatdetector.hpp"

Beatdetector::Beatdetector() : buffersize(1024), fftsize(512){
    
    
    beatParams.add(sensitivity.set("Sensitivity", 0.0, -1.0, 1.0));
    beatParams.add(threshhold.set("Threshhold", 0.20, 0, 1.5));
    beatParams.add(gain.set("Gain", 0.0, 1.0, 2.0));
    
    
    usefft = true;
    
    int historyPos = 0;
    int fftsize = 512;
    int buffersize = 1024;
    int samplesCounted = 0;
    
    smoothedVolume = 0.0;
    mappedVolume = 0.0;
    
    enableBeatDetect();
    
    channel.assign(buffersize, 0.0);
    channelHistory.assign(400, 0.0);
    
    
    for(int i = 0; i < fftsize; i++)
        fftSmoothed[i] = 0;
    
    // bidimensional array, every subband includes 43 eHis values
    for(int i = 0; i < FFT_SUBBANDS; i++){
        for(int l = 0; l < ENERGY_HISTORY; l++){
            energyHistory[i][l] = 0;
        }
        fftSubbands[i] = 0;
        averageEnergy[i] = 0;
        fftVariance[i] = 0;
        beatConstant[i] = 0;

    }
    // Declare FFT
    audio_input = new float[buffersize];
    magnitude = new float[fftsize];
    phase = new float[fftsize];
    power = new float[fftsize];
    magnitude_average = new float[fftsize];
    
    for (int i = 0; i < fftsize; i++) {
        magnitude[i] = 0;
        phase[i] = 0;
        power[i] = 0;
        magnitude_average[i] = 0;
    }
    
}

void Beatdetector::update(int time){
    updateFft();
    updateRMS();
    updateMicIn(gain);
}

void Beatdetector::updateRMS(){
    mappedVolume = ofMap(smoothedVolume, 0, 0.50, 0, 1, true);
    channelHistory.push_back(mappedVolume);
    
    if (channelHistory.size() >= 400) {
        channelHistory.erase(channelHistory.begin(), channelHistory.begin() + 1);
    }
}


void Beatdetector::updateMicIn(float damping){
    for (int i = 0; i < FFT_SUBBANDS; i++) {
        setGain(damping, i);
    }
}

void Beatdetector::updateFft(){
    if (usefft) {
        in_fft = magnitude;
        for (int i = 0; i < fftsize; i++) {
            //take max, either smoothed value or the incomming fft
            if (fftSmoothed[i] < in_fft[i]) {
                fftSmoothed[i] = in_fft[i];
            }
            // multiply amplitude value of recieved frequency by 0.9 & let decrease
            fftSmoothed[i] *= 0.95f;
        }
        if (detectbeat) {
            //subband calculation
            for(int i = 0; i < FFT_SUBBANDS; i++) {
                fftSubbands[i] = 0;
                //fftSubbands[i] *= gain;
                //16
                for(int b = 0; b < fftsize/FFT_SUBBANDS; b++) {
                    //16 fft_size_bins = 1 Subband
                    //[i * 8 +1 ~ 8 ] -> 8
                    //take the sum with 8, subband value
                    //like from 0 - 7, 8 -> 15, 16 -> 23
                    fftSubbands[i] +=  in_fft[i*(fftsize/FFT_SUBBANDS)+b];
                }
                // fftSubbands[i] = Subband Energy
                //turn added frequency division into average value 0.625
                fftSubbands[i] = fftSubbands[i]*(float)FFT_SUBBANDS/(float)fftsize;
                
                //calculation of subband value
                for(int b=0; b < fftsize/FFT_SUBBANDS; b++) {
                    fftVariance[i] += pow(in_fft[i*(fftsize/FFT_SUBBANDS)+b] - fftSubbands[i], 2);
                }
                fftVariance[i] = fftVariance[i]*(float)FFT_SUBBANDS/(float)fftsize;
                
                //change fixed number c dynamically acording to dispersion value
                //c becomes coefficient of threshhold of detection
                
                // C -> Rap/Techno 1.4, Rock 1.1
                beatConstant[i] = (-0.0025714*fftVariance[i])+1.5142857;
            }
            
            //calculate energy average
            for(int i = 0; i < FFT_SUBBANDS; i++) {
                averageEnergy[i] = 0;
                
                for(int h = 0; h < ENERGY_HISTORY; h++) {
                    //entire energy average += each subband energy history
                    averageEnergy[i] += energyHistory[i][h];
                }
                //divide total sum with history count, change to weighted average
                averageEnergy[i] /= ENERGY_HISTORY;
            }
            for(int i = 0; i < FFT_SUBBANDS; i++) {
                //add calculated subband energy to position of historyPos of energyHistory
                energyHistory[i][historyPos] = fftSubbands[i] *gain;
            }
            // % = modulo -> Rest
            // representing pseudo enviroment list by looping subscript of arrangement
            historyPos = (historyPos+1) % ENERGY_HISTORY;
        }
    }
}

bool Beatdetector::checkBeat(int subband){
    if(fftSubbands[subband] * gain > (averageEnergy[subband] * beatConstant[subband] + (averageEnergy[subband] *sensitivity) ) && (fftSubbands[subband] > threshhold)){
        return true;
    }
}

//void Beatdetector::getAverageEnergyTotal(int SubbandStart, int SubbandEnd){}



void Beatdetector::setGain(float damping, int subband){
    fftSubbands[subband] *= damping;
}

float Beatdetector::getSmoothedVolume(){
    return smoothedVolume;
}

float Beatdetector::getAverageEnergy(int subband){
    return averageEnergy[subband];
}

float Beatdetector::getBand(int subband){
    return fftSubbands[subband];
}

float Beatdetector::getMagnitude(){
    return * magnitude;
}

float Beatdetector::getOnsetValue(int subband){
    float onsetValue = 0.0;
    if (fftSubbands[subband] * gain > (averageEnergy[subband] * beatConstant[subband] + (averageEnergy[subband] *sensitivity) ) && (fftSubbands[subband] > threshhold)){
        onsetValue = ((fftSubbands[subband] * gain) - (averageEnergy[subband] * beatConstant[subband] + (averageEnergy[subband] *sensitivity)));
    }
    return onsetValue;
}


int Beatdetector::getBufferSize(){
    return buffersize;
}

void Beatdetector::audioReceived(float* input, int bufferSize, int nChannels) {
    
    //copy *input to audio_input
    memcpy(audio_input, input, sizeof(float) * bufferSize);
    float avg_power = 0.0f;
    //analyse spectrum with fft class, becomes fft amplitude based on magnitude
    FFT.powerSpectrum(0, (int)fftsize, audio_input, buffersize, magnitude, phase, power, &avg_power);
    
    for (int i = 0; i < fftsize; i++) {
        // square root / magnitude was too intense so ^1/2
        magnitude[i] = powf(magnitude[i], 0.5);
    }
    
        //calculation of average fft amplitude value
    for (int i = 0; i < fftsize; i++) {
        float x = 0.01;
        magnitude_average[i] = (magnitude[i] * x) + (magnitude_average[i] * (1 - x));
    }
}





