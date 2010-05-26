uniform sampler2D texture0;

void main()
{
    vec2 texLookup;
    vec4 col = texture2D(texture0, gl_TexCoord[0]);
    
    float change = 2.0/512.0;
    
    vec4 accum;
    
    for(int i = -2; i <= 2; i++)
    {
      for(int j = -2; j <= 2; j++)
      {
        if(i != 0 && j != 0)
        {
          texLookup = gl_TexCoord[0] + vec2(i * change, j * change);
          texLookup.s = clamp(texLookup.s, 0.001, 0.999);
          texLookup.t = clamp(texLookup.t, 0.001, 0.999);
          
          accum += texture2D(texture0, texLookup);
        }
      }
    }

    accum /= 16;

    col = max(col, accum);
    gl_FragData[0] = col;//(col + accum) / 2.0;
}
