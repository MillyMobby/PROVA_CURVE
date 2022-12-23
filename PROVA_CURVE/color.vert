#version 330 core    
layout (location = 0) in vec4 pos;	   // attribute location 0    
layout (location = 1) in vec3 color;   // attribute location 1    

uniform mat4 orthoMatrix;
uniform mat4 viewMatrix;
uniform mat4 transform;
uniform int isLast;
out vec3 Color;	// Output a color to the fragment shader  

void main()    
{  
   //
 
		mat4 PVM = transpose(orthoMatrix) *transpose(viewMatrix);
		gl_Position = PVM * pos;
		
		//gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);   
   Color = color;    
}
