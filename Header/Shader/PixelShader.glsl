#version 330 core

in vec2 TexCoord;
in vec4 MeshColor;

out vec4 FinalColor;

uniform sampler2D Texture;

void main()
{
	FinalColor = texture(Texture, TexCoord) * MeshColor;
}