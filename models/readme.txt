loader.cpp: reads in vertices and triangular faces from a Blender .obj file and echoes them to stdin.

objtest.cpp: same as loader.cpp, but actually draws the figure (uses skeleton of Dr. Wood's lab 7).
Movement controls shown upon running objtest.cpp.

A sample (modified) Blender .obj file included. It's a humanoid figure with outstretched arms.

Compiling instructions:

To compile loader.cpp: g++ -lGL -lglut -o loader loader.cpp
To compile objtest.cpp: g++ -lGL -lglut -o objtest objtest.cpp

To run loader: ./loader [.obj file]
To run objtest: ./objtest [.obj file]
