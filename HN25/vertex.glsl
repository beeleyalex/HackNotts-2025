#version 330 core

layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoordIn;

out vec4 fragPos;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	mat4 transform = projection * view * model;
	gl_Position = transform * vec4(pos, 1.0);
	fragPos = model * vec4(pos, 1.0);
	texCoord = texCoordIn;
}