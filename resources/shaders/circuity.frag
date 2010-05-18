// phong settings
uniform float amb_contrib;
uniform float diff_contrib;

// texture samplers
uniform sampler2D circuit1_tex;
uniform sampler2D circuit2_tex;
uniform sampler2D circuit3_tex;
uniform sampler2D circuit4_tex;
uniform sampler2D sweeper_tex;

// time and random offset inputs
uniform float time;
uniform float rand_offset;

// edge settings
uniform float edge_strength;

// building dimensions (for edge glow)
uniform float width;
uniform float height;

// passed in from vertex shader
varying vec3 view_vec;
varying vec3 light_vec;
varying vec3 normal_vec;

void main()
{
    // normalize all our vectors
    vec3 view = normalize(view_vec);
    vec3 light = normalize(light_vec);
    vec3 normal = normalize(normal_vec);
    
    // calculate phong lighting vectors
    float n_dot_l = clamp(dot(normal, light), 0.0, 1.0);
    vec3 reflection = normalize(((2.0 * normal) * n_dot_l) - light);
    float v_dot_r = clamp(dot(view, reflection), 0.0, 1.0);
   
    // phong lighting term
    vec4 ambient = amb_contrib * vec4(1.0, 1.0, 1.0, 1.0);
    vec4 diffuse = diff_contrib * n_dot_l * vec4(1.0, 1.0, 1.0, 1.0);
    vec4 phong = ambient + diffuse;
   
    // get sweeper texture coords
    vec2 sweep_right_coord = gl_TexCoord[0].st;
    vec2 sweep_left_coord = 1.0 - gl_TexCoord[0].st;
    vec2 sweep_up_coord = gl_TexCoord[0].ts;
    vec2 sweep_down_coord = 1.0 - gl_TexCoord[0].ts;
    
    // animate them (divide by primes to randomize things a bit)
    sweep_right_coord.s -= (time + rand_offset) / 2.0;
    sweep_left_coord.s -= (time + rand_offset) / 5.0;
    sweep_up_coord.s -= (time + rand_offset) / 3.0;
    sweep_down_coord.s -= (time + rand_offset) / 7.0;
    
    // sample sweeper texture
    vec4 sweep_right = texture2D(sweeper_tex, sweep_right_coord);
    vec4 sweep_left = texture2D(sweeper_tex, sweep_left_coord);
    vec4 sweep_up = texture2D(sweeper_tex, sweep_up_coord);
    vec4 sweep_down = texture2D(sweeper_tex, sweep_down_coord);
   
    // get circuit samples
    vec4 circuit1 = texture2D(circuit1_tex, gl_TexCoord[0].st);
    vec4 circuit2 = texture2D(circuit2_tex, gl_TexCoord[0].st);
    vec4 circuit3 = texture2D(circuit3_tex, gl_TexCoord[0].st);
    vec4 circuit4 = texture2D(circuit4_tex, gl_TexCoord[0].st);
   
    // combine each layer (sweeper * alpha map * phong)
    vec4 layer1 = sweep_right * circuit1 * phong;
    vec4 layer2 = sweep_up * circuit2 * phong;
    vec4 layer3 = sweep_left * circuit3 * phong;
    vec4 layer4 = sweep_down * circuit4 * phong;
   
    // edge highlights
    vec2 edge_amt = vec2(16.0 * pow((gl_TexCoord[0].s - 0.5) / width, 4.0),
                         16.0 * pow((gl_TexCoord[0].t - 0.5) / height, 4.0));
    vec4 edge = edge_strength * (((edge_amt.s + edge_amt.t) / 2.0) * vec4(1.0, 1.0, 1.0, 1.0));
   
    // final composition
    vec4 final = layer1 + layer2 + layer3 + layer4 + edge;
   
    // set the fragment color
    gl_FragColor = final;
}