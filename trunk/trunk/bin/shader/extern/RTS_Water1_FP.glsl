
//uniform sampler2D LightMapSampler;
uniform sampler2D DiffuseMapSampler;
uniform sampler2D NormalMapSampler;
uniform sampler2D ReflectionMapSampler;

//#define USE_BOTTOM_TEXTURE

#ifdef USE_BOTTOM_TEXTURE
uniform sampler2D FundSampler;
#endif

#if 1
// Optimized 41 cycles
uniform mediump float Glossines;
//uniform mediump float Time;


varying   	mediump vec2 vCoord0;
varying		highp vec2 vCoord01;
varying   	highp vec2 vCoord02;
varying   	lowp float RefLev;

//varying		lowp    vec3 lightVec;
//varying		mediump   vec3 eyeVec;

varying   	lowp    vec4 vColor0;
varying   	lowp	vec3 specCol0;

varying lowp vec3 V;       //creating the eye vector  
varying lowp vec3 L;     //creating the light vector  
varying lowp vec3 H;

 // main procedure, the original name was f
void main()
{
    lowp vec4 DiffuseMap;
    lowp vec4 NormalMap;
    lowp vec4 ReflectionMap;
   lowp vec3 v05 = vec3(0.5,0.5,0.5);
   lowp vec4 v405 = vec4(0.5,0.5,0.5,0.5);
//   lowp vec4 v42 = vec4(2,0.5,0.5,0.5);
   lowp vec3 v1 = vec3(1.,1.,1.);
   // Bump

   // Diffuse color 
   DiffuseMap =  texture2D(DiffuseMapSampler, vCoord0);
   lowp vec3 diffuseColor = DiffuseMap.rgb;// * vColor0.xyz;

//   lowp vec4 normalT0 = texture2D(NormalMapSampler, vCoord01)*2.0 - 1.0;         
//   lowp vec4 normalT1 = texture2D(NormalMapSampler, vCoord02)*2.0 - 1.0;         
//   lowp vec4 normalT = normalize(normalT0 + normalT1);         
//   lowp vec3 bump = normalize(2.*normalT - 1.).rgb;
   
   lowp vec4 normalT0 = texture2D(NormalMapSampler, vCoord01) - v405;         
   lowp vec4 normalT1 = texture2D(NormalMapSampler, vCoord02) - v405;         
   lowp vec3 normalT = 0.8 * (normalT0 + normalT1).xyz;         
   lowp vec3 bump = normalT;//(2.*normalT - 1.0);
   

	//Light
   lowp vec3 N = bump;//normalize(bump.xyz * 2.0 - 1.0).xyz;   //normalized the normal vector 
   lowp float NdotL = max(0.0, dot(N, L)); //calculate the diffuse  

   diffuseColor = diffuseColor * (0.1 + NdotL);
//   diffuseColor = diffuseColor * NdotL;

// Reflection
   lowp vec3 R = reflect(V, N);   

#if 1
	R += v05;
#else   
   // Spherical coords
   lowp float p = dot(R,R);// sqrt(R.x * R.x + R.y * R.y + (R.z + 1.) * (R.z + 1.));
   if(p != 0.)
	   R.xy = R.xy / p + 0.5;
   else
	   R.xy = vec2(0.,0.);
#endif   
   lowp vec4 reflection = texture2D(ReflectionMapSampler, R.xy ) * RefLev;

   // Light
   lowp vec3 specCol = specCol0;
   //highp vec3 H = normalize(L + V);
   // Compute specular color + glossines

   lowp float NdotH = max(0.0, dot(N,H));

   //clamp(dot(reflect(-L, N), lVec), 0.0, 1.0)
   lowp float factor = 0.05;//pow(NdotH, Glossines);
   specCol = specCol * factor;

#if 0
//   // Waves foam
   mediump float waveAmplitude = (normalT0.a + normalT1.a) * factor;
   highp vec3 fp = fract(1.7 * NdotH + 5.0 * bump + 2.1 * V );
//   fp *= (1.0 - fp * waveAmplitude);
//   mediump float glitter = dot(fp,fp)*0.1; //clamp(1.0 - 17.0 * (fp.x + fp.y + fp.z), 0.0, 1.0);
   lowp vec3 glitterColor = fp * waveAmplitude*waveAmplitude * 0.1;
   specCol = specCol + glitterColor;
#elseif 0
//   // Waves foam
   lowp float waveAmplitude = normalT.a;//*normalT.a ;
   //mediump vec3 fp = fract(1.7 * NdotL + 5.0 * bump + 2.1 * V );
   //fp *= (1.0 - fp * waveAmplitude);
//   mediump float glitter = clamp(1.0 - 17.0 * (fp.x + fp.y + fp.z), 0.0, 1.0);
   lowp float glitter = waveAmplitude;// * dot(V,v1);
   //glitter = clamp(glitter, 0.0, 1.0);
   //lowp vec3 glitterColor = vec3(0,0,0);//diffuseColor * (.1 + glitter * 8.1);
   mediump float f = 0.1 + glitter * 0.1;
   lowp vec3 glitterColor = vec3(f,f,f);
   specCol = specCol + glitter * 0.2;//glitterColor;
#endif
   
   diffuseColor = diffuseColor + reflection.rgb;
   

   gl_FragColor = vec4(diffuseColor.xyz, vColor0.a);//DiffuseMap.w);
}
#else
// Unoptimized 150 cycles
   
uniform mediump float SpecLevel;
uniform mediump float Glossines;
uniform mediump float ReflectionLevel;
uniform lowp vec4 specColor;
uniform mediump float Time;

varying   mediump    vec2 vCoord0;
//varying   mediump    vec2 vCoord1;
varying     highp      vec3 lightVec;
varying     highp      vec3 eyeVec;
varying   mediump    vec4 vColor0;

 // main procedure, the original name was f
void main()
{
    lowp vec4 DiffuseMap;
    lowp vec4 NormalMap;
    lowp vec4 ReflectionMap;
    mediump vec2 tcoord;
	mediump float timer = Time;

//    LightMap   = texture2D(LightMapSampler, vCoord1.xy);
   // Bump
   mediump float c1 = 0.2;
   mediump float c2 = 0.4;

   tcoord = vCoord0.xy * 0.2;
   tcoord.x += 0.02  * (timer*c1);
   tcoord.y += 0.02 * (timer*c1);
   mediump vec4 normalT0 = texture2D(NormalMapSampler, tcoord)*2.0 - 1.0;         

   tcoord = vCoord0.xy * 0.1;
   tcoord.x -= 0.01  * (timer*c2);
   tcoord.y -= 0.01 * (timer*c2);
   mediump vec4 normalT1 = texture2D(NormalMapSampler, tcoord)*2.0 - 1.0;         
   mediump vec3 bump = normalize(normalT0 + normalT1).rgb;

   // diffuse color 
   DiffuseMap = texture2D(DiffuseMapSampler, vCoord0.xy + bump.xy*0.05);
   lowp vec3 diffuseColor = DiffuseMap.rgb * vColor0.xyz;

   // Water bottom
#ifdef USE_BOTTOM_TEXTURE
   lowp vec4 DiffuseMap1 = texture2D(FundSampler, vCoord0.xy);
#endif

 // Normal map and specular  
   highp vec3 V = normalize(eyeVec);       //creating the eye vector  
   highp vec3 L = normalize(lightVec);     //creating the light vector  
   highp vec3 N = normalize(bump.xyz * 2.0 - 1.0).xyz;   //normalized the normal vector 
 
   highp float NdotL = max(0.0, dot(N, L)); //calculate the diffuse  

   // Find the reflection vector
   highp vec3 R = reflect(V, N);   
   // Spherical coords
   highp float p = sqrt(R.x * R.x + R.y * R.y + (R.z + 1.) * (R.z + 1.));
   highp float s = 0.;
   highp float t = 0.;
   if(p != 0.)
   {
      s = 0.5 * (1. + R.x / p);
      t = 0.5 * (1. + R.y / p);
   }
   lowp vec2 st = vec2(s,t);
   lowp vec4 reflection = texture2D(ReflectionMapSampler, st);
   
   // Light
   highp vec3 H = normalize(L + V);
   highp float NdotH = max(0.0, dot(N,H));

   // Waves foam
   mediump float waveAmplitude = normalT0.a + normalT1.a;
   highp vec3 fp = fract(1.7 * NdotH + 5.0 * bump + 2.1 * V );
   fp *= (1.0 - fp * waveAmplitude);
   mediump float glitter = clamp(1.0 - 17.0 * (fp.x + fp.y + fp.z), 0.0, 1.0);
   lowp vec3 glitterColor = diffuseColor * (.1 + glitter * 8.1);

   // Compute specular color + glossines
   lowp vec3 specCol = specColor.rgb * SpecLevel;
   specCol = specCol * pow(NdotH, Glossines) +  glitterColor;

#ifdef USE_BOTTOM_TEXTURE
   diffuseColor = mix(diffuseColor, DiffuseMap1.rgb, vColor0.a);
#endif

   //diffuseColor = diffuseColor * NdotH * 0.7 + specCol;
   diffuseColor = diffuseColor * NdotL + specCol + (reflection.rgb * ReflectionLevel);

    gl_FragColor = vec4(diffuseColor.xyz, vColor0.a);//DiffuseMap.w);

} // main end
#endif
