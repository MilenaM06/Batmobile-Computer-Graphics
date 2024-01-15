#version 330 core

layout(location = 0) in vec2 inPos;

uniform float uTachometer;

void main()
{

    vec2 fixedPoint = vec2(-0.48, -0.75);
    float length = 0.27;

    float angle = uTachometer * 0.0174 / 100 * 2;
    vec2 offset = vec2(cos(angle), -sin(angle)) * length;
    
    vec2 finalPos = fixedPoint - offset;

    if (gl_VertexID == 0)
        gl_Position = vec4(fixedPoint, 0.0, 1.0);
    else if (gl_VertexID == 1)
        gl_Position = vec4(finalPos, 0.0, 1.0);

}