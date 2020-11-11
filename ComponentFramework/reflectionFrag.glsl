#version 330

in vec3 eyeDir;
in  vec3 vertNormal;
in  vec3 lightDir;
out vec4 fragColor;

uniform samplerCube cubeTexture; 
/// uniform mat4 modelViewMatrix; 
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform float elapsedTime;

void main() { 
mat3 scale = mat3(vec3(-1.0,0.0,0.0),
					  vec3(0.0,1.0,0.0),
					  vec3(0.0,0.0,1.0));

	mat4 modelViewMatrix = viewMatrix * modelMatrix;
	vec3 reflection = reflect(eyeDir, vertNormal);
	vec3 refraction = refract (eyeDir, vertNormal, 1.0/1.6);
	reflection = vec3 (inverse (modelViewMatrix) * vec4 (reflection, 0.0));
	refraction = vec3 (inverse (modelViewMatrix) * vec4 (refraction, 0.0));
	vec3 col1 = vec3(0.0, 0.7, 0.4);
	vec3 col2 = vec3(0.0, 0.0, 0.2);

	float intensity = dot(-eyeDir, normalize(vertNormal));

	fragColor =  texture(cubeTexture, reflection);
	/// fragColor = texture(cubeTexture, mix(refraction, reflection, (intensity * elapsedTime)));
	/// fragColor = texture(cubeTexture, refraction);
	/// fragColor = texture(cubeTexture, mix(col1, col2, intensity));
}
