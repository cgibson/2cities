uniform sampler2D texture0;
uniform sampler2D texture1;

void main()
{
    vec2 texLookup;
    vec4 col = texture2D(texture0, gl_TexCoord[0].st);
    
    float change = 2.0/512.0;
    
    vec4 accum = vec4(0.0);
    
    for(int i = -2; i <= 2; i++)
    {
      for(int j = -2; j <= 2; j++)
      {
        if(i != 0 && j != 0)
        {
          texLookup = gl_TexCoord[0].st + vec2(float(i) * change, float(j) * change);
          texLookup.s = clamp(texLookup.s, 0.001, 0.999);
          texLookup.t = clamp(texLookup.t, 0.001, 0.999);
          
          accum += texture2D(texture0, texLookup);
        }
      }
    }

    accum /= 16.0;

    col = max(col, accum);
    
    gl_FragData[0] = col;//vec4(1,0,1,1);//col;//(col + accum) / 2.0;
}
