#version 330 core

uniform float uSpeed;
uniform vec3 uFixedPoint;
mat3 rotationMatrix(float angle)
{
    float c = cos(angle);
    float s = sin(angle);

    return mat3(
        c, -s, 0.0,
        s, c, 0.0,
        0.0, 0.0, 1.0
    );
}

mat4 orthoProjection(float left, float right, float bottom, float top, float near, float far)
{
    return mat4(
        2.0 / (right - left), 0.0, 0.0, -(right + left) / (right - left),
        0.0, 2.0 / (top - bottom), 0.0, -(top + bottom) / (top - bottom),
        0.0, 0.0, -2.0 / (far - near), -(far + near) / (far - near),
        0.0, 0.0, 0.0, 1.0
    );
}

void main()
{
    float length = 0.18;

    float aspectRatio = 2.0;
    
    float angle = uSpeed * 300.0;
    vec3 offset = rotationMatrix(angle) * vec3(length * aspectRatio, 0.0, 0.0);
    
    vec3 finalPos = uFixedPoint - offset;

    mat4 projection = orthoProjection(-1.0 * aspectRatio, 1.0 * aspectRatio, -1.0, 1.0, -1.0, 1.0);

    if (gl_VertexID == 0)
        gl_Position = projection * vec4(uFixedPoint, 1.0);
    else if (gl_VertexID == 1)
        gl_Position = projection * vec4(finalPos, 1.0);
}
