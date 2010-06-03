uniform float progression;
uniform vec4 trail_clr;
uniform sampler2D shockwave_tex;

void main(void)
{
    // compute distance from the center of the face
    float dx = gl_TexCoord[0].s - 0.5;
    float dy = gl_TexCoord[0].t - 0.5;
    float dist = sqrt(dx * dx + dy * dy);

    // compute the trail amount
    float trail_amt = 0.0;
    if (dist > 0.05 && dist < 0.5)
    {
        trail_amt = (dist - 0.05) / 0.45;
        trail_amt *= trail_amt;
    }
    
    // compute the ring amount
    float ring_amt = 0.0;
    if (dist > 0.45 && dist < 0.5)
    {
        ring_amt = (dist - 0.45) / 0.05;
        ring_amt *= ring_amt;
    }

    // lookup the trail modulation
    vec2 mod_coords = vec2(0.5 + dx * progression * 4.0, 0.5 + dy * progression * 4.0);
    vec4 trail_mod = texture2D(shockwave_tex, mod_coords);

    // compute ring and trail colors
    vec4 trail = trail_clr * trail_amt * trail_mod + trail_clr * trail_amt * trail_amt;
    vec4 ring = (1.0 - trail_clr) * ring_amt;

	// set the alpha of the entire effect
	ring.a *= (1.0 - progression * progression);
	trail.a *= (1.0 - progression * progression);

    // final composite
    vec4 final = ring + trail;

    gl_FragColor = final;
}
