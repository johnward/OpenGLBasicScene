#version 330 core
layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec2 vertexColor;

uniform mat4 MVP;

// Output data ; will be interpolated for each fragment.
out vec2 fragmentColor;

void main() {

    // The color of each vertex will be interpolated
    // to produce the color of each fragment
    fragmentColor = vertexColor;
    gl_Position =  MVP * vec4(vertexPos,1);
}