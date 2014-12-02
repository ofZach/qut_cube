


#pragma once

#include "ofMain.h"
#include "ofxOsc.h"

class communicationManager {
    
    
    int     frame;
    bool    bReset;
    int     scene;
    
    
    ofxOscReceiver client;
    
    void setup(){
        
        client.setup(6667);
        
    }
    
    void update(){
        
        ofxOscMessage m;
        while (client.hasWaitingMessages()){
            client.getNextMessage(&m);
            string addr = m.getAddress();
            
            if (addr == "/frame"){
                frame = m.getArgAsInt32(0);
            }
            
            if (addr == "/scene"){
                scene = m.getArgAsInt32(0);
            }
            
            if (addr == "/reset"){
                bReset = true;
            }
        }
    }
    
};

