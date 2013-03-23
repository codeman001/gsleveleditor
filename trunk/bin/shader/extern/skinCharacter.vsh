#define LIGHTING

#define MAX_BONES	64

attribute highp vec4 inVertexPosition;
attribute highp vec4 inVertexColor;
attribute highp vec3 inVertexNormal;
attribute highp vec2 inTexCoord0;
attribute highp vec4 inBoneIndex;
attribute highp vec4 inBoneWeight;

uniform highp mat4 uMvpMatrix;
uniform highp mat4 uBoneMatrix[MAX_BONES];

varying highp vec4 varVertexColor;
varying highp vec2 varTexCoord0;

#ifdef LIGHTING

uniform highp vec4  uAmbientColor;


uniform highp vec4  uLightDirection1;
uniform highp vec4  uLightDiffuse1;
uniform highp vec4  uLightSpecular1;

uniform highp vec4  uLightDirection2;
uniform highp vec4  uLightDiffuse2;
uniform highp vec4  uLightSpecular2;

uniform highp vec4  uPointLightPosition1;
uniform highp vec4  uPointLightDiffuse1;
uniform highp vec4  uPointLightSpecular1;
uniform highp vec3  uPointLightAttenuation1;

uniform highp vec4  uPointLightPosition2;
uniform highp vec4  uPointLightDiffuse2;
uniform highp vec4  uPointLightSpecular2;
uniform highp vec3  uPointLightAttenuation2;

uniform highp vec4  uPointLightPosition3;
uniform highp vec4  uPointLightDiffuse3;
uniform highp vec4  uPointLightSpecular3;
uniform highp vec3  uPointLightAttenuation3;


uniform highp vec4  uMaterialAmbient;
uniform highp vec4  uMaterialEmission;
uniform highp vec4  uMaterialDiffuse;
uniform highp vec4  uMaterialSpecular;
uniform highp float uMaterialShininess;

#endif

void main(void)
{	    
	// ---------------------------------
	// SKINNING 
	// ---------------------------------
    highp mat4 netMatrix 		= mat4(0.0);
	highp vec3 netPosition 		= vec3(0.0);
	highp vec3 netNormal		= vec3(0.0);
	
	highp int index = int(inBoneIndex[0]);
	netMatrix = inBoneWeight[0] * uBoneMatrix[index];
		
	index = int(inBoneIndex[1]);
	netMatrix += inBoneWeight[1] * uBoneMatrix[index];
	
	index = int(inBoneIndex[2]);
	netMatrix += inBoneWeight[2] * uBoneMatrix[index];

	index = int(inBoneIndex[3]);
	netMatrix += inBoneWeight[3] * uBoneMatrix[index];	
	
	netPosition = (netMatrix * inVertexPosition).xyz;	
	netNormal 	= (netMatrix * vec4(inVertexNormal, 0.0)).xyz;	
		
	// ---------------------------------
	// LIGHTING
	// ---------------------------------
#ifdef LIGHTING
	// calc light color
	highp vec4 lightColor = uMaterialEmission + uMaterialAmbient*uAmbientColor;
	
	// ---------------------------------
	// DIRECTION LIGHT
	// ---------------------------------
	// calc light direction 1	
	highp vec3 lightDir = -uLightDirection1.xyz;	
		
	// compute cos(Light, Normal)
	highp float NdotL = max(dot(normalize(netNormal), lightDir), 0.0);
	lightColor += NdotL * uLightDiffuse1 * uMaterialDiffuse;
	
	
	// calc light direction 2
	// ---------------------------------
	lightDir = -uLightDirection2.xyz;				
		
	// compute cos(Light, Normal)
	NdotL = max(dot(normalize(netNormal), lightDir), 0.0);
	lightColor += NdotL * uLightDiffuse2 * uMaterialDiffuse;
	
	
	// ---------------------------------
	// POINT LIGHT
	// ---------------------------------
	highp 	float fDistance;
	highp 	float fAttenuation;	
	highp	vec3  fvDirection;
	highp	vec4  fvLightColor;
	
	// Light 1
	fvDirection			= uPointLightPosition1.xyz - netPosition.xyz;
	fDistance			= length(fvDirection);	
	fAttenuation		= max(0.0, 1.0/(fDistance * uPointLightAttenuation1.y + 0.0001));
	NdotL				= max(0.0, dot(normalize(fvDirection), normalize(netNormal.xyz)));
	fvLightColor		= uPointLightDiffuse1 * uMaterialDiffuse * (NdotL * fAttenuation);
	lightColor += fvLightColor;
		
	// Light 2
	fvDirection			= uPointLightPosition2.xyz - netPosition.xyz;
	fDistance			= length(fvDirection);	
	fAttenuation		= max(0.0, 1.0/(fDistance * uPointLightAttenuation2.y + 0.0001));
	NdotL				= max(0.0, dot(normalize(fvDirection), normalize(netNormal.xyz)));
	fvLightColor		= uPointLightDiffuse2 * uMaterialDiffuse * (NdotL * fAttenuation);
	lightColor += fvLightColor;
		
	// Light 3	
	fvDirection			= uPointLightPosition3.xyz - netPosition.xyz;
	fDistance			= length(fvDirection);	
	fAttenuation		= max(0.0, 1.0/(fDistance * uPointLightAttenuation3.y + 0.0001));
	NdotL				= max(0.0, dot(normalize(fvDirection), normalize(netNormal.xyz)));
	fvLightColor		= uPointLightDiffuse3 * uMaterialDiffuse * (NdotL * fAttenuation);
	lightColor += fvLightColor;		
#endif

#ifdef LIGHTING
	varVertexColor = inVertexColor * lightColor;
#else
	varVertexColor = inVertexColor;
#endif

	varTexCoord0 = inTexCoord0;
	gl_Position = uMvpMatrix * vec4(netPosition,1.0);
}