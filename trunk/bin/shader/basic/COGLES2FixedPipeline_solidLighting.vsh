#define MAX_LIGHTS 2
#define MAX_POINTLIGHTS 4

attribute mediump vec4 inVertexPosition;
attribute mediump vec4 inVertexColor;
attribute mediump vec3 inVertexNormal;
attribute mediump vec2 inTexCoord0;

uniform mediump mat4  uMvpMatrix;

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

uniform mediump int   uNumLight;
uniform mediump int   uPointLight;

uniform mediump vec4  uMaterialAmbient;
uniform mediump vec4  uMaterialEmission;
uniform mediump vec4  uMaterialDiffuse;
uniform mediump vec4  uMaterialSpecular;
uniform mediump float uMaterialShininess;
uniform mediump int   uColorMaterial;

varying mediump vec4 varVertexColor;
varying mediump vec2 varTexCoord0;

void main(void)
{	
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
	mediump float NdotL = max(dot(normalize(inVertexNormal.xyz), lightDir), 0.0);
	lightColor += NdotL * uLightDiffuse[0] * uMaterialDiffuse;
	
	
	// calc light direction 2
	// ---------------------------------
	lightDir = -uLightDirection[1].xyz;			
	
	// add material ambient color
	lightColor += uLightAmbient[1] * uMaterialAmbient;
		
	// compute cos(Light, Normal)
	NdotL = max(dot(normalize(inVertexNormal.xyz), lightDir), 0.0);
	lightColor += NdotL * uLightDiffuse[1] * uMaterialDiffuse;
	
	
	
	// ---------------------------------
	// POINT LIGHT
	// ---------------------------------
	

	
		
	// ---------------------------------
	// VERTEX COLOR
	// ---------------------------------
	lightColor.a = uMaterialDiffuse.a;
	varVertexColor = inVertexColor * lightColor;	
	varTexCoord0 = inTexCoord0;
	gl_Position = uMvpMatrix * inVertexPosition;	
}