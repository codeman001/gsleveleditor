attribute highp vec4 inVertexPosition;
attribute highp vec4 inVertexColor;
attribute highp vec3 inVertexNormal;
attribute highp vec2 inTexCoord0;
attribute highp vec4 inBoneIndex;
attribute highp vec4 inBoneWeight;

uniform highp mat4 uMvpMatrix;
uniform highp mat4 uBoneMatrix[62];

varying highp vec4 varVertexColor;
varying highp vec2 varTexCoord0;

void main(void)
{	    
	// ---------------------------------
	// SKINNING 
	// ---------------------------------
    highp mat4 netMatrix 	= mat4(0.0);
	highp vec3 netPosition 	= vec3(0.0);
	highp vec3 netNormal	= vec3(0.0);
	
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
	
	varVertexColor = inVertexColor;
	varTexCoord0 = inTexCoord0;
	gl_Position = uMvpMatrix * vec4(netPosition,1.0);
}