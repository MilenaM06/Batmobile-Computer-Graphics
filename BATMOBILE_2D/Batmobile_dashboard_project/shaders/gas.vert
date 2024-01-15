#version 330 core

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec3 inCol;

out vec3 colVec;

uniform float uGas;

void main(){

	if (gl_VertexID == 0 || gl_VertexID == 1)
        gl_Position = vec4(inPos.x, inPos.y, 0.0, 1.0);
     else
        gl_Position = vec4(inPos.x, inPos.y + uGas, 0.0, 1.0);

	colVec = inCol;
}