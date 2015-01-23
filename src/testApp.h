#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxPostProcessing.h"
#include "ofxTerrain.h"


class Params { //Pas sur que ca soit utile
public:
    void setup(float windInput, float lifetimeInput, float Wiggle);
    ofVec3f wind;
    int lifetime;
    float wiggle;
};

class Particle {
public:
    Particle();
    void setup(ofVec3f startPos); //input d'un valeur pour réduire ou allonger la vie de la particule
    void update(float dt);
    void draw();

    ofVec3f pos;
    ofVec3f wind;
    float time;
    float lifetime;
    bool live;

};

class testApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();
		void createNewPoint();
		void translateCam();

		bool recording;
		bool showGui;

		int cloudVerts;
		int checkVert;

		ofxTerrain terrain;

        float time0;
		ofMesh mesh;
		ofMesh cloudMesh;
		ofMesh overlay;

		ofImage image;
		ofEasyCam easyCam;

		ofColor cloudCol;

		ofVec3f lastPos;
		ofVec3f lastCenter; //center of the map's field of view

		ofShader shader;

		vector<ofVec3f> offsets; //offsets for Perlin noise
		vector<Particle> p;

		vector<int> highPoints;

		ofxPanel gui;
		ofxFloatSlider windParam;
		ofxVec3Slider center;
		ofxFloatSlider radius;
		ofxIntSlider maxCloudHeight, minCloudHeight;

		float scale; //scale for geography

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
