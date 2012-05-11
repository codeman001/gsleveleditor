
attribute highp      vec4 Vertex;
attribute mediump    vec2 TexCoord0;
attribute mediump   vec4   Color0;
attribute highp    vec4 Normal;
attribute highp    vec4 Tangent;
attribute highp    vec4 Binormal;

uniform lowp vec3 lightPosition;

uniform   highp   mat4 WorldViewProjectionMatrix;
uniform   highp   mat4 matworld;
uniform   highp   mat4 matworldi;
uniform   highp   mat4 matviewi;

uniform   highp   float TexFactorX;
uniform   highp   float TexFactorY;


varying	  mediump    vec2 vCoord0;
varying   mediump    vec4 vColor0;
varying	  highp      vec3 vEye;
varying	  highp      vec3 vNormal;
varying	  highp      vec3 vTangent;

 // main procedure, the original name was v
void main()
{
	vCoord0 = TexCoord0;
	vCoord0.x += TexFactorX;
	vCoord0.y += TexFactorY;

	vColor0 = Color0;
	vNormal = Normal.xyz;
	vTangent = Binormal.xyz;
	  
	gl_Position = WorldViewProjectionMatrix * Vertex;
	
	highp vec4 osIPos = matworldi * matviewi[3];              //put world space eye position in object space 
    highp vec3 osIVec = osIPos.xyz - Vertex.xyz;		//object space eye vector 
    vEye = osIVec;                      				//tangent space eye vector passed out 
} // main end
