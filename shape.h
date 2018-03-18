#ifndef FORCE_RUSH_SHAPE_H
#define FORCE_RUSH_SHAPE_H

#include <vector>
#include <initializer_list>

namespace fr
{
	class Vector2Di
	{
		public:
			Vector2Di() {}
			Vector2Di(int load_x, int load_y);
			Vector2Di(double load_length, double load_angle);
			Vector2Di operator+(Vector2Di input);
			Vector2Di operator-(Vector2Di input);
			Vector2Di operator*(double input);
			Vector2Di operator/(double input);
			int operator*(Vector2Di input);
			double GetIntersectingAngle(Vector2Di input);

			int x;
			int y;
			double length;
			double angle;
	};

	class Point
	{
		public:
			Point() {}
			Point(int load_x, int load_y) : x(load_x), y(load_y) {}
			Vector2Di operator-(Point input);
			Point operator+(Vector2Di input);
			int x;
			int y;
	};

	class Rect
	{
		public:
			Rect() {}
			Rect(int load_x, int load_y, int load_w, int load_h) : x(load_x), y(load_y), w(load_w), h(load_h) {}
			int x;
			int y;
			int w;
			int h;
	};

	class Triangle
	{
		public:
			Triangle() {}
			Triangle(Point load_p1, Point load_p2, Point load_p3) : p1(load_p1), p2(load_p2), p3(load_p3), current_p1(load_p1), current_p2(load_p2), current_p3(load_p3) {}
			void SetRotation(Point load_center, double load_angle);
			bool IsPointOnTriangle(Point input);
			Point p1;
			Point p2;
			Point p3;
			Point current_p1;
			Point current_p2;
			Point current_p3;
			Point center;
			double angle;
	};

	class Shape
	{
		public:
			Shape() {}
			Shape(std::initializer_list<Triangle> load_triangle);
			bool IsPointOnShape(Point input);
			std::vector<Triangle> m_triangle;
	};
};

#endif