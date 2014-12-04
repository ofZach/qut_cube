//
//  lineTypes.h
//  zoomDraw
//
//  Created by zach on 2/12/14.
//
//

#ifndef zoomDraw_lineTypes_h
#define zoomDraw_lineTypes_h

#include "ofMain.h"

typedef struct {
    
    ofVec2f     pos;
    ofVec2f     normal;
    float       width;
    int         importance;

} drawnPoint;


typedef struct {
    
    vector < int >      indices;
    vector < ofVec2f >  normals;
    
} multiResLine;



// this is for batching of triangles.  we need to pack the data as small as possible for uploading.

struct ptAndTex{

    // short instead of float?
    
    float x;
    float y;
    
    // can these be bytes?  do we have allignement issues?
    
    float u;
    float t;
    
    
    ptAndTex(float _x=0, float _y=0, float _u=0, float _t=0):x(_x), y(_y), u(_u), t(_t) {   }
    void set(float _x=0, float _y=0, float _u=0, float _t=0){
        x = _x;
        y = _y;
        u = _u;
        t = _t;
    }
    
    
    //      Specify texture coordinates using 2 or 4 unsigned bytes (GL_UNSIGNED_BYTE) or unsigned short (GL_UNSIGNED_SHORT).   Do not pack multiple sets of texture coordinates into a single attribute.
    
};


#endif
