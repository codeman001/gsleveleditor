attribute mediump vec4 inVertexPosition;
attribute mediump vec4 inVertexColor;
attribute mediump vec3 inVertexNormal;
attribute mediump vec2 inTexCoord0;
attribute mediump vec4 inBoneIndex;
attribute mediump vec4 inBoneWeight;

uniform mediump mat4 uMvpMatrix;
uniform mediump mat4 uBoneMatrix[62];

varying mediump vec4 varVertexColor;
varying mediump vec2 varTexCoord0;

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
	
	varVertexColor = inVertexColor;
	varTexCoord0 = inTexCoord0;
	gl_Position = uMvpMatrix * vec4(netPosition,1.0);
}