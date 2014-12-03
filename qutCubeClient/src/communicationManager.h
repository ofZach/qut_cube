


#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

class ofApp; // forward declaration

class communicationManager {
    
public: 
    int     frame;
    bool    bReset;
    int     scene;

    ofxOscReceiver client;
    
    void setup();
    void update();
    
    ofApp * appPtr;
    
};

