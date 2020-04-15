#version 330										
                                                	
in vec4 vCol;										
                                                  	
out vec4 colour;	
                                                  	
void main()											
{				
	mat4 aMat4 = mat4(1.0, 0.0, 0.0, 0.0,  // 1. column 
                  0.0, 1.0, 0.0, 0.0,  // 2. column 
                  0.0, 0.0, 1.0, 0.0,  // 3. column 
                  0.0, 0.0, 0.0, 1.0); // 4. column	

	colour = vCol;									                                                  	
}