#shader vertex
#version 330 core

layout(location=0) in vec4 position;
layout(location = 1) in vec3 color;
//has to be in column major

out vec3 vertexcolor;

void main()
{
   gl_Position=position;
   vertexcolor = color;
};

#shader fragment
#version 330 core

layout(location=0) out vec4 color;
in vec3 vertexcolor;
uniform vec4 u_Color;

void main()
{
	color = /* u_Color; */vec4(vertexcolor, 1.0);
};