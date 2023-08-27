#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoord;

//out v_TexCoord;

uniform mat4 u_MVP;

void main()
{
    gl_Position = u_MVP * vec4(position, 1.0);
    //v_TexCoord = texCoord;
}

#shader fragment
#version 330 core

//in vec2 v_TexCoord;
out vec4 color;

void main()
{
    color = vec4(1.0, 0.0, 0.0, 1.0);
}