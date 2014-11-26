#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxPostProcessing.h"


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
    void setup(ofVec3f startPos); //input d'un valeur pour réduite ou allonger la vie de la particule
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

		bool recording;
		bool showGui;

		int cloudVerts;
		int checkVert;

        float time0;
		ofMesh mesh;
		ofMesh cloudMesh;
		ofImage image;
		ofEasyCam easyCam;

		ofColor cloudCol;

		ofVec3f lastPos;

		ofShader shader;

		vector<ofVec3f> offsets; //offsets for Perlin noise

		vector<Particle> p;

		ofxPanel gui;
		ofxFloatSlider windParam;

		ofxPostProcessing post;

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
