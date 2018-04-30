#version 330 core

in vec3 color;
in vec2 UV;

in float distance;
in vec3 surfaceNormal;
in vec3 toLightVector;
in vec4 ShadowCoord;
in vec3 cameraDirection;
in vec3 toCameraVector;

uniform sampler2D textureSampler;
uniform sampler2D texture2Sampler;
uniform vec3 pickingColor;
uniform vec2 pickingVector;
uniform vec2 materialVector;




out vec4 colorOut;
void main(){
	vec3 unitLightVector = normalize(toLightVector);

	float lightLevel = dot(normalize(surfaceNormal), normalize(toLightVector));
	if(materialVector.x == 1.0f)
		lightLevel *= 1.4;


	vec4 textureColor = mix(texture(textureSampler, UV), texture(texture2Sampler, UV), 0.5f);
	vec4 materialColor = vec4(color, 1.0);

	float brightness = max(lightLevel, 0.2);
	if(pickingVector.x != 1.0f){
		if(materialVector.x != 1.0f){
			colorOut = brightness * textureColor;
		}else{
			colorOut = brightness * materialColor;
		}
		
	}else{
		colorOut = vec4(pickingColor, 1.0);
	}
	

};