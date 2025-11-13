#version 430 core
in vec4 vs_color;
in vec2 vsTexCoord;

uniform sampler2D texture1;
//uniform sampler2D texture2;


//out vec4 color;
out vec4 flagColor;


void main(void)
{
//    color = vs_color;
	vec4 tex1 = texture(texture1, vsTexCoord); 
//	vec4 tex2 = texture(texture2, vsTexCoord);

//	//RGB의 값이 전부 0.5보다 높으면 흰색으로 간주.
//	if ( tex1[0] > 0.9 && tex1[1] >0.9 && tex1[2] >0.9){ 
//		flagColor =tex2;
//	}
//	else{
//		flagColor = tex1;
//	}
	flagColor = tex1;
}