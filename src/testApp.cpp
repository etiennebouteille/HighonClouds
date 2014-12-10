#include "testApp.h"

//--------------------------------------------------------------

Params param;

void testApp::createNewPoint(){
    bool vertAdded = false;

    int random = ofRandom(0, 3);
    if(random == 0){ //1 chance in 4 to change checkVertex (avoid clouds appearing in scan lines
        checkVert += 20;
    }

    if(checkVert >= highPoints.size()){
            checkVert = 0;
    }

    while (vertAdded == false){
    ofVec3f vert = mesh.getVertex(highPoints[checkVert]);
    if(vert.distance(center) < radius){
    vert.z += ofRandom(100*0.015, 3000*0.015);

    Particle newP;
    newP.setup(vert);            //Start a new particle
    p.push_back( newP );     //Add this particle to array

    cloudMesh.addVertex(vert);
    cloudMesh.addColor(cloudCol);
    vertAdded = true;
    }
    checkVert++;
    if(checkVert >= highPoints.size()){
            checkVert = 0;
    }
    }
}


void testApp::setup(){
    //ofSetFrameRate(60);
    recording = false;
    mesh.setMode(OF_PRIMITIVE_LINES);
    cloudMesh.setMode(OF_PRIMITIVE_LINES);
    lastCenter = ofVec3f(0, 0, 0);

    image.loadImage("displace.png");
    image.resize(600, 600);

    cloudCol = ofColor(230, 230, 230, 220);

    gui.setup();
    gui.add(windParam.setup("wind", 5, 0, 30));
    gui.add(center.setup("Center", ofVec3f(1200, 1200, 0), ofVec3f(0, 0, -500), ofVec3f(2400, 2400, 500)));
    gui.add(radius.setup("Center", 800, 0, 1500));
    showGui = false;

    param.setup(windParam, 10, 0.6); //input de la vitesse du vent et du lifetime et du wiggle//TODO : GUI

    checkVert = 0;



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
            float z = ofMap(intensity, 0, 255, 0, 4810*0.015);  //converts lightness of the pixel to height of the vertex

            ofVec3f pos(x*4, y*4, z);
            mapColor.a = ofMap(intensity, 0, 220, 100, 255);  //makes the flat areas more transparent

            mesh.addVertex(pos);
            mesh.addColor(mapColor);
            if(z > 2500*0.015 && x%4 == 0){
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

    ///Generation des nuages

    cloudVerts = 0;

    lastPos = ofVec3f(0, 0, 0);
    for (int i=0; i<highPoints.size(); i++){
        ofVec3f vert = mesh.getVertex(highPoints[i]);
        float distance = vert.distance(lastPos);
            if (distance > 40){            //CHANGE DISTANCE TO CHANGE CLOUD DENSITY --doesn't work anymore I think
                float z = vert.z + ofRandom(100*0.015, 3000*0.015);
                ofVec3f pos(vert.x, vert.y, z);

                Particle newP;
                newP.setup(pos);            //Start a new particle
                p.push_back( newP );     //Add this particle to array

                cloudMesh.addVertex(pos);
                cloudMesh.addColor(cloudCol);

                offsets.push_back(ofVec3f(ofRandom(0, 100000), ofRandom(0, 100000), ofRandom(0, 100000)));
                cloudVerts++;
                lastPos = pos;
                }
    }

    time0 = ofGetElapsedTimef();
    cout << "highPoints = " << highPoints.size() << endl;

}

//--------------------------------------------------------------
void testApp::update(){

    param.setup(windParam, 20, 0.4);

    ///Time initialisation///
    float time = ofGetElapsedTimef();
	float dt = ofClamp( time - time0, 0, 0.1 );
	time0 = time;


	///Remove dead particle and replace them///
	int i=0;
	while (i < p.size()) {
		if ( !p[i].live ) {
			p.erase( p.begin() + i );
			cloudMesh.removeVertex(i);
			createNewPoint();
		}
		else {
			i++;
		}
	}

	///Update map visibility
	int numVerts = mesh.getNumVertices();
	ofColor mapColor(230, 230, 230, 0);
	ofVec3f centerVec(center->x, center->y, center->z);
	//if (centerVec.x != lastCenter.x || centerVec.y != lastCenter.y || centerVec.z != lastCenter.z){
	if(showGui){
	for(int i=0; i<numVerts; i++){
        ofVec3f vert = mesh.getVertex(i);
        float distance = ofMap(vert.distance(centerVec), 0, radius, 255, 0, true);
        mapColor.a = distance;
        mesh.setColor(i, mapColor);
        /*if(distance < 10){
            mesh.setVertex(i, ofVec3f(vert.x, vert.y, 0));
        }*/

	}
	}
	lastCenter = centerVec;

    ///Draw cloudpoint///
	cloudMesh.clearIndices();
	for (int a=0; a<p.size(); a++){
            ofVec3f verta = cloudMesh.getVertex(a);
            for (int b=a+1; b<cloudVerts; b++){
                ofVec3f vertb = cloudMesh.getVertex(b);
                float distance = verta.distance(vertb);
                if (distance <= 40){
                    cloudMesh.addIndex(a);
                    cloudMesh.addIndex(b);
                }
            }
    }


    ///Refresh vertex position///
	for (int i=0; i<p.size(); i++) {
		p[i].update( dt );
		ofVec3f vertUpdate = p[i].pos;
		cloudMesh.setVertex(i, vertUpdate);
	}

}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(28);

    easyCam.begin();
        ofPushMatrix();
            ofTranslate(-ofGetWidth()*0.8, -ofGetHeight());
            mesh.draw();
            cloudMesh.draw();
            if (recording){
                ofSaveFrame();
            }
        ofPopMatrix();
    easyCam.end();

    if (showGui){
    gui.draw();
    }

    ofDrawBitmapString(ofToString(ofGetFrameRate())+"fps", 10, 15);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == 'r'){
        recording = !recording;
    }

    if (key == 'h'){
        showGui = !showGui;
    }

    if (key == 's'){
        ofSaveFrame();
    }

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){

}

//--------------------------------------------------------------
//-----------------------   Particle   -------------------------
//--------------------------------------------------------------
Particle::Particle(){
    live = false;
}

void Particle::setup(ofVec3f startPos){

    ofColor c = ofColor(230);

    pos = startPos;
    wind = param.wind;
    wind.z = ofRandom(wind.z * 0.8, wind.z *1.2);
    time = 0;
    float lt = param.lifetime;
    lifetime = ofRandom(lt*0.5, lt*1.5);
    live = true;

}

void Particle::update(float dt){
    if ( live ) {
		//Update pos
		pos += wind * dt;    //Euler method
		pos.z += ofSignedNoise(time) * param.wiggle;

		//Update time and check if particle should die
		time += dt;
		if ( time >= lifetime ) {
			live = false;   //Particle is now considered as died
		}
	}

}

void Particle::draw(){

}


//--------------------------------------------------------------
//----------------------  Params -------------------------------
//--------------------------------------------------------------

void Params::setup(float windInput, float lifetimeInput, float Wiggle) {
    wind = ofVec3f(windInput, 0, 0);
    lifetime = lifetimeInput;
    wiggle = Wiggle;
}
