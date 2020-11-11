#version 330
in  vec4 vVertex;
in  vec4 vNormal;

out vec3 eyeDir;
out vec3 vertNormal;
out vec3 lightDir;
 
uniform mat4 projectionMatrix;
/// uniform mat4 modelViewMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat3 normalMatrix;
uniform vec3 lightPos;
uniform float elapsedTime;

void main() {
	
	mat4 modelViewMatrix = viewMatrix * modelMatrix;
	vertNormal = normalMatrix * vNormal.xyz; /// Rotate the normal to the correct orientation 
	eyeDir = normalize(vec3(modelViewMatrix * vVertex)); /// Create the eye vector 
	lightDir = normalize(lightPos - eyeDir.xyz); /// Create the light direction 

	vec4 fakeVert = vVertex;
	fakeVert.x += vNormal.x * elapsedTime;
	fakeVert.y += vNormal.y * elapsedTime;
	fakeVert.z += vNormal.z * elapsedTime;

	/// vec4 translate = vNormal;
	/// translate.x += elapsedTime;
	/// translate.y += elapsedTime;
	/// translate.z += elapsedTime;

	/// vec4 scale = vec4(2.0f, 1.5f, 3.0f, 1.0f);
	/// scale.x *= elapsedTime;
	/// scale.y *= elapsedTime;
	/// scale.z *= elapsedTime;

	gl_Position =  projectionMatrix * modelViewMatrix * vVertex; 
}
