#version 420

layout(location = 0) in vec2 Position;
layout(location = 1) in vec2 UV;


uniform mat4 M;
uniform mat4 V;
uniform mat4 P;

out vec2 UV1;

void main()
{

	UV1=UV;
	float x=Position.x;
	float y=Position.y;
	gl_Position=P*V*vec4(x,y, 0, 1);

}

