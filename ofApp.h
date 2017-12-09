#pragma once

#include "ofMain.h"
#include "Beatdetector.hpp"
#include "ofxGui.h"

class ofApp : public ofBaseApp{
    Beatdetector beat;

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
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    void audioReceived(float * input, int bufferSize, int nChannels);
    void drawFftData(float width, float height);
    void drawAverageEnergy(float width, float height);
    void drawBeatDetect(float width, float height, int bandpicker);
    void drawThreshholdLine(float width, float height);
    
    ofParameterGroup params;
    ofParameter<int>bandpicker;
    ofxPanel gui;
    
};