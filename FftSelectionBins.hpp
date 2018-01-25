//
//  FftSelectionBins.hpp
//  BA_FIX
//
//  Created by Maximilian Deistler on 25.01.18.
//
//

#ifndef FftSelectionBins_hpp
#define FftSelectionBins_hpp


#include <ofMain.h>


class FftSelectionBins {
    
public:
    void setup(float width, float height);
    void draw(int xPos, int yPos);
    //void setup(int _width, int _height);
    bool checkInside(float px, float py);
    
    float height, width, xPos, yPos;
    
    ofRectangle rectangle;
    ofColor binColor;
};

#endif /* FftSelectionBins_hpp */
