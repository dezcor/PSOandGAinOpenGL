#version 320 es
precision mediump float;
in vec3 fragmentColor;
//out vec3 outColor;
out  vec4 FragColor;
in vec3 TexCoord;

uniform sampler2D ourTexture;

void main(){
  if(TexCoord.z < 0.5f)
    FragColor = vec4(fragmentColor, 1.0);
  else
    FragColor = texture(ourTexture, vec2(TexCoord.x,TexCoord.y)) * vec4(fragmentColor, 1.0);
}