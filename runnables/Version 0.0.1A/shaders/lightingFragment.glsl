#version 330 core

in vec3 color;
in vec2 UV;

uniform sampler2D textureSampler;
uniform sampler2D texture2Sampler;

out vec4 colorOut;
void main(){

	colorOut = mix(texture(textureSampler, UV), texture(texture2Sampler, UV), 0.5f);
};