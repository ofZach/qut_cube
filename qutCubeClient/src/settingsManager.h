

#pragma once 

#include "ofMain.h"
#include "ofxXmlSettings.h"


//----------------------------------------------
typedef struct {
    ofRectangle interiorPct;
    ofRectangle exteriorPct;
} displayRect;
//----------------------------------------------
typedef struct {
    int clientID;
    string IPaddress;
    displayRect rects[2];
    int nViews; // will be 2 for each client but for now...
} clientSetting;
//----------------------------------------------
typedef struct {
    ofRectangle overallDimensions;
} worldSetting;
//----------------------------------------------


// IP
#ifdef WIN32
#include <winsock.h>
#endif


//----------------------------------------------
struct IPv4{
    unsigned char b1, b2, b3, b4;
};







//--------------------------------------------------------------------------------------------
class settingsManager {
    
    
public:
    
    //----------------------------------------------
    bool getMyIP(IPv4 & myIP){
        
#ifdef WIN32
        char szBuffer[1024];
        WSADATA wsaData;
        WORD wVersionRequested = MAKEWORD(2, 0);
        if(::WSAStartup(wVersionRequested, &wsaData) != 0)
            return false;
        if(gethostname(szBuffer, sizeof(szBuffer)) == SOCKET_ERROR)
        {
            WSACleanup();
            return false;
        }
        
        struct hostent *host = gethostbyname(szBuffer);
        if(host == NULL)
        {
            WSACleanup();
            return false;
        }
        
        //Obtain the computer's IP
        myIP.b1 = ((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b1;
        myIP.b2 = ((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b2;
        myIP.b3 = ((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b3;
        myIP.b4 = ((struct in_addr *)(host->h_addr))->S_un.S_un_b.s_b4;
        
        WSACleanup();
#endif
        return true;
        
    }

    
    
    //----------------------------------------------
    int whichClientAmI(){
        IPv4 ip;
        getMyIP(ip);
        int lastNum = (int)ip.b4;
        
        int who = 0;
        
        for (int i = 0; i < clients.size(); i++){
            string ip = clients[i].IPaddress;
            vector < string > address = ofSplitString(ip,".");
            int lastNumClient = (int)ofToInt(address[3]);
            if (lastNumClient == lastNum){
                who = clients[i].clientID;
            }
        }
        return who;
    }
    
    //----------------------------------------------
    void loadSettings ( string xmlFileName ){
        
        ofxXmlSettings settings;
        settings.load("displaySettings.xml");
        settings.pushTag("clients");
        int nClients = settings.getNumTags("client");
        
        
        for (int i = 0; i < nClients; i++){
            settings.pushTag("client", i);
            
            clientSetting client;
            
            client.clientID     = settings.getValue("id", 0);
            client.IPaddress    = settings.getValue("ip", "127.0.0.1");
            client.nViews       = settings.getNumTags("view");
            
            //cout << "----------------------------------------------" << endl;
            //cout << client.clientID  << " " << client.IPaddress << endl;
            
            for (int j = 0; j < client.nViews; j++){
                
                settings.pushTag("view", j);
                client.rects[j].interiorPct.x       = settings.getValue("interiorPct:x", 0.0);
                client.rects[j].interiorPct.y       = settings.getValue("interiorPct:y", 0.0);
                client.rects[j].interiorPct.width   = settings.getValue("interiorPct:w", 0.0);
                client.rects[j].interiorPct.height  = settings.getValue("interiorPct:h", 0.0);
                
                client.rects[j].exteriorPct.x       = settings.getValue("exteriorPct:x", 0.0);
                client.rects[j].exteriorPct.y       = settings.getValue("exteriorPct:y", 0.0);
                client.rects[j].exteriorPct.width   = settings.getValue("exteriorPct:w", 0.0);
                client.rects[j].exteriorPct.height  = settings.getValue("exteriorPct:h", 0.0);
                
                //cout << client.rects[j].interiorPct <<endl;
                //cout << client.rects[j].exteriorPct <<endl;
                
                settings.popTag();
            }
            
            settings.popTag();
            
            clients.push_back(client);
        }
 
    }

    //----------------------------------------------
    void drawDiagnostics(){
        
        ofRectangle bigPixDim;
        ofRectangle smallPixDim;
        ofRectangle smallInWorldDim;
        ofRectangle total;
        
        bigPixDim.set(0,0,1920, 1750);
        smallPixDim.set(0, 0, 1080, 1920);
        float aspectRatio = 1080.0 / 1920.0;
        smallInWorldDim.set(0,0, 1920/10.0, (1/aspectRatio) * (1920/10.0));
        total.set(0,0, 1920*2, 1750 + smallInWorldDim.getHeight());
        
        
        ofScale(0.3,0.3);
        for (int i = 0; i < clients.size(); i++){
            
            for (int j = 0; j < clients[i].nViews; j++){
                float x = total.width * clients[i].rects[j].interiorPct.x;
                float y = total.height * clients[i].rects[j].interiorPct.y;
                float w = total.width * clients[i].rects[j].interiorPct.width;
                float h = total.height * clients[i].rects[j].interiorPct.height;
                
                ofSetColor(100 + ofRandom(20));
                ofRect(x,y,w,h);
            }
        }
        
    }
    
    //----------------------------------------------
    vector < clientSetting > clients;
    worldSetting world;
    
};