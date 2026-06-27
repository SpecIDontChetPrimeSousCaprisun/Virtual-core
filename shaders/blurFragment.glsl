#version 100

precision mediump float;

uniform sampler2D scene;
uniform vec2 texelSize;

varying vec2 uv;

void main()
{
    vec3 color = vec3(0.0);

    color += texture2D(scene, uv + vec2(-8.0, 0.0) * texelSize).rgb * 0.05;
    color += texture2D(scene, uv + vec2(-6.0, 0.0) * texelSize).rgb * 0.09;
    color += texture2D(scene, uv + vec2(-4.0, 0.0) * texelSize).rgb * 0.12;
    color += texture2D(scene, uv + vec2(-2.0, 0.0) * texelSize).rgb * 0.15;

    color += texture2D(scene, uv).rgb * 0.18;

    color += texture2D(scene, uv + vec2( 2.0, 0.0) * texelSize).rgb * 0.15;
    color += texture2D(scene, uv + vec2( 4.0, 0.0) * texelSize).rgb * 0.12;
    color += texture2D(scene, uv + vec2( 6.0, 0.0) * texelSize).rgb * 0.09;
    color += texture2D(scene, uv + vec2( 8.0, 0.0) * texelSize).rgb * 0.05;

    gl_FragColor = vec4(color, 1.0);
}
