/**
 * Loader for modified Blender .obj files. Reads in vertices first, then faces.
 * Code snippets taken from Dr. Zoe Wood
 *
 * @author Kennedy Owen
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

//forward declarations of functions
void readLine(char* str);
void readStream(istream& is);

//very simple data structure to store 3D points
typedef struct Vector3
{
    float x;
    float y;
    float z;

    Vector3(float in_x, float in_y, float in_z) : x(in_x), y(in_y), z(in_z) {}
    Vector3() {}
} Vector3;

//data structure to store triangle -
////note that v1, v2, and v3 are indexes into the vertex array
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
////stl vector to store all the vertices in the mesh
vector<Vector3 *> Vertices;

//Open the file for reading.
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

//prints out all all vertices and faces.
void printVerticesAndFaces()
{
    for(int i = 0; i < Vertices.size(); i++)
    {
        printf("Vertex %d located at: %f %f %f \n", i, Vertices[i]->x, Vertices[i]->y, Vertices[i]->z);
    }
    for(int i = 0; i < Triangles.size(); i++)
    {
        printf("Triangular face %d has vertices: %d %d %d \n", i, Triangles[i]->v1, Triangles[i]->v2, Triangles[i]->v3);
    }
}

int main(int argc, char** argv)
{
    //make sure a file to read is specified
    if (argc > 1)
    {
        cout << "file " << argv[1] << endl;
        //read-in the mesh file specified
        ReadFile(argv[1]);

        //cout << "center " << center.x << " " << center.y << " " << center.z << endl;
        //cout << "scale by " << 1.0/(float)max_extent << endl;
    }
    else
    {
        cout << "format is: meshparser filename" << endl;
    }

    printVerticesAndFaces();
    return 0;
}

