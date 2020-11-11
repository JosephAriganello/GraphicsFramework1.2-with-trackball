#version 330
in  vec4 vVertex;
in  vec4 vNormal;

out vec3 normal;
out vec3 lightDir;
out vec4 eyePos;

uniform mat4 projectionMatrix;
/// uniform mat4 modelViewMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat3 normalMatrix;
uniform vec4 lightPos;

void main() {
	mat4 modelViewMatrix = viewMatrix * modelMatrix;
	normal = normalMatrix * vNormal.xyz; /// Rotate the normal to the correct orientation 
	eyePos = modelViewMatrix * vVertex; /// Create the eye vector 
	lightDir = normalize(lightPos.xyz - eyePos.xyz); /// Create the light direction
	gl_Position =  projectionMatrix * modelViewMatrix * vVertex; 
}