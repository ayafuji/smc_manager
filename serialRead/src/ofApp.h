#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxXmlSettings.h"
#include "ofxNetwork.h"
#include "ofxOsc.h"
#include "ofxBezierWarpManager.h"

class ofApp : public ofBaseApp{

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
    void sendManagedOSCMessage(string _topic, int _signal);
    
    int nBytesRead = 0;
    ofSerial ems1_serial, ems2_serial, sensor1_serial, sensor2_serial;
    char bytesReadString[4];
    
    //gui
    ofxPanel gui_serial;
    ofxToggle gui_ems_1_button;
    ofxToggle gui_ems_2_button;
    ofxToggle gui_enable_osc;
    ofxFloatSlider gui_minutes1;
    ofxFloatSlider gui_minutes2;
    ofxFloatSlider gui_minutes3;
    ofxFloatSlider gui_minutes4;
    ofxFloatSlider gui_minutes5;
    ofxFloatSlider gui_minutes6;
    ofxFloatSlider gui_minutes7;
    ofxFloatSlider gui_minutes8;
    ofxFloatSlider gui_minutes9;
    bool isGuiShow;
    bool isSave;
    
    float flashProb;

    //XML
    ofxXmlSettings xml;
    
    //udp
    ofxUDPManager udpConnection;
    
    //osc
    ofxOscSender sender;
    
    //bezier warp
    ofxBezierWarpManager bezManager;
    ofFbo fbo;
    
    //draw
    bool isTilt;
    
    bool isTilt1Enable, isTilt2Enable, isLeadEnable;
    float processTime;
    bool emsEnable;

};
