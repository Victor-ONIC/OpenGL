#version 330 core

in vec3 color;
in vec2 texCoord;

out vec4 outColor;

uniform sampler2D aTexture;

void main()
{
	outColor = texture(aTexture, texCoord) * vec4(color, 1.0f);
}
