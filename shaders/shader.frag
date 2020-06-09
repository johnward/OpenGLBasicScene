#version 330										
                                                	
in vec4 vCol;
in vec2 TexCoord;		
// flat in vec3 Normal; // does do interletation
in vec3 Normal;			
in vec3 FragPos;					
                                                  	
out vec4 colour;	

struct DirectionalLight
{
    vec3 colour;
    float ambientIntensity;
    vec3 direction;
    float diffuseIntensity;
};

struct Material
{
    float specularIntensity;
    float shininess;
};

uniform sampler2D theTexture;
uniform DirectionalLight directionalLight;
uniform Material material;

uniform vec3 eyePosition; // camera
                                                  	
void main()											
{				
    vec4 ambientColour = vec4(directionalLight.colour, 1.0f) * directionalLight.ambientIntensity;

    float diffuseFactor = max(dot(normalize(Normal), normalize(directionalLight.direction)), 0.0f);
    
    // This is how much light we need to add, like ambient colour taking into account the diffuse factor
    vec4 diffuseColour = vec4(directionalLight.colour, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;

    vec4 specularColour = vec4(0,0,0,0);

    if(diffuseFactor > 0.0f)
    {
        vec3 fragToEye = normalize(eyePosition - FragPos);
        vec3 reflectedVertex = normalize(reflect(directionalLight.direction, normalize(Normal)));
        float specularFactor = dot(fragToEye, reflectedVertex);

        if (specularFactor > 0.0f)
        {
            specularFactor = pow(specularFactor, material.shininess);
            specularColour = vec4(directionalLight.colour * material.specularIntensity * specularFactor, 1.0f);
        }
    }

	colour = texture(theTexture, TexCoord) * (ambientColour + diffuseColour + specularColour);								                                                  	
}