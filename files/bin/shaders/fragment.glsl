#version 330 core

in vec3 color;
in vec2 UV;

in vec3 surfaceNormal;
in vec3 toLightVector;
in vec4 ShadowCoord;
in vec3 cameraDirection;

uniform sampler2D textureSampler;
uniform sampler2D texture2Sampler;
uniform vec3 pickingColor;
uniform vec2 pickingVector;




out vec4 colorOut;
void main(){
	float lightLevel = dot(normalize(surfaceNormal), normalize(toLightVector));




	vec4 ambientColor = mix(texture(textureSampler, UV), texture(texture2Sampler, UV), 0.5f);

	float brightness = max(lightLevel, 0.1);
	if(pickingVector.x != 1.0f){
		colorOut = brightness * ambientColor;
	} else {
		colorOut = vec4(pickingColor, 1.0);
	}
	
};