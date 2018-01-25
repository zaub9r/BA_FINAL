//
//  FftSelectionBins.cpp
//  BA_FIX
//
//  Created by Maximilian Deistler on 25.01.18.
//
//

#include "FftSelectionBins.hpp"

/*void FftSelectionBins::setup(int _width, int _height){
    width = _width;
    height = _height;
}

*/
void FftSelectionBins::setup(float _width, float _height){
    width = _width;
    height = _height;
    binColor.set(255, 255, 255, 0);
}


void FftSelectionBins::draw(int _xPos, int _yPos){
    xPos = _xPos;
    yPos = _yPos;
    ofSetColor(binColor);
    //ofNoFill();
    rectangle.set(xPos, yPos, width, height);
    ofDrawRectangle(rectangle);
}


bool FftSelectionBins::checkInside(float px, float py){
    if( px > rectangle.x && py > rectangle.y && px < rectangle.x + rectangle.width && py < rectangle.y + rectangle.height ){
        return true;
    }
    return false;
}

/*
 bool RectClass::inside(float px, float py){
 }
 */