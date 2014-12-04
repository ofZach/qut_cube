//
//  linesWithScale.cpp
//  zoomDraw
//
//  Created by zach on 2/11/14.
//
//

#include "linesWithScale.h"
#include "ofApp.h"




float length2f(ofPoint & a, ofPoint & b){
    return sqrt( (b.x-a.x) * (b.x - a.x) + (b.y - a.y)*(b.y - a.y));
}

ofPolyline linesWithScale::getSmoothPoints ( ofPolyline & pts ){
    
    ofPolyline smoothedPoints;
    if (pts.size() > 2) {
        
        for (unsigned int i = 2; i < pts.size(); ++i) {
            ofPoint prev2 = pts[i - 2];
            ofPoint prev1 = pts[i - 1];
            ofPoint cur = pts[i];
            ofPoint midPoint1 =  (prev1 + prev2) * 0.5f;
            ofPoint midPoint2 =  (cur + prev1) * 0.5f;
            
            int segmentDistance = 2;
            float distance = length2f(midPoint1, midPoint2);
            int numberOfSegments = MIN(128, MAX(floorf(distance / segmentDistance), 32));
            
            float t = 0.0f;
            float step = 1.0f / numberOfSegments;
            for (int j = 0; j < numberOfSegments; j++) {
                ofPoint newPoint;
                newPoint = (midPoint1 * powf(1 - t, 2)) + prev1 * (2.0f * (1 - t) * t) + (midPoint2 * t * t);
                //newPoint.width = powf(1 - t, 2) * ((prev1.width + prev2.width) * 0.5f) + 2.0f * (1 - t) * t * prev1.width + t * t * ((cur.width + prev1.width) * 0.5f);
                smoothedPoints.addVertex(newPoint);
                t += step;
            }
            
            if (i == pts.size() - 1){
                ofPoint finalPoint;
                finalPoint = midPoint2;
                //finalPoint.width = (cur.width + prev1.width) * 0.5f;
                smoothedPoints.addVertex(finalPoint);
            }
        }
        
        
        //! we need to leave last 2 points for next draw
        pts.getVertices().erase( pts.getVertices().begin() + 0,  pts.getVertices().begin() + ( pts.getVertices().size() - 2));
        
    }
    
    smoothedPoints = smoothedPoints.getResampledBySpacing(3.0);
    //cout << smoothedPoints.size() << endl;
    return smoothedPoints;
    
    
}

//------------------------------------------------------------------------------------------------
void linesWithScale::reallyFinish(){
    
    solver->setPoints(line);
    solver->start();
    
   
    

}


void linesWithScale::computeTexCoords(){
    
    
    //textureCoords.clear();

    for (int i = textureCoords.size()/2; i < line.size(); i++){
        float pct =  sin(distance / 1000.0 * TWO_PI) * 0.5 + 0.5;
        //cout << pct << endl;
        int i_m_1 = MAX(0, i-1);
        
        
        distance += (line[i].pos - line[i_m_1].pos).length();
        textureCoords.push_back(ofPoint(bottom.x + pct * (top.x-bottom.x), bottom.y));
        textureCoords.push_back(ofPoint(bottom.x+ pct * (top.x-bottom.x), top.y));
    }
    
    
}


//------------------------------------------------------------------------------------------------
void linesWithScale::calculateLowRes(){
    

    // can this be optimized?...
    
    
    for (int i = 0; i < line.size(); i++){
        
        float pct = 0.4;    // start at half?
        for (int j = 0; j < 6; j++){
            int importanceThresh = MAX(2, pct * line.size());
            pct *= 0.7;
            if (line[i].importance < importanceThresh){
                lineMultiRes[j].indices.push_back(i);
            }
        }
        
    }
    
    // do we have 0 counts???
    // what does this look like...
    // cout << lineMultiRes[ 5 ].indices.size() << endl;
    // cout << lineMultiRes[0].indices.size() << " " << lineMultiRes[1].indices.size() << endl;
    // then do normals
    
    for (int i = 0; i < 6; i++){
        for (int  j = 0; j < lineMultiRes[i].indices.size(); j++){
            int j_m_1 = MAX(0, j-1);
            int j_p_1 = MIN( lineMultiRes[i].indices.size()-1, j+1);
            
            int index_a = lineMultiRes[i].indices[j_m_1];
            int index_b = lineMultiRes[i].indices[j_p_1];
            
            ofPoint diff = line[index_b].pos - line[index_a].pos;
            diff.normalize();
            ofVec2f perp(diff.y, -diff.x);
            
            lineMultiRes[i].normals.push_back(perp);
            
        }
    }
    
    
}

//------------------------------------------------------------------------------------------------
void linesWithScale::finish(){
    
    // this is rediculous. please fix:
    
    
//    float dist = 0;
//    for (int i = 0; i < line.size(); i++){
//        int i_m_1 = MAX(0, i-1);
//        int i_p_1 = MIN( line.size()-1, i+1);
//        dist += (line[i] - line[i_m_1]).length();
//        ofPoint diff = line[i_p_1] - line[i_m_1];
//        diff.normalize();
//        ofPoint perp(diff.y, -diff.x);
//        normals.push_back(perp);
//        distances.push_back(dist);
//    }
    
    
    
}


//------------------------------------------------------------------------------------------------
void linesWithScale::addPointForSmoothLine( ofPoint newPt, int eventType /* 0 = down, 1 = drag, 2 = up */){
    if (eventType == 0){
        tempLine.clear();
        tempLine.addVertex( newPt );
    } else {
        
        if (tempLine.getVertices().size() > 0){
            if ( length2f(newPt, tempLine[tempLine.size() -1]) < 3) return;
        }
        tempLine.addVertex( newPt );
        ofPolyline smoothed = getSmoothPoints(tempLine);
        
        int currentCount = line.size();
        for (int i = 0; i < smoothed.size(); i++){
            drawnPoint pt;
            pt.pos = ofVec2f(smoothed[i].x, smoothed[i].y);
            pt.width = smoothed[i].z;
            pt.importance = 0;
            line.push_back(pt);
        }
        
        if (currentCount > 0) currentCount -= 1;    // the last point is kind of bad, so let's fix it too.  (since it's i_p_1 was itself...)
        
        for (int i = currentCount; i < line.size(); i++){
            int i_m_1 = MAX(0, i-1);
            int i_p_1 = MIN( line.size()-1, i+1);
            ofPoint diff = line[i_p_1].pos - line[i_m_1].pos;
            diff.normalize();
            line[i].normal.set(diff.y, -diff.x);
            
            lineHighRes.indices.push_back(i);
            lineHighRes.normals.push_back(line[i].normal);
            
        }
        
        computeTexCoords();
        ///line.addVertices(smoothed.getVertices());
        //finish(); // keep calcing normals.
    }
    
}

//------------------------------------------------------------------------------------------------
linesWithScale::linesWithScale(){
    drawnPct = 1;
    distance = 0;
    solver = new simplificationSolver();
    top = ((ofApp*)ofGetAppPtr())->ink.getTextureReference().getCoordFromPercent(0, 0.99);
    bottom = ((ofApp*)ofGetAppPtr())->ink.getTextureReference().getCoordFromPercent(1, 0.01);
    
    
}

//------------------------------------------------------------------------------------------------
void linesWithScale::update(float currentScale){
    
    
    if (solver->bSolved == true && solver->bGotData == false){
        
        for (int i = 0; i < line.size(); i++){
            line[i] = solver->pts[i];
        }
        solver->cleanup();
        
        solver->bGotData = true;
        
        //calcSimplificaiton();
        calculateLowRes();
        
    }
    
    
    float scaleAmount = scale / currentScale;
    float pct = ofMap(log(scaleAmount), 1.2, -3.5, 0, 1);
    if (pct < 0 || pct > 1){
        drawnPct *= 0.96;
    } else {
        drawnPct = 0.9 * drawnPct + 0.1 * 1.0;
    }
}



//------------------------------------------------------------------------------------------------
void linesWithScale::appendMesh( lineBatcher * LB, float currentScale){
  
    
    if (drawnPct < 0.01) return;
    
    multiResLine * lineToDraw;
    
    float scaleAmount = scale / currentScale;
    float scalePct = log(scaleAmount);
    
    
    // TODO: 2 controllable vals
    float lineScale = powf(scaleAmount, 0.4);
    float moveScale = powf(scaleAmount, 1.4);

    int whichOne = ofMap(scalePct, 0, -3.0, 0, 6, true);
    
    if (whichOne > 5) whichOne = 5;
    
    if (lineMultiRes[0].indices.size() < 3) lineToDraw = &lineHighRes;
    else lineToDraw = &lineMultiRes[ whichOne ];
    
    LB->newLine();
    
    int drawTill = drawnPct * line.size();
    
    ofPoint a, b;
    int count = 0;
    int lastIndex = 0;
    int lasti = 0;
    
    for (int i = 0; i < lineToDraw->indices.size();  i++){
        
        
        int indx = lineToDraw->indices[i];
        int indx2 = lineToDraw->indices[ MIN(i+1, lineToDraw->indices.size()-1) ];
        
        if (indx < drawTill){
            
            count++;
            
            // TODO: there could be min max on size (with scaling) here...
            
            a = line[indx].pos * moveScale + lineToDraw->normals[i] * line[indx].width * lineScale;
            b = line[indx].pos * moveScale - lineToDraw->normals[i] * line[indx].width * lineScale;
            
//            if (i == 0){
//                ofPoint capPos = line[indx].pos  *moveScale;
//                float angle = atan2(lineToDraw->normals[i].y, lineToDraw->normals[i].x) + PI;
//                float startAnge = angle; //atan2(normals[0].y, normals[0].x) + PI;
//                float w = line[indx].width;
//                for (int j = 0; j < 10; j++){
//                    ofPoint aa = capPos; // + ofPoint(cos(startAnge), sin(startAnge)) * w *lineScale;
//                    ofPoint bb = capPos + ofPoint(cos(angle), sin(angle)) * w*lineScale;
//                    angle += PI/9.0;
//                    
//                    ofPoint midTex = (textureCoords[indx * 2] + textureCoords[indx * 2+1]) / 2.0;
//                    
//                    ofPoint tex = textureCoords[indx * 2+1] + ofMap(j, 0, 9, 0,1) *
//                                    (textureCoords[indx * 2] - textureCoords[indx * 2+1]);
//                    
//                    LB->addPointSet(aa.x, aa.y, midTex.x, midTex.y,bb.x, bb.y, tex.x, tex.y);
//                }
//            }
            
            lastIndex = indx;
            lasti = i;
            
            LB->addPointSet(a.x, a.y,
                            textureCoords[indx * 2].x, textureCoords[indx * 2].y,
                            b.x, b.y,
                            textureCoords[indx * 2+1].x, textureCoords[indx * 2+1].y);

            
        }
        
    }
    
    
//    if (lastIndex > 0){
//        ofPoint capPos = line[lastIndex].pos  *moveScale;
//        float angle = atan2(lineToDraw->normals[lasti].y, lineToDraw->normals[lasti].x) + PI;
//        float w = line[lastIndex].width;
//        float dir = 1;
//        if (angle < 0) dir = -1;
//        for (int j = 0; j < 20; j++){
//            ofPoint aa = capPos; // + ofPoint(cos(startAnge), sin(startAnge)) * w *lineScale;
//            ofPoint bb = capPos + ofPoint(cos(angle), sin(angle)) * w*lineScale;
//            angle -= PI/19.0 * dir;
//            
//            ofPoint midTex = (textureCoords[lastIndex * 2] + textureCoords[lastIndex * 2+1]) / 2.0;
//            
//            ofPoint tex = textureCoords[lastIndex * 2+1] + ofMap(j, 0, 19, 0,1 ) *
//            (textureCoords[lastIndex * 2] - textureCoords[lastIndex * 2+1]);
//            
//            LB->addPointSet(aa.x, aa.y, midTex.x, midTex.y,bb.x, bb.y, tex.x, tex.y);
//        }
//        
//    }



    
    if (count){
        //ECP->addEndcaps(b, a, textureCoords[0 * 2], textureCoords[0 * 2+1], LB, -1);
    }
}

//------------------------------------------------------------------------------------------------
void linesWithScale::appendMesh( ofMesh & mesh, float currentScale){
    
    
    
    
    multiResLine * lineToDraw;


    if (drawnPct < 0.01) return;
    
    float scaleAmount = scale / currentScale;
    float scalePct = log(scaleAmount);
    float lineScale = powf(scaleAmount, 0.4);

    float moveScale = powf(scaleAmount, 1.4);

    
    int whichOne = ofMap(scalePct, 0, -3.0, 0, 6, true);
    
    if (whichOne > 5) whichOne = 5;
    
    if (lineMultiRes[0].indices.size() < 3) lineToDraw = &lineHighRes;
    else lineToDraw = &lineMultiRes[ whichOne ];
    
    
    
    
    //        first end cap
    //        ofPoint capPos = ofPoint(line[0].x, line[0].y);
    //        float angle = atan2(normals[0].y, normals[0].x) + PI;
    //        float startAnge = angle; //atan2(normals[0].y, normals[0].x) + PI;
    //        float w = line[0].z;
    //        for (int i = 0; i < 5; i++){
    //
    //            if (i == 0 && mesh.getNumVertices() > 0){
    //                mesh.addVertex(mesh.getVertex(mesh.getNumVertices()-1));
    //            }
    //
    //            if (i == 0)mesh.addVertex((capPos + ofPoint(cos(startAnge), sin(startAnge)) * w)*scaleAmount );
    //            mesh.addVertex((capPos + ofPoint(cos(startAnge), sin(startAnge)) * w)*scaleAmount );
    //            mesh.addVertex((capPos + ofPoint(cos(angle), sin(angle)) * w)*scaleAmount );
    //            angle += PI/4.0;
    //        }
    //line.size() * drawnPct;
    
    
    int drawTill = drawnPct * line.size();
    
    
    
    //cout << lineToDraw->indices.size() << endl;
    
    for (int i = 0; i < lineToDraw->indices.size();  i++){
        
        
        int indx = lineToDraw->indices[i];
        if (indx < drawTill){
            
            if (i == 0 && mesh.getNumVertices() > 0){
                mesh.addTexCoord(mesh.getTexCoord(mesh.getNumTexCoords()-1));
                mesh.addVertex(mesh.getVertex(mesh.getNumVertices()-1));
            }
            
            //cout << textureCoords[indx * 2] << endl;
            
            mesh.addTexCoord( textureCoords[indx * 2]);
            mesh.addVertex( (line[indx].pos * moveScale + lineToDraw->normals[i] * line[indx].width * lineScale) );
            
            if (i == 0 && mesh.getNumVertices() > 0){
                mesh.addTexCoord(mesh.getTexCoord(mesh.getNumTexCoords()-1));
                mesh.addVertex(mesh.getVertex(mesh.getNumVertices()-1));
            }
            
            mesh.addTexCoord( textureCoords[indx * 2+1]);
            mesh.addVertex( (line[indx].pos * moveScale - lineToDraw->normals[i] * line[indx].width * lineScale) );
        }
       
    }
    
//    if (line.size() > 0){
//        ofDrawBitmapStringHighlight( ofToString(scalePct), line[0].pos *scaleAmount);
//    }
}
