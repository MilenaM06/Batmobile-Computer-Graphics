#version 330 core

out vec4 FragColor;
uniform float uSpeed;

void main()
{
    float t = clamp(uSpeed*150, 0.0, 1.0);

    vec4 yellowColor = vec4(1.0, 0.9, 0.2, 1.0);
    vec4 redColor = vec4(1.0, 0.0, 0.0, 1.0);
    FragColor = mix(yellowColor, redColor, t);
}