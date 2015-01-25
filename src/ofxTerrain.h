#ifndef OFXTERRAIN_H_INCLUDED
#define OFXTERRAIN_H_INCLUDED

#include "ofMain.h"

class ofxTerrain
{
    public:

    void setup();
    void draw();
    void displayLocations(ofEasyCam camera);
    void addLocation(ofVec3f vertex, string title);

    ofMesh mesh;
    ofImage image;
    vector<string> locationsName;
    vector<ofVec3f> locationsVert;
    ofEasyCam easyCam;
};


#endif // OFXTERRAIN_H_INCLUDED
