#version 150

// these are for the programmable pipeline system
uniform mat4 modelViewProjectionMatrix;
in vec4 position;

// the time value is passed into the shader by the OF app.
uniform float mouseRange;
uniform vec2 mousePos;
uniform vec4 mouseColor;


void main()
{
    vec4 pos = position;
    gl_Position = pos;
}
