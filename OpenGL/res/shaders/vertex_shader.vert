#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in vec2 aTexCoord;

out vec3 color;
out vec2 texCoord;

void main()
{
	gl_Position = vec4(aPos, 0.0f, 1.0f);
	color = aColor;
	texCoord = aTexCoord;
}
