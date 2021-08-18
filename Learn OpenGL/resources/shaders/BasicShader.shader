#shader vertex
#version 330 core

layout(location=0) in vec4 position;
layout(location = 1) in vec3 color;
//has to be in column major

//uniform vec3 translateVal;
//mat4 translationMatrix = mat4(1.0f,0.0f,0.0f,0.0f,
//							  0.0f,1.0f,0.0f,0.0f,
//							  0.0f,0.0f,1.0f,0.0f,
//							  translateVal.x,translateVal.y,0,1.0f);
//
////column major and row major same
//uniform vec3 u_ScaleValues;
//uniform vec2 rotate;
//mat4 scaleMatrix = mat4(u_ScaleValues.x,	0.0f,			0.0f,			0.0f,
//						0.0f,			u_ScaleValues.y,	0.0f,			0.0f,
//						0.0f,				0.0f,		u_ScaleValues.z,	0.0f,
//						0.0f,				0.0f,			0.0f,			1.0f);

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
	color = vec4(vertexcolor, 1.0);
};