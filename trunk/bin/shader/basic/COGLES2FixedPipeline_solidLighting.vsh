#define MAX_LIGHTS 2
#define MAX_POINTLIGHTS 4

attribute highp vec4 inVertexPosition;
attribute highp vec4 inVertexColor;
attribute highp vec3 inVertexNormal;
attribute highp vec2 inTexCoord0;

uniform highp mat4  uMvpMatrix;

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

varying highp vec4 varVertexColor;
varying highp vec2 varTexCoord0;

void main(void)
{	
	// calc light color
	highp vec4 lightColor = uMaterialEmission + uMaterialAmbient*uAmbientColor;
	
	// ---------------------------------
	// DIRECTION LIGHT
	// ---------------------------------
	// calc light direction 1	
	highp vec3 lightDir = -uLightDirection1.xyz;			
		
	// compute cos(Light, Normal)
	highp float NdotL = max(dot(normalize(inVertexNormal.xyz), lightDir), 0.0);
	lightColor += NdotL * uLightDiffuse1 * uMaterialDiffuse;
	
	
	// calc specular light
	//highp vec3 eyeDir = uEyePosLocal - inVertexPosition.xyz;
	//highp vec3 eyeDirection = normalize(eyeDir);
	//highp vec3 halfAngle = normalize(lightDir + eyeDirection);
		
	//highp float NdotH = dot(normalize(inVertexNormal.xyz), halfAngle);
	//highp float spec = pow(max(NdotH, 0.0), max(uMaterialShininess, 1.0));
	//highp vec4 specColor = uLightSpecular1 * spec * uMaterialSpecular;
	//lightColor += specColor;
	
	
	// calc light direction 2
	// ---------------------------------
	lightDir = -uLightDirection2.xyz;
		
	// compute cos(Light, Normal)
	NdotL = max(dot(normalize(inVertexNormal.xyz), lightDir), 0.0);
	lightColor += NdotL * uLightDiffuse2 * uMaterialDiffuse;
	
	// calc specular light
	
	
	// ---------------------------------
	// POINT LIGHT
	// ---------------------------------	
	highp 	float fDistance;
	highp 	float fAttenuation;	
	highp	vec3  fvDirection;
	highp	vec4  fvLightColor;
	
	// Light 1
	fvDirection			= uPointLightPosition1.xyz - inVertexPosition.xyz;
	fDistance			= length(fvDirection);	
	fAttenuation		= max(0.0, 1.0/(fDistance * uPointLightAttenuation1.y + 0.0001));
	NdotL				= max(0.0, dot(normalize(fvDirection), normalize(inVertexNormal.xyz)));
	fvLightColor		= uPointLightDiffuse1 * uMaterialDiffuse * (NdotL * fAttenuation);
	lightColor += fvLightColor;
		
	// Light 2
	fvDirection			= uPointLightPosition2.xyz - inVertexPosition.xyz;
	fDistance			= length(fvDirection);	
	fAttenuation		= max(0.0, 1.0/(fDistance * uPointLightAttenuation2.y + 0.0001));
	NdotL				= max(0.0, dot(normalize(fvDirection), normalize(inVertexNormal.xyz)));
	fvLightColor		= uPointLightDiffuse2 * uMaterialDiffuse * (NdotL * fAttenuation);
	lightColor += fvLightColor;
		
	// Light 3	
	fvDirection			= uPointLightPosition3.xyz - inVertexPosition.xyz;
	fDistance			= length(fvDirection);	
	fAttenuation		= max(0.0, 1.0/(fDistance * uPointLightAttenuation3.y + 0.0001));
	NdotL				= max(0.0, dot(normalize(fvDirection), normalize(inVertexNormal.xyz)));
	fvLightColor		= uPointLightDiffuse3 * uMaterialDiffuse * (NdotL * fAttenuation);
	lightColor += fvLightColor;	
	
	// ---------------------------------
	// VERTEX COLOR
	// ---------------------------------
	lightColor.a = uMaterialDiffuse.a;
	varVertexColor = inVertexColor * lightColor;	
	varTexCoord0 = inTexCoord0;
	gl_Position = uMvpMatrix * inVertexPosition;	
}