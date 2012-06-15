//====================================================//
// Ocean.vert                                         //
// Project Tikal                                      //
// Ocean Vertex Shader v1.0, september 2009           //
// by Andres J. Porfirio ... andresjesse@yahoo.com.br //
//====================================================//

attribute vec4 inVertexPosition;
attribute vec2 inTexCoord0;

uniform mat4  MvpMatrix;

uniform float WaterTime;
uniform float WaveSize;
uniform float WaveSpeed;

varying vec4 waterpos;
varying vec2 bumpCoord0;
varying vec2 originalCoord;

void main(void)
{	
	originalCoord = inTexCoord0;
	bumpCoord0.xy = inTexCoord0.xy*WaveSize + WaterTime*vec2(-0.01,-0.01)*WaveSpeed;
	
	gl_Position = MvpMatrix * inVertexPosition;
	waterpos = gl_Position;
}

