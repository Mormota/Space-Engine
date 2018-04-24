#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normals;
layout (location = 2) in vec2 inUV;
layout (location = 3) in vec3 Color;



uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


out vec3 color;
out vec2 UV;
out vec3 surfaceNormal;
out vec3 toLightVector;
out vec3 cameraDirection;

void main(){

	gl_Position = projection * view * model * vec4(pos, 1.0);

	vec3 lightPos = vec3(250.0f, 25.0f, 25.0f);
	color = pos;
	UV = inUV;

	color = Color;

	cameraDirection = vec3(0,0,0) - ( view * model * vec4(pos,1.0)).xyz;

	vec4 relative = vec4(pos, 1.0);
	

	surfaceNormal = (model * vec4(normals, 0.0)).xyz;
	toLightVector = normalize(lightPos - (model * relative).xyz);
};