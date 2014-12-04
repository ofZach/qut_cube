

#include "communicationManager.h"
#include "ofApp.h"

    
void communicationManager::setup(){
    client.setup(6667);
}

void communicationManager::update(){
    ofxOscMessage m;
    while (client.hasWaitingMessages()){
        client.getNextMessage(&m);
        
        string addr = m.getAddress();
        
        if (addr == "/frame"){
            appPtr->frameEvent(m.getArgAsInt32(0));
        }
        
        if (addr == "/scene"){
            appPtr->sceneEvent(m.getArgAsInt32(0));
        }
        
        if (addr == "/scale"){
            appPtr->scaleEvent(m.getArgAsFloat(0));
        }
        
        if (addr == "/reset"){
            bReset = true;
        }
        
        if (addr == "/debug"){
            bool bDebug = m.getArgAsInt32(0) == 0 ? false : true;
            appPtr->debugEvent(bDebug);
        }
    }
}
    


