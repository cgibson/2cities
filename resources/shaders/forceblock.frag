// force being applied to the block (0 -> 1)
uniform float force;

// shock ripple effect (0 -> 1)
uniform float shock;

uniform vec3 light_position;
uniform vec4 light_diffuse;
uniform vec4 light_ambient;

uniform vec4 material_ambient;
uniform vec4 material_diffuse;
uniform vec4 material_specular;
uniform float material_shininess;

varying vec3 normal;
varying vec3 lightPos;
varying vec3 v;

float linearize(float z)
{
  float n = 20;
  float f = 100.0;
  return (z - n) / (f - n);
}

void main()
{   
	vec3 light_vec = normalize(lightPos);
	float NdotL = clamp(dot(normal,light_vec), 0.0, 1.0);
	
    // edge highlights
    vec2 glow_amt = vec2(64.0 * pow(abs((gl_TexCoord[0].s - 0.5)), 6.0),
                         64.0 * pow(abs((gl_TexCoord[0].t - 0.5)), 6.0));
    vec4 glow = (((glow_amt.s + glow_amt.t) / 2.0) * vec4(1.0, 1.0, 1.0, 1.0));
   
    // compute force color
    float force_amt = clamp(force, 0.0, 1.0);
    vec4 force_clr = vec4(force_amt, force_amt, force_amt, 1.0) * material_diffuse + vec4(1-force_amt, 1-force_amt, 1-force_amt, 1.0) * material_ambient;
   
    // compute shock contribution
    float shock_amt = clamp(shock, 0.0, 1.0);
    vec4 shock_clr = vec4(shock_amt, shock_amt, shock_amt, 1.0);
   
    // final composition
    vec4 final = (glow * force_clr) + (glow * shock_clr);
    final.a = 1.0;
    
    vec4 diffuse = material_ambient * NdotL * 0.1;//* light.diffuse * NdotL;
    vec4 ambient = vec4(0.0,0.0,0.0,1);// * light.ambient;
    vec4 phong = ambient + diffuse;
    
    // set the fragment color
    gl_FragData[0] = final + phong;
    gl_FragData[1] = ((clamp(force - 0.3, 0.0, 1.0) * 0.2 * vec4(1,0,0,1)) + (max(shock, 1.0) * 0.05 * vec4(1,1,1,1))) + vec4(0,0,0,1);
    gl_FragData[1] *= clamp(linearize( length(v)), 0.0, 1.0) * 0.5 + 1;
}
