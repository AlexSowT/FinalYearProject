#version 330 core
//in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D ourTexture1;
void main()
{
	//color = vec4(ourColor, 1.0f);
	color = texture(ourTexture1, TexCoord);
}