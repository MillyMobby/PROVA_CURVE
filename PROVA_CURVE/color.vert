#version 330 core    
layout (location = 0) in vec3 pos;	   // Position in attribute location 0    
layout (location = 1) in vec3 color;   // Color in attribute location 1    
out vec3 Color;	// Output a color to the fragment shader  

void main()    
{    
   gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);    
   Color = color;    
}
