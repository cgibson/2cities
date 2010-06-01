uniform sampler2D image_tex;
uniform sampler2D alpha_tex;
uniform vec3 tint_clr;
uniform bool use_alpha;
uniform bool use_tint;

void main()
{
	vec4 image = texture2D(image_tex, gl_TexCoord[0].st);
	if (use_alpha)
	{
		vec4 alpha = texture2D(alpha_tex, gl_TexCoord[0].st);
		image.a = alpha.r;
	}
	if (use_tint)
	{
		image.rgb = image.rgb * tint_clr.rgb;
	}
	
	gl_FragColor = image;
}
