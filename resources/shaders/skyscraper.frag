// phong settings
uniform float amb_contrib;
uniform float diff_contrib;

// edge settings
uniform float min_glow;
uniform float max_glow;

// time and random offset
uniform float time;
uniform float rand_offset;

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
    phong.a = 1.0; // reset alpha
    
    // edge highlights
    vec2 edge_amt = vec2(256.0 * pow(abs((gl_TexCoord[0].s - 0.5)), 8.0),
                         256.0 * pow(abs((gl_TexCoord[0].t - 0.5)), 8.0));
    vec4 edge = (((edge_amt.s + edge_amt.t) / 2.0) * vec4(1.0, 1.0, 1.0, 1.0));
    float glow = (((0.5 * sin(time + rand_offset)) + 0.5) * (max_glow - min_glow)) + min_glow;
   
    // final composition
    vec4 final = vec4(glow, glow, glow, 1.0) * edge * phong;
    final.a = 1.0;

    // set the fragment color
    gl_FragData[0] = final;
    gl_FragData[1] = vec4(0,0,0,1);
    gl_FragData[2] = vec4(0,0,0, 1);
}
