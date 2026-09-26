#version 410

uniform mat4 uWorldTransform;
uniform mat4 uView;
uniform mat4 uProjection;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragTexCoord;

void main()
{
    vec4 worldPos = uWorldTransform * vec4(inPosition, 1.0);

    fragPos = worldPos.xyz;
    fragNormal = mat3(transpose(inverse(uWorldTransform))) * inNormal;
    fragTexCoord = inTexCoord;

    gl_Position = uProjection * uView * worldPos;
}