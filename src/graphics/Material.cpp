#include "Material.h"
#include "../graphics/graphics.h"

Material::Material(GLfloat in_ambient[4], GLfloat in_diffuse[4], GLfloat in_specular[4], GLfloat in_shininess[1])
{
  int i;
  for(i = 0; i < 4; i++)
  {
    ambient[i] = in_ambient[i];
    diffuse[i] = in_diffuse[i];
    specular[i] = in_specular[i];
  }
  shininess = in_shininess[0];
}

void Material::applyMaterial(Shader sh, const char *uniform)
{
  GLint old = shader::current_shader;
  int loc = glGetUniformLocation(sh.getProgram(), "material_ambient");
  glUniform4fv(loc, 1, ambient);
  loc = glGetUniformLocation(sh.getProgram(), "material_diffuse");
  glUniform4fv(loc, 1, diffuse);
  loc = glGetUniformLocation(sh.getProgram(), "material_specular");
  glUniform4fv(loc, 1, specular);
  loc = glGetUniformLocation(sh.getProgram(), "material_shininess");
  glUniform1f(loc, shininess);
  shader::setProgram(old);
}

void Material::interpolateFloats(GLfloat *list1, GLfloat *list2, 
                       GLfloat **ret, int count, float t)
{
  float diff;
  for(int i = 0; i < count; i++)
  {
    diff = list2[i] - list1[i];
    (*ret)[i] = list1[i] + diff * t;
  }
}
