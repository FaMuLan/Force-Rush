#ifndef FORCE_RUSH_SHAPE_H
#define FORCE_RUSH_SHAPE_H

#include <vector>
#include <initializer_list>

namespace fr
{
	class Vector
	{
		public:
			Vector() {}
			Vector(int load_x, int load_y);
			Vector(double load_length, double load_angle);
			Vector operator+(Vector input);
			Vector operator-(Vector input);
			Vector operator*(double input);
			Vector operator/(double input);
			int operator*(Vector input);
			double GetIntersectingAngle(Vector input);

			int x;
			int y;
			double length;
			double angle;
	};
	
	class Matrix
	{
		public:
			Matrix operator*(Matrix &m2);
			void LoadIdentity();
			void Translate(float tx, float ty, float tz);
			void Rotate(float angle, float x, float y, float z);
			void Frustum(float left, float right, float bottom, float top, float near_z, float far_z);
			void Perspective(float fovy, float aspect, float near_z, float far_z);
			float m[4][4];
	};

	class Point
	{
		public:
			Point() {}
			Point(int load_x, int load_y) : x(load_x), y(load_y) {}
			Vector operator-(Point input);
			Point operator+(Vector input);
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