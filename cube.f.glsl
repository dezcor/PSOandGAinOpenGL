#version 320 es
precision mediump float;
in vec3 fragmentColor;
//out vec3 outColor;
out  vec4 FragColor;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main(){
  FragColor = texture2D(ourTexture, TexCoord) * vec4(fragmentColor, 1.0);
}