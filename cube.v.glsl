#version 320 es
layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec2 aTextCoord;

out vec3 fragmentColor;
// Values that stay constant for the whole mesh.
uniform mat4 MVP;
out vec2 TexCoord;
  
void main(){
  // Output position of the vertex, in clip space : MVP * position
  gl_Position =  MVP * vec4(vertexPosition_modelspace,1.0);
  fragmentColor = vertexColor;
  TexCoord = aTextCoord;
}