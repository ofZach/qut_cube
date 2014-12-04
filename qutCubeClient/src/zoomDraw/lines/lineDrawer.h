

#pragma once

#include "ofMain.h"

class lineDrawer {
public:
    ofImage ink;
    void loadImage(){
        ink.loadImage("images/ink6.png");
    }
    
    

    void appendMesh( ofVboMesh & mesh, ofPolyline & line, float scale, float width, vector < float > widths){
        
        if (line.size() < 3) return;
        
        // I'll do endcaps later
        
        ofPoint top = ink.getTextureReference().getCoordFromPercent(0, 0);
        ofPoint bottom = ink.getTextureReference().getCoordFromPercent(1, 1);
        
        //ink.getTextureReference().bind();
        
        //ofVboMesh mesh;
        //mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
        
        float dist = 0;
        
        for (int i = 0; i < line.size(); i++){
            
            float pct =  sin(dist / 1000.0 * TWO_PI) * 0.5 + 0.5;
            
            //float pct = ofMap(i, 0, line.size(), 0, 1);
            int i_m_1 = MAX(0, i-1);
            int i_p_1 = MIN( line.size()-1, i+1);
            
            dist += (line[i] - line[i_m_1]).length();
            
            ofPoint diff = line[i_p_1] - line[i_m_1];
            diff.normalize();
            ofPoint perp(diff.y, -diff.x);
            
            if (i == 0 && mesh.getNumVertices() > 0){
                
                mesh.addVertex(mesh.getVertex(mesh.getNumVertices()-1));
                mesh.addTexCoord(mesh.getTexCoord(mesh.getNumTexCoords()-1));
                mesh.addVertex(mesh.getVertex(mesh.getNumVertices()-1));
                mesh.addTexCoord(mesh.getTexCoord(mesh.getNumTexCoords()-1));
                
                
                if (widths.size() == 0){
                        mesh.addVertex( (line[i] + perp * width)*scale);
                        mesh.addTexCoord( ofPoint(bottom.x + pct * (top.x-bottom.x), bottom.y));
                    
                        mesh.addVertex(mesh.getVertex(mesh.getNumVertices()-1));
                        mesh.addTexCoord(mesh.getTexCoord(mesh.getNumTexCoords()-1));
                    
                        mesh.addVertex( (line[i] - perp * width)*scale);
                        mesh.addTexCoord( ofPoint(bottom.x+ pct * (top.x-bottom.x), top.y) );
                    } else {
                        
                        float scalef = width / 6.0;
                        mesh.addVertex( (line[i] + perp * widths[i] * scalef)*scale);
                        mesh.addTexCoord( ofPoint(bottom.x + pct * (top.x-bottom.x), bottom.y));
                        
                        mesh.addVertex(mesh.getVertex(mesh.getNumVertices()-1));
                        mesh.addTexCoord(mesh.getTexCoord(mesh.getNumTexCoords()-1));
                        
                        mesh.addVertex( (line[i] - perp * widths[i] * scalef)* scale);
                        mesh.addTexCoord( ofPoint(bottom.x+ pct * (top.x-bottom.x), top.y) );
                        
                    }
               
               
     
                
                
            } else {
                if (widths.size() == 0){
                    mesh.addVertex( (line[i] + perp * width)*scale);
                    mesh.addTexCoord( ofPoint(bottom.x + pct * (top.x-bottom.x), bottom.y));
                    
                    mesh.addVertex( (line[i] - perp * width)*scale);
                    mesh.addTexCoord( ofPoint(bottom.x+ pct * (top.x-bottom.x), top.y) );
                } else {
                    
                    float scalef = width / 6.0;
                    mesh.addVertex( (line[i] + perp * widths[i] * scalef)*scale);
                    mesh.addTexCoord( ofPoint(bottom.x + pct * (top.x-bottom.x), bottom.y));
                    
                    mesh.addVertex( (line[i] - perp * widths[i] * scalef)* scale);
                    mesh.addTexCoord( ofPoint(bottom.x+ pct * (top.x-bottom.x), top.y) );
                    
                }
            }
            
            
            
            
        }
        //mesh.draw();
        //ink.unbind();
        
    }
    
    void draw( ofPolyline & line, float width, vector < float > widths){
        
        if (line.size() < 3) return;
        
        // I'll do endcaps later
        
        ofPoint top = ink.getTextureReference().getCoordFromPercent(0, 0);
        ofPoint bottom = ink.getTextureReference().getCoordFromPercent(1, 1);
        
        ink.getTextureReference().bind();
        
        ofVboMesh mesh;
        mesh.setMode(OF_PRIMITIVE_TRIANGLE_STRIP);
        
        float dist = 0;
        
        for (int i = 0; i < line.size(); i++){
            
            float pct =  1; // sin(dist / 1000.0 * TWO_PI) * 0.5 + 0.5;
            
            //float pct = ofMap(i, 0, line.size(), 0, 1);
            int i_m_1 = MAX(0, i-1);
            int i_p_1 = MIN( line.size()-1, i+1);
            
            //dist += (line[i] - line[i_m_1]).length();
            
            ofPoint diff = line[i_p_1] - line[i_m_1];
            diff.normalize();
            ofPoint perp(diff.y, -diff.x);
            
            if (widths.size() == 0){
                mesh.addVertex( line[i] + perp * width);
                mesh.addTexCoord( ofPoint(bottom.x + pct * (top.x-bottom.x), bottom.y));
                
                mesh.addVertex( line[i] - perp * width);
                mesh.addTexCoord( ofPoint(bottom.x+ pct * (top.x-bottom.x), top.y) );
            } else {
                
               float scalef = width / 6.0;
                mesh.addVertex( line[i] + perp * widths[i] * scalef);
                mesh.addTexCoord( ofPoint(bottom.x + pct * (top.x-bottom.x), bottom.y));
                
                mesh.addVertex( line[i] - perp * widths[i] * scalef);
                mesh.addTexCoord( ofPoint(bottom.x+ pct * (top.x-bottom.x), top.y) );
                
            }
            
        }
        mesh.draw();
        ink.unbind();
        
    }
    
    
};