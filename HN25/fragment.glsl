#version 330 core

in vec4 fragPos;
out vec4 FragColour;
in vec2 texCoord;

uniform sampler2D tex;

void main()
{
	vec4 colour = texture(tex, texCoord);
	if(colour.x >= 0.8 && colour.y >= 0.8 && colour.z >= 0.8)
	{
		discard;
	}
	FragColour = colour;
	//FragColour = vec4(fragPos.x, fragPos.y, fragPos.z, 1.0);
}