#version 100
precision mediump float;

varying vec2 TexCoord;

uniform vec2 objectSize;
uniform sampler2D tex;
uniform vec3 colorChange;
uniform vec3 color;
uniform bool useColor;

void main() {
  vec2 pixelPos = TexCoord * objectSize;

  if (TexCoord.x > 0.1 && TexCoord.x < 0.9 && TexCoord.y > 0.1 && TexCoord.y < 0.9) {
    discard;
  }

  gl_FragColor = vec4(
    color.rgb,
    1.0
  );
}
