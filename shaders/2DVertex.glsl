#version 100

attribute vec2 aPos;
attribute vec2 aUV;

varying vec2 TexCoord;

uniform mat4 model;
uniform mat4 projection;

void main()
{
    gl_Position = projection * model * vec4(aPos, 0.0, 1.0);
    TexCoord = aUV;
}
