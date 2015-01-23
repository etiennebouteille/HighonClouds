#include "ofxTerrain.h"
#include "ofMain.h"

using namespace std;

void ofxTerrain::setup()
{
    mesh.setMode(OF_PRIMITIVE_LINES);
    vector<int> highPoints;

    image.loadImage("displace.png");
    image.resize(600, 600);

    float scale = 0.04;

    ///Get geography from image///

    int createdVertex = 0;
    ofColor mapColor(230, 230, 230, 255);

    int w = image.getWidth();
    int h = image.getHeight();
    for (int y=0; y<w; y++){
        //if(y%4 == 0){
        for(int x=0; x<h; x++){
            ofColor col = image.getColor(x, y);
            float intensity = col.getLightness();
            float z = ofMap(intensity, 0, 255, 0, 4810*scale);  //converts lightness of the pixel to height of the vertex

            ofVec3f pos(x*6, y*6, z);
            mapColor.a = ofMap(intensity, 0, 220, 100, 255);  //makes the flat areas more transparent

            mesh.addVertex(pos);
            mesh.addColor(mapColor);
            if(z > 2500*scale && x%4 == 0){
                highPoints.push_back (createdVertex);  //Array containing the points above a certain altitude
            }
            createdVertex++;
        }
        //}
    }

    ///Draw Geography

    for (int i=0; i<600; i++){
        for (int j=1; j<600; j++){
            mesh.addIndex(i*600+(j-1));
            mesh.addIndex(i*600+j);
        }
    }
}

void ofxTerrain::draw()
{
    mesh.draw();
}
