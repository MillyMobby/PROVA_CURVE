#version 330 core    
layout (location = 0) in vec3 pos;	   // attribute location 0    
layout (location = 1) in vec3 color;   // attribute location 1    

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
out vec3 Color;	// Output a color to the fragment shader  

void main()    
{  
   gl_Position = vec4(pos.x, pos.y, pos.z, 1.0);    

//   mat4 PVM = transpose(projectionMatrix) * transpose(viewMatrix);
//   gl_Position = PVM * vec4(pos, 1.0);
   Color = color;    
}
