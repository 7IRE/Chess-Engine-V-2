#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragPosition;
in vec3 fragNormal;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

// Lighting system configuration constants
#define     MAX_LIGHTS              4
#define     LIGHT_DIRECTIONAL       0
#define     LIGHT_POINT             1

struct Light {
    int enabled;
    int type;
    vec3 position;
    vec3 target;
    vec4 color;
};

// Array holding our actual structured light variables (from rlights.h)
uniform Light lights[MAX_LIGHTS];
uniform vec3 viewPos;

void main()
{
    // Pull the primary texture color (defaulting to clean white mapping)
    vec4 texelColor = texture(texture0, fragTexCoord);
    vec3 normal = normalize(fragNormal);
    vec3 viewD = normalize(viewPos - fragPosition);
    vec3 specular = vec3(0.0);
    
    // Accumulate base ambient lighting factor so shadows aren't pitch black
    vec3 lightDot = vec3(0.2, 0.2, 0.2); 

    for (int i = 0; i < MAX_LIGHTS; i++)
    {
        if (lights[i].enabled == 1)
        {
            vec3 light = vec3(0.0);

            if (lights[i].type == LIGHT_DIRECTIONAL)
            {
                light = -normalize(lights[i].target - lights[i].position);
            }
            else if (lights[i].type == LIGHT_POINT)
            {
                light = normalize(lights[i].position - fragPosition);
            }

            // Calculate diffuse reflection bounce value
            float NdotL = max(dot(normal, light), 0.0);
            lightDot += lights[i].color.rgb*NdotL;

            // Calculate shiny specular highlight reflection maps
            float specCo = 0.0;
            if (NdotL > 0.0) specCo = pow(max(0.0, dot(viewD, reflect(-(light), normal))), 16.0); // 16.0 = material shininess multiplier
            specular += lights[i].color.rgb*specCo;
        }
    }

    // Multiply structural surface properties with geometry calculations
    finalColor = (texelColor * colDiffuse * fragColor * vec4(lightDot, 1.0)) + vec4(specular, 0.0);
}