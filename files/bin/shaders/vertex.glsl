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
out vec3 toCameraVector;
out vec3 cameraDirection;
out float distance;

void main(){

	gl_Position = projection * view * model * vec4(pos, 1.0);

	vec3 lightPos = vec3(250.f, 50.0f, 250.0f);
	color = pos;
	UV = inUV;

	float distanceFromCenter;
	float horizontalDistance  = sqrt(pos.x * pos.x + pos.z * pos.z);
	distanceFromCenter = sqrt(horizontalDistance * horizontalDistance + pos.y * pos.y);

	distance = distanceFromCenter;

	color = Color;

	cameraDirection = vec3(0,0,0) - ( view * model * vec4(pos,1.0)).xyz;

	vec4 relative = vec4(pos, 1.0);
	

	surfaceNormal = (model * vec4(normals, 0.0)).xyz;
	toLightVector = normalize(lightPos - (model * relative).xyz);
	toCameraVector = (inverse(view) * vec4(0.0, 0.0, 0.0, 1.0)).xyz;
};