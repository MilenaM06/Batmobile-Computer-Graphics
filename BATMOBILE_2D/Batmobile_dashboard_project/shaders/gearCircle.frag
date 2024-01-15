#version 330 core 

out vec4 outCol;

uniform int uGear;
uniform float uTime;
void main()
{
   if (uGear == 0  && (sin(uTime * uGear * 3) > 0.0))
        outCol = vec4(0.0, 0.0, 0.0, 1.0); 
   else if(uGear == 1 && (sin(uTime * uGear * 3) > 0.0))
        outCol = vec4(1.0, 0.0, 0.0, 1.0);  //red
   else if(uGear == 2 && (sin(uTime * uGear * 3) > 0.0))
        outCol = vec4(1.0, 0.5, 0.0, 1.0);  //orange
   else if(uGear == 3 && (sin(uTime * uGear * 3) > 0.0))
        outCol = vec4(0.5, 0.0, 0.5, 1.0);  //purple
   else if(uGear == 4 && (sin(uTime * uGear * 3) > 0.0))
        outCol = vec4(0.0, 0.0, 1.0, 1.0);  //blue
   else if(uGear == 5 && (sin(uTime * uGear * 3) > 0.0))
        outCol = vec4(0.0, 1.0, 0.0, 1.0);  ///green
    else
        outCol = vec4(0.0, 0.0, 0.0, 1.0);

};