#include "ofApp.h"
#include "ofxOsc.h"

ofxOscSender sender[12];

//--------------------------------------------------------------
void ofApp::setup(){

    for (int i = 0; i < 12; i++){
         sender[i].setup("172.21.4." + ofToString(i + 11), 6667);
    }
    
    ofSetFrameRate(24);
   
}

float scaleSmooth = 0;
//--------------------------------------------------------------
void ofApp::update(){

    
    ofxOscMessage m;
    m.setAddress("/frame");
    m.addIntArg(ofGetFrameNum());
    for (int i = 0; i < 12; i++){
        sender[i].sendMessage(m);
    }
    
    
    
    scaleSmooth = 0.99f * scaleSmooth + 0.01 * ofMap(mouseX, 0,ofGetWidth(), -1, 35, true);
    
    cout << powf(10, scaleSmooth) << endl;
    
    
    
//    ofxOscMessage m2;
//    m2.setAddress("/scale");
//    m2.addFloatArg(powf(10, scaleSmooth));
//    for (int i = 0; i < 12; i++){
//        sender[i].sendMessage(m2);
//    }

    ofxOscMessage m2;
    m2.setAddress("/projector");
    m2.addFloatArg( ofMap(mouseX, 0, ofGetWidth(), 0, 1));
    for (int i = 0; i < 12; i++){
        sender[i].sendMessage(m2);
    }

    
    
    //cout << scale << endl;
    
    
}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

    if (key == 'd'){
        ofxOscMessage m;
        m.setAddress("/debug");
        m.addIntArg(0);
        for (int i = 0; i < 12; i++){
            sender[i].sendMessage(m);
        }
    } else if (key == 'D'){
        ofxOscMessage m;
        m.setAddress("/debug");
        m.addIntArg(1);
        for (int i = 0; i < 12; i++){
            sender[i].sendMessage(m);
        }
        
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

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
