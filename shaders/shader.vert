#version 330										
													
layout (location = 0) in vec3 pos;					
                									
out vec4 vCol;										

uniform mat4 view;							
uniform mat4 model;	
uniform mat4 projection;										
            													
void main()														
{		
	
	gl_Position = projection * view * model * vec4(pos, 1.0);	

	//vCol = view;		
	//vCol = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	vCol = vec4(clamp(pos, 0.0f, 1.0f), 1.0f);					
}