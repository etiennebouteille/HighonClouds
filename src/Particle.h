#ifndef PARTICLE_H_INCLUDED
#define PARTICLE_H_INCLUDED

#include "ofMain.h"

class Params { //Pas sur que ca soit utile
public:
    Params();
    void setup(float windInput, float lifetimeInput, float Wiggle);
    ofVec3f wind;
    int lifetime;
    float wiggle;
};

class Particle {
public:
    Particle();
    void setup(ofVec3f startPos); //input d'une valeur pour réduire ou allonger la vie de la particule
    void update(float dt);
    void draw();

    ofVec3f pos;
    ofVec3f wind;
    float time;
    float lifetime;
    bool live;

    Params param;
};



#endif // PARTICLE_H_INCLUDED
