#ifndef OFXTERRAIN_H_INCLUDED
#define OFXTERRAIN_H_INCLUDED

#include "Particle.h"

class ofxTerrain
{
    public:

    void setup();
    void draw();
    void displayLocations(ofEasyCam camera);
    void addLocation(int Vertex, string title);

    ofMesh mesh;
    float scale;

    //Clouds
    void setupClouds();
    void updateClouds(int minCloudHeight, int maxCloudHeight, ofVec3f center, int radius);
    void createNewPoint();
    bool cloudSetup;
    ofMesh cloudMesh;
    vector<int> highPoints;
    float time0;
    ofColor cloudCol;
    int checkVert;
    int cloudVerts;
    int minCloudHeight, maxCloudHeight;
    int radius;
    ofVec3f center;
    ofVec3f lastPos;
    vector<Particle> p;

    ofImage image;
    vector<string> locationsName;
    vector<ofVec3f> locationsVert;
    ofEasyCam easyCam;
};


#endif // OFXTERRAIN_H_INCLUDED
