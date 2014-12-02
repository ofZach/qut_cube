#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){


    
    font.loadFont("frabk.ttf", 100);
    
    SM.loadSettings("displaySettings.xml");
    
#ifdef WIN32
    int windoww = GetSystemMetrics(SM_CXVIRTUALSCREEN);
    int windowh = GetSystemMetrics(SM_CYVIRTUALSCREEN);
    ofSetWindowShape(windoww, windowh);
    HWND hwnd = ofGetWin32Window();
	LONG lStyle = GetWindowLong(hwnd, GWL_STYLE);
    lStyle &= ~(WS_CAPTION | WS_THICKFRAME | WS_MINIMIZE | WS_MAXIMIZE | WS_SYSMENU);
    SetWindowLong(hwnd, GWL_STYLE, lStyle);
    LONG lExStyle = GetWindowLong(hwnd, GWL_EXSTYLE);
    lExStyle &= ~(WS_EX_DLGMODALFRAME | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE);
    SetWindowLong(hwnd, GWL_EXSTYLE, lExStyle);
    SetWindowPos(hwnd, NULL, 0,0,0,0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);
    ofSetWindowPosition(0,0);
#endif

    SM.getMyIP(ip);
    clientID = SM.whichClientAmI();
    

}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw(){

    
    string ipStr = ofToString((int)ip.b1) + "." +
    ofToString((int)ip.b2) + "." +
    ofToString((int)ip.b3) + "." +
    ofToString((int)ip.b4);
    ofSetColor(ofColor::red);
    font.drawString(ipStr, 100,200);
    
    font.drawString("client: " + ofToString(clientID), 100,350);
    
    
    //SM.drawDiagnostics();
    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

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
