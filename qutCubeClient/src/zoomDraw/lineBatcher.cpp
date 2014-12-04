//
//  lineBatcher.cpp
//  zoomDraw
//
//  Created by zach on 2/18/14.
//
//

#include "lineBatcher.h"


/*  notes:
 
 
 The size of any OpenGL buffer object is set when you call glBufferData. That is, OpenGL will allocate the amount of memory you specify in the second argument of glBufferData (which isn't listed in the OP). In fact, if you call, for example glBufferData( GL_ARRAY_BUFFER, bufferSize, NULL, GL_DYNAMIC_DRAW ); OpenGL will create a buffer of bufferSize bytes of uninitialized data.
 
 You can load any amount of data (up to the size of the buffer) using glBufferSubData, glMapBuffer, or any of the other routines for passing data. The only way to resize the buffer is to call glBufferData with a new size for the same buffer id (the value returned from glGenBuffers).
 
 That said, you can always use a subset of the data in the buffer (which would be akin to deleting vertices), and if you render using glDrawElements, you can randomly access elements in the buffer. Adding vertices to a buffer would require allocating a larger buffer, and then you'd need to reload all of the data in the buffer.
 
 
 */


lineBatcher::lineBatcher(){
    resetCounter();
    
}

void lineBatcher::resetCounter(){
    counter = 0;
}

void lineBatcher::allocateGPUmem(){
    memset(pts, 0, 200000*sizeof(ptAndTex));
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ptAndTex) *  200000, pts, GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
}

void lineBatcher::setupVbo(){
    screen.set(-ofGetWidth()/2 + 100,-ofGetHeight()/2 + 100,ofGetWidth()-200, ofGetHeight()-200);
    glGenBuffers(1, &VBO);
}



void lineBatcher::drawLoadData(){
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(ptAndTex) * counter, pts);
}

void lineBatcher::drawStartState(){
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(2, GL_FLOAT, sizeof(ptAndTex), NULL);
    glClientActiveTexture(GL_TEXTURE0);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glTexCoordPointer(2, GL_FLOAT, sizeof(ptAndTex), ((char*)NULL)+2*sizeof(float) );
}

void lineBatcher::drawStart(){
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
}


void lineBatcher::draw(){
    glDrawArrays(GL_TRIANGLE_STRIP, 0, counter);
}

void lineBatcher::drawEnd(){
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

// find the fastest thing here...

void lineBatcher::addPointSet( float x1, float y1, float u1,float t1,
                               float x2, float y2, float u2,float t2 ){
    

    // if we have a lot of points, bail:
    if (counter > 200000 - 5) return;
    
    // if these two are offscreen, and the last one is offscreen, let's start a new line.  (I think this will have glitches.  maybe make the rect bigger then the screen slightly)
    
     
    
    pts[counter++].set(x1, y1, u1, t1 );
    
    if (bAmInNewLine == true){
        doubleLastPoint();
        bAmInNewLine= false;
    }
    
    pts[counter++].set( x2, y2, u2, t2 );
    
}

void lineBatcher::doubleLastPoint(){
    if (counter == 0) return;
    pts[counter] = pts[counter-1];
    counter++;
}

void lineBatcher::newLine(){
    doubleLastPoint();
    bAmInNewLine = true;
}