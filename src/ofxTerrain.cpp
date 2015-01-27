#include "ofxTerrain.h"
#include "Particle.h"

//using namespace std;

void ofxTerrain::setup()
{
    ///Font Setup///
    ofTrueTypeFont::setGlobalDpi(72);

	locationsFont.loadFont("MavenProMedium.otf", 18, true, true);
	locationsFont.setLineHeight(18.0f);
	locationsFont.setLetterSpacing(1.037);

    ///Terrain Setup///
    mesh.setMode(OF_PRIMITIVE_LINES);
    cloudMesh.setMode(OF_PRIMITIVE_LINES);

    image.loadImage("displace.png");
    image.resize(600, 600);

    scale = 0.04;
    cloudSetup = false;

    ///Get geography from image///

    int createdVertex = 0;
    ofColor mapColor(230, 230, 230, 255);

    int w = image.getWidth();
    int h = image.getHeight();
    for (int y=0; y<w; y++)
    {
        //if(y%4 == 0){
        for(int x=0; x<h; x++)
        {
            ofColor col = image.getColor(x, y);
            float intensity = col.getLightness();
            float z = ofMap(intensity, 0, 255, 0, 4810*scale);  //converts lightness of the pixel to height of the vertex

            ofVec3f pos(x*6, y*6, z);
            mapColor.a = ofMap(intensity, 0, 220, 100, 255);  //makes the flat areas more transparent

            mesh.addVertex(pos);
            mesh.addColor(mapColor);
            if(z > 2500*scale && x%4 == 0)
            {
                highPoints.push_back(createdVertex);  //Array containing the points above a certain altitude
            }
            createdVertex++;
        }
        //}
    }

    ///Draw Geography

    for (int i=0; i<600; i++)
    {
        for (int j=1; j<600; j++)
        {
            mesh.addIndex(i*600+(j-1));
            mesh.addIndex(i*600+j);
        }
    }
}

void ofxTerrain::draw()
{
    mesh.draw();

    if(cloudSetup)
    {
       cloudMesh.draw();
       //cout << highPoints.size() << endl;
    }

}

void ofxTerrain::setupClouds()
{
    cloudCol = ofColor(230, 230, 230, 200);
    checkVert = 0;
    cloudSetup = true;

    ///Generation des nuages

    cloudVerts = 0;

    lastPos = ofVec3f(0, 0, 0);
    for (int i=0; i<highPoints.size(); i++)
    {
        ofVec3f vert = mesh.getVertex(highPoints[i]);
        float distance = vert.distance(lastPos);
        if (distance > 40)             //CHANGE DISTANCE TO CHANGE CLOUD DENSITY --doesn't work anymore I think
        {
            float z = vert.z + ofRandom(3000*scale, 1500*scale);
            ofVec3f pos(vert.x, vert.y, z);

            Particle newP;
            newP.setup(pos);            //Start a new particle
            p.push_back( newP );     //Add this particle to array

            cloudMesh.addVertex(pos);
            cloudMesh.addColor(cloudCol);

            //offsets.push_back(ofVec3f(ofRandom(0, 100000), ofRandom(0, 100000), ofRandom(0, 100000)));
            cloudVerts++;
            lastPos = pos;
        }
    }

    time0 = ofGetElapsedTimef();
}

void ofxTerrain::updateClouds(int MinCloudHeight, int MaxCloudHeight, ofVec3f Center, int Radius)
{
    minCloudHeight = MinCloudHeight;
    maxCloudHeight = MaxCloudHeight;
    center = Center;
    radius = Radius;

    ofColor transparentCloud = ofColor(230, 230, 230, 0);

    ///Time initialisation///
    float time = ofGetElapsedTimef();
    float dt = ofClamp( time - time0, 0, 0.1 );
    time0 = time;


    ///Remove dead particle and replace them///
    int i=0;
    while (i < p.size())
    {
        if ( !p[i].live )
        {
            p.erase( p.begin() + i );
            cloudMesh.removeVertex(i);
            createNewPoint();
        }
        else
        {
            i++;
        }
    }

    ///Draw cloudpoint///
    cloudMesh.clearIndices();
    for (int a=0; a<p.size(); a++)
    {
        ofVec3f verta = cloudMesh.getVertex(a);
        for (int b=a+1; b<cloudVerts; b++)
        {
            ofVec3f vertb = cloudMesh.getVertex(b);
            float distance = verta.distance(vertb);
            if (distance <= 40)
            {
                cloudMesh.addIndex(a);
                cloudMesh.addIndex(b);
            }
        }
    }


    ///Refresh vertex position///
    for (int i=0; i<p.size(); i++)
    {
        p[i].update( dt );
        cloudMesh.setVertex(i, p[i].pos);
        cloudMesh.setColor(i, cloudCol.getLerped(transparentCloud, p[i].timeLeft()));
    }

    ///Update map visible radius
    int numVerts = mesh.getNumVertices();
    ofColor mapColor(230, 230, 230, 0);
    //ofVec3f centerVec(center->x, center->y, center->z);
        for(int i=0; i<numVerts; i++)
        {
            ofVec3f vert = mesh.getVertex(i);
            float distance = ofMap(vert.distance(Center), 0, radius, 255, 0, true);
            mapColor.a = distance;
            mesh.setColor(i, mapColor);
            /*if(distance < 10){
                mesh.setVertex(i, ofVec3f(vert.x, vert.y, 0));
            }*/

        }
}

void ofxTerrain::createNewPoint()
{
    bool vertAdded = false;

    int random = ofRandom(0, 3);
    if(random == 0)  //1 chance in 4 to change checkVertex (avoid clouds appearing in scan lines)
    {
        checkVert += 20;
    }

    if(checkVert >= highPoints.size())
    {
        checkVert = 0;
    }

    while (vertAdded == false)
    {
        ofVec3f vert = mesh.getVertex(highPoints[checkVert]);
        if(vert.distance(center) < radius)
        {
            vert.z += ofRandom(maxCloudHeight*scale, minCloudHeight*scale);

            Particle newP;
            newP.setup(vert);        //Start a new particle
            p.push_back( newP );     //Add this particle to array

            cloudMesh.addVertex(vert);
            cloudMesh.addColor(cloudCol);
            vertAdded = true;
        }
        checkVert++;
        if(checkVert >= highPoints.size())
        {
            checkVert = 0;
        }
    }
}

void ofxTerrain::addLocation(int Vertex, string title)
{
    ofVec3f vertex = mesh.getVertex(Vertex);
    locationsVert.push_back(vertex);
    locationsName.push_back(title);
}

void ofxTerrain::displayLocations(ofEasyCam camera)
{
    for(int i=0; i<locationsVert.size(); i++)
    {
        ofVec3f pos = camera.worldToScreen(locationsVert[i]);

        //Point
        ofFill();
        ofSetColor(202, 88, 82, 190);
        ofCircle(pos.x-5, pos.y+5, 3, 3);
        //Rectangle
        ofRectangle rect = locationsFont.getStringBoundingBox(locationsName[i], pos.x, pos.y);
        ofSetColor(240, 255);
        ofRectRounded(rect.x-5, rect.y-4, rect.width*1.1, rect.height*1.4, 5);
        //String
        ofSetColor(202, 88, 82, 255);
        locationsFont.drawString(locationsName[i], pos.x, pos.y);
    }
}
