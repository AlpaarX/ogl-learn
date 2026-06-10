#version 430 core

in vec3 v_vertexColors;

uniform float u_Xoffset;
uniform float u_Scale;

out vec4 color;

void main()
{
   color = vec4(v_vertexColors.r + u_Xoffset, v_vertexColors.g + u_Scale, v_vertexColors.b + u_Xoffset - u_Scale, 1.0);
}


