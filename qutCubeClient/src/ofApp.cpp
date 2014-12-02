#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){


    
    font.loadFont("frabk.ttf", 100);
    
    SM.loadSettings("displaySettings.xml");
    
    screenBounds.set(0,0, ofGetWidth(), ofGetHeight());
    
    
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
    
    screenBounds.set(0,0, windoww, windowh);
    
#endif
    
    

    SM.getMyIP(ip);
    clientID = SM.whichClientAmI();
    
    ofRectangle overallRect = SM.get2dResolution();
    fbo.allocate(overallRect.width, overallRect.height);
    
    
    fbo.begin();
    
    for (int i = 0; i < overallRect.width; i+=100){
        for (int j = 0; j < overallRect.height; j+=100){
            
            float pctx = ofMap(i, 0, overallRect.getWidth(), 100, 250);
            float pcty = 0;
            
            if ((j/100) % 2 == 0){
                if ((i/100) % 2 == 0) ofSetColor((int)pctx);
                else ofSetColor((int)pcty);
            } else {
                if ((i/100) % 2 == 0) ofSetColor((int)pcty);
                else ofSetColor((int)pctx);
            }
            
            //ofSetColor(ofRandom(0,255));
            ofRect(i,j, 100, 100);
        }
    }
    
    fbo.end();
    
}

//--------------------------------------------------------------
void ofApp::update(){

    
}

//--------------------------------------------------------------
void ofApp::draw(){

    
    
    ofSetColor(255);
    //fbo.draw(0,0, ofGetWidth(), ofGetHeight());
    
    
    clientSetting SC = SM.clients[clientID];
    
    int nViews = SC.nViews;
    
    
    
    for (int i = 0; i < nViews; i++){
        ofRectangle extr = SC.rects[i].exteriorPct;
        ofRectangle intr = SC.rects[i].interiorPct;
        
        float x = screenBounds.width * extr.x;
        float y = screenBounds.height * extr.y;
        float w = screenBounds.width * extr.width;
        float h = screenBounds.height * extr.height;

        float xt =  intr.x;
        float yt =  intr.y;
        float wt =  intr.width;
        float ht =  intr.height;

        ofMesh mesh;
        mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);

        mesh.addVertex( ofPoint(x,y) );
        mesh.addTexCoord( fbo.getTextureReference().getCoordFromPercent(xt, yt) );

        mesh.addVertex( ofPoint(x+w,y) );
        mesh.addTexCoord( fbo.getTextureReference().getCoordFromPercent(xt+wt, yt) );

        mesh.addVertex( ofPoint(x,y+h) );
        mesh.addTexCoord( fbo.getTextureReference().getCoordFromPercent(xt, yt+ht) );
        
        mesh.addVertex( ofPoint(x+w,y+h) );
        mesh.addTexCoord( fbo.getTextureReference().getCoordFromPercent(xt+wt, yt+ht) );
        
        fbo.getTextureReference().bind();
        mesh.draw();
        fbo.getTextureReference().unbind();
        
    }
    // which client am I
    
    
//    ofRectangle bigPixDim;
//    ofRectangle smallPixDim;
//    ofRectangle smallInWorldDim;
//    ofRectangle total;
//    
//    bigPixDim.set(0,0,1920, 1750);
//    smallPixDim.set(0, 0, 1080, 1920);
//    float aspectRatio = 1080.0 / 1920.0;
//    smallInWorldDim.set(0,0, 1920/10.0, (1/aspectRatio) * (1920/10.0));
//    total.set(0,0, 1920*2, 1750 + smallInWorldDim.getHeight());
//    
//    
//    ofScale(0.3,0.3);
//    for (int i = 0; i < SM.clients.size(); i++){
//        for (int j = 0; j < SM.clients[i].nViews; j++){
//            
//            float x = total.width * SM.clients[i].rects[j].interiorPct.x;
//            float y = total.height * SM.clients[i].rects[j].interiorPct.y;
//            float w = total.width * SM.clients[i].rects[j].interiorPct.width;
//            float h = total.height * SM.clients[i].rects[j].interiorPct.height;
//            
//            float xt =  SM.clients[i].rects[j].interiorPct.x;
//            float yt =  SM.clients[i].rects[j].interiorPct.y;
//            float wt =  SM.clients[i].rects[j].interiorPct.width;
//            float ht =  SM.clients[i].rects[j].interiorPct.height;
//            
//            ofMesh mesh;
//            mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
//            
//            mesh.addVertex( ofPoint(x,y) );
//            mesh.addTexCoord( fbo.getTextureReference().getCoordFromPercent(xt, yt) );
//            
//            mesh.addVertex( ofPoint(x+w,y) );
//            mesh.addTexCoord( fbo.getTextureReference().getCoordFromPercent(xt+wt, yt) );
//            
//            mesh.addVertex( ofPoint(x,y+h) );
//            mesh.addTexCoord( fbo.getTextureReference().getCoordFromPercent(xt, yt+ht) );
//            
//            mesh.addVertex( ofPoint(x+w,y+h) );
//            mesh.addTexCoord( fbo.getTextureReference().getCoordFromPercent(xt+wt, yt+ht) );
//            
//            
//            fbo.getTextureReference().bind();
//            mesh.draw();
//            fbo.getTextureReference().unbind();
//            //ofSetColor(100 + ofRandom(20));
//            
//            //ofRect(x,y,w,h);
//            
//            
//        }
//    }
    
    //fbo.draw(0,0);
    
    
    string ipStr = ofToString((int)ip.b1) + "." + ofToString((int)ip.b2) + "." + ofToString((int)ip.b3) + "." + ofToString((int)ip.b4);
    ofSetColor(ofColor::red);
    font.drawString(ipStr, 100,200);
    font.drawString("client: " + ofToString(clientID), 100,350);
    
    
    
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
