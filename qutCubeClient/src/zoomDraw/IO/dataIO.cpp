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
    
    
    
    ifstream myfile;
    myfile.open ( ofToDataPath("example.txt").c_str());
    string lineStr;
    
    //    NSString * path = [NSString stringWithUTF8String: (ofxiOSGetDocumentsDirectory() + fileName + "/" ).c_str() ];
    //    path = [path stringByAppendingPathComponent:@"lineSettings.plist"];
    //    NSArray * settings = [[NSArray alloc] initWithContentsOfFile:path];
    //    NSLog(@"array: %@", settings);
    
    //string filePathName = ofxiOSGetDocumentsDirectory() + fileName;
    //string filePathName = fileName;
    
    //ofBuffer buffer = ofBufferFromFile(filePathName);
    //char * buff  = buffer.getBinaryBuffer();            // get the raw data;
    getline(myfile,lineStr);
    app->scale = ofToFloat(lineStr); //*((float *)(buff));                     //TODO: nice to have this animate a lil, not a lot...
    //buff += sizeof(float);
    //myfile << app->scale << endl;
    
    
    
    //
    getline(myfile,lineStr);
    //app->cam.setZoom(*((float *)(buff)));
    //myfile << (float)*((float *)(buff)) << endl;
    //buff += sizeof(float);
    
    
    getline(myfile,lineStr);
    int nLines = ofToInt(lineStr); //*((int *)(buff));
    //myfile << (int)*((int *)(buff)) << endl;
    //buff += sizeof(int);
    
    
    app->currentLines.clear();
    
    for (int i = 0; i < nLines; i++){
        
        linesWithScale line;
        
        
        getline(myfile,lineStr);
        float scale =  ofToFloat(lineStr); //*((float *)(buff));
        //buff += sizeof(float);
        //myfile << scale << endl;
        
        getline(myfile,lineStr);
        int nPoints =  ofToInt(lineStr); //*((int *)(buff));
        //buff += sizeof(int);
        //myfile << nPoints << endl;
        
        line.scale = scale;
        
        for (int j = 0; j < nPoints; j++){
            
            drawnPoint DP;
            
            
            
            getline(myfile,lineStr);
            float x = ofToFloat(lineStr); // *((float *)(buff));
            //buff += sizeof(float);
            
            getline(myfile,lineStr);
            float y =  ofToFloat(lineStr); //*((float *)(buff));
            //buff += sizeof(float);
            
            getline(myfile,lineStr);
            float nx = ofToFloat(lineStr); // *((float *)(buff));
            //buff += sizeof(float);
            getline(myfile,lineStr);
            float ny = ofToFloat(lineStr); // *((float *)(buff));
            //buff += sizeof(float);
            
            getline(myfile,lineStr);
            float w =  ofToFloat(lineStr); //*((float *)(buff));
            //buff += sizeof(float);
            getline(myfile,lineStr);
            int imp = ofToFloat(lineStr); // *((int *)(buff));
            //buff += sizeof(int);
            
            //            myfile << x << endl;
            //            myfile << y << endl;
            //            myfile << nx << endl;
            //            myfile << ny << endl;
            //            myfile << w << endl;
            //            myfile << imp << endl;
            
            
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
    
    myfile.close();
    
    
    
    
}





