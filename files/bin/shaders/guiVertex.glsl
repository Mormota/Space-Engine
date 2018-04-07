#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normals;
layout (location = 2) in vec2 inUV;



uniform mat4 model;

out vec3 color;
out vec2 UV;
out vec3 surfaceNormal;
out vec3 toLightVector;
void main(){
	UV = inUV;

	vec4 relative = vec4(pos, 1.0);
	gl_Position = model * relative;
};