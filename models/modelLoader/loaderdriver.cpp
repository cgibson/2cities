#include "loader.h"

//OS-specific headers.
#ifdef _WIN32
    #include <windows.h>
    #include <winGL/glew.h>
	#include <winGL/glut.h>
#else
	#include <GL/glut.h>
#endif

#define FLT_MIN 1.1754E-38F
#define FLT_MAX 1.1754E+38F

using namespace loader;

/* LIGHTING STUFF */

/*globals for lighting - use a white light and apply materials */
int light;

//light position
GLfloat light_pos[4] = {1.0, 1.0, 1.5, 1.0};

//light color (ambiant, diffuse and specular)
GLfloat light_amb[4] = {0.6, 0.6, 0.6, 1.0};
GLfloat light_diff[4] = {0.6, 0.6, 0.6, 1.0};
GLfloat light_spec[4] = {0.8, 0.8, 0.8, 1.0};

int mat = 0;

//set up some materials
typedef struct materialStruct
{
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat shininess[1];
} materialStruct;

materialStruct RedFlat =
{
    {0.3, 0.0, 0.0, 1.0},
    {0.9, 0.0, 0.0, 1.0},
    {0.0, 0.0, 0.0, 1.0},
    {0.0}
};
materialStruct GreenShiny =
{
    {0.0, 0.3, 0.0, 1.0},
    {0.0, 0.9, 0.0, 1.0},
    {0.2, 1.0, 0.2, 1.0},
    {8.0}
};

materialStruct BlueSomething =
{
    {0.0, 0.0, 0.3, 1.0},
    {0.0, 0.0, 0.9, 1.0},
    {0.5, 0.5, 0.5, 1.0},
    {4.0}
};

//for computing the center point and extent of the model
Vector3 center;
float max_x, max_y, max_z, min_x, min_y, min_z;
float max_extent;

//other globals
int GW;
int GH;
int display_mode;
int view_mode;
int drawAll = 1;
int normals; //Draw normals
int fillin = 1; //Fill triangles
float front = 0.5;
float back = 15.0;
float *colors;

//camera globals
GLfloat eye[3] = {0, 0, 3.0};
GLfloat lookat[3] = {0, 0, 0};
GLfloat up[3] = {0, 1, 0};

//forward declarations of functions
void init_lighting();
void pos_light();
void materials(materialStruct materials);
void drawModel();
void drawObjects();
void display();

void init_lighting()
{
    //turn on light0
    glEnable(GL_LIGHT0);
    //Enable auto normalization
    glEnable(GL_NORMALIZE);
    //set up the diffuse, ambient and specular components for the light
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diff);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_amb);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_spec);
    //specify our lighting model as 1 normal per face
    glShadeModel(GL_FLAT);
}

void pos_light()
{
    //set the light's position
    glMatrixMode(GL_MODELVIEW);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
}

void materials(materialStruct materials)
{
    glMaterialfv(GL_FRONT, GL_AMBIENT, materials.ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materials.diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, materials.specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, materials.shininess);
}

void drawSphere()
{
    glColor3f(1.0, 0.0, 0.0);
    glutWireSphere(0.35, 10, 10);
}

void reshape(int w, int h)
{
    GW = w;
    GH = h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    if (view_mode == 0)
    {
        glOrtho( -2.0*(float)w/h, 2.0*(float)w/h, -2.0, 2.0, 1.0, 15.0);
    }
    else
    {
        gluPerspective(60.0, (float)GW/GH, front, back);
    }
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);

    glutPostRedisplay();
}

//Draws a read-in .obj model using vertex, normal and color arrays.
//Note that the colors are just randomly generated initially.
void drawModel()
{
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_VERTEX_ARRAY);
    glNormalPointer(GL_FLOAT, 0, model::normalarray);
    glColorPointer(3, GL_FLOAT, 0, model::colorarray);
    glVertexPointer(3, GL_FLOAT, 0, model::vertexarray);
    glPushMatrix();
    {
        glDrawArrays(GL_TRIANGLES, 0, model::totalfaces * 3);
    }
    glPopMatrix();
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
}

void drawObjects()
{
    unsigned int j;
    //transforms for the mesh
    glPushMatrix();

    //set up the camera
    gluLookAt(1.0, 1.0, 3.0, //eye position
            0.0, 0.0, 0.0, //point we are looking at
            0.0, 1.0, 0.0);  //up vector

    //position the light in the scene
    pos_light();

    //leave these transformations in as they center and scale each mesh correctly
    //scale object to window
//    glScalef(1.0/(float)max_extent, 1.0/(float)max_extent, 1.0/(float)max_extent);

    /* NOTE: MAKE SURE TO ADD INVERSE SCALE SOMEWHERE HERE FOR PROJ 3 */

    //translate the object to the orgin
    glTranslatef(-(center.x), -(center.y), -(center.z));
    
    //draw model here
    drawModel();
    
    glPopMatrix();

    //transforms for the sphere
    glPushMatrix();
    //draw the glut sphere behind the mesh
    glTranslatef(1.25, 0.0, -2.0);
    drawSphere();
    glPopMatrix();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    //set up the camera
    gluLookAt(eye[0], eye[1], eye[2], lookat[0], lookat[1], lookat[2], up[0], up[1], up[2]);
    drawObjects();
    glPopMatrix();
    glutSwapBuffers();

}

void keyboard(unsigned char key, int x, int y)
{
    switch(key)
    {
    case 'q' : case 'Q' :
        printf("Thanks for stopping by!\n");
	free(model::vertexarray);
	free(model::normalarray);
        exit(EXIT_SUCCESS);

    case 'd' : case 'D' :
        if(drawAll == 0)
        {
            drawAll = 1;
        }
        else
        {
            drawAll = 0;
        }
        glutPostRedisplay();
        break;

    case 'n' : case 'N' :
        if(normals == 0)
        {
            normals = 1;
        }
        else
        {
            normals = 0;
        }
        glutPostRedisplay();
        break;

    case 'e' : case 'E' :
        if(fillin == 0)
        {
            fillin = 1;
        }
        else
        {
            fillin = 0;
        }
        glutPostRedisplay();
        break;

    case 'v':
        if(view_mode == 0)
        {
            view_mode = 1;
        }
        else
        {
            view_mode = 0;
        }
        reshape(GW, GH);
        break;

    case 'l':
        light = !light;
        if (light)
        {
            glEnable(GL_LIGHTING);
        }
        else
        {
            glDisable(GL_LIGHTING);
        }
        glutPostRedisplay();
        break;

        //simple way to toggle the materials
    case 'm':
        if(mat == 0)
        {
            materials(RedFlat);
            mat = 1;
        }
        else if(mat == 1)
        {
            materials(GreenShiny);
            mat = 2;
        }
        else if(mat == 2)
        {
            materials(BlueSomething);
            mat = 0;
        }

        glutPostRedisplay();
        break;

    case 'z':
        eye[0] += 0.1;
        lookat[0] += 0.1;
        glutPostRedisplay();
        break;

    case 'Z':
        eye[0] -= 0.1;
        lookat[0] -= 0.1;
        glutPostRedisplay();
        break;

    case 'x':
        eye[2] += 0.1;
        lookat[2] += 0.1;
        glutPostRedisplay();
        break;

    case 'X':
        eye[2] -= 0.1;
        lookat[2] -= 0.1;
        glutPostRedisplay();
        break;


    default:
        break;
    }
}

void introMessage()
{
    printf("Testing out Blender .obj file export.\n\n");
    printf("Controls:\n");
    printf("\tl - turn light on/off (important!)\n");
    printf("\tm - switch material color\n");
    printf("\tv - switch view mode\n");
    printf("\tz/Z - strafe left/right\n");
    printf("\tx/X - zoom in/out\n");
    printf("\tq - quit\n");
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        cout << "Format is: loaderdriver <mesh_obj_file>" << endl;
        exit(EXIT_FAILURE);
    }

    loader::readFile(argv[1]);

    //set up my window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Sample .obj Loader Output");
    glClearColor(1.0, 1.0, 1.0, 1.0);

    //register glut callback functions
    glutDisplayFunc( display );
    glutReshapeFunc( reshape );
    glutKeyboardFunc(keyboard);
    //enable z-buffer
    glEnable(GL_DEPTH_TEST);
    //initialization
    max_x = max_y = max_z = FLT_MIN;
    min_x = min_y = min_z = FLT_MAX;
    center.x = 0;
    center.y = 0;
    center.z = 0;
    display_mode = 0;
    max_extent = 1.0;
    view_mode = 1;
    normals = 0;
    fillin = 1;
    GW = GH = 600;
    light = 0;

    max_extent = max_x - min_x;
    if(max_y - min_y > max_extent)
    {
        max_extent = max_y - min_y;
    }
    center.x = center.x/Vertices.size();
    center.y = center.y/Vertices.size();
    center.z = center.z/Vertices.size();

    init_lighting();
    introMessage();

    glutMainLoop();

    return 0;
}
