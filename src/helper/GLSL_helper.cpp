#include "GLSL_helper.h"

using namespace std;

int printOglError (const char *file, int line) {
	/* Returns 1 if an OpenGL error occurred, 0 otherwise. */
	GLenum glErr;
	int    retCode = 0;
	
	glErr = glGetError ();
	while (glErr != GL_NO_ERROR)
    {
		printf ("glError in file %s @ line %d: %s\n", file, line, gluErrorString (glErr));
		retCode = 1;
		glErr = glGetError ();
    }
	return retCode;
}

void printShaderInfoLog (GLuint shader)
{
	GLint     infologLength = 0;
	GLint     charsWritten  = 0;
	GLchar *infoLog;
	
	printOpenGLError ();  // Check for OpenGL errors
	glGetShaderiv (shader, GL_INFO_LOG_LENGTH, &infologLength);
	printOpenGLError ();  // Check for OpenGL errors
	
	if (infologLength > 0)
    {
		infoLog = (GLchar *)malloc (infologLength);
		if (infoLog == NULL)
        {
			puts ("ERROR: Could not allocate InfoLog buffer");
			exit (1);
        }
		glGetShaderInfoLog (shader, infologLength, &charsWritten, infoLog);
		printf ("Shader InfoLog:\n%s\n\n", infoLog);
		free (infoLog);
    }
	printOpenGLError();  // Check for OpenGL errors
}

/* Print out the information log for a program object */
void printProgramInfoLog (GLuint program)
{
	GLint     infologLength = 0;
	GLint     charsWritten  = 0;
	GLchar *infoLog;
	
	printOpenGLError ();  // Check for OpenGL errors
	glGetProgramiv (program, GL_INFO_LOG_LENGTH, &infologLength);
	printOpenGLError ();  // Check for OpenGL errors
	
	if (infologLength > 0)
    {
		infoLog = (GLchar *)malloc (infologLength);
		if (infoLog == NULL)
        {
			puts ("ERROR: Could not allocate InfoLog buffer");
			exit (1);
        }
		glGetProgramInfoLog (program, infologLength, &charsWritten, infoLog);
		printf ("Program InfoLog:\n%s\n\n", infoLog);
		free (infoLog);
    }
	printOpenGLError ();  // Check for OpenGL errors
}

//A helper routine to make it easier to set uniform variables in the shader
GLint getUniLoc(GLuint program, const GLchar *name) {
	GLint loc;
	
	loc = glGetUniformLocation(program, name);
	
	if (loc ==1) {
		printf("No such uniform named %s\n", name);
	}
	
	printOpenGLError();
	return loc;
}

void getGLversion() {
	int major, minor;
	
	major = minor =0;
	const char *verstr = (const char *)glGetString(GL_VERSION);
	
	if ((verstr == NULL) || (sscanf(verstr, "%d.%d", &major, &minor) !=2)) {
		printf("Invalid GL_VERSION format %d %d\n", major, minor);
	}
	if( major <2) {
		printf("This shader example will not work due to opengl version, which is %d %d\n", major, minor);
		exit(0);
	}
}

// textfile.cpp
//
// simple reading and writing for text files
//
// www.lighthouse3d.com
//
// You may use these functions freely.
// they are provided as is, and no warranties, either implicit,
// or explicit are given
//////////////////////////////////////////////////////////////////////

char *textFileRead(string fn) {
	
	
	FILE *fp;
	char *content = NULL;
	
	int count=0;
	
	if (!fn.empty()) {
		fp = fopen(fn.c_str(),"rt");
		
		if (fp != NULL) {
			
			fseek(fp, 0, SEEK_END);
			count = ftell(fp);
			rewind(fp);
			
			if (count > 0) {
				content = (char *)malloc(sizeof(char) * (count+1));
				count = fread(content,sizeof(char),count,fp);
				content[count] = '\0';
			}
			fclose(fp);
		}
	}
	return content;
}

int textFileWrite(char *fn, char *s) {
	
	FILE *fp;
	int status = 0;
	
	if (fn != NULL) {
		fp = fopen(fn,"w");
		
		if (fp != NULL) {
			
			if (fwrite(s,sizeof(char),strlen(s),fp) == strlen(s))
				status = 1;
			fclose(fp);
		}
	}
	return(status);
}

/* function to load a vertex and fragment shader */
int InstallShader(const GLchar *vShaderText, const GLchar *fShaderText, int* shaderID) {
	GLuint VertVS, FragFS; //handles to shader object
	GLint vCompiled, fCompiled, linked; //status of shader
	
	GLint shadeProg;
	VertVS = glCreateShader(GL_VERTEX_SHADER);
	FragFS = glCreateShader(GL_FRAGMENT_SHADER);
	
	//load the source
	glShaderSource(VertVS, 1, &vShaderText, NULL);
	glShaderSource(FragFS, 1, &fShaderText, NULL);
	
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
	
	*shaderID = shadeProg;
	
	return shadeProg;
	
}





