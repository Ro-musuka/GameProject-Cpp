#version 410

in vec3 fragWorldPos;
in vec3 fragNormal;
in vec2 fragTexCoord;

out vec4 outColor;

uniform vec3 lightDir;
uniform vec3 objectColor;
uniform vec3 lightColor;

void main()
{
    vec3 N = normalize(fragNormal);

    float diffuse = max(dot(N, -lightDir), 0.0);
    float ambient = 0.25;
    
    vec3 color = objectColor * (ambient + diffuse);
    outColor = vec4(color, 1.0);
}