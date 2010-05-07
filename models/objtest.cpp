/*
 * Test for .obj Blender export file render.
 * @author Kennedy Owen
 *
 * Takes code from the basic program to read in a mesh file
 * (of .m format from H. Hoppe) formatted for .obj files, from Dr. Zoe Wood.
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <GL/glut.h>
#include <assert.h>
#include <map>
#include <string.h>

using namespace std;

#define FLT_MIN 1.1754E-38F
#define FLT_MAX 1.1754E+38F

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
typedef struct materialStruct {
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat shininess[1];
} materialStruct;

materialStruct RedFlat = {
    {0.3, 0.0, 0.0, 1.0},
    {0.9, 0.0, 0.0, 1.0},
    {0.0, 0.0, 0.0, 1.0},
    {0.0}
};
materialStruct GreenShiny = {
    {0.0, 0.3, 0.0, 1.0},
    {0.0, 0.9, 0.0, 1.0},
    {0.2, 1.0, 0.2, 1.0},
    {8.0}
};

materialStruct BlueSomething = {
    {0.0, 0.0, 0.3, 1.0},
    {0.0, 0.0, 0.9, 1.0},
    {0.5, 0.5, 0.5, 1.0},
    {4.0}
};

void init_lighting() {
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

void pos_light() {
    //set the light's position
    glMatrixMode(GL_MODELVIEW);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
}

void materials(materialStruct materials) {
    glMaterialfv(GL_FRONT, GL_AMBIENT, materials.ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, materials.diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, materials.specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, materials.shininess);
}

/* MESH STUFF */

//very simple data structure to store 3d points
typedef struct Vector3
{
    float x;
    float y;
    float z;
    Vector3(float in_x, float in_y, float in_z) : x(in_x), y(in_y), z(in_z) {}
    Vector3() {}
} Vector3;

//data structure to store triangle -
//note that v1, v2, and v3 are indexes into the vertex array
typedef struct Tri{
    int v1;
    int v2;
    int v3;

    Vector3 normal;
    Vector3 center;
    Vector3 color;
    Tri(int in_v1, int in_v2, int in_v3) : v1(in_v1), v2(in_v2), v3(in_v3),
    normal(0, 1, 0), center(0, 0, 0){}
    Tri() : normal(0, 1, 0) {}
} Tri;

//stl vector to store all the triangles in the mesh
vector<Tri *> Triangles;
//stl vector to store all the vertices in the mesh
vector<Vector3 *> Vertices;

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

//camera globals
GLfloat eye[3] = {0, 0, 3.0};
GLfloat lookat[3] = {0, 0, 0};
GLfloat up[3] = {0, 1, 0};

//forward declarations of functions
void readLine(char* str);
void readStream(istream& is);
void drawTri(Tri * t);
void drawObjects();
void display();
void DrawAllVerts(void);
void drawNormals(void);
void computeNormals(void);
void computeCenters(void);
void fillTriangles(void);

//open the file for reading
void ReadFile(char* filename) {

    printf("Reading coordinates from %s\n", filename);

    ifstream in_f(filename);
    if (!in_f)  {
        printf("Could not open file %s\n", filename);
    } else {
        readStream(in_f);
    }

}

//process the input stream from the file
void readStream(istream& is)
{
    char str[256];
    for (;is;) {
        is >> ws;
        is.get(str,sizeof(str));
        if (!is) break;
        is.ignore(9999,'\n');
        readLine(str);
    }
}

//process each line of input save vertices and faces appropriately
//Reads in .obj files exported from Blender.
void readLine(char* str)
{
    int indx = 0, vi = 0;
    float x, y, z;
    float r, g, b;
    int mat;

    if (str[0]=='#') return;
    //read a vertex or face
    if (str[0]=='v' && !strncmp(str, "v ", 2))
    {
        Vector3* v;
        if (sscanf(str,"v %g %g %g", &x, &y, &z) !=3)
        {
            printf("an error occurred in reading vertices\n");
#ifdef _DEBUG
            exit(EXIT_FAILURE);
#endif
        }
        v = new Vector3(x, y, z);
        //store the vertex
        Vertices.push_back(v);
        //house keeping to display in center of the scene
        center.x += v->x;
        center.y += v->y;
        center.z += v->z;
        if (v->x > max_x) max_x = v->x; if (v->x < min_x) min_x = v->x;
        if (v->y > max_y) max_y = v->y; if (v->y < min_y) min_y = v->y;
        if (v->z > max_z) max_z = v->z; if (v->z < min_z) min_z = v->z;
    }
    else if (str[0]=='f' && !strncmp(str,"f ",2))
    {
        Tri* t;
        t = new Tri();
        char* s=str+1;
        int fi=0;
        for (int t_i = 0;;t_i++) {
            while (*s && isspace(*s)) s++;
            //if we reach the end of the line break out of the loop
            if (!*s) break;
            //save the position of the current character
            char* beg=s;
            //advance to next space
            while (*s && isdigit(*s)) s++;
            //covert the character to an integer
            int j=atoi(beg);
            //the first number we encounter will be the face index, don't store it
            if (fi<0) { fi=j; continue; }
            //otherwise process the digit we've grabbed in j as a vertex index
            //the first number will be the face id the following are vertex ids
            if (t_i == 0)
                t->v1 = j;
            else if (t_i == 1)
                t->v2 = j;
            else if (t_i == 2)
                t->v3 = j;
            //if there is more data to process break out
            if (*s =='{') break;
        }
        //possibly process colors if the mesh has colors
        if (*s && *s =='{'){
            char *s1 = s+1;
            cout << "trying to parse color " << !strncmp(s1,"rgb",3) << endl;
            //if we're reading off a color
            if (!strncmp(s1,"rgb=",4)) {
                //grab the values of the string
                if (sscanf(s1,"rgb=(%g %g %g) matid=%d",&r,&g,&b,&mat)!=4)
                {
                    printf("error during reading rgb values\n");
#ifdef _DEBUG
                    exit(EXIT_FAILURE);
#endif
                }
                t->color.x = r; t->color.x = g; t->color.x = b;
                cout << "set color to: " << r << " " << g << " " << b << endl;
            }
        }
        //store the triangle read in
        Triangles.push_back(t);
    }
}

//testing routine for parser - left in just as informative code about accessing data
void printFirstThree() {
    printf("first vertex: %f %f %f \n", Vertices[0]->x, Vertices[0]->y, Vertices[0]->z);
    printf("first face: %d %d %d \n", Triangles[0]->v1, Triangles[0]->v2, Triangles[0]->v3);
    printf("second vertex: %f %f %f \n", Vertices[1]->x, Vertices[1]->y, Vertices[1]->z);
    printf("second face: %d %d %d \n", Triangles[1]->v1, Triangles[1]->v2, Triangles[1]->v3);
    printf("third vertex: %f %f %f \n", Vertices[2]->x, Vertices[2]->y, Vertices[2]->z);
    printf("third face: %d %d %d \n", Triangles[2]->v1, Triangles[2]->v2, Triangles[2]->v3);
}

//drawing routine to draw triangles as wireframe
void drawTria(Tri* t) {
    if(display_mode == 0) {
        glBegin(GL_LINE_LOOP);
        glColor3f(0.0, 0.0, 0.5);
        //note that the vertices are indexed starting at 0, but the triangles
        //index them starting from 1, so we must offset by -1!!!
        glVertex3f(Vertices[t->v1 - 1]->x,
                Vertices[t->v1 - 1]->y,
                Vertices[t->v1 - 1]->z);
        glVertex3f(Vertices[t->v2 - 1]->x,
                Vertices[t->v2 - 1]->y,
                Vertices[t->v2 - 1]->z);
        glVertex3f(Vertices[t->v3 - 1]->x,
                Vertices[t->v3 - 1]->y,
                Vertices[t->v3 - 1]->z);
        glEnd();
    }
}

void drawSphere() {
    glColor3f(1.0, 0.0, 0.0);
    glutWireSphere(0.35, 10, 10);
}

//debugging routine which just draws the vertices of the mesh
void DrawAllVerts() {

    glColor3f(0.0, 0.0, 0.5);
    glBegin(GL_POINTS);
    for (unsigned int k=0; k < Vertices.size(); k++) {
        glVertex3f(Vertices[k]->x, Vertices[k]->y, Vertices[k]->z);
    }
    glEnd();

}

void reshape(int w, int h) {
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

void drawObjects() {
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
    glScalef(1.0/(float)max_extent, 1.0/(float)max_extent, 1.0/(float)max_extent);

    /* NOTE: MAKE SURE TO ADD INVERSE SCALE SOMEWHERE HERE FOR PROJ 3 */

    //translate the object to the orgin
    glTranslatef(-(center.x), -(center.y), -(center.z));
    //draw the wireframe mesh
    if(drawAll == 0)
    {
        for(j = 0; j < Triangles.size(); j++)
        {
            drawTria(Triangles[j]);
        }
    }
    //printf("Triangles.size(): %d\n", Triangles.size());

    /* This was only for debugging, to see the vertex locations */
    /*else
      {
      DrawAllVerts();
      }*/
    if(normals == 1)
    {
        drawNormals();
    }
    if(fillin == 1)
    {
        fillTriangles();
    }
    glPopMatrix();

    //transforms for the sphere
    glPushMatrix();
    //draw the glut sphere behind the mesh
    glTranslatef(1.25, 0.0, -2.0);
    drawSphere();
    glPopMatrix();
}

void display() {

    float numV = Vertices.size();

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

/* Computes the normal for the triangle passed in. */
void computeNormals()
{
    float vect1[3];
    float vect2[3];
    float norm[3];
    float normalize;

    for(int triangle = 0; triangle < Triangles.size(); triangle++)
    {
        /* Compute the vectors for each triangle. */
        vect1[0] = ((Vertices[(Triangles[triangle]->v2) - 1]->x) -
                (Vertices[(Triangles[triangle]->v1) - 1]->x));
        vect1[1] = ((Vertices[(Triangles[triangle]->v2) - 1]->y) -
                (Vertices[(Triangles[triangle]->v1) - 1]->y));
        vect1[2] = ((Vertices[(Triangles[triangle]->v2) - 1]->z) -
                (Vertices[(Triangles[triangle]->v1) - 1]->z));
        vect2[0] = ((Vertices[(Triangles[triangle]->v3) - 1]->x) -
                (Vertices[(Triangles[triangle]->v1) - 1]->x));
        vect2[1] = ((Vertices[(Triangles[triangle]->v3) - 1]->y) -
                (Vertices[(Triangles[triangle]->v1) - 1]->y));
        vect2[2] = ((Vertices[(Triangles[triangle]->v3) - 1]->z) -
                (Vertices[(Triangles[triangle]->v1) - 1]->z));

        /* Compute the normalized normal of the two vectors using cross product. */
        norm[0] = (vect1[1] * vect2[2] - vect1[2] * vect2[1]);
        norm[1] = (vect1[2] * vect2[0] - vect1[0] * vect2[2]);
        norm[2] = (vect1[0] * vect2[1] - vect1[1] * vect2[0]);
        normalize = sqrt(norm[0] * norm[0] + norm[1] * norm[1] + norm[2] * norm[2]);
        Triangles[triangle]->normal.x = (norm[0] / normalize);
        Triangles[triangle]->normal.y = (norm[1] / normalize);
        Triangles[triangle]->normal.z = (norm[2] / normalize);
    }
}

/* Compute the center of mass for all triangles. */
void computeCenters()
{
    /* Calculate the x, y, and z points of the triangle's 'middle'. */
    for(int triangle = 0; triangle < Triangles.size(); triangle++)
    {
        Triangles[triangle]->center.x = ((Vertices[(Triangles[triangle]->v1) - 1]->x) +
                (Vertices[(Triangles[triangle]->v2) - 1]->x) +
                (Vertices[(Triangles[triangle]->v3) - 1]->x)) / 3.0;
        Triangles[triangle]->center.y = ((Vertices[(Triangles[triangle]->v1) - 1]->y) +
                (Vertices[(Triangles[triangle]->v2) - 1]->y) +
                (Vertices[(Triangles[triangle]->v3) - 1]->y)) / 3.0;
        Triangles[triangle]->center.z = ((Vertices[(Triangles[triangle]->v1) - 1]->z) +
                (Vertices[(Triangles[triangle]->v2) - 1]->z) +
                (Vertices[(Triangles[triangle]->v3) - 1]->z)) / 3.0;
    }
}

/* Draw the normal for each triangle. */
void drawNormals()
{
    glBegin(GL_LINES);
    glColor3f(0.0, 0.0, 0.0);
    for(int i = 0; i < Triangles.size(); i++)
    {
        /* Draw the normal at the center of the triangle. Phew, ugly! */
        glVertex3f(Triangles[i]->center.x,
                Triangles[i]->center.y, Triangles[i]->center.z);
        glVertex3f((Triangles[i]->center.x + 0.02 * ((Triangles[i]->normal.x) -
                        Vertices[(Triangles[i]->v1) - 1]->x)), (Triangles[i]->center.y +
                    0.02 * ((Triangles[i]->normal.y) - Vertices[(Triangles[i]->v1) - 1]->y)),
                (Triangles[i]->center.z + 0.02 * ((Triangles[i]->normal.z) -
                                                  Vertices[(Triangles[i]->v1) - 1]->z)));

    }
    glEnd();
}

void fillTriangles()
{
    glBegin(GL_TRIANGLES);
    glColor3f(0.0, 0.0, 0.5);
    for(int i = 0; i < Triangles.size(); i++)
    {
        glNormal3f(Triangles[i]->normal.x, Triangles[i]->normal.y, Triangles[i]->normal.z);
        glVertex3f(Vertices[(Triangles[i]->v1) - 1]->x,
                Vertices[(Triangles[i]->v1) - 1]->y, Vertices[(Triangles[i]->v1) - 1]->z);
        glVertex3f(Vertices[(Triangles[i]->v2) - 1]->x,
                Vertices[(Triangles[i]->v2) - 1]->y, Vertices[(Triangles[i]->v2) - 1]->z);
        glVertex3f(Vertices[(Triangles[i]->v3) - 1]->x,
                Vertices[(Triangles[i]->v3) - 1]->y, Vertices[(Triangles[i]->v3) - 1]->z);
    }
    glEnd();
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

int main( int argc, char** argv ) {

    //set up my window
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(600, 600);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("CSC 476 Sample .obj Loader Output");
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

    //make sure a file to read is specified
    if (argc > 1) {
        cout << "file " << argv[1] << endl;
        //read-in the mesh file specified
        ReadFile(argv[1]);
        //Compute the normals of all the triangles.
        //only for debugging
        if (Vertices.size() > 4)
            printFirstThree();

        //once the file is parsed find out the maximum extent to center and scale mesh
        max_extent = max_x - min_x;
        if (max_y - min_y > max_extent) max_extent = max_y - min_y;
        //cout << "max_extent " << max_extent << " max_x " << max_x << " min_x " << min_x << endl;
        //cout << "max_y " << max_y << " min_y " << min_y << " max_z " << max_z << " min_z " << min_z << endl;

        center.x = center.x/Vertices.size();
        center.y = center.y/Vertices.size();
        center.z = center.z/Vertices.size();
        //cout << "center " << center.x << " " << center.y << " " << center.z << endl;
        //cout << "scale by " << 1.0/(float)max_extent << endl;
    } else {
        cout << "format is: meshparser filename" << endl;
    }
    /* Compute normals and centers of the trianglular faces. */
    computeNormals();
    computeCenters();
    init_lighting();

    introMessage();

    glutMainLoop();
}

