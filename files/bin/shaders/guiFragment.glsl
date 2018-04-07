#version 330 core

in vec3 color;
in vec2 UV;

in vec3 surfaceNormal;
in vec3 toLightVector;

uniform sampler2D textureSampler;
//uniform sampler2D texture2Sampler;

out vec4 colorOut;
void main(){


	colorOut = texture(textureSampler, UV);
};