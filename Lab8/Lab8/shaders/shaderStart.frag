#version 410 core

in vec3 fNormal;
in vec4 fPosEye;
in vec2 fTexCoords;

out vec4 fColor;

//lighting
uniform	vec3 lightDir;
uniform	vec3 lightColor;

//illumination maps
uniform sampler2D ambientTexture;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

vec3 ambient;
float ambientStrength = 0.2f;
vec3 diffuse;
vec3 specular;
float specularStrength = 0.5f;
float shininess = 32.0f;

uniform vec3 lightPosEye;

float constant = 1.0f;
float linear = 0.0045f;
float quadratic = 0.0075f;

void computeLightComponents()
{		

	//compute distance to light source
	float dist = length(lightPosEye - fPosEye.xyz);
	//compute attenuation
	float att = 1.0f/(constant + linear * dist + quadratic * (dist * dist));

	vec3 cameraPosEye = vec3(0.0f);//in eye coordinates, the viewer is situated at the origin
	
	//transform normal
	vec3 normalEye = normalize(fNormal);	
	
	//compute light direction -> lumini obiectuale
	//vec3 lightDirN = normalize(lightDir);

	//compute light direction -> lumini punctiforme
	vec3 lightDirN = normalize(lightPosEye - fPosEye.xyz);
	
	//compute view direction 
	vec3 viewDirN = normalize(cameraPosEye - fPosEye.xyz);

	//compute half vector
	vec3 halfVector = normalize(lightDirN + viewDirN);
		
	//compute ambient light
	ambient = att * ambientStrength * lightColor;
	
	//compute diffuse light
	diffuse = att* max(dot(normalEye, lightDirN), 0.0f) * lightColor;
	
	//compute specular light
	vec3 reflection = reflect(-lightDirN, normalEye);


	float specCoeff = pow(max(dot(normalEye, halfVector), 0.0f), shininess);
	specular = att* specularStrength * specCoeff * lightColor;
}

void main() 
{
	computeLightComponents();
	
	vec3 baseColor = vec3(0.9f, 0.35f, 0.0f);//orange
	
	//ambient *= baseColor;
	//diffuse *= baseColor;
	//specular *= baseColor;
	
	//maparea texturii pe obiect
	ambient *= texture(ambientTexture, fTexCoords);
	diffuse *= texture(diffuseTexture, fTexCoords);
	specular *= texture(specularTexture, fTexCoords);
	
	vec3 color = min((ambient + diffuse) + specular, 1.0f);
    
    fColor = vec4(color, 1.0f);
}
