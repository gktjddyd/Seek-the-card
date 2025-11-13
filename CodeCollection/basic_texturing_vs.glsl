#version 430 core							
										
layout(location = 0) in vec3 pos;			
layout(location = 1) in vec3 color;			
layout(location = 2) in vec2 texCoord;		

uniform mat4 model2;
uniform mat4 view2;
uniform mat4 projection2;
							
out vec3 vsColor;							
out vec2 vsTexCoord;						
											
void main(void)								
{					
	vec3 vsPos = vec3(model2 * vec4(pos, 1.0));
	gl_Position = projection2 * view2 * vec4(vsPos, 1.0);
											
	vsColor = color;						
	vsTexCoord = texCoord;					
}											


