// force being applied to the block (0 -> 1)
uniform float force;

// shock ripple effect (0 -> 1)
uniform float shock;

void main()
{   
    // edge highlights
    vec2 glow_amt = vec2(64.0 * pow(abs((gl_TexCoord[0].s - 0.5)), 6.0),
                         64.0 * pow(abs((gl_TexCoord[0].t - 0.5)), 6.0));
    vec4 glow = (((glow_amt.s + glow_amt.t) / 2.0) * vec4(1.0, 1.0, 1.0, 1.0));
   
    // compute force color
    float force_amt = clamp(force, 0.0, 1.0);
    vec4 force_clr = vec4(force_amt, 1.0 - force_amt, 0.0, 1.0);
   
    // compute shock contribution
    float shock_amt = clamp(shock, 0.0, 1.0);
    vec4 shock_clr = vec4(shock_amt, shock_amt, shock_amt, 1.0);
   
    // final composition
    vec4 final = (glow * force_clr) + (glow * shock_clr);
    final.a = 1.0;
    
    // set the fragment color
    gl_FragColor = final;
}
