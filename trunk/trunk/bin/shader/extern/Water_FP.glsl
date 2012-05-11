uniform sampler2D             DiffuseMapSampler;
uniform sampler2D             BumpMapSampler;
uniform samplerCube           CubeMapSampler;

uniform   highp   mat4 matworld;

//uniform float fadeExp;
//uniform float fadeBias;

varying	  mediump    vec2     vCoord0;
varying   mediump    vec4     vColor0;
varying	  highp      vec3     vEye;
varying	  highp      vec3     vNormal;
varying	  highp      vec3     vTangent;

 // main procedure, the original name was f
void main()
{
  lowp vec4 baseColor = texture2D(DiffuseMapSampler, vCoord0);

  highp vec3 eye = normalize(vEye);
  highp vec3 nrm = normalize(vNormal);
  highp vec3 tgt = normalize(vTangent);
  highp vec3 bit = normalize(cross(nrm, tgt));
  
  highp mat3 objTangentXf = mat3(tgt.x, bit.x, nrm.x,
							     tgt.y, bit.y, nrm.y,  
							     tgt.z, bit.z, nrm.z);

  highp vec3 bump = texture2D(BumpMapSampler, vCoord0).xyz;
  bump = normalize( ( bump * 2.0 ) - 1.0 );

  lowp vec3 bumpR = normalize(objTangentXf * bump);

  highp vec4 reflVec = matworld * vec4(reflect(eye, bumpR), 0.0);
  
  lowp vec4 refl = textureCube(CubeMapSampler, vec3(reflVec.x, reflVec.y, reflVec.z));
  
  lowp float lrp = 1.0 - dot(eye, bumpR);
  const lowp float fadeExp = 3.0;
  const lowp float fadeBias = 0.14;

   // Interpolate between the water color and reflection
  gl_FragColor = mix(baseColor, refl, clamp(fadeBias + pow(lrp, fadeExp), 0.0, 1.0)) * vColor0;
  
} // main end
