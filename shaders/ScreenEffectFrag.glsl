#version 100
precision mediump float;

varying vec2 TexCoord;

uniform vec2 objectSize;
uniform sampler2D tex;
uniform vec3 colorChange;
uniform vec3 color;
uniform bool useColor;

void main() {
  // normalized distance from nearest edge (0 = at edge, 1 = past the border)
  float dx = min(TexCoord.x, 1.0 - TexCoord.x) / 0.1;
  float dy = min(TexCoord.y, 1.0 - TexCoord.y) / 0.1;
  float edgeFactor = min(dx, dy);

  // discard center pixels
  if (edgeFactor >= 1.0) discard;

  // alpha is 1 at the edge, 0 at the border
  float alpha = 1.0 - clamp(edgeFactor, 0.0, 1.0);

  gl_FragColor = vec4(color.rgb, alpha);
}
