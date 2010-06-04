uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform int renderType;

#define FULL_SCENE 0
#define NORMALS 1
#define DEPTH 2
#define BLOOM 3

void main()
{
    vec2 texLookup;
    vec4 col = texture2D(texture0, gl_TexCoord[0].st);
    vec4 glowcol = texture2D(texture1, gl_TexCoord[0].st);
    vec4 normals = texture2D(texture2, gl_TexCoord[0].st);
    
    //gl_FragColor = normals;
    
    float change = 0.5/512.0;
    vec2 texcoord = vec2(gl_TexCoord[0]);
    
    vec4 sum = vec4(0.0);
    
    for(int i = -3; i <= 3; i++)
    {
      for(int j = -3; j <= 3; j++)
      {
        sum += texture2D(texture1, texcoord + vec2(j, i)*0.004) * 0.25;
      }
    }
    
    if (glowcol.r < 0.3)
    {
       gl_FragColor = sum*sum*0.012 + col;
    }
    else if (glowcol.r < 0.5)
	{
      gl_FragColor = sum*sum*0.009 + col;
	}
	else
	{
	  gl_FragColor = sum*sum*0.0075 + col;
	}
	
}
