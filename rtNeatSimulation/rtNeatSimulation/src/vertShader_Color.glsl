#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 ourColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void  main(){
	vec4 modolPos = model * vec4( position, 1.0 );
    vec4 viewPos = view * modolPos;
    gl_Position = projection * viewPos;
	ourColor = color;

}