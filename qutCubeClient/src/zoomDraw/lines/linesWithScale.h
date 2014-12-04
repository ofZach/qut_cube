//
//  linesWithScale.h
//  zoomDraw
//
//  Created by Firm Read on 2/3/14.
//
//

#ifndef zoomDraw_linesWithScale_h
#define zoomDraw_linesWithScale_h

#include "ofMain.h"
#include "vwLineSimplifier.h"
#include "lineTypes.h"
#include "lineBatcher.h"




//typedef struct {
//    ofVec2f     pos;
//    ofVec2f     normal;
//    float       width;
//    int         importance;
//    
//    // flags for if calculates?
//    
//} drawnPoint;


class endCapProducer {
    
public:
    
    ofPoint * positions;
    int * angles;
    ofPoint * texPct;
    int nPos;
    
    void setup(int nPositions){
        
        nPos = nPositions;
        positions = new ofPoint[nPositions];
        angles = new int[nPositions];
        texPct = new ofPoint[nPositions];
        
        float angleChange = TWO_PI / (float)nPositions;
        float angleAdder = 0;
        for (int i = 0; i < nPositions; i++){
            angles[i] =  RAD_TO_DEG * angleAdder;
            positions[i] = ofPoint (cos(angleAdder), sin(angleAdder));
            texPct[i] = positions[i] * 0.5 + ofPoint(0.5, 0.5); // 0 - 1
            angleAdder += angleChange;
        }
    }
    
    
    void addEndcaps ( ofPoint a, ofPoint b, ofPoint texA, ofPoint texB, lineBatcher * LB, int direction = -1){
        
        direction = 1;
        
        ofPoint midPt = (a + b) / 2.0;
        ofPoint texMidPt = (texA + texB) / 2.0;
        ofPoint diff = b - a;
        
        float rad = diff.length() / 2.0;
        int angle = atan2( diff.y, diff.x) * RAD_TO_DEG;
        
        while (angle < 0) angle += 360;         // keep me between 0 and 360
        while (angle > 360) angle -= 360;
        
        int indexBetween = 0;
        
        if (angle <= angles[0] && angle >= angles[1]) indexBetween = 0;
        else
            for (int i = 0; i < nPos-1; i++){
                if (angle > angles[i] && angle <= angles[i+1]){
                    indexBetween = i+1;
                }
            }
        int start = indexBetween;
        int count = (nPos/2);
        //int end = indexBetween - (nPos/2 - 1);
        
//        LB->addPointSet(b.x, b.y, texMidPt.x, texMidPt.y,  a.x, a.y, texMidPt.x, texMidPt.y);
        
        for (int i = 0; i < count; i++){
            int ind = start + i * direction;
            if (ind < 0) ind += nPos;
            if (ind >= nPos) ind -= nPos;
            
            ofPoint pos = midPt + positions[ind] * rad;
            ofPoint tex = texA + ofMap(positions[ind].x, -1,1, 0,1) * (texB - texA);
            
            
            //LB->addPointSet(b.x, b.y, texMidPt.x, texMidPt.y, pos.x, pos.y, tex.x, tex.y);
        }
        
       
        
    }
    
    
};


class simplificationSolver : public ofThread{
    
public:
    
    drawnPoint * pts;
    int count;
    bool bSolved;
    bool bGotData;
    
    //--------------------------
    simplificationSolver(){
        bSolved = false;
        bGotData = false;
        
        
    }
    
    void setPoints(vector < drawnPoint > & ptsToSolve){
        pts = new drawnPoint[ptsToSolve.size()];
        count = ptsToSolve.size();
        for (int i = 0; i < ptsToSolve.size(); i++){
            pts[i] = ptsToSolve[i];
        }
        bSolved = false;
    }
    
    void start(){
        startThread(true, false);   // blocking, verbose
    }
    
    void stop(){
        stopThread();
    }
    
    //--------------------------
    void threadedFunction(){
        vector < drawnPoint > drawnPointVec;
        for (int i = 0; i < count; i++){
            drawnPointVec.push_back(pts[i]);
        }
        calcSimplificaiton(drawnPointVec);
        for (int i = 0; i < count; i++){
            pts[i] = drawnPointVec[i];
        }
        bSolved = true;
    }
    
    void cleanup(){
        delete [ ] pts;
    }
    
    
    
};



class linesWithScale {
    
public:
    
    
    float scale;
    
    
    //--------------------------------------
    ofPolyline tempLine;
    
    vector < drawnPoint > line;
    
    float distance;
    void computeTexCoords();
    vector < ofPoint > textureCoords;
    
    ofPoint top;// = ((testApp*)ofGetAppPtr())->ink.getTextureReference().getCoordFromPercent(0, 0);
    ofPoint bottom;// = ((testApp*)ofGetAppPtr())->ink.getTextureReference().getCoordFromPercent(1, 1);
    
    
    multiResLine lineLowRes;
    multiResLine lineHighRes;
    multiResLine lineMultiRes[6];
 
    float drawnPct;
    
    linesWithScale();
    ofPolyline      getSmoothPoints ( ofPolyline & pts );
    void            reallyFinish();
    void            addPointForSmoothLine( ofPoint newPt, int eventType /* 0 = down, 1 = drag, 2 = up */);
    void            update(float currentScale);
    
    
    void            appendMesh( ofMesh & mesh, float currentScale);
    
    void            appendMesh( lineBatcher * LB, float currentScale);
    
    
    void            finish();
    
    void            calculateLowRes();
    
    simplificationSolver * solver;
    endCapProducer * ECP;
    
};

#endif
