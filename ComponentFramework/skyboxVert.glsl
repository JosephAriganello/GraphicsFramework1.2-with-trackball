/// My vertex shader
#version 330
in  vec4 vVertex;
out vec3 textureCoords;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
void main() {
	textureCoords = vec3(vVertex);
	gl_Position =  projectionMatrix * modelMatrix * vVertex;
}
