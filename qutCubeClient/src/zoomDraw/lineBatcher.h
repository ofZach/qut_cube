

#pragma once


#include "lineTypes.h"
#include "ofMain.h"


class lineBatcher {
    
    
public:
    
    ofRectangle screen;
    
    ptAndTex pts[200000];        // 100000 * 4 * 8            // 400kb....
    
    lineBatcher();
    
    
    GLuint VBO;
    void setupVbo();
    
    void allocateGPUmem();
    
    void drawStart();       // load data set mode
    void drawLoadData();
    void drawStartState();
    void draw();            // draw
    void drawEnd();         // reset
    
    
    void resetCounter();
    void addPointSet( float x1, float y1, float u1,float t1,
                      float x2, float y2, float u2,float t2 );
    void newLine();         // demarcate a new line
    void doubleLastPoint(); // for line strips that don't touch.
    bool bAmInNewLine;      // double up new points.
    
    int counter;
    
};
