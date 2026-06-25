#version 100
precision mediump float;

varying vec2 TexCoord;

uniform vec2 objectSize;
uniform float cornerRadius;
uniform float alpha;
uniform sampler2D tex;
uniform vec3 colorChange;
uniform vec3 color;
uniform bool useColor;

void main() {
  vec2 pixelPos = TexCoord * objectSize;
  float r = cornerRadius;

  if (r > 0.0) {
      bool inCorner = false;
      vec2 corner;

      // top-left
      if (pixelPos.x < r && pixelPos.y < r) {
          corner = vec2(r, r);
          inCorner = true;
      }
      // top-right
      else if (pixelPos.x > objectSize.x - r &&
              pixelPos.y < r) {
          corner = vec2(objectSize.x - r, r);
          inCorner = true;
      }
      // bottom-left
      else if (pixelPos.x < r &&
              pixelPos.y > objectSize.y - r) {
          corner = vec2(r, objectSize.y - r);
          inCorner = true;
      }
      // bottom-right
      else if (pixelPos.x > objectSize.x - r &&
              pixelPos.y > objectSize.y - r) {
          corner = vec2(objectSize.x - r,
                        objectSize.y - r);
          inCorner = true;
      }

      if (inCorner && distance(pixelPos, corner) > r)
          discard;
  }

  vec4 finalColor;

  if (useColor) {
    finalColor = vec4(color, 1);
  } else {
    finalColor = texture2D(tex, TexCoord);
  }

  gl_FragColor = vec4(
      finalColor.rgb + colorChange,
      finalColor.a * alpha
  );
}
