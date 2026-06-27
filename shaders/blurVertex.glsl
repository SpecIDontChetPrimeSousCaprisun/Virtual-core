#version 100

attribute vec2 position;
attribute vec2 texCoord;

varying vec2 uv;

void main()
{
    uv = texCoord;
    gl_Position = vec4(position, 0.0, 1.0);
}
