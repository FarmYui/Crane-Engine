// Basic Color Texture Shader

#type vertex
#version 330 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TextureCoordinate;

out vec2 v_TextureCoordinate;

uniform mat4 u_Model;
uniform mat4 u_ViewProjection;			

void main()
{
	v_TextureCoordinate = a_TextureCoordinate;
	gl_Position = u_ViewProjection * u_Model * vec4(a_Position ,1.0f);
}



#type fragment
#version 330 core
			
layout(location = 0) out vec4 color;

in vec2 v_TextureCoordinate;

uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	color = texture(u_Texture, v_TextureCoordinate) * u_Color;
}