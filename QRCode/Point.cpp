#include "stdafx.h"
#include "Point.h"
#include<math.h>

Point::Point()
{
	x = 0;
	y = 0;

}

Point::Point(int a, int b)
{
	x = a;
	y = b;

}

Point::~Point()
{

}

void Point::translate(int dx, int dy)
{
	x += dx;
	y += dy;
}

void Point::set(int a, int b)
{
	x = a;
	y = b;
}

/*CString Point::toString()
{
	CString re="(" + x + "," + y + ")";
	return re;
}*/

Point Point::getCenter(Point p1, Point p2) 
{
	return Point((p1.x+p2.x)/2,(p1.y+p2.y)/2);
}
	
bool Point::equals(Point compare)
{
	return (x == compare.x && y == compare.y);
}
	
int Point::distanceOf(Point other)
{
	int x2 = other.x;
	int y2 = other.y;
	return (int)(sqrt((double)((x - x2)*(x - x2) + (y - y2)*(y - y2))));
}

