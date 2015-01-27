#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxPostProcessing.h"
#include "ofxTerrain.h"




class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void translateCam();

		bool recording;
		bool showGui;

		int cloudVerts;
		int checkVert;

		ofxTerrain terrain;
		ofMesh mesh;

		ofEasyCam easyCam;

		ofVec3f lastCenter; //center of the map's field of view

		ofxPanel gui;
		ofxFloatSlider windParam;
		ofxVec3Slider center;
		ofxFloatSlider radius;
		ofxIntSlider maxCloudHeight, minCloudHeight;
		ofxIntSlider vertexTarget;

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

};
