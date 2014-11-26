#include "testApp.h"

//--------------------------------------------------------------

Params param;

void testApp::createNewPoint(){
    bool born(false);

    while (born == false){
        int numVerts = mesh.getNumVertices();
        ofVec3f vert = mesh.getVertex(checkVert);
        float distance = vert.distance(lastPos);
            if (vert.z > 120 && distance > 80){            //CHANGE DISTANCE TO CHANGE CLOUD DENSITY
                float z = vert.z + ofRandom(100, 130);
                ofVec3f pos(vert.x, vert.y, z);

                Particle newP;
                newP.setup(pos);            //Start a new particle
                p.push_back( newP );     //Add this particle to array

                cloudMesh.addVertex(pos);
                cloudMesh.addColor(cloudCol);

                lastPos = pos;
                born = true;

                ofVec3f camPos = easyCam.getPosition();
                float camDist = camPos.distance(pos);
                cout << camDist << endl;
            }
            checkVert++;
            if (checkVert>numVerts){
                checkVert = 0;
            }
        }
    }


void testApp::setup(){
    ofSetFrameRate(60);
    recording = false;
    mesh.setMode(OF_PRIMITIVE_LINES);
    cloudMesh.setMode(OF_PRIMITIVE_LINES);

    image.loadImage("displace.png");
    image.resize(200, 200);

    cloudCol = ofColor(50);

    int w = image.getWidth();
    int h = image.getHeight();

    gui.setup();
    gui.add(windParam.setup("wind", 5, 0, 30));

    post.init(ofGetWidth(), ofGetHeight());
    //post.createPass<DofAltPass>();


    param.setup(windParam, 10, 0.4); //input de la vitesse du vent et du lifetime et du wiggle//TODO : GUI

    checkVert = 0;



    ///Get geography from image///

    for (int y=0; y<w; y++){
        if(y%4 == 0){
        for(int x=0; x<h; x++){
            ofColor col = image.getColor(x, y);
            float intensity = col.getLightness();
            float z = ofMap(intensity, 0, 255, 0, 200);

            ofVec3f pos(x*5, y*5, z);

            mesh.addVertex(pos);
            mesh.addColor(cloudCol);
        }
        }
    }

    ///Draw Geography

    for (int i=0; i<50; i++){
        for (int j=1; j<w; j++){
            mesh.addIndex(i*200+(j-1));
            mesh.addIndex(i*200+j);
        }
    }

    ///Generation des nuages

    cloudVerts = 0;

    lastPos = ofVec3f(0, 0, 0);
    int numVerts = mesh.getNumVertices();
    for (int i=0; i<numVerts; i++){
        ofVec3f vert = mesh.getVertex(i);
        float distance = vert.distance(lastPos);
        if (vert.z > 120 && distance > 80){            //CHANGE DISTANCE TO CHANGE CLOUD DENSITY
            float z = vert.z + ofRandom(100, 130);
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

    ///Draw cloudpoint///
	cloudMesh.clearIndices();
	for (int a=0; a<p.size(); a++){
            ofVec3f verta = cloudMesh.getVertex(a);
            for (int b=a+1; b<cloudVerts; b++){
                ofVec3f vertb = cloudMesh.getVertex(b);
                float distance = verta.distance(vertb);
                if (distance <= 50){
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
    ofBackground(230);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);


    post.begin(easyCam);
        ofPushMatrix();
            ofTranslate(-ofGetWidth()/2, -ofGetHeight()/2);
            mesh.draw();
            cloudMesh.draw();
            //post.debugDraw();
            //cout << easyCam.getPosition() << endl;
            if (recording){
                ofSaveFrame();
            }
        ofPopMatrix();
    post.end();

    if (showGui){
    gui.draw();
    }

}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    if (key == 'r'){
        recording = !recording;
    }

    if (key == 'h'){
        showGui = !showGui;
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
    wind = ofVec3f(ofRandom(windInput*0.6, windInput*1.4), 0, 0);
    lifetime = lifetimeInput;
    wiggle = Wiggle;

}
