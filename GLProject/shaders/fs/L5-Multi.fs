

#version 330 core
out vec4 FragColor;

//no need
struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

//no need
struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
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
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 color);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 color,int i);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 color);

void main()
{    
    // properties
    vec3 dx = dFdx(fragPos);
    vec3 dy = dFdy(fragPos);
    vec3 norm = normalize(cross(dx, dy));
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 color = texture(textureSample, texCoord).rgb * vec3(objectColor);
    
    // == =====================================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == =====================================================
    // phase 1: directional lighting
    //vec3 result = CalcDirLight(dirLight, norm, viewDir, color);
    // phase 2: point lights
    vec3 result = vec3(0.0,0.0,0.0);
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
        result += CalcPointLight(pointLights[i], norm, fragPos, viewDir, color,i);    
    // phase 3: spot light
    //result += CalcSpotLight(spotLight, norm, fragPos, viewDir, color);    
    //if(shadow)
    //FragColor = vec4(vec3(0.0,0.0,0.0), 1.0);
    //else
    FragColor = vec4(result, 1.0);
}

// calculates the color when using a directional light.
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 color)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    //float spec;
    //if(blinn)
    //{
     //   vec3 halfwayDir = normalize(lightDir + viewDir);  
    //    //spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
    //    spec = max(dot(normal, halfwayDir), 0.0);

    //}
    //else
   // {
        vec3 reflectDir = reflect(-lightDir, normal);
       float spec = max(dot(viewDir, reflectDir), 0.0);
    //}
    
    // combine results
    vec3 ambient = light.ambient * color;
    vec3 diffuse = light.diffuse * diff * color;
    vec3 specular = light.specular * spec * color;
    
    return (ambient + diffuse + specular);
}
// calculates the color when using a point light.
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 color,int i)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    float spec =0.0;
    //if(blinn){
    //    vec3 halfwayDir = normalize(lightDir + viewDir);  
    //    spec = max(dot(normal, halfwayDir),0.0f);
    //}
    //else{
         vec3 reflectDir = reflect(-lightDir, normal);
         spec = max(dot(viewDir, reflectDir), 0.0);
    //}
   
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
    if(i+1 == shadowNumber){
        return (ambient);
    }
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 color)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = max(dot(viewDir, reflectDir), 0.0);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * color;
    vec3 diffuse = light.diffuse * diff * color;
    vec3 specular = light.specular * spec * color;
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

