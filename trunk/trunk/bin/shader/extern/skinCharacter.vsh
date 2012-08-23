#define LIGHTING

#ifdef LIGHTING
	#define MAX_LIGHTS 2
	#define MAX_POINTLIGHTS 4
#endif

#define MAX_BONES	58

attribute mediump vec4 inVertexPosition;
attribute mediump vec4 inVertexColor;
attribute mediump vec3 inVertexNormal;
attribute mediump vec2 inTexCoord0;
attribute mediump vec4 inBoneIndex;
attribute mediump vec4 inBoneWeight;

uniform mediump mat4 uMvpMatrix;
uniform mediump mat4 uBoneMatrix[MAX_BONES];

varying mediump vec4 varVertexColor;
varying mediump vec2 varTexCoord0;

#ifdef LIGHTING

uniform mediump vec4  uAmbientColor;

uniform mediump vec4  uLightDirection   [MAX_LIGHTS];
uniform mediump vec4  uLightAmbient     [MAX_LIGHTS];
uniform mediump vec4  uLightDiffuse     [MAX_LIGHTS];
uniform mediump vec4  uLightSpecular    [MAX_LIGHTS];

uniform mediump vec4  uPointLightPosition   	[MAX_POINTLIGHTS];
uniform mediump vec4  uPointLightAmbient     	[MAX_POINTLIGHTS];
uniform mediump vec4  uPointLightDiffuse     	[MAX_POINTLIGHTS];
uniform mediump vec4  uPointLightSpecular    	[MAX_POINTLIGHTS];
uniform mediump vec3  uPointLightAttenuation	[MAX_POINTLIGHTS];

uniform mediump vec4  uMaterialAmbient;
uniform mediump vec4  uMaterialEmission;
uniform mediump vec4  uMaterialDiffuse;
uniform mediump vec4  uMaterialSpecular;
uniform mediump float uMaterialShininess;

#endif

void main(void)
{	    
	// ---------------------------------
	// SKINNING 
	// ---------------------------------
    mediump mat4 netMatrix 		= mat4(0.0);
	mediump vec3 netPosition 	= vec3(0.0);
	mediump vec3 netNormal		= vec3(0.0);
	
	mediump int index = int(inBoneIndex[0]);
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
	mediump vec4 lightColor = uMaterialEmission + uAmbientColor;
	
	// ---------------------------------
	// DIRECTION LIGHT
	// ---------------------------------
	// calc light direction 1	
	mediump vec3 lightDir = -uLightDirection[0].xyz;			
	
	// add material ambient color
	lightColor += uLightAmbient[0] * uMaterialAmbient;
		
	// compute cos(Light, Normal)
	mediump float NdotL = max(dot(normalize(netNormal), lightDir), 0.0);
	lightColor += NdotL * uLightDiffuse[0] * uMaterialDiffuse;
	
	
	// calc light direction 2
	// ---------------------------------
	lightDir = -uLightDirection[1].xyz;			
	
	// add material ambient color
	lightColor += uLightAmbient[1] * uMaterialAmbient;
		
	// compute cos(Light, Normal)
	NdotL = max(dot(normalize(netNormal), lightDir), 0.0);
	lightColor += NdotL * uLightDiffuse[1] * uMaterialDiffuse;
#endif

#ifdef LIGHTING
	varVertexColor = inVertexColor * lightColor;
#else
	varVertexColor = inVertexColor;
#endif

	varTexCoord0 = inTexCoord0;
	gl_Position = uMvpMatrix * vec4(netPosition,1.0);
}