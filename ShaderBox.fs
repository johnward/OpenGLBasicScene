#version 330 core

out vec4 FragColor;
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;


void main()
{
   //FragColor = ourColor;
   FragColor = texture(texture1, TexCoord); //* vec4(ourColor, 1.0);  // change to vec4 because the output of texture() is vec4   
}