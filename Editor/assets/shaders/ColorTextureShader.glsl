// Basic Color Texture Shader

#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TextureCoordinates;
layout(location = 3) in float a_TextureIndex;

out vec4 v_Color;
out vec2 v_TextureCoordinates;
out float v_TextureIndex;

uniform mat4 u_ViewProjection;

void main()
{
	v_Color = a_Color;
	v_TextureCoordinates = a_TextureCoordinates;
	v_TextureIndex = a_TextureIndex;

	gl_Position = u_ViewProjection * vec4(a_Position ,1.0f);
}



#type fragment
#version 330 core
			
layout(location = 0) out vec4 color;

in vec4 v_Color;
in vec2 v_TextureCoordinates;
in float v_TextureIndex;

uniform sampler2D u_Textures[32];

void main()
{
	int index = int(v_TextureIndex);
	color = texture(u_Textures[index], v_TextureCoordinates) * v_Color;
}