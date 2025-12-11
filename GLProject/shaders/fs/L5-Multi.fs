

#version 330 core
out vec4 FragColor;

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 2

in vec3 fragPos;
in vec3 fragNormal;
in vec2 texCoord;

uniform vec3 objectColor;
uniform sampler2D textureSample;
uniform vec3 viewPos;
//uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform bool blinn;
uniform int shadowNumber;
//uniform SpotLight spotLight;

// function prototypes
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 color,int i);

void main()
{    
    // properties
    vec3 dx = dFdx(fragPos);
    vec3 dy = dFdy(fragPos);
    vec3 norm = normalize(cross(dx, dy));
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 color = texture(textureSample, texCoord).rgb * vec3(objectColor);
    
    // phase 2: point lights
    vec3 result = vec3(0.0,0.0,0.0);
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, fragPos, viewDir, color,i);    
  
    FragColor = vec4(result, 1.0);
}

// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 color,int i)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    float spec =0.0;
         vec3 reflectDir = reflect(-lightDir, normal);
         spec = max(dot(viewDir, reflectDir), 0.0);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * color;
    vec3 diffuse = light.diffuse * diff * color;
    vec3 specular = light.specular * spec * color;
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    //this is to shadow (take only ambient value) one light (which is the sun oblivousley) and 
    //and take the others as is , which is the moon light 
    //i assumed and worked like ambient light is only from the sun which i bieleve it to be 
    //i can also make the ambient light from a third light and remove the light of the sun on KSOF or KHSOF
    //SOLAR AND LUNAR ECLISPE i learned this words recently after searching 
    //anyway the other way was to use two different shaders one for moon and one for sun 
    //so i don't bother editing glsl but i don't have to do that because iam already well knowleged in glsl 
    //and cpp and c language , (btw this is for my partner marwan Daoudi to read explaining what i did here)
    if(i+1 == shadowNumber){
        return (ambient);
    }
    return (ambient + diffuse + specular);
}

