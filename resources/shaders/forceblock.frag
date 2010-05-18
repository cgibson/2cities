// force being applied to the block
uniform float force;

// edge settings
uniform float edge_strength;

void main()
{   
    // edge highlights
    vec2 edge_amt = vec2(64.0 * pow(abs((gl_TexCoord[0].s - 0.5)), 6.0),
                         64.0 * pow(abs((gl_TexCoord[0].t - 0.5)), 6.0));
    vec2 black_edge_amt = vec2(256.0 * pow(abs((gl_TexCoord[0].s - 0.5)), 8.0),
                         256.0 * pow(abs((gl_TexCoord[0].t - 0.5)), 8.0));
    vec4 edge = clamp(edge_strength, 0.4, 1.0) * (((edge_amt.s + edge_amt.t) / 2.0) * vec4(1.0, 1.0, 1.0, 1.0));
    vec4 black_edge = vec4(1,1,1,1) - (((edge_amt.s + edge_amt.t) / 2.0) * vec4(1.0, 1.0, 1.0, 1.0));
   
    // compute force color
    float force_amt = clamp(force / 100.0, 0.0, 1.0);
    vec4 force_color = vec4(force_amt, 1.0 - force_amt, 0.0, 1.0);
   
    // final composition
    vec4 final = vec4(0,0,0,1) + (edge * force_color) * black_edge;
   
   final.a = 1.0;
    // set the fragment color
    gl_FragColor = final;
}
