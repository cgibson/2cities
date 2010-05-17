// force being applied to the block
uniform float force;

// edge settings
uniform float edge_strength;

void main()
{   
    // edge highlights
    vec2 edge_amt = vec2(16.0 * pow((gl_TexCoord[0].s - 0.5), 4.0),
                         16.0 * pow((gl_TexCoord[0].t - 0.5), 4.0));
    vec4 edge = edge_strength * (((edge_amt.s + edge_amt.t) / 2.0) * vec4(1.0, 1.0, 1.0, 1.0));
   
    // compute force color
    float force_amt = clamp(force / 100.0, 0.0, 1.0);
    vec4 force_color = vec4(force_amt, 1.0 - force_amt, 0.0, 1.0);
   
    // final composition
    vec4 final = edge * force_color;
   
    // set the fragment color
    gl_FragColor = final;
}
