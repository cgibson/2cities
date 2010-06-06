#ifndef _CUSTOMOBJECT_H_
#define _CUSTOMOBJECT_H_

#ifdef _WIN32
    #include <windows.h>
    #include <winGL/glew.h>
	#include <winGL/glut.h>
#else
	#include <GL/glut.h>
#endif

#include "../helper/Vector.h"
#include "../helper/Point.h"
#include "../system/enum.h"
#include "../math/quaternion.h"
#include "../scene/factory.h"
#include "../scene/WorldObject.h"
#include "../scene/Tesselator.h"

/**
 * Incomplete CustomObject class.
 *
 * @author David Haken
 * @version 9 May 2010
 */

using namespace enumeration;

/**
 * CustomObject class. Block for building phase - no physics required.
 */
class CustomObject : public WorldObject
{
	protected:
	Point max, min;
	Tesselation buildingType;
	int buildingCost;

	public:
	CustomObject(unsigned int newid, unsigned int newplayerid, ObjectType newtype, Point newmax, Point newmin) : WorldObject(newid, newplayerid, newtype)
	{
		//id = newid;
		//playerid = newplayerid;
		//type = newtype;
		max = newmax;
		min = newmin;
		buildingType = TESS_NONE;
	}

	virtual void draw();
	virtual void update(int elapsedTime);
	virtual void think(int elapsedTime);

	int get_max_x(){return max.getx();}
	int get_max_y(){return max.gety();}
	int get_max_z(){return max.getz();}
	int get_min_x(){return min.getx();}
	int get_min_y(){return min.gety();}
	int get_min_z(){return min.getz();}

	void set_max_x(int x){max.setx(x);updateCost();}
	void set_max_y(int y){max.sety(y);updateCost();}
	void set_max_z(int z){max.setz(z);updateCost();}
	void set_min_x(int x){min.setx(x);updateCost();}
	void set_min_y(int y){min.sety(y);updateCost();}
	void set_min_z(int z){min.setz(z);updateCost();}

	Point get_max(){return Point(get_max_x(), get_max_y(), get_max_z());}
	Point get_min(){return Point(get_min_x(), get_min_y(), get_min_z());}
	Vector get_max_vector(){return Vector(get_max_x(), get_max_y(), get_max_z());}
	Vector get_min_vector(){return Vector(get_min_x(), get_min_y(), get_min_z());}
	void set_max(Point newmax);
	void set_min(Point newmin);

	int getCost() { return buildingCost; };
	int updateCost();
	Tesselation getBuildingType() { return buildingType; };	
	void setBuildingType(Tesselation newTess) { buildingType = newTess; };

	void draw_rectangle3D();
	void draw_rectangle2D();
	void adjust_bases();
	void print_rectangle();
	Face check_click(Point click);
	int compute_area();
	int compute_volume();
	bool encapsulates(Point max_testpt, Point min_testpt);
	int distance2Face(Face f, Point pt);
	Point whichPoint(Face f);
	bool semiEncapsulates(Face f, Point max_testpt, Point min_testpt);
	void orientRect(Point p1, Point p2);
};

#endif
