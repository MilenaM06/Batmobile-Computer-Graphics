#version 330 core

out vec4 outCol;

uniform float uSpeed;

void main()
{
    if (uSpeed < 30.0)
        outCol = vec4( 1.0, 0.9, 0.2, 1.0); 
    else
        outCol = vec4(1.0, 0.0, 0.0, 1.0); 
}