//
//  dataIO.cpp
//  zoomDraw
//
//  Created by zach on 2/7/14.
//
//

#include "dataIO.h"
#include "ofApp.h"



//void dataIO::deleteDocumentDir(){
//    
//    ofDirectory dir = ofxiOSGetDocumentsDirectory();
//    dir.listDir();
//    vector<string> names;
//    for (int i =0; i<dir.size(); i++){
//        string name = dir.getPath(i);
//        ofFile file(name);
//        file.remove(true);  // recursive careful!
//    }
//}




/*
 typedef struct {
 
 ofVec2f     pos;
 ofVec2f     normal;
 float       width;
 int         importance;
 
 } drawnPoint;
 */

//--------------------------------------------------------------

void dataIO::loadData(string fileName){
    
    
    
    
    ofApp * app = (ofApp * ) ofGetAppPtr();

    
    
//    NSString * path = [NSString stringWithUTF8String: (ofxiOSGetDocumentsDirectory() + fileName + "/" ).c_str() ];
//    path = [path stringByAppendingPathComponent:@"lineSettings.plist"];
//    NSArray * settings = [[NSArray alloc] initWithContentsOfFile:path];
//    NSLog(@"array: %@", settings);
    
    //string filePathName = ofxiOSGetDocumentsDirectory() + fileName;
    string filePathName = fileName;
    
    ofBuffer buffer = ofBufferFromFile(filePathName);
    char * buff  = buffer.getBinaryBuffer();            // get the raw data;
    
    app->scale =*((float *)(buff));                     //TODO: nice to have this animate a lil, not a lot...
    buff += sizeof(float);
    
    
    //app->cam.setZoom(*((float *)(buff)));
    buff += sizeof(float);
    
    int nLines = *((int *)(buff));
    buff += sizeof(int);
    
    app->currentLines.clear();
    
    for (int i = 0; i < nLines; i++){
        
        linesWithScale line;
        
        float scale =  *((float *)(buff));
        buff += sizeof(float);
        int nPoints =  *((int *)(buff));
        buff += sizeof(int);
        
        line.scale = scale;
        
        for (int j = 0; j < nPoints; j++){
            
            drawnPoint DP;
            
            float x =  *((float *)(buff));
            buff += sizeof(float);
            float y =  *((float *)(buff));
            buff += sizeof(float);
            
            float nx =  *((float *)(buff));
            buff += sizeof(float);
            float ny =  *((float *)(buff));
            buff += sizeof(float);
            
            float w =  *((float *)(buff));
            buff += sizeof(float);
            int imp =  *((int *)(buff));
            buff += sizeof(int);
            
            DP.pos.set(x,y);
            DP.normal.set(nx, ny);
            DP.width = w;
            DP.importance = imp;
            
            line.line.push_back(DP);
            line.lineHighRes.indices.push_back(j);
            line.lineHighRes.normals.push_back(DP.normal);
        }
        
        line.calculateLowRes();                     // <----------------- should this be loaded not computed?
        line.computeTexCoords();                    // <----------------- should this be loaded not computed?
        app->currentLines.push_back(line);
        
    }
    
    
    
}




