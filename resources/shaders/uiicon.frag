uniform sampler2D image_tex;
uniform sampler2D alpha_tex;
uniform bool use_alpha;

void main()
{
	vec4 image = texture2D(image_tex, gl_TexCoord[0].st);
	if (use_alpha)
	{
		vec4 alpha = texture2D(alpha_tex, gl_TexCoord[0].st);
		image.a = alpha.r;
	}
	
	gl_FragColor = image;
}
