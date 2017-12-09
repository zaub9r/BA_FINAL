#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSoundStreamSetup(0, 1, this, 44100, beat.getBufferSize(), 4);
    gui.setup();
    gui.setPosition(100,100);
    params.setName("Params");
    params.add(bandpicker.set("BandNumber", 0, 0, FFT_SUBBANDS));
    gui.add(params);
    
    beat.beatParams.setName("BeatParams");
    gui.add(beat.beatParams);
}

//--------------------------------------------------------------
void ofApp::update(){
    beat.update(ofGetElapsedTimef());
    for (int i = 0 ; i < FFT_SUBBANDS; i++) {
        beat.setGain(beat.gain, i);
    }
    //cout >> beat.getBand(0) >>;
    //cout >> beat.getAverageEnergy(1) >>;
}

void ofApp::audioReceived(float* input, int bufferSize, int nChannels){
    beat.audioReceived(input, bufferSize, nChannels);
    
}


void ofApp::drawAverageEnergy(float width, float height){
    ofPushMatrix();
    
    float widthOfBands = width / ((float)(FFT_SUBBANDS));
    
    //Hoehen EnergyAvg
    for (int i =  (FFT_SUBBANDS/3) * 2; i < FFT_SUBBANDS ; i++) {
        ofSetColor(255, 0, 0);
        ofDrawRectangle(460 + (i *widthOfBands), (height + 250), widthOfBands, -((beat.getAverageEnergy(i) * beat.beatConstant[i] * beat.multiplier)) *30);
    }
    
    //Mitten EnergyAvg
    for (int i =  (FFT_SUBBANDS/3); i < (FFT_SUBBANDS/3) * 2 ; i++){
        ofSetColor(255, 0, 0);
        ofDrawRectangle(400 + (i *widthOfBands), (height + 250), widthOfBands, -((beat.getAverageEnergy(i) * beat.beatConstant[i] * beat.multiplier)) *30);
    }
    
    //Tiefen EnergyAvg
    for (int i =  0; i < (FFT_SUBBANDS/3); i++){
        ofSetColor(255, 0, 0);
        ofDrawRectangle(320 + (i *widthOfBands), (height + 250), widthOfBands, -((beat.getAverageEnergy(i) * beat.beatConstant[i] * beat.multiplier)) *30);
    }
    ofPopMatrix();
}


void ofApp::drawBeatDetect(float width, float height, int bandnumber){
    if (beat.checkBeat(bandnumber)) {
        ofSetColor(0, 255, 255);
        ofDrawRectangle(width, height, 100, 100);
    }
}


void ofApp::drawFftData(float width, float height){
    ofPushMatrix();
    
    float widthOfBands = width / ((float)(FFT_SUBBANDS));
    int totalBands = FFT_SUBBANDS;
    //int h = -height / 2.0f;
    
    ofSetColor(255, 255, 255, 255);
    // Hoehen
    ofDrawBitmapStringHighlight("Hoehen", 440 + height, ofGetHeight() - 90 );
    for (int i =  (FFT_SUBBANDS/3) * 2; i < FFT_SUBBANDS ; i++){
        if (bandpicker == i) {
            ofSetColor(105, 0, 200);
        }else{
            ofSetColor(0, 0, 0);
        }
        ofDrawRectangle(460 + (i * widthOfBands), 250 + height, widthOfBands, -(beat.getBand(i)*30));
    }
    // Mitten
    ofDrawBitmapStringHighlight("Mitten", 220 + height, ofGetHeight() - 90 );
    for (int i =  (FFT_SUBBANDS/3); i < (FFT_SUBBANDS/3) * 2 ; i++){
        if (bandpicker == i) {
            ofSetColor(105, 0, 200);
        }else{
            ofSetColor(0, 0, 0);
        }
        ofDrawRectangle(400 + (i * widthOfBands), 250 + height, widthOfBands, -(beat.getBand(i)*30));
    }
    // Tiefen
    ofDrawBitmapStringHighlight("Tiefen", 0 + height, ofGetHeight() - 90 );
    for (int i = 0 ; i < FFT_SUBBANDS/3; i++) {
        if (bandpicker == i) {
            ofSetColor(105, 0, 200);
        }else {
            ofSetColor(0, 0, 0);
        }
        ofDrawRectangle(320 + (i * widthOfBands), 250 + height, widthOfBands, -(beat.getBand(i)*30));
                        //(beat.getBand(i) * h)
    }
    
    ofPopMatrix();
}


void ofApp::drawThreshholdLine(float width, float height){
    ofPushMatrix();
    
    float widthOfBands = width / ((float)(FFT_SUBBANDS));
    
    int totalBands = FFT_SUBBANDS;
    
    
    ofSetColor(255, 150, 120);
    ofSetLineWidth(0.5);
    ofDrawLine(320, (250 + height -(beat.threshhold *30)), 350 + (widthOfBands * totalBands), (250 + height -(beat.threshhold *30)));
    ofPopMatrix();


}


//--------------------------------------------------------------
void ofApp::draw(){
    gui.draw();
    drawFftData(ofGetWidth()/2, ofGetHeight()/2);
    drawAverageEnergy(ofGetWidth()/2, ofGetHeight()/2);
    drawBeatDetect(ofGetWidth()/2, ofGetHeight()/2, bandpicker);
    drawThreshholdLine(ofGetWidth()/2, ofGetHeight()/2);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
