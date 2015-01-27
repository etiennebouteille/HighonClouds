#include "Particle.h"

Particle::Particle()
{
    live = false;
}

void Particle::setup(ofVec3f startPos)
{
    ofColor c = ofColor(230);

    pos = startPos;
    wind = param.wind;
    wind.z = ofRandom(wind.z * 0.8, wind.z *1.2);
    time = 0;
    float lt = param.lifetime;
    lifetime = ofRandom(lt*0.5, lt*1.5);
    live = true;

}

void Particle::update(float dt)
{
    if ( live )
    {
        //Update pos
        pos += wind * dt;    //Euler method
        pos.z += ofSignedNoise(time) * param.wiggle;

        //Update time and check if particle should die
        time += dt;
        if ( time >= lifetime )
        {
            live = false;   //Particle is now considered as died
        }
    }
}

float Particle::timeLeft()
{
    return (time/lifetime);
}

void Particle::draw()
{

}

///Params class///

Params::Params()
{
    wind = ofVec3f(5, 0, 0);
    lifetime = 10;
    wiggle = 0.6;
}

void Params::setup(float windInput, float lifetimeInput, float Wiggle)
{
    wind = ofVec3f(windInput, 0, 0);
    lifetime = lifetimeInput;
    wiggle = Wiggle;
}

