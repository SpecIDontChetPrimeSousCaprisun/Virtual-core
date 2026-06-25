#version 100

attribute vec2 aPos;
attribute vec2 aUV;

varying vec2 TexCoord;

uniform mat4 projection;
uniform vec2 pivot;
uniform float rotation;

void main()
{
    TexCoord = aUV;

    float rad = radians(rotation);

    mat2 rot = mat2(
        cos(rad), -sin(rad),
        sin(rad),  cos(rad)
    );

    vec2 rotatedPos = pivot + rot * (aPos - pivot);

    gl_Position = projection * vec4(rotatedPos, 0.0, 1.0);
}
