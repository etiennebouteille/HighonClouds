#include "testApp.h"

//--------------------------------------------------------------

Params param;

void testApp::translateCam()
{
    ofVec3f targetVert = mesh.getVertex(12000);
    ofPushMatrix();
    easyCam.setPosition(easyCam.getX(), targetVert.y, easyCam.getZ());
    easyCam.setTarget(targetVert);
    ofPopMatrix();
}


void testApp::setup()
{
    //ofSetFrameRate(60);
    recording = false;

    gui.setup();
    gui.add(windParam.setup("wind", 5, 0, 30));
    gui.add(center.setup("Center", ofVec3f(1200, 1200, 0), ofVec3f(0, 0, -500), ofVec3f(2400, 2400, 500)));
    gui.add(radius.setup("Radius", 800, 0, 1500));
    gui.add(maxCloudHeight.setup("maxCloudHeight", 3000, 0, 6000));
    gui.add(minCloudHeight.setup("minCloudHeight", 1500, 0, 4000));
    gui.add(vertexTarget.setup("Vertex Target", 96710, 95000, 97000));
    showGui = false;

    param.setup(windParam, 10, 0.6); //input de la vitesse du vent et du lifetime et du wiggle//TODO : GUI

    terrain.setup();
    terrain.setupClouds();

    terrain.addLocation(vertexTarget, "Chamonix");
    terrain.addLocation(76900, "Annecy");
    terrain.addLocation(87000, "Courmayeur");
}

//--------------------------------------------------------------
void testApp::update()
{
    param.setup(windParam, 20, 0.4);
    terrain.updateClouds(minCloudHeight, maxCloudHeight, center, radius);
}


//--------------------------------------------------------------
void testApp::draw()
{
    ofBackground(28);

    easyCam.begin();
    ofPushMatrix();
    ofTranslate(-ofGetWidth()*0.8, -ofGetHeight());
    terrain.draw();

    if (recording)
    {
        ofSaveFrame();
    }
    ofPopMatrix();
    easyCam.end();

    terrain.displayLocations(easyCam);

    if (showGui)
    {
        gui.draw();
    }

    //ofDrawBitmapString(ofToString(ofGetFrameRate())+"fps", 10, 15);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key)
{
    if (key == 'r')
    {
        recording = !recording;
    }

    if (key == 'h')
    {
        showGui = !showGui;
    }

    if (key == 's')
    {
        ofSaveFrame();
    }

    if (key == 't')
    {
        translateCam();
    }

}

//--------------------------------------------------------------
void testApp::keyReleased(int key)
{

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y )
{

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg)
{

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo)
{

}

