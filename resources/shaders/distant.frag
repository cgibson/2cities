// texture samplers
uniform sampler2D sweeper_tex;
uniform sampler2D circuit_tex;
uniform sampler2D skyline_alpha;

// time input
uniform float time;

void main()
{
    vec2 skyline_coords = gl_TexCoord[0].st;
    vec4 alpha = texture2D(skyline_alpha, skyline_coords);
    if (alpha.r < 0.9 || alpha.g < 0.9 || alpha.b < 0.9)
    {
        discard;
    }

    vec2 coords = gl_TexCoord[0].st * 2.0;

    // get sweeper texture coords
    vec2 sweep_right_coord = coords.st;
    vec2 sweep_left_coord = 1.0 - coords.st;
    vec2 sweep_up_coord = coords.ts;
    vec2 sweep_down_coord = 1.0 - coords.ts;
    
    // animate them (divide by primes to randomize things a bit)
    sweep_right_coord.s -= time / 2.0;
    sweep_left_coord.s -= time / 5.0;
    sweep_up_coord.s -= time / 3.0;
    sweep_down_coord.s -= time / 7.0;
    
    // sample sweeper texture
    vec4 sweep_right = texture2D(sweeper_tex, sweep_right_coord);
    vec4 sweep_left = texture2D(sweeper_tex, sweep_left_coord);
    vec4 sweep_up = texture2D(sweeper_tex, sweep_up_coord);
    vec4 sweep_down = texture2D(sweeper_tex, sweep_down_coord);
   
    // get circuit samples
    vec2 circuit1_coords = vec2(coords.s / 2.0, (coords.t / 2.0) + 0.5);
    vec4 circuit1 = texture2D(circuit_tex, circuit1_coords);
    vec2 circuit2_coords = vec2((coords.s / 2.0) + 0.5, (coords.t / 2.0) + 0.5);
    vec4 circuit2 = texture2D(circuit_tex, circuit2_coords);
    vec2 circuit3_coords = vec2(coords.s / 2.0, coords.t / 2.0);
    vec4 circuit3 = texture2D(circuit_tex, circuit3_coords);
    vec2 circuit4_coords = vec2((coords.s / 2.0) + 0.5, coords.t / 2.0);
    vec4 circuit4 = texture2D(circuit_tex, circuit4_coords);
   
    // combine each layer (sweeper * alpha map)
    vec4 layer1 = sweep_right * circuit1 * 0.65;
    vec4 layer2 = sweep_up * circuit2 * 0.65;
    vec4 layer3 = sweep_left * circuit3 * 0.65;
    vec4 layer4 = sweep_down * circuit4 * 0.65;
   
    // final composition
    vec4 final = layer1 + layer2 + layer3 + layer4;
    final.a = final.r;
   
    // set the fragment color
    gl_FragColor = final;
}
