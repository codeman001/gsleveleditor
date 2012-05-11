//====================================================================//
// Ocean.frag                                                         //
// Project Tikal                                                      //
// Ocean Fragment Shader v1.0, september 2009                         //
// reflection funcion is based in the SIO2 demo from sio2.g0dsoft.com //
// everything else by Andres J. Porfirio ... andresjesse@yahoo.com.br //
//====================================================================//


uniform sampler2D WaterMap;
uniform sampler2D NormalMap;
uniform float WaterTransparent;
uniform float WaterBrightness;

varying vec4 waterpos;
varying vec2 bumpCoord0;
varying vec2 originalCoord;


vec4 reflection()
{
	vec4 projCoord = waterpos / waterpos.w;
	//projCoord += vec4(1.0);
	//projCoord *= 0.5;
	//projCoord = clamp(projCoord, 0.001, 0.999);	
	//projCoord.y = 1.0 - projCoord.y;
	
	vec4 tnmap = texture2D(NormalMap, bumpCoord0);
	return texture2D(WaterMap, vec2(projCoord) + tnmap.xz);
}

void main(void)
{    
    vec4 ref = reflection();
	
	ref*=WaterBrightness;	
	ref.a = WaterTransparent;
	
    gl_FragColor = ref;	
}
