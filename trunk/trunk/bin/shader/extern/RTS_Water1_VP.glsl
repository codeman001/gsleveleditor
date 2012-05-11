
attribute highp      vec4 Vertex;
attribute mediump    vec2 TexCoord0;
//attribute mediump    vec2 TexCoord1;
attribute mediump    vec4 Color0;
attribute highp      vec4 Normal;

#define COMPUTE_TNGT_AND_BINORMAL  
 
#ifndef COMPUTE_TNGT_AND_BINORMAL   
attribute highp      vec4 Tangent;
attribute highp      vec4 Binormal;
#endif

uniform lowp float SpecLevel;
uniform lowp vec4 specColor;
uniform mediump float SpeedFactor;

uniform   highp   mat4 WorldViewProjectionMatrix;
uniform   highp   mat4 matworld;
uniform   highp   mat4 matworldi;
uniform   highp   mat4 matviewi;
uniform   highp   mat4 matview;
uniform highp float Time;
uniform lowp float ReflectionLevel;

varying     mediump    vec2 vCoord0;
varying     highp    vec2 vCoord01;
varying     highp    vec2 vCoord02;
//varying     mediump    vec2 vCoord1;
varying     mediump    vec4 vColor0;
varying     lowp 	vec3 specCol0;
varying   	lowp float RefLev;

varying mediump vec3 V;       //creating the eye vector  
varying mediump vec3 L;     //creating the light vector  
varying mediump vec3 H;

 // main procedure, the original name was v
void main()
{
	highp      vec3 lightVec;
	highp      vec3 eyeVec;
	
	RefLev = ReflectionLevel;// * 0.8;

	specCol0 = specColor.rgb * SpecLevel * 0.05;
#if 0
   mediump float c2 = 0.004 * Time;
   vCoord0 =  TexCoord0;
   vCoord01 = vCoord0 * 0.2 + c2;
   vCoord02 = vCoord0 * 0.1 - c2;
#else
   highp vec2 speed1 = vec2(3.,4.)*Time*0.01 * SpeedFactor;
   highp vec2 speed2 = vec2(-1.,-3.)*Time*0.02 * SpeedFactor;
   vCoord0 =  TexCoord0;
   vCoord01 = (vCoord0 + speed1 ) /** 0.2*/;
   vCoord02 = (vCoord0 + speed2 ) /** 0.1*/;
#endif
 //  vCoord1 = TexCoord1;
   //vColor0 = vec4(0.,1.,0.1,1.);//Color0;
   vColor0 = Color0;
   //vColor0 = vColor0 * fract(TexCoord0.x*TexCoord0.y);
   
   vec4 vertex = Vertex;
//   vertex.z = 4.0 * (sin(Vertex.x + Time * 1.2) + cos(Vertex.y + Time * 1.2));
   gl_Position = WorldViewProjectionMatrix * vertex;

#ifdef COMPUTE_TNGT_AND_BINORMAL   
   highp vec3 Tangent2;
   
   if (abs(dot(Normal.xyz, vec3(1.0, 0.0, 0.0))) < 0.5)
      Tangent2 = cross(Normal.xyz, vec3(1.0, 0.0, 0.0)); 
   else
      Tangent2 = cross(Normal.xyz, vec3(0.0, 1.0, 0.0)); 
      
   highp vec3 Binormal2 = cross(Normal.xyz, Tangent2);
#else
   highp vec3 Tangent2 = Tangent.xyz;
   highp vec3 Binormal2 = Binormal.xyz;
#endif   

   // Compute per vertex lightVec and eyeVec 
   
highp mat3 objTangentXf = mat3(Tangent2.x, Binormal2.x, Normal.x,  
								Tangent2.y, Binormal2.y, Normal.y,                        
								Tangent2.z, Binormal2.z, Normal.z);
                        
    highp vec3 wsLPos = (matworld * vertex).xyz;                //put the vert position in world space 
   
    highp vec3 wsLVec = wsLPos - (matviewi * vec4(-0.321, -0.557, 0.766, 0.0)).xyz;

    highp vec3 osLVec = (matworldi *  vec4(wsLVec, 0)).xyz;     //transform the light vector to object space 
   
    lightVec = objTangentXf * osLVec;                     //tangent space light vector passed out 
                                                          //this code was added by the standard material 
    highp vec4 osIPos = matworldi * matviewi[3];                //put world space eye position in object space 
    highp vec3 osIVec = osIPos.xyz - vertex.xyz;                //object space eye vector 
    eyeVec = objTangentXf *  osIVec;                      //tangent space eye vector passed out 
    lightVec = eyeVec;
	V = normalize(eyeVec);       //creating the eye vector  
	L = normalize(lightVec);     //creating the light vector  
   	H = normalize(L + V);
//    lightVec = vec3(eyeVec.x, eyeVec.y, -eyeVec.z);
   
} // main end
