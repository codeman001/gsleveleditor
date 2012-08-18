#define MAX_LIGHTS 4

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

uniform mediump vec4  uPointLightPosition   	[MAX_LIGHTS];
uniform mediump vec4  uPointLightAmbient     	[MAX_LIGHTS];
uniform mediump vec4  uPointLightDiffuse     	[MAX_LIGHTS];
uniform mediump vec4  uPointLightSpecular    	[MAX_LIGHTS];
uniform mediump vec3  uPointLightAttenuation	[MAX_LIGHTS];

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

mediump vec3 gNormal;

mediump vec4 lightEquation(int lidx)
{		
	mediump vec4 color = vec4(0.0);
	mediump vec3 lightDir = -uLightDirection[lidx].xyz;			
	
	// add material ambient color
	color += uLightAmbient[lidx] * uMaterialAmbient;
		
	// compute cos(Light, Normal)
	mediump float NdotL = max(dot(normalize(gNormal), lightDir), 0.0);
	color += NdotL * uLightDiffuse[lidx] * uMaterialDiffuse;
		
	// compute specular color cos(hvec, Normal)
	// mediump vec3 hvec = normalize(lightDir + vec3(0.0, 0.0, 1.0));
	// mediump float NdotH = dot(gNormal, hvec);
	// if(NdotH > 0.0)
	//	color += pow(NdotH, uMaterialShininess) * uLightSpecular[lidx] * uMaterialSpecular;
	
	return color;
}

mediump vec4 computeLighting(void)
{
	// base color is ambient color
	mediump vec4 color = uMaterialEmission + uAmbientColor;

	// add with light color
	for ( int i = 0; i < uNumLight; ++i)
	{
		color += lightEquation(i);
	}
	
	color.a = uMaterialDiffuse.a;
	return color;
}

void main(void)
{	
	gNormal = inVertexNormal.xyz;
	varVertexColor = inVertexColor * computeLighting();
	
	varTexCoord0 = inTexCoord0;
	gl_Position = uMvpMatrix * inVertexPosition;	
}