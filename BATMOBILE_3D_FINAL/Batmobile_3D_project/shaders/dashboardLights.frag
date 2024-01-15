#version 330 core 

out vec4 outCol;

uniform bool uLightActive;
uniform vec3 uLightColor;

void main()
{

    if (uLightActive == true) {
        outCol = vec4(uLightColor, 1.0); 
    }else{
        outCol = vec4(0.0, 0.0, 0.0, 1.0);
    }
};