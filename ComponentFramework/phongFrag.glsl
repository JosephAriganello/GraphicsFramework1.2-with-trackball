#version 330
in  vec3 vertNormal;
in  vec3 lightDir;
out vec4 fragColor;

void main() { 
	/// I could have passed these in as Uniforms but for simplicity, 
	/// I'll just define them here: specular, diffuse, ambient for the surface material 
	const vec4 ks = vec4(0.7, 0.3, 0.9, 0.0);
	const vec4 kd = vec4(0.6, 0.8, 0.6, 0.0); /// const means it cannot be changed just like C++
	const vec4 ka = 0.1 * kd;					
	
	/// The reflect() method this most GPU expensive step in this proceedure
	/// The Blinn-Phong method is faster.   	
	/// vec3 reflection = normalize(reflect(-lightDir, vertNormal));
	vec3 halfVec = normalize(lightDir + vertNormal);
	float diff = max(dot(vertNormal, lightDir), 0.0);
	float spec = max(0.0, dot(vertNormal, halfVec));
	if(diff != 0.0){
		spec = pow(spec,16.0);
	}

	float gamma = 2.2;

	fragColor =  pow((ka + (diff * kd) + (spec * ks)), vec4(1.0/gamma));
} 

