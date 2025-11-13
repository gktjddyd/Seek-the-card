#version 430 core

out vec4 fragColor;
  
in vec2 vsTexCoord;

uniform sampler2D screenTexture;
uniform float kernelChoose[9];

float offsetX = 1.0f / float(textureSize(screenTexture,0).x);  
float offsetY = 1.0f / float(textureSize(screenTexture,0).y);  

void main()
{
    vec2 offsets[9] = vec2[](
        vec2(-offsetX,  offsetY), 
        vec2( 0.0f,    offsetY), 
        vec2( offsetX,  offsetY),
        vec2(-offsetX,  0.0f),   
        vec2( 0.0f,    0.0f),  
        vec2( offsetX,  0.0f),   
        vec2(-offsetX, -offsetY), 
        vec2( 0.0f,   -offsetY), 
        vec2( offsetX, -offsetY) 
    );

   float kernel[9];
   for (int i=0; i < 9; i++){
         kernel[i] = kernelChoose[i];
    }


    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(screenTexture, vsTexCoord.st + offsets[i]));
    }
    
    vec3 color = vec3(0.0);
    for(int i = 0; i < 9; i++)
        color += sampleTex[i] * kernel[i];
    
    fragColor = vec4(color, 1.0);

   
}
