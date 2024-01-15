#version 330 core

layout(location = 0) in vec2 inPos;
layout(location = 1) in vec3 inCol;

out vec3 colVec;

uniform float uOdometer;

void main(){

     if (gl_VertexID == 0 || gl_VertexID == 1)
        gl_Position = vec4(inPos.x, inPos.y, 0.0, 1.0);
     else{
     if(uOdometer == 10.0){
            gl_Position = vec4(inPos.x, 1.0, 0.0, 1.0);
      }else if(uOdometer >= 9.0){
            gl_Position = vec4(inPos.x, 0.9, 0.0, 1.0);
      }else if(uOdometer >= 8.0){
            gl_Position = vec4(inPos.x, 0.8, 0.0, 1.0);
      }else if(uOdometer >= 7.0){
            gl_Position = vec4(inPos.x, 0.7, 0.0, 1.0);
      }else if(uOdometer >= 6.0){
            gl_Position = vec4(inPos.x, 0.6, 0.0, 1.0);
      }else if(uOdometer >= 5.0){
            gl_Position = vec4(inPos.x, 0.5, 0.0, 1.0);
      }else if(uOdometer >= 4.0){
            gl_Position = vec4(inPos.x, 0.4, 0.0, 1.0);
      }else if(uOdometer >= 3.0){
            gl_Position = vec4(inPos.x, 0.3, 0.0, 1.0);
      }else if(uOdometer >= 2.0){
            gl_Position = vec4(inPos.x, 0.2, 0.0, 1.0);
      }else if(uOdometer >= 1.0){
            gl_Position = vec4(inPos.x, 0.1, 0.0, 1.0);
      }
	 }

	colVec = inCol;
}