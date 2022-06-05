#version 330 core

in vec2 texCoord;

out vec4 outColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	outColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.2);
}
