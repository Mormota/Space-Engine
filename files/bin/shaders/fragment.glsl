#version 330 core

in vec3 color;
in vec2 UV;

in vec3 surfaceNormal;
in vec3 toLightVector;

uniform sampler2D textureSampler;
uniform sampler2D texture2Sampler;

out vec4 colorOut;
void main(){
	float lightLevel = dot(surfaceNormal, toLightVector);

	float brightness = max(lightLevel, 0.3);

	colorOut = brightness * mix(texture(textureSampler, UV), texture(texture2Sampler, UV), 0.5f);
};