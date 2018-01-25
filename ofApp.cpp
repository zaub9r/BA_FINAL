#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    
    //O S C
    sender.setup(HOST, PORT);
    
    //M o u s e
    selectedBand = -1;
    mouseX = ofGetMouseX();
    mouseY = ofGetMouseY();
    setupSelectionBins((ofGetWidth()/1.5), (ofGetHeight()/5.5));
    
    
    
    
    
    // G e n e r a l
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    bgColor.set(255, 255, 255);
    ofSetBackgroundColor(bgColor);
    
    selectionColor.set(220, 90, 190, 100);
    blank.set(255,255,255,0);
    //ofSetBackgroundColor(0, 0, 0);
    
    ofSoundStreamSetup(0, 1, this, 44100, beat.getBufferSize(), 4);
    //soundstream.setup(this, 0, 2, 44100, beat.getBufferSize(), 4);
    //soundstream.printDeviceList();
    
    
    // G U I
    gui.setup();
    gui.setPosition((ofGetWidth() * 0.8), (ofGetHeight()* 0.8));
    //params.setName("Bandpicker");
    //params.add(bandpicker.set("BandNumber", 0, 0, FFT_SUBBANDS));
    gui.add(params);
    beat.beatParams.setName("Settings");
    gui.add(beat.beatParams);
}

//--------------------------------------------------------------
void ofApp::update(){
    beat.update();
    
    updateBands();
    updateOSC();
   // std::cout << "value: " << beat.getMappedFftValue(1)<< endl;
    

    //cout >> beat.getAverageEnergy(1) >>;
}

/////////////////////////////////////////
// O S C ///////////////////////////////
///////////////////////////////////////

void ofApp::updateOSC(){
    float spawnRateValue;
    bool triggerSpawnRate;
    float rmsValue;
    
    if (isBeat(selectedBand)) {
        spawnRateValue = (beat.getOnsetValue(selectedBand) * 100);
            }else{spawnRateValue = 0;}
            sendMessageFloat(spawnRateValue, "spawnrate");
            //std::cout << "value: " << spawnRateValue << endl;
  
    
    /*   if(beat.checkBeat(bandpicker) == true && (beat.getOnsetValue(bandpicker) > 0.05)){
                triggerSpawnRate = true;
        }else{triggerSpawnRate = false;}
    sendMessageBool(triggerSpawnRate, "trigger");
    std::cout << "value: " << triggerSpawnRate << endl;
    */
}

void ofApp::sendMessageFloat(float value, string path){
    
    ofxOscMessage m;
    m.setAddress("/particles/" + path);
    m.addFloatArg(value);
    sender.sendMessage(m, false);
}

void ofApp::sendMessageBool(bool bln, string path){
    ofxOscMessage m;
    m.setAddress("/particles/" + path);
    m.addBoolArg(bln);
    sender.sendMessage(m, false);
}

void ofApp::setupSelectionBins(float width, float height){
    float widthOfBands = width / ((float)(FFT_SUBBANDS));
    float heightOfBands = height;
    
    for (int i = 0; i < FFTSELECTIONBINS; i++) {
        bins[i].setup((widthOfBands), (heightOfBands));
    }
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
    
    buffersCounted++;

}

void ofApp::updateBands(){
    mousePressed(mouseX, mouseY, 0);
}


bool ofApp::isBeat(int subband){
    if(beat.checkBeat(subband) == true && (beat.getOnsetValue(subband) > 0.05)){
        return true;
    }else{
        return false;
    }
}




/////////////////////////////////////////
// D R A W S////////////////////////////
///////////////////////////////////////





void ofApp::drawOnsetValue(float xPos, float yPos, float width, float height, float distance){
    
    // Onset Value
    ofPushMatrix();
    
    float widthOfBands = width / ((float)(FFT_SUBBANDS));
    mappedOnsetValue = ofMap(beat.getOnsetValue(selectedBand), 0, 2.0, 0, 1);
    
    ofSetColor(0, 0, 0);
    ofDrawBitmapString("onset confidence (scale: 0 - 1): " + ofToString(mappedOnsetValue), xPos + distance + widthOfBands * (FFT_SUBBANDS/3), yPos - 5);
    
    ofNoFill();
    ofDrawRectangle(xPos + distance + widthOfBands * (FFT_SUBBANDS/3), yPos, width, height);
    ofFill();
    ofDrawRectangle(xPos + distance + widthOfBands * (FFT_SUBBANDS/3), yPos, mappedOnsetValue * 20, height);

    ofPopMatrix();

    // Timer
    float timer = ofGetElapsedTimeMillis() - startTime;
    
    if(beat.checkBeat(selectedBand) == true && (beat.getOnsetValue(selectedBand) > 0.05)){
        startTime = ofGetElapsedTimeMillis();
    }
    ofDrawBitmapString("last onset: " + ofToString(timer) + "ms", xPos + distance + widthOfBands * (FFT_SUBBANDS/3), yPos - 30);
}


void ofApp::drawThreshholdLine(float xPos, float yPos, float width, float height, float distance){
    ofPushMatrix();
    
    float widthOfBands = width / ((float)(FFT_SUBBANDS));
    
    //Tiefen
    ofSetColor(120, 25, 120);
    ofSetLineWidth(0.8);
    ofDrawLine(xPos, (yPos -(beat.threshhold *30)), xPos + (widthOfBands * (FFT_SUBBANDS/3)), (yPos -(beat.threshhold *30)));
    //Mitten
    ofDrawLine(xPos + ((FFT_SUBBANDS/3) * widthOfBands) + distance,(yPos -(beat.threshhold *30)), xPos + ((FFT_SUBBANDS/3) * widthOfBands) + (widthOfBands * (FFT_SUBBANDS/3)) + distance, (yPos -(beat.threshhold *30)));
    //Hšhen
    ofDrawLine(xPos + (((FFT_SUBBANDS/3) * 2) * widthOfBands) + (distance*2),(yPos -(beat.threshhold *30)), xPos + ((FFT_SUBBANDS * widthOfBands) ) + (distance*2), (yPos -(beat.threshhold *30)));
    
    ofPopMatrix();
}


void ofApp::drawAverageEnergy(float xPos, float yPos, float width, float height, float distance){
    ofPushMatrix();
    
    float widthOfBands = width / ((float)(FFT_SUBBANDS));
    
    //Hoehen EnergyAvg
    for (int i =  (FFT_SUBBANDS/3) * 2; i < FFT_SUBBANDS ; i++) {
        ofSetColor(255, 0, 0, 80);
        ofDrawRectangle(xPos + (i * widthOfBands) + (distance *2), yPos, widthOfBands, -((beat.getAverageEnergy(i) * beat.beatConstant[i] * 30)));
    }
    
    //Mitten EnergyAvg
    for (int i =  (FFT_SUBBANDS/3); i < (FFT_SUBBANDS/3) * 2 ; i++){
        ofSetColor(255, 0, 0, 80);
        ofDrawRectangle(xPos + (i * widthOfBands) + distance, yPos, widthOfBands, -((beat.getAverageEnergy(i) * beat.beatConstant[i] * 30)));
    }
    
    //Tiefen EnergyAvg
    for (int i =  0; i < (FFT_SUBBANDS/3); i++){
        ofSetColor(255, 0, 0, 80);
        ofDrawRectangle(xPos + (i * widthOfBands), yPos, widthOfBands, -((beat.getAverageEnergy(i) * beat.beatConstant[i] * 30)));
    }
    ofPopMatrix();
}



// F F T

void ofApp::drawSelectionBins(float xPos, float yPos,float width, float height,float distance){
    float widthOfBands = width / ((float)(FFT_SUBBANDS));
    
    //LOW's
    for (int i = 0; i < (FFTSELECTIONBINS/3); i++) {
        
        bins[i].draw(xPos + (i * widthOfBands), yPos - height);
        if (selectedBand == i) {
            bins[i].binColor.set(selectionColor);
        }else{
            bins[i].binColor.set(blank);
        }
    }
    // MID's
    for (int i =  (FFTSELECTIONBINS/3); i < (FFTSELECTIONBINS/3) * 2 ; i++){
        
        bins[i].draw(xPos + (widthOfBands * i)  + distance, yPos - height);
        if (selectedBand == i) {
            bins[i].binColor.set(selectionColor);
        }else{
            bins[i].binColor.set(blank);
            
        }
    }
    // HI'S
    for (int i =  (FFTSELECTIONBINS/3) * 2; i < FFTSELECTIONBINS ; i++){
        bins[i].draw(xPos + (i * widthOfBands) + (distance *2), yPos - height);
        if (selectedBand == i) {
            bins[i].binColor.set(selectionColor);
        }else{
            bins[i].binColor.set(blank);
        }
    }
}

void ofApp::drawFftData(float xPos, float yPos, float width, float height, float distance){
    ofPushMatrix();
    float widthOfBands = width / ((float)(FFT_SUBBANDS));
    // Hoehen
    ofDrawBitmapStringHighlight("highs:", (xPos + (distance*2) + widthOfBands * ((FFT_SUBBANDS/3) * 2)) + 5 , yPos + 25);
    // Border
    ofNoFill();
    ofSetColor(0, 0, 0);
    ofDrawRectangle(xPos + (distance*2) + widthOfBands * ((FFT_SUBBANDS/3) * 2),yPos, widthOfBands * ((FFT_SUBBANDS/3) + 1) , -height);
    ofFill();
    for (int i =  (FFT_SUBBANDS/3) * 2; i < FFT_SUBBANDS ; i++){
            ofSetColor(0, 0, 0);
            ofDrawRectangle(xPos + (i * widthOfBands) + (distance *2), yPos, widthOfBands, -(beat.getBandValue(i)*30));
    }
    
    // Mitten
    ofDrawBitmapStringHighlight("mids:", xPos + distance + widthOfBands * (FFT_SUBBANDS/3) + 5, yPos + 25);
    // Border
    ofNoFill();
    ofSetColor(0, 0, 0);
    ofDrawRectangle(xPos + distance + widthOfBands * (FFT_SUBBANDS/3), yPos, widthOfBands * (FFT_SUBBANDS/3) , -height);
    ofFill();
    for (int i =  (FFT_SUBBANDS/3); i < (FFT_SUBBANDS/3) * 2 ; i++){
        ofSetColor(0, 0, 0);
        ofDrawRectangle(xPos + (i * widthOfBands) + distance, yPos, widthOfBands, -(beat.getBandValue(i)*30));
    }
    
    // Tiefen
    ofDrawBitmapStringHighlight("lows:", xPos, yPos + 25);
    // Border
    ofNoFill();
    ofSetColor(0, 0, 0);
    //ofDrawRectangle(float x1, float y1, float width,  float height)
    ofDrawRectangle(xPos,  yPos, widthOfBands * ((FFT_SUBBANDS/3)), -height);
    ofFill();
    
    for (int i = 0 ; i < FFT_SUBBANDS/3; i++) {
        ofSetColor(0, 0, 0);
        ofDrawRectangle(xPos + (i * widthOfBands), yPos, widthOfBands, -(beat.getBandValue(i)*30));
    }
    ofPopMatrix();
}


void ofApp::drawAudioChannel(float xPos, float yPos, float width, float height){
    ofSetColor(0, 0, 0);
    ofDrawBitmapString("audiochannel:", xPos, yPos - 5);
    ofPushStyle();
    ofPushMatrix();
    
    
    float factor = (float)(width/beat.channel.size());
    
    ofNoFill();
    ofSetColor(0, 0, 0);
    ofDrawRectangle(xPos, yPos, width, height);
    
    
    
    //ofTranslate(0, 0, 0);
    
    ofBeginShape();
    ofSetLineWidth(1);
    for (unsigned int i = 0; i < beat.channel.size(); i++){
        if(i == 0){ofVertex((i + xPos), (yPos + height/2));}
        
        ofVertex((((i * factor) + xPos)), (yPos + height/2) -beat.channel[i]  * 20);
        
    }
    ofEndShape(false);
    
    ofPopMatrix();
    ofPopStyle();
    

}

void ofApp::drawRMS(float xPos, float yPos, float width, float height){
    ofSetColor(0, 0, 0);
    ofDrawBitmapString("average volume (scale: 0 - 1): " + ofToString(beat.mappedVolume), xPos, yPos - 5);
    ofDrawBitmapString("number of buffers counted: " + ofToString(buffersCounted), xPos, yPos - 20);
    
    
    ofPushStyle();
    ofPushMatrix();
    
    float factor = (float)(width/beat.channelHistory.size());
   
    ofFill();
    ofSetColor(0,0,0);
    ofBeginShape();
    
    for (unsigned int i = 0; i < beat.channelHistory.size(); i++){
        
        if( i == 0 ){
            ofVertex((i * factor + xPos), (yPos + height/2));
        }
        
        ofVertex(i * factor + xPos, (yPos + height/2) - beat.channelHistory[i] * 52);
        
        
        
        if(i == beat.channelHistory.size() -1){
            ofVertex(i * factor + xPos, (yPos + height /2));
        }
    }
    ofEndShape(false);
    
    ofPopMatrix();
    ofPopStyle();
    
    ofNoFill();
    ofSetColor(0, 0, 0);
    ofDrawRectangle(xPos, yPos, width, height);
}



void ofApp::drawOSC(int xPos,int yPos){
    string buf;
    buf = "sending osc messages to" + string(HOST) + ofToString(PORT);
    ofDrawBitmapString(buf, xPos, yPos);
}


void ofApp::drawFPS(int xPos, int yPos){
    ofDrawBitmapString("FPS: " + ofToString(ofGetFrameRate()), xPos, yPos);

}




//--------------------------------------------------------------
void ofApp::draw(){
    gui.draw();

    //
    
    
    //ofDrawLine((ofGetWidth()/2), 0, (ofGetWidth()/2), ofGetHeight());
    
    
    
    
    drawFftData((ofGetWidth()/(8.5)), (ofGetHeight()/2), (ofGetWidth()/1.5), (ofGetHeight()/5.5), 50);
    drawSelectionBins((ofGetWidth()/(8.5)), (ofGetHeight()/2),(ofGetWidth()/1.5),(ofGetHeight()/5.5), 50);
    
    drawThreshholdLine((ofGetWidth()/(8.5)), (ofGetHeight()/2),(ofGetWidth()/1.5), (ofGetHeight()/5.5), 50);
    drawAverageEnergy((ofGetWidth()/(8.5)), (ofGetHeight()/2),(ofGetWidth()/1.5), (ofGetHeight()/5.5), 50);
    drawOnsetValue((ofGetWidth()/4), (ofGetHeight()* 0.8), (ofGetWidth()/20), (ofGetWidth()/20), 50);
    drawAudioChannel(((ofGetWidth()/2) - (ofGetWidth()/4)),(ofGetHeight()/18),(ofGetWidth()/5),(ofGetHeight()/5));
    drawRMS(((ofGetWidth()/2) +  (ofGetWidth()/18)), (ofGetHeight()/18), (ofGetWidth()/5), (ofGetHeight()/5));
   // drawOSC(10, 10);
    drawFPS(10, (ofGetHeight()*0.95));

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == 'f'){
        ofToggleFullscreen();
    }

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
    for (int i = 0; i < FFTSELECTIONBINS; i++) {
        if (button == 0 && bins[i].checkInside(x, y)){
            selectedBand = i;
           // std::cout << "value: " << selectedBand << endl;
        }
    }
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


