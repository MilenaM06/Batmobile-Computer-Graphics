#version 330 core 

in vec3 colVec;
out vec4 outCol;

void main()
{
    outCol = vec4(colVec, 1.0);
};