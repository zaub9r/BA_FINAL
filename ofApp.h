#pragma once

#include "ofMain.h"
#include "Beatdetector.hpp"
#include "ofxGui.h"
#include "ofxOsc.h"
#include "FftSelectionBins.hpp"


#define HOST "localhost"
#define PORT 12345
#define FFTSELECTIONBINS 64


class ofApp : public ofBaseApp{
    
    Beatdetector beat;
    ofxOscSender sender;
    FftSelectionBins bins[FFTSELECTIONBINS];
    
    
	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
	
    
    
    void audioReceived(float * input, int bufferSize, int nChannels);
    
    void sendMessageFloat(float value, string path);
    void sendMessageBool(bool bln, string path);
    void setupSelectionBins(float width, float height);
    
    //draws
    void drawOnsetValue(float xPos, float yPos, float width, float height, float distance);
    void drawFftData(float xPos, float yPos, float width, float height, float distance);
    void drawAverageEnergy(float xPos, float yPos, float width, float height, float distance);

    //void drawBeatDetect(float width, float height, int bandpicker);
    void drawThreshholdLine(float xPos, float yPos, float width, float height, float distance);
    void drawAudioChannel(float xPos, float yPos, float width, float height);
    void drawRMS(float xPos, float yPos, float width, float height);
    void drawOSC(int xPos,int yPos);
    void drawFPS(int xPos, int yPos);
    void drawSelectionBins(float xPos, float yPos, float width, float height, float distance );
    
    //OSC
    void updateOSC();
    void updateBands();
    
    
    //ofSoundStream soundstream;
    
    
    //Params
    ofParameterGroup params;
    //ofParameter<int>bandpicker;
    ofxPanel gui;
    
    bool isBeat(int subband);
    
    //Vals
    float startTime;
    float mappedOnsetValue;
    float mouseX, mouseY;
    
    ofColor bgColor;
    ofColor selectionColor;
    ofColor blank;
    
    int buffersCounted;
    int selectedBand;
   };
