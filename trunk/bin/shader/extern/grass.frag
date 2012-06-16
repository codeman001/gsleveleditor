uniform sampler2D grassTexture;

varying vec2 originalCoord;

void main (void)
{
	gl_FragColor = texture2D( grassTexture, originalCoord.st) ;
}
