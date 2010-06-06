uniform sampler2D texture0;
uniform sampler2D texture1;
uniform sampler2D texture2;
uniform int renderType;
uniform int test_count;

#define FULL_SCENE 0
#define NORMALS 1
#define DEPTH 2
#define BLOOM 3

void main()
{
    vec2 texLookup;
    vec4 col = texture2D(texture0, gl_TexCoord[0].st);
    vec4 glowcol = texture2D(texture1, gl_TexCoord[0].st);
    //vec4 dist = texture2D(texture2, gl_TexCoord[0].st);
    
    //gl_FragColor = normals;
    
    vec2 texcoord = vec2(gl_TexCoord[0]);
    
    vec4 sum = vec4(0.0);
    
    int min = -test_count;
    int max = test_count;
    
    float change = 0.004 * (3.0 / float(test_count));
    float add = 3.0 / float(test_count);
    
    for(int i = min; i <= max; i++)
    {
      for(int j = min; j <= max; j++)
      {
        sum += texture2D(texture1, texcoord + vec2(j, i)*change) * 0.25 * add;
      }
    }
    
    if (glowcol.r < 0.3)
    {
       gl_FragColor = sum*sum*0.012*add + col;
    }
    else if (glowcol.r < 0.5)
	{
      gl_FragColor = sum*sum*0.009*add + col;
	}
	else
	{
	  gl_FragColor = sum*sum*0.0075*add + col;
	}
	//gl_FragColor = dist;
}
