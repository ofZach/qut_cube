#pragma once

#include "ofMain.h"
#include "settingsManager.h"
#include "communicationManager.h"

//------------------------------ zoom draw
#include "linesWithScale.h"
#include "dataIO.h"
#include "ofxPanZoom.h"
#include "lineBatcher.h"


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
    
    
    
    void scaleEvent(float _scale){
        scale = _scale;
    }
    
    void projectorEvent(float overlapPct){
        
        SM.clients[0].rects[0].interiorPct.height = overlapPct;
        
        SM.clients[0].rects[1].interiorPct.y = 0.8367880727 - overlapPct;
        SM.clients[0].rects[1].interiorPct.height = overlapPct;
        
        //scale = _scale;
    }

    
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
    
        void * window;
    
    
    //--------------------
    
    ofImage ink;
    lineBatcher LB;
    lineBatcher LB_drawnLine;       // if we have a currently drawn line, let's drawn on top of FBO
    float scale;
    float logScale;
    float logScalePrevFrame;
    float scaleChange;
    vector < linesWithScale > currentLines;
    ofPoint halfWindow;
    ofPoint     halfWindowCatch;
    bool    bZoomedSinceDrawStart;
    bool    bEnableZoom;
    bool    bIsRetina;
    ofFbo lineFbo;
    ofFbo       wholeScreen;
    ofFbo       smallFbo;
    ofImage     smallScreenshot;
    ofVbo       meshy;
    ofMesh      mesh;
    ofxPanZoom  cam;
    dataIO dat;
    
    
};
