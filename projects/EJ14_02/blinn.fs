#version 330 core

out vec4 FragColor;

in vec3 normal;
in vec3 fragPos;
in vec2 uv;
in vec4 fragPosLighSpace0;
in vec4 fragPosLighSpace1;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    int shininess;
};
uniform Material material;

struct SpotLight {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    float cutOff;
    float outerCutOff;
};

#define NUMBER_SPOT_LIGHTS 2
uniform SpotLight spotLight[NUMBER_SPOT_LIGHTS];

uniform vec3 viewPos;

uniform sampler2D depthMap0;
uniform sampler2D depthMap1;



uniform bool isShadow;

float ShadowCalculation(vec4 fragPosLighSpace, float bias, bool perspective, sampler2D depthMap) {
    vec3 projCoords = fragPosLighSpace.xyz / fragPosLighSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    float currentDepth = projCoords.z;


    float shadow = 0;
    vec2 texelSize = 1.0 / textureSize(depthMap, 0);
    for(int x = -1; x <= 1; ++x) {
        for (int y = -1; y <=1; ++y) {
            float pcf = texture(depthMap, projCoords.xy + vec2(x,y) * texelSize).r;
            shadow += currentDepth -bias > pcf ? 1.0 : 0.0;
        }
    }
    shadow /= 9.0;
    
    if (perspective == false && projCoords.z > 1.0) {
        shadow = 0.0;
    }

    else if (perspective == true && texture( depthMap, (fragPosLighSpace.xy/fragPosLighSpace.w) ).z  >  (fragPosLighSpace.z-bias)/fragPosLighSpace.w ) {
        shadow = 0.0;
    }
    return shadow;
}


vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 viewDir, vec3 fragPos) {
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant +
        light.linear * distance +
        light.quadratic * distance * distance);

    vec3 albedo = vec3(texture(material.diffuse, uv));
    vec3 ambient = albedo * light.ambient;

    vec3 norm = normalize(normal);
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * albedo * light.diffuse;

    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
    vec3 specular = spec * vec3(texture(material.specular, uv)) * light.specular;

    float theta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    float shadow = 0;
    if(isShadow){
        float bias = max(0.05 * (1.0 - dot(normal, light.direction)), 0.005);
        shadow = ShadowCalculation(fragPosLighSpace0, bias, true, depthMap0);
        shadow += ShadowCalculation(fragPosLighSpace1, bias, true, depthMap1);
    }

    return (ambient + (1.0 - shadow) * ((diffuse * intensity) + (specular * intensity))) * attenuation ;

}

void main() {

    vec3 albedo = vec3(texture(material.diffuse, uv));
    vec3 specular = vec3(texture(material.specular, uv));

    vec3 norm = normalize(normal);
    vec3 viewDir = normalize(viewPos - fragPos);

    vec3 finalColor;

    for (int i = 0; i < NUMBER_SPOT_LIGHTS; ++i) {
     finalColor += CalcSpotLight(spotLight[i], norm, viewDir, fragPos);
	}


    FragColor = vec4(finalColor, 1.0f);
}