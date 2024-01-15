#version 330 core

out vec4 outCol;

uniform float uTachometer;

void main()
{

    float t;
    if (uTachometer < 5000.00) {
        t = clamp(uTachometer / 5000.00, 0.0, 1.0);
    } else {
        t = 1.0;
    }
    
    vec4 yellowColor = vec4(1.0, 0.9, 0.2, 1.0);
    vec4 redColor = vec4(1.0, 0.0, 0.0, 1.0);
    outCol = mix(yellowColor, redColor, t);
}