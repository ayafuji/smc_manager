#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofBackground(0, 0, 0);
    ofSetFrameRate(120);
    
    ofSetFullscreen(true);
    
    //XML
    xml.load("config.xml");
    
    string ems1_name = xml.getValue("serial:ems1", "/dev/cu.usbserial-A1057P90");
    string ems2_name = xml.getValue("serial:ems2", "/dev/cu.usbmodem14131");
    string sensor1_name = xml.getValue("serial:sensor1", "/dev/cu.usbmodem14241");
    
    string upd_host = xml.getValue("udp:ip", "localhost");
    int udp_port = xml.getValue("udp:port", 19999);
    
    string protocol = xml.getValue("connection", "osc");
    
    // EMS 側 arduino セットアップ
    ems1_serial.setup(ems1_name,9600);
    ems2_serial.setup(ems2_name,9600);
    
    for(auto a: ems1_serial.getDeviceList()) {
        ofLogWarning() << a.getDeviceName();
    }
    
    // センサ側 arduino セットアップ
    sensor1_serial.setup(sensor1_name, 9600);
    
    //OSC セットアップ
    if(protocol == "osc") {
        ofLogWarning() << "OSC setup";
        if(gui_enable_osc) sender.setup(upd_host.c_str(),udp_port);
    }
    
    // gui セットアップ
    gui_serial.setup();
    gui_serial.add(gui_ems_1_button.setup("EMS 1", false));
    gui_serial.add(gui_ems_2_button.setup("EMS 2", false));
    gui_serial.add(gui_enable_osc.setup("OSC ENALBE", false));
    gui_serial.add(gui_minutes1.setup("Minutes 1", 50, 0, 100));
    gui_serial.add(gui_minutes2.setup("Minutes 2", 50, 0, 100));
    gui_serial.add(gui_minutes3.setup("Minutes 3", 50, 0, 100));
    gui_serial.add(gui_minutes4.setup("Minutes 4", 50, 0, 100));
    gui_serial.add(gui_minutes5.setup("Minutes 5", 50, 0, 100));
    gui_serial.add(gui_minutes6.setup("Minutes 6", 50, 0, 100));
    gui_serial.add(gui_minutes7.setup("Minutes 7", 50, 0, 100));
    gui_serial.add(gui_minutes8.setup("Minutes 8", 50, 0, 100));
    gui_serial.add(gui_minutes9.setup("Minutes 9", 50, 0, 100));
    gui_serial.loadFromFile("serial_config.xml");
    
    isGuiShow = true;
    
    //bezier warp
    fbo.allocate(ofGetWidth(), ofGetHeight());
    bezManager.setup(10); //WarpResolution
    bezManager.addFbo(&fbo);
    
    //draw
    isTilt = false;
    
    processTime = 0;
    
    emsEnable = false;
}

void ofApp::sendManagedOSCMessage(string _topic, int _signal) {
    
    if(gui_enable_osc) {
        ofxOscMessage m;
        m.setAddress(_topic);
        m.addIntArg(_signal);
        sender.sendMessage(m, false);
    } else {
        return;
    }
}

//--------------------------------------------------------------
void ofApp::update(){
    
    if(isSave) {
        gui_serial.saveToFile("serial_config.xml");
        isSave = false;
    }
    
    float e = ofGetElapsedTimef() - processTime;
    if (0 < e && e < 60 * 1) {
        flashProb = gui_minutes1;
    } else if (e < 60 && e < 60 * 2) {
        flashProb = gui_minutes2;
    } else if (e < 60 * 2 && e < 60 * 3) {
        flashProb = gui_minutes3;
    } else if (e < 60 * 3 && e < 60 * 4) {
        flashProb = gui_minutes4;
    } else if (e < 60 * 4 && e < 60 * 5) {
        flashProb = gui_minutes5;
    } else if (e < 60 * 5 && e < 60 * 6) {
        flashProb = gui_minutes6;
    } else if (e < 60 * 6 && e < 60 * 7) {
        flashProb = gui_minutes7;
    } else if (e < 60 * 7 && e < 60 * 8) {
        flashProb = gui_minutes8;
    } else if (e < 60 * 8 && e < 60 * 9) {
        flashProb = gui_minutes9;
    }
    
    nBytesRead = 0;
    int nRead = 0;
    char bytesRead[3];
    unsigned char bytesReturned[3];
    
    memset(bytesReturned, 0, 3);
    memset(bytesReadString, 0, 4);
    
    // シリアル通信で受け取ったデータを読み込む
    while ((nRead = sensor1_serial.readBytes(bytesReturned, 3)) > 0) {
        nBytesRead = nRead;
    };
    
    if (nBytesRead > 0) {
        memcpy(bytesReadString, bytesReturned, 3);
        int x = ofToInt(bytesReadString);
        
        if(x == 1 || x == 11) {
            ofLogWarning() << "Tilt Switch Signal : " << x;
            
            if(x == 1) {
                sendManagedOSCMessage("/tilt1", 1);
                ems2_serial.writeByte(1);
                
            }
            
            if (x == 11) {
                sendManagedOSCMessage("/tilt2", 1);
                ems1_serial.writeByte(1);
            }
            
        } else {
            
            ofLogWarning() << "Lead Switch Signal : " << x;
            sendManagedOSCMessage("/ir", 1);
            
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    

    
    fbo.begin();
    if(isTilt) {
        ofSetColor(255, 255, 255);
    } else {
        ofSetColor(0, 0, 0);
    }
    
    ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
    
    fbo.end();
    
    // draw bezier
    ofSetColor(255, 255, 255);
    bezManager.draw();
    
    if(isGuiShow) {
        gui_serial.draw();
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    string message;
    
    switch (key) {
        case ' ':
            ems1_serial.writeByte(1);
            break;
            
        case 'p':
            message = ofGetFrameNum();
            udpConnection.Send(message.c_str(),message.length());
            break;
            
        case 'd':
            if(isGuiShow) {
                isGuiShow = false;
            } else {
                isGuiShow = true;
            }
            
        case 's':
            if(!isSave) {
                isSave = true;
            }
        case 'o':
            processTime = ofGetElapsedTimef();
            break;
        case 'l':
            if(emsEnable) {
                emsEnable = false;
            } else {
                emsEnable = true;
            }
            ems2_serial.writeByte(0);
            ems1_serial.writeByte(0);
            
        default:
            break;
    }
    
    // send key event
    bezManager.keyPressed(key);
    
    // show / hide guide
    if (key == OF_KEY_RETURN) {
        bezManager.toggleGuideVisible();
    }
    // save settings
    if (key == 'u') {
        bezManager.saveSettings();
    }
    // load settings
    if (key == 'i') {
        bezManager.loadSettings();
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    switch (key) {
        case ' ':
            ems1_serial.writeByte(0);
            break;
            
        default:
            break;
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    // send drag event
    bezManager.mouseDragged(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    // send press event
    bezManager.mousePressed(x, y, button);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
