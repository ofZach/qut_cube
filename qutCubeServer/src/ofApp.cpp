#include "ofApp.h"
#include "ofxOsc.h"

ofxOscSender sender[12];

//--------------------------------------------------------------
void ofApp::setup(){

    for (int i = 0; i < 12; i++){
         sender[i].setup("172.21.4." + ofToString(i + 11), 6667);
    }
   
}

//--------------------------------------------------------------
void ofApp::update(){

    
    ofxOscMessage m;
    m.setAddress("/frame");
    m.addIntArg(ofGetFrameNum());
    for (int i = 0; i < 12; i++){
        sender[i].sendMessage(m);
    }
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
