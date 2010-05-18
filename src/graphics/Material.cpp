#include "Material.h"

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

void Material::applyMaterial(GLint program, const char *uniform)
{
  glUseProgram(program);
  int loc = glGetUniformLocation(program, "material.ambient");
  glUniform4fv(loc, 1, ambient);
  printf("I'm getting %d, %d\n", program, loc);
  loc = glGetUniformLocation(program, "material.diffuse");
  glUniform4fv(loc, 1, diffuse);
  loc = glGetUniformLocation(program, "material.specular");
  glUniform4fv(loc, 1, specular);
  loc = glGetUniformLocation(program, "material.shininess");
  glUniform1f(loc, shininess);
}

void Material::applyMaterial(int program, int location) {
  //glUniform4fv(location, 4, const_cast<GLfloat*>(data));
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
