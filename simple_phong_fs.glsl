#version 430 core

in vec3 vsNormal;  
in vec3 vsPos;  
in vec2 vsTexCoord;

struct Material { 
    sampler2D diffuse; 
    sampler2D specular; 
    float shininess; 

	vec3 defaultAmbient;
	vec3 defaultDiffuse;
	vec3 defaultSpecular;
	int useDiffuseMap;
	int useSpecularMap;
}; 

struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
    vec3 position;

	float c1;
	float c2;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

};
struct SpotLight{
    vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	float c1;
	float c2;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

//그냥 Light 값만 받음.
struct Light { 
    vec3 position; 

    vec3 ambient; 
    vec3 diffuse; 
    vec3 specular; 
}; 

//pointLight 개수: pointNum 
#define pointNum 1

uniform Material material;
uniform vec3 viewPos; 
uniform Light light; 
uniform DirLight dirLight;
uniform PointLight pointLights[pointNum];
uniform SpotLight spotLight;

// Normal_Map 도전
uniform sampler2D normalMap;

uniform int useNormal;

out vec4 fragColor;

vec3 matAmbientColor;
vec3 matDiffuseColor;
vec3 matSpecularColor;

//method header 광원 별 컬러 정의 Header 부분 Abstraction
vec3 CalcDirLight (DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight (PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight (SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);


void main()
{
	if(useNormal != 0)
	{
		//노멀 매핑
		vec3 normal2 = normalize(2.0 * texture(normalMap, vsTexCoord).rgb - 1.0);

		vec3 norm = normalize(vsNormal);
		vec3 viewDir = normalize(viewPos - vsPos);

		matAmbientColor = material.defaultAmbient;
		matDiffuseColor = material.defaultDiffuse;
		matSpecularColor = material.defaultSpecular;

		if(material.useDiffuseMap != 0)
		{
			matAmbientColor = vec3(texture(material.diffuse, vsTexCoord));
			matDiffuseColor = vec3(texture(material.diffuse, vsTexCoord));
		}

		if(material.useSpecularMap != 0)
		{
			matSpecularColor = vec3(texture(material.specular, vsTexCoord));
		}
    
		//기존 코드---------------------------------------------------
//		// ambient
//		vec3 ambient = light.ambient * matAmbientColor;
//  	
//		// diffuse 
//		vec3 lightDir = normalize(light.position - vsPos);
//		float diff = max(dot(norm, lightDir), 0.0);
//		vec3 diffuse = light.diffuse * diff * matDiffuseColor;
//    
//		// specular
//		vec3 reflectDir = reflect(-lightDir, norm);  
//		float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
//		vec3 specular = light.specular * spec * matSpecularColor;  
//        
//		vec3 result = ambient + diffuse + specular;
//		fragColor = vec4(result, 1.0);
// Combine results from different light sources

		//수정코드 -------------------------------------
		vec3 result = vec3(0.0,0.0,0.0);

		// Directional light
		result = CalcDirLight(dirLight, norm, viewDir);

		// Point lights
		for (int i = 0; i < pointNum; i++)
		{
			result += CalcPointLight(pointLights[i], norm, vsPos, viewDir);
		}

		// Spotlight
		result += CalcSpotLight(spotLight, norm, vsPos, viewDir);

		fragColor = vec4(result, 1.0);
	}
	else
		fragColor = vec4(material.defaultDiffuse, 1.0);
} 

//directional light 계산 method
vec3 CalcDirLight (DirLight light, vec3 normal, vec3 viewDir){
	vec3 lightDir = normalize(-light.direction);
	// 디퓨즈 쉐이딩
	float diff = max(dot(normal, lightDir), 0.0);

	//스페큘러 쉐이딩
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	//결과 결합.
	vec3 ambient = light.ambient * matAmbientColor;
	vec3 diffuse = light.diffuse * diff* matDiffuseColor;
	vec3 specular = light.specular * spec * matSpecularColor;

	return (ambient+ diffuse + specular);
}
// point light 계산 함수
vec3 CalcPointLight (PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
	vec3 lightDir = normalize(light.position - fragPos);
	//디퓨즈 쉐이딩
	float diff = max(dot(normal, lightDir), 0.0);

	//스페큘러 쉐이딩
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir),0.0), material.shininess);

	// 거리계산
	float distance = length(light.position - fragPos);
	float attenuation = 4.0/ (1.0+ light.c1 * distance + light.c2 * (distance * distance));

	//결과 조합.
	vec3 ambient = light.ambient * matAmbientColor;
	vec3 diffuse = light.diffuse * diff* matDiffuseColor;
	vec3 specular = light.specular * spec * matSpecularColor;

	return (ambient + diffuse + specular) * attenuation;
}

//spotLight 계산 함수
vec3 CalcSpotLight (SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir){
	vec3 lightDir = normalize(light.position - fragPos);
	//디퓨즈 쉐이딩
	float diff = max(dot(normal, lightDir), 0.0);

	//스페큘러 쉐이딩
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir),0.0), material.shininess);

	//soft edge spotLight intensity
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta- light.outerCutOff) / epsilon, 0.0, 1.0);

	//attenuation
	float distance = length(light.position - fragPos);
	float attenuation = 900.0 / (1.0+ light.c1 * distance + light.c2 * (distance * distance));
	//결과 조합.
	vec3 ambient = light.ambient * matAmbientColor;
	vec3 diffuse = light.diffuse * diff* matDiffuseColor;
	vec3 specular = light.specular * spec * matSpecularColor;
	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;

	return (ambient + diffuse + specular);
}