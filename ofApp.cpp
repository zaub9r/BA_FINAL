#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    //Onclick Listener
    //ofAddListener(Beatdetector().mouseMoved, this, &Beatdetector::mouseMoved);
    ofSetVerticalSync(true);
    
    
    ofSoundStreamSetup(0, 1, this, 44100, beat.getBufferSize(), 4);
    //soundstream.setup(this, 0, 2, 44100, beat.getBufferSize(), 4);
    //soundstream.printDeviceList();
    
    // G U I
    gui.setup();
    gui.setPosition(800,100);
    params.setName("Bandpicker");
    params.add(bandpicker.set("BandNumber", 0, 0, FFT_SUBBANDS));
    gui.add(params);
    beat.beatParams.setName("Settings");
    gui.add(beat.beatParams);
}

//--------------------------------------------------------------
void ofApp::update(){
    beat.update(ofGetElapsedTimef());
    //cout >> beat.getBand(0) >>;
    //cout >> beat.getAverageEnergy(1) >>;
}

void ofApp::audioReceived(float* input, int bufferSize, int nChannels){
    beat.audioReceived(input, bufferSize, nChannels);
    
    
    // RMS
    float currentVolume = 0.0;
    int samplesCounted = 0;
    
    for (int i  = 0; i < beat.buffersize; i++) {
        beat.channel[i] = input[i];
        currentVolume += beat.channel[i] * beat.channel[i];
        samplesCounted += 1;
    }
    
    currentVolume /= (float)samplesCounted;
    currentVolume = sqrt(currentVolume);
    
    beat.smoothedVolume *= 0.95;
    beat.smoothedVolume += 0.07 * currentVolume;
    

}


void ofApp::drawAverageEnergy(float width, float height, float xPos, float yPos, float distance){
    ofPushMatrix();
    
    float widthOfBands = width / ((float)(FFT_SUBBANDS));
    
    //Hoehen EnergyAvg
    for (int i =  (FFT_SUBBANDS/3) * 2; i < FFT_SUBBANDS ; i++) {
        ofSetColor(255, 0, 0, 80);
        ofDrawRectangle(xPos + (i * widthOfBands) + (distance *2), yPos, widthOfBands, -((beat.getAverageEnergy(i) * beat.beatConstant[i] + (beat.getAverageEnergy(i) * beat.sensitivity))) *30);
    }
    
    //Mitten EnergyAvg
    for (int i =  (FFT_SUBBANDS/3); i < (FFT_SUBBANDS/3) * 2 ; i++){
        ofSetColor(255, 0, 0, 80);
        ofDrawRectangle(xPos + (i * widthOfBands) + distance, yPos, widthOfBands, -((beat.getAverageEnergy(i) * beat.beatConstant[i] + (beat.getAverageEnergy(i) * beat.sensitivity))) *30);
    }
    
    //Tiefen EnergyAvg
    for (int i =  0; i < (FFT_SUBBANDS/3); i++){
        ofSetColor(255, 0, 0, 80);
        ofDrawRectangle(xPos + (i * widthOfBands), yPos, widthOfBands, -((beat.getAverageEnergy(i) * beat.beatConstant[i] + (beat.getAverageEnergy(i) * beat.sensitivity))) *30);
    }
    ofPopMatrix();
}


void ofApp::drawBeatDetect(float width, float height, int bandnumber){
    if (beat.checkBeat(bandnumber)) {
        ofFill();
        ofSetColor(0, 100, 255);
        ofDrawRectangle(width , height, 10*beat.getOnsetValue(bandnumber), 100);
    }
}

void ofApp::drawFftData(float width, float height, float xPos, float yPos, float distance){
    ofPushMatrix();
    float widthOfBands = width / ((float)(FFT_SUBBANDS));
    int totalBands = FFT_SUBBANDS;
    
    // Hoehen
    ofDrawBitmapStringHighlight("Hoehen", (xPos + (distance*2) + widthOfBands * ((FFT_SUBBANDS/3) * 2)) + 5 , yPos + 25);
    // Border
    ofNoFill();
    ofSetColor(255, 255, 255);
    ofDrawRectangle(xPos + (distance*2) + widthOfBands * ((FFT_SUBBANDS/3) * 2),yPos, widthOfBands * (FFT_SUBBANDS/3) +6 , -height);
    ofFill();
   
    for (int i =  (FFT_SUBBANDS/3) * 2; i < FFT_SUBBANDS ; i++){
        if (bandpicker == i) {
            ofSetColor(105, 0, 200);
        }else{
            ofSetColor(255,255,255);
        }
        ofDrawRectangle(xPos + (i * widthOfBands) + (distance *2), yPos, widthOfBands, -(beat.getBand(i)*30));
    }
    
    // Mitten
    ofDrawBitmapStringHighlight("Mitten", xPos + distance + widthOfBands * (FFT_SUBBANDS/3) + 5, yPos + 25);
    // Border
    ofNoFill();
    ofSetColor(255, 255, 255);
    ofDrawRectangle(xPos + distance + widthOfBands * (FFT_SUBBANDS/3), yPos, widthOfBands * (FFT_SUBBANDS/3) , -height);
    ofFill();
    for (int i =  (FFT_SUBBANDS/3); i < (FFT_SUBBANDS/3) * 2 ; i++){
        if (bandpicker == i) {
            ofSetColor(105, 0, 200);
        }else{
            ofSetColor(255, 255, 255);
        }
        ofDrawRectangle(xPos + (i * widthOfBands) + distance, yPos, widthOfBands, -(beat.getBand(i)*30));
    }
    
    // Tiefen
    ofDrawBitmapStringHighlight("Tiefen", xPos, yPos + 25);
    // Border
    ofNoFill();
    ofSetColor(255, 255, 255);
    //ofDrawRectangle(float x1, float y1, float width,  float height)
    ofDrawRectangle(xPos,  yPos, widthOfBands * ((FFT_SUBBANDS/3)), -height);
    ofFill();
    
        for (int i = 0 ; i < FFT_SUBBANDS/3; i++) {
        if (bandpicker == i) {
            ofSetColor(105, 0, 200);
        }else {
            ofSetColor(255, 255, 255);
        }
        ofDrawRectangle(xPos + (i * widthOfBands), yPos, widthOfBands, -(beat.getBand(i)*30));
    }
    ofPopMatrix();
}


void ofApp::drawThreshholdLine(float width, float height, float xPos, float yPos, float distance){
    ofPushMatrix();
    
    float widthOfBands = width / ((float)(FFT_SUBBANDS));
    
    //Tiefen
    ofSetColor(120, 0, 120);
    ofSetLineWidth(0.5);
    ofDrawLine(xPos, (yPos -(beat.threshhold *30)), xPos + (widthOfBands * (FFT_SUBBANDS/3)), (yPos -(beat.threshhold *30)));
    //Mitten
    ofDrawLine(xPos + ((FFT_SUBBANDS/3) * widthOfBands) + distance,(yPos -(beat.threshhold *30)), xPos + ((FFT_SUBBANDS/3) * widthOfBands) + (widthOfBands * (FFT_SUBBANDS/3)) + distance, (yPos -(beat.threshhold *30)));
    //Hšhen
    ofDrawLine(xPos + (((FFT_SUBBANDS/3) * 2) * widthOfBands) + (distance*2),(yPos -(beat.threshhold *30)), xPos + ((FFT_SUBBANDS * widthOfBands) ) + (distance*2), (yPos -(beat.threshhold *30)));
    
    ofPopMatrix();
}

void ofApp::drawAudioChannel(float width, float height, float xPos, float yPos){
    ofSetColor(255, 255, 255);
    ofDrawBitmapString("Audiochannel", xPos, yPos - 5);
    ofPushStyle();
    ofPushMatrix();
    
    //Rectanglesize/SampleAnzahl
    
    float factor = (float)(width/beat.channel.size());
    
    ofNoFill();
    ofSetColor(250, 250, 250);
    ofDrawRectangle(xPos, yPos, width, height);
    
    
    
    //ofTranslate(0, 0, 0);
    ofBeginShape();
    ofSetLineWidth(1);
    for (unsigned int i = 0; i < beat.channel.size(); i++){
        if(i == 0){ofVertex((i + xPos), (yPos + height/2));}
        
        ofVertex((((i * factor) + xPos)), (yPos + height/2) -beat.channel[i]  * 25);
        
    }
    ofEndShape(false);
    
    ofPopMatrix();
    ofPopStyle();


}

void ofApp::drawRMS(float width, float height, float xPos, float yPos){
    ofSetColor(255, 255, 255);
    ofDrawBitmapString("RMS Value (Scale: 0 - 1): " + ofToString(beat.mappedVolume), xPos, yPos - 5);
    
    ofPushStyle();
    ofPushMatrix();
    
    float factor = (float)(width/beat.channelHistory.size());
   
    ofFill();
    ofSetColor(230, 255, 255);
    ofBeginShape();
    
    for (unsigned int i = 0; i < beat.channelHistory.size(); i++){
        if( i == 0 ){ofVertex((i * factor + xPos), (yPos + height/2));}
        
        ofVertex(i * factor + xPos, (yPos + height/2) - beat.channelHistory[i] * 25);
        
        if(i == beat.channelHistory.size() -1){ofVertex(i * factor + xPos, (yPos + height /2));}
    }
    ofEndShape(false);
    
    ofPopMatrix();
    ofPopStyle();
    
    ofNoFill();
    ofSetColor(255, 255, 255, 255);
    ofDrawRectangle(xPos, yPos, width, height);
}


//--------------------------------------------------------------
void ofApp::draw(){
    gui.draw();
    //float width, float height, float xPos, float yPos, float distance)
    drawFftData((ofGetWidth()/1.5), (ofGetHeight()/5), (ofGetWidth()/20), ofGetHeight() * 0.8 , 50);
    drawThreshholdLine((ofGetWidth()/1.5), (ofGetHeight()/5), (ofGetWidth()/20), ofGetHeight() * 0.8 , 50);
    drawAverageEnergy((ofGetWidth()/1.5), (ofGetHeight()/5), (ofGetWidth()/20), ofGetHeight() * 0.8 , 50);
    
    
    //float width, float height, float xPos, float yPos
    drawAudioChannel((ofGetWidth()/5),(ofGetHeight()/5),(ofGetWidth()/20),ofGetHeight()/3);
    
    //drawRMS(float width, float height, float xPos, float yPos)
    drawRMS(ofGetWidth()/5, (ofGetHeight()/5), (ofGetWidth()/20), ofGetHeight()/20);
    drawBeatDetect(ofGetWidth()/2, ofGetHeight()/2, bandpicker);
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
