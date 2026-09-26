// ----------------------------------------------------------------
// From Game Programming in C++ by Sanjay Madhav
// Copyright (C) 2017 Sanjay Madhav. All rights reserved.
// 
// Released under the BSD License
// See LICENSE in root directory for full details.
// ----------------------------------------------------------------

#version 410

uniform mat4 uWorldTransform;
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat4 uMatrixPalette[96];

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in uvec4 inSkinBones;
layout(location = 4) in vec4 inSkinWeights;

out vec2 fragTexCoord;
out vec3 fragNormal;
out vec3 fragWorldPos;

void main()
{
	vec4 pos = vec4(inPosition, 1.0);
	
	// Skin the position
	vec4 skinnedPos = (uMatrixPalette[inSkinBones.x] * pos) * inSkinWeights.x
		+ (uMatrixPalette[inSkinBones.y] * pos) * inSkinWeights.y
	 	+ (uMatrixPalette[inSkinBones.z] * pos) * inSkinWeights.z
	 	+ (uMatrixPalette[inSkinBones.w] * pos) * inSkinWeights.w;

	vec4 worldPos = uWorldTransform * skinnedPos;
	
	fragWorldPos = worldPos.xyz;
	// Transform to clip space
	gl_Position = uProjection * uView * worldPos;

	// Skin the vertex normal
	vec4 skinnedNormal = vec4(inNormal, 0.0f);
	skinnedNormal = (uMatrixPalette[inSkinBones.x] * skinnedNormal) * inSkinWeights.x
		+ (uMatrixPalette[inSkinBones.y] * skinnedNormal) * inSkinWeights.y
		+ (uMatrixPalette[inSkinBones.z] * skinnedNormal) * inSkinWeights.z
		+ (uMatrixPalette[inSkinBones.w] * skinnedNormal) * inSkinWeights.w;
		
	fragNormal = (uWorldTransform * skinnedNormal).xyz;

	// Pass along the texture coordinate to frag shader
	fragTexCoord = inTexCoord;
}