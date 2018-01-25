//
//  OSCOutput.cpp
//  BA_FIX
//
//  Created by Maximilian Deistler on 20.01.18.
//
//

#include "OSCOutput.hpp"



void OSCOutput::setup(){
    // open an outgoing connection to HOST:PORT
    sender.setup(HOST, PORT);
}


void OSCOutput::draw(){

    

}

void OSCOutput::sendMessageFloat(float value, string path){

        ofxOscMessage m;
        m.setAddress("/particles/" + path);
        m.addFloatArg(value);
        sender.sendMessage(m, false);
    }
