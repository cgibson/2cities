#include "shader.h"
#include "../helper/GLSL_helper.h"

namespace shader{

  GLint current_shader = 0;

  void reset()
  {
    glUseProgram(0);
    current_shader = 0;
  }

  void setProgram(GLint program)
  {
    glUseProgram(program);
    current_shader = program;
  }

};

Shader::Shader()
{
  shader_program = 0;
}

void Shader::enable()
{
  glUseProgram(shader_program);
  shader::current_shader = shader_program;
}

GLint Shader::getProgram()
{
  return shader_program;
}

int Shader::compile(const GLchar *vert, const GLchar *frag)
{
  //printf("VERTEX SHADER####################\n%s\n\n", vert);
  //printf("FRAGMNET SHADER##################\n%s\n\n", frag);
  GLuint VertVS, FragFS; //handles to shader object
	GLint vCompiled, fCompiled, linked; //status of shader

	GLint shadeProg = -1;
	VertVS = glCreateShader(GL_VERTEX_SHADER);
	FragFS = glCreateShader(GL_FRAGMENT_SHADER);

	//load the source
	glShaderSource(VertVS, 1, &vert, NULL);
	glShaderSource(FragFS, 1, &frag, NULL);

	//compile vertex shader and print log
	glCompileShader(VertVS);
	/* check shader status requires helper functions */
	printOpenGLError();
	glGetShaderiv(VertVS, GL_COMPILE_STATUS, &vCompiled);
	printShaderInfoLog(VertVS);

	//compile shader and print log
	glCompileShader(FragFS);
	/* check shader status requires helper functions */
	printOpenGLError();
	glGetShaderiv(FragFS, GL_COMPILE_STATUS, &fCompiled);
	printShaderInfoLog(FragFS);

	if (!vCompiled || !fCompiled) {
		printf("Error compiling the shader vertex: %d frag: %d\n", vCompiled, fCompiled);
		return -1;
	}

	//create a program object and attach the compiled shader
	shadeProg = glCreateProgram();
	glAttachShader(shadeProg, VertVS);
	glAttachShader(shadeProg, FragFS);

	glLinkProgram(shadeProg);
	/* check shader status requires helper functions */
	printOpenGLError();
	glGetProgramiv(shadeProg, GL_LINK_STATUS, &linked);
	printProgramInfoLog(shadeProg);
	//printf("sucessfully installed shader %d\n", shadeProg);
  printf("test? %d\n", shadeProg);
  printf("test? %d\n", shader_program);
  shader_program = shadeProg;

	return shadeProg;
}

int DefaultShader::load(string file)
{
  string vertfile = file;
  string fragfile = file;
  vertfile.append(".vert");
  fragfile.append(".frag");

  printf("Loading %s\n", vertfile.c_str());
  printf("Loading %s\n", fragfile.c_str());

	//install the shader
	if (compile(textFileRead(vertfile), textFileRead(fragfile)) == -1) {
		printf("Error installing shader!\n");
		return 0;
	}
	return true;
}

void PassShader::setFBO(FBOHelper fbo)
{
  assignedFBO = fbo;
  useFBO = true;
}

void PassShader::setTextures(GLuint *tex, int count)
{
  textures = (GLuint*)malloc(sizeof(GLuint*) * count);
  memcpy(textures, tex, sizeof(GLuint*) * count);
}

void PassShader::update()
{
	glActiveTextureARB( GL_TEXTURE0 );
	glBindTexture(GL_TEXTURE_2D, textures[0] );

	int loc = glGetUniformLocation(shader_program, "texture0");
	glUniform1i(loc, 0);
}

void PassShader::enable()
{
  Shader::enable();
  if(useFBO)
    assignedFBO.enable();
}

void PassShader::disable()
{
  if(useFBO)
    assignedFBO.disable();
}
