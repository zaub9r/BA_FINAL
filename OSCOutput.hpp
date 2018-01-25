//
//  OSCOutput.hpp
//  BA_FIX
//
//  Created by Maximilian Deistler on 20.01.18.
//
//

#ifndef OSCOutput_hpp
#define OSCOutput_hpp

#include "ofMain.h"
#include "ofxOsc.h"


#define HOST "localhost"
#define PORT 12345



class OSCOutput {

public:
    
    void setup();
    void draw();
    void sendMessageFloat(float value, string path);
    
    ofxOscSender sender;
    ofTrueTypeFont font;
    
};







































#endif /* OSCOutput_hpp */
