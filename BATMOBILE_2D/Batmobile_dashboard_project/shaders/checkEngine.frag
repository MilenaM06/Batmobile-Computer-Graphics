#version 330 core 

out vec4 outCol;

uniform bool uCheckEngine;
uniform float uTime;

void main()
{

    if ((uCheckEngine == true) && (sin(uTime * 15.0) > 0.0)) {
        outCol = vec4(1.0, 0.9, 0.2, 1.0); 
    }else{
        outCol = vec4(0.0, 0.0, 0.0, 1.0);
    }
};