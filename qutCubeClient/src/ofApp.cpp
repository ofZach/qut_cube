#include "ofApp.h"
#include "ofAppGLFWWindow.h"


void scroll(GLFWwindow* aa,double a,double b){
    cout << "here baby " << a << " " << b << endl;
}

//--------------------------------------------------------------
void ofApp::setup(){

    
    ofAppGLFWWindow * wp = (ofAppGLFWWindow *) window;
    
    

    glfwSetScrollCallback (wp->getGLFWWindow(), scroll);
    

    font.loadFont("frabk.ttf", 100);

    SM.loadSettings("displaySettings.xml");

    screenBounds.set(0,0, ofGetWidth(), ofGetHeight());

    CM.setup();
    CM.appPtr = this;
    
    
    
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
    //ofSetWindowPosition(0,0);

    screenBounds.set(0,0, windoww, windowh);

    if (SM.whichClientAmI() < 2){
        screenBounds.set(0,0,1920, 1750);
    }

    //cout << 1920 << " " << 1750 << " " << windoww << " " << windowh << endl;
#endif



    SM.getMyIP(ip);
    clientID = SM.whichClientAmI();

    ofRectangle overallRect = SM.get2dResolution();
    fbo.allocate(overallRect.width, overallRect.height);


    fbo.begin();

    for (int i = 0; i < overallRect.width; i+=50){
        for (int j = 0; j < overallRect.height; j+=50){

            float pctx = ofMap(i, 0, overallRect.getWidth(), 100, 250);
            float pcty = 0;

            if ((j/50) % 2 == 0){
                if ((i/50) % 2 == 0) ofSetColor((int)pctx);
                else ofSetColor((int)pcty);
            } else {
                if ((i/50) % 2 == 0) ofSetColor((int)pcty);
                else ofSetColor((int)pctx);
            }

            //ofSetColor(ofRandom(0,255));
            ofRect(i,j, 50, 50);
        }
    }

    fbo.end();


    
    frame = 0;
    scene = 0;
    
    //---------------------------------- zoom draw
    
    ink.loadImage("images/ink8.jpg");
    dat.loadData("melissa2/lines.dat");
    ink.getTextureReference().bind();
    ink.getTextureReference().unbind();
    ofSetCircleResolution(80);
    scale = 1;
    logScale = log(scale);
    logScalePrevFrame = logScale;
    
    
    ofRectangle bigPixDim;
    ofRectangle smallPixDim;
    ofRectangle smallInWorldDim;
    ofRectangle total;

    bigPixDim.set(0,0,1920, 1750);
    smallPixDim.set(0, 0, 1080, 1920);
    float aspectRatio = 1080.0 / 1920.0;
    smallInWorldDim.set(0,0, 1920/10.0, (1/aspectRatio) * (1920/10.0));
    total.set(0,0, 1920*2, 1750 + smallInWorldDim.getHeight());
    
    
    halfWindow.set(total.width/2, total.height/2);
    halfWindowCatch = halfWindow;
    cam.setZoom(1.0f);
	cam.setMinZoom(1e-37);
	cam.setMaxZoom(1e+37);
	cam.setScreenSize( ofGetWidth(), ofGetHeight() ); //tell the system how large is out screen
    bZoomedSinceDrawStart = false;
    bEnableZoom = true;
    lineFbo.allocate( ofGetWidth(), ofGetHeight(), GL_RGB);
    smallFbo.allocate(640,480, GL_RGB);
    wholeScreen.allocate( ofGetWidth(), ofGetHeight(), GL_RGB);
    smallScreenshot.allocate(200, 200, OF_IMAGE_COLOR);
    for (int i= 0; i < 200*200; i++){
        smallScreenshot.setColor(i % 200, i/200, ofColor(i%200, i/200, 255));
    }
    smallScreenshot.update();
    LB.setupVbo();
    LB.allocateGPUmem();
    LB_drawnLine.setupVbo();
    LB_drawnLine.allocateGPUmem();
    ofSetVerticalSync(false);
    ofSetFrameRate(0);


}

void ofApp::frameEvent(int newFrame){
    
    if (ofGetMousePressed()) return;
    
    int prevFrame = frame;
    frame = newFrame;
    if ((newFrame - prevFrame) != 1){
        missedFrameEnergy = 1.0;
    }
    
}
void ofApp::sceneEvent(int newScene){
    scene = newScene;
}

void ofApp::debugEvent(bool bBeDebug){
    
    if (bBeDebug){
        #ifdef WIN32
                HWND cmd = GetConsoleWindow(); // get handle to console window
                ShowWindow(cmd, SW_SHOW);
                ::SetForegroundWindow(cmd);
        #endif
    } else {
        #ifdef WIN32
                HWND cmd = ofGetWin32Window(); // get handle to console window
                ::SetForegroundWindow(cmd);
        #endif
        
    }
}

void ofApp::resetEvent(){
    
}





//--------------------------------------------------------------
void ofApp::update(){

    CM.update();
    missedFrameEnergy *= 0.99f;
    
    #ifdef WIN32
    if (ofGetFrameNum() < 50){
        //HWND hwnd = ofGetWin32Window();
        //cout << hwnd << endl;
        MoveWindow((HWND)ofGetWin32Window(),0, 0, screenBounds.width,screenBounds.height, false);
        //SetWindowPos(hwnd, NULL, 50 + 50 * sin(ofGetElapsedTimef()),0,0,0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_NOOWNERZORDER);

    }
#endif
    
    
    //---------- zoom
    
    cam.update(0.016f);
	cam.lookAt(halfWindow);
    //scale = 0.9 * scale + 0.1 * (1.0/cam.getZoom());
    //scale = ofMap(ofGetMouseX(), 0, ofGetWidth(), 1, 1000000, true);
    logScale = log(scale);
    float dxScale = logScale -logScalePrevFrame;
    scaleChange = dxScale;
    logScalePrevFrame = logScale;
    
    if (fabs(scaleChange) > 0.0001){
        for (int i = 0; i < currentLines.size(); i++){
            currentLines[i].update(scale);
        }
    }

}

//--------------------------------------------------------------
void ofApp::draw(){

    
   
    
    /*fbo.begin();
    LB.resetCounter();
    for (int i = 0; i < currentLines.size(); i++){
        currentLines[i].appendMesh(&LB, scale);
    }
    
    
    ofClear(0,0,0);
    ofPushMatrix();
    ofSetColor(255);
    ofEnableAlphaBlending();
    glBlendFunc(GL_ONE, GL_SRC_COLOR);
    
    //if (bIsRetina) ofTranslate(halfWindow/2);
    //else
    ofTranslate(halfWindow);
    ofScale(1.5, 1.5,0);
    
    ink.bind();
    LB.drawStart();
    LB.drawLoadData();
    LB.drawStartState();
    LB.draw();
    LB.drawEnd();
    
    ink.unbind();
    ofDisableBlendMode();
    ofPopMatrix();
    
    ofSetColor(255);
    
    fbo.end();
    
    */

    ofBackground(ofColor::pink);

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



    string ipStr = ofToString((int)ip.b1) + "." + ofToString((int)ip.b2) + "." + ofToString((int)ip.b3) + "." + ofToString((int)ip.b4);
    ofSetColor(ofColor::red);
    font.drawString(ipStr, 100,200);
    font.drawString("client: " + ofToString(clientID) + "\nframe : " + ofToString(frame) + "\nscene : " + ofToString(scene), 100,350);
    
    ofSetColor(ofColor::cyan);
    ofSetLineWidth(10);
    ofLine(0,0,screenBounds.width, screenBounds.height);
    ofLine(0,screenBounds.width,0, screenBounds.height);

    
    if (missedFrameEnergy > 0.01){
        ofSetColor(255,0,255,255*missedFrameEnergy);
            ofRect(screenBounds);
        
    }

    
    
    
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
