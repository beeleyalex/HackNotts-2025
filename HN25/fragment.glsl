#version 330 core

in vec4 fragPos;
out vec4 FragColour;
in vec2 texCoord;

uniform sampler2D tex;

void main()
{
	FragColour = texture(tex, texCoord);
	//FragColour = vec4(fragPos.x, fragPos.y, fragPos.z, 1.0);
}