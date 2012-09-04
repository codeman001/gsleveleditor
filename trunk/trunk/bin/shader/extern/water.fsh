uniform sampler2D WaterMap;
uniform sampler2D NormalMap;
uniform float WaterTransparent;
uniform float WaterBrightness;

varying vec4 waterpos;
varying vec2 bumpCoord0;
varying vec2 originalCoord;

void main(void)
{        
	vec4 projCoord = waterpos / (waterpos.w * 5.0);
	vec4 tnmap = texture2D(NormalMap, bumpCoord0);
	vec4 ref = texture2D(WaterMap, vec2(projCoord) + tnmap.xz);
	
	ref *= WaterBrightness;	
	ref.a = WaterTransparent;
	
    gl_FragColor = ref;	
}
