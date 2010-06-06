// build grid shader

uniform vec4 material_ambient;
uniform vec4 material_diffuse;
uniform vec4 material_specular;
uniform float material_shininess;
uniform vec4 grid_diffuse;

uniform float grid_size;
uniform float line_pct;
uniform float field_width;
uniform float field_depth;
uniform float border_size;
uniform float division_size;

uniform vec3 light_position;
uniform vec4 light_diffuse;
uniform vec4 light_ambient;

varying vec3 lightPos;
varying vec3 N;
varying vec3 v;  
varying vec3 pos;  
  
float linearize(float z)
{
  float n = 20;
  float f = 100.0;
  return (z - n) / (f - n);
}

#define Integral(x, p, notp) ((floor(x)*(p)) + max(fract(x)-(notp), 0.0))
  
void main (void) 
{
    float thick_size = 5.0;
    vec3 view_vec = normalize(v);
    vec3 light_vec = normalize(lightPos);
    vec3 normal = normalize(N);
    
    vec2 position1 = v.xz / grid_size;
    vec2 position2 = v.xz / (grid_size * thick_size);
    
    vec2 useBrick1;
    vec2 fw1;
    vec2 useBrick2;
    vec2 fw2;
    
    fw1 = fwidth(position1);
    fw2 = fwidth(position2);
    
    position1 = fract(position1);
    position2 = fract(position2);
    
    vec2 pct1 = vec2(1.0 - line_pct, 1.0 - line_pct);
    vec2 upct1 = vec2(line_pct, line_pct);
    
    vec2 pct2 = vec2(1.0 - (line_pct / 2.0), 1.0 - (line_pct / 2.0));
    vec2 upct2 = vec2(line_pct / 2.0, line_pct / 2.0);
    
    useBrick1 = (Integral(position1 + fw1, pct1, upct1) -
                Integral(position1, pct1, upct1)) / fw1;
    useBrick2 = (Integral(position2 + fw2, pct2, upct2) -
                Integral(position2, pct2, upct2)) / fw2;
    
    vec4 diffuse1 = mix(grid_diffuse, material_diffuse, clamp( (useBrick1.x * useBrick1.y), 0.0, 1.0));
    vec4 diffuse2 = mix(grid_diffuse, material_diffuse, clamp( (useBrick2.x * useBrick2.y), 0.0, 1.0));
    
    float balance = 0.2;
    
    vec4 diffuse = vec4(0,0,0,1);
    
    vec3 reflection = normalize( -reflect(view_vec,normal) );

    float NdotL = clamp(dot(normal,light_vec), 0.0, 1.0);
    
    float width = field_width / 2.0;
    float depth = field_depth / 2.0;

    if((v.x < width) && (v.x > -width) &&
       (v.z < depth) && (v.z > -depth))
    {
	  if((v.x < division_size) && (v.x > -division_size))
	  {
		float colorflip = sin((v.x + v.z) * 2.0);
        if(colorflip > 0.0)
          diffuse = vec4(0.5,0,0,1);
        else
          diffuse = vec4(0.1,0,0,1);
	  }
        diffuse += (diffuse1 * balance) + (diffuse2 * (1.0 - balance));
        
    }else if((v.x < (width + border_size)) && (v.x > -(width + border_size)) &&
             (v.z < (depth + border_size)) && (v.z > -(depth + border_size)))
    {
      float colorflip = sin((v.x + v.z) * 2.0);
      if(colorflip > 0.0)
        diffuse = vec4(1,1,0,1);
      else
        diffuse = vec4(0,0,0,1);
    }else{
      discard;
    }
    
    diffuse = diffuse * NdotL;
    
    vec4 ambient = material_ambient;
    vec4 phong = ambient + diffuse;
    
    gl_FragData[0] = phong;
    gl_FragData[1] = vec4(0,0,0,1);
    gl_FragData[2] = vec4(linearize( length(pos)));
    gl_FragData[2].a = 1.0;
  }
