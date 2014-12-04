//
//  vwLineSimplifier.cpp
//  zoomDraw
//
//  Created by zach on 2/12/14.
//
//

#include "vwLineSimplifier.h"


class triangle {
public:
    int indices[3];
    float area;
    triangle * prev;
    triangle * next;
};

bool compareTri ( triangle * i, triangle * j){
    
    // important note here:
    // http://stackoverflow.com/questions/12290479/stdsort-fails-on-stdvector-of-pointers
    
    if (i->area != j->area) return i->area < j->area;
    else return false;
    
}

//--------------------------------------------------------------
inline float triArea(ofPoint d0, ofPoint d1, ofPoint d2)
{
    double dArea = ((d1.x - d0.x)*(d2.y - d0.y) - (d2.x - d0.x)*(d1.y - d0.y))/2.0;
    return (dArea > 0.0) ? dArea : -dArea;
}

//--------------------------------------------------------------
void calcSimplificaiton( vector < drawnPoint > & line){
    
    
  int total = line.size();
    
    
    // if we have 100 points, we have 98 triangles to look at
    int nTriangles = total - 2;
    
    
    triangle * triangles[ nTriangles ];
    
    for (int i = 1; i < total-1; i++){
        triangle * tempTri = new triangle();
        tempTri->indices[0] = i-1;
        tempTri->indices[1] = i;
        tempTri->indices[2] = i+1;
        tempTri->area = triArea(        line[tempTri->indices[0]].pos,
                                        line[tempTri->indices[1]].pos,
                                        line[tempTri->indices[2]].pos);
        triangles[i-1] = tempTri;
    }
    
    // set the next and prev triangles, use NULL on either end. this helps us update traingles that might need to be removed
    for (int i = 0; i < nTriangles; i++){
        triangles[i]->prev = (i == 0 ? NULL : triangles[i-1]);
        triangles[i]->next = (i == nTriangles-1 ? NULL : triangles[i+1]);
    }
    
    std::vector<triangle*> trianglesVec;
    
    for (int i = 0; i < nTriangles; i++){
        trianglesVec.push_back(triangles[i]);
    }
    
    
    
    int count = 0;
    while ( !trianglesVec.empty()){
        
        
        
        ofSort(trianglesVec,compareTri);
        
        triangle * tri = trianglesVec[0];
        
        line[tri->indices[1]].importance = total - count;         // store the "importance" of this point in numerical order of removal (but inverted, so 0 = most improtant, n = least important.  end points are 0.
        count ++;
        
        
        if (tri->prev != NULL){
            tri->prev->next = tri->next;
            tri->prev->indices[2] = tri->indices[2];  // check!
            
            tri->prev->area = triArea(      line[tri->prev->indices[0]].pos,
                                            line[tri->prev->indices[1]].pos,
                                            line[tri->prev->indices[2]].pos);
            
        }
        
        if (tri->next != NULL){
            tri->next->prev = tri->prev;
            tri->next->indices[0] = tri->indices[0];  // check!
            
            
            tri->next->area = triArea(      line[tri->next->indices[0]].pos,
                                            line[tri->next->indices[1]].pos,
                                            line[tri->next->indices[2]].pos);
            
            
        }
        
        trianglesVec.erase(trianglesVec.begin());
        
        
        
    }
    
    // free the memory we just allocated above.
    for (int i = 0; i < nTriangles; i++){
        delete triangles[i];
    }
    
    
    
}
