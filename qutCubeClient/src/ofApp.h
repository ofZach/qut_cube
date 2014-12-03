#pragma once

#include "ofMain.h"
#include "settingsManager.h"
#include "communicationManager.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    
        void frameEvent(int newFrame);
        void sceneEvent(int newScene);
        void debugEvent(bool bBeDebug);
        void resetEvent();
    
        int frame;
        int scene;

    
    
        communicationManager CM;
        settingsManager SM;
        ofTrueTypeFont font;
        IPv4 ip;
        int clientID;
    
        ofFbo fbo;
        ofRectangle screenBounds;
    
        float missedFrameEnergy;
    
    
};
