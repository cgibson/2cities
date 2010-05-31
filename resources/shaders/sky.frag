// texture samplers
uniform sampler2D cloud_tex;

// time
uniform float time;

void main()
{
    // get cloud texture coords, animate it, and sample it
    vec2 cloud_coords = gl_TexCoord[0].st;
    vec2 cloud_coords1 = vec2(cloud_coords.s + (time / 57.0), cloud_coords.t);
    vec2 cloud_coords2 = vec2(cloud_coords.s, cloud_coords.t + (time / 53.0));
    vec4 cloud_clr = (texture2D(cloud_tex, cloud_coords1)) + (texture2D(cloud_tex, cloud_coords2));
    cloud_clr = (1.0 - cloud_clr) * 2.0;

    // final composition
    vec4 final = 1.0 - cloud_clr;
    final.a = final.r;

    // set the fragment color
    gl_FragColor = final;
}
