#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normals;
layout (location = 2) in vec2 inUV;



uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out vec3 color;
out vec2 UV;
out vec3 surfaceNormal;
out vec3 toLightVector;
void main(){
	vec3 lightPos = vec3(250.0f, 25.0f, 25.0f);
	color = pos;
	UV = inUV;

	vec4 relative = vec4(pos, 1.0);
	gl_Position = projection * view * model * relative;

	surfaceNormal = (model * vec4(normals, 0.0)).xyz;
	toLightVector = normalize(lightPos - (model * relative).xyz);
};