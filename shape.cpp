#include "shape.h"
#include <cmath>

fr::Vector::Vector(int load_x, int load_y)
{
	x = load_x;
	y = load_y;
	length = sqrt(x * x + y * y);
	angle = atan(double(y) / double(x));
}

fr::Vector::Vector(double load_length, double load_angle)
{
	length = load_length;
	angle = load_angle;
	x = load_length * cos(load_angle);
	y = load_length * sin(load_angle);
}

fr::Vector fr::Vector::operator+(fr::Vector input)
{
	return Vector(x + input.x, y + input.y);
}

fr::Vector fr::Vector::operator-(fr::Vector input)
{
	return Vector(x - input.x, y - input.y);
}

fr::Vector fr::Vector::operator*(double input)
{
	return Vector(int(x * input), int(y * input));
}

fr::Vector fr::Vector::operator/(double input)
{
	return Vector(int(x / input), int(y / input));
}

int fr::Vector::operator*(fr::Vector input)
{
	return x * input.x + y * input.y;
}

double fr::Vector::GetIntersectingAngle(Vector input)
{
	int output = angle - input.angle;
	output = output > 0 ? output : -output;
	return output;
}

fr::Matrix fr::Matrix::operator*(fr::Matrix &m2)
{
	Matrix output;
	for (int i=0; i<4; i++)
	{
		output.m[i][0] =
			(m[i][0] * m2.m[0][0]) +
			(m[i][1] * m2.m[1][0]) +
			(m[i][2] * m2.m[2][0]) +
			(m[i][3] * m2.m[3][0]);

		output.m[i][1] =
			(m[i][0] * m2.m[0][1]) +
			(m[i][1] * m2.m[1][1]) +
			(m[i][2] * m2.m[2][1]) +
			(m[i][3] * m2.m[3][1]);

		output.m[i][2] =
			(m[i][0] * m2.m[0][2]) + 
			(m[i][1] * m2.m[1][2]) +
			(m[i][2] * m2.m[2][2]) +
			(m[i][3] * m2.m[3][2]);

		output.m[i][3] =
			(m[i][0] * m2.m[0][3]) + 
			(m[i][1] * m2.m[1][3]) +
			(m[i][2] * m2.m[2][3]) +
			(m[i][3] * m2.m[3][3]);
	}
	return output;
}

void fr::Matrix::LoadIdentity()
{
	m[0][0] = 1.0f;
	m[1][1] = 1.0f;
	m[2][2] = 1.0f;
	m[3][3] = 1.0f;
	m[0][1] = 0.0f;
	m[0][2] = 0.0f;
	m[0][3] = 0.0f;
	m[1][0] = 0.0f;
	m[1][2] = 0.0f;
	m[1][3] = 0.0f;
	m[2][0] = 0.0f;
	m[2][1] = 0.0f;
	m[2][3] = 0.0f;
	m[3][0] = 0.0f;
	m[3][1] = 0.0f;
	m[3][2] = 0.0f;
}

void fr::Matrix::Translate(float tx, float ty, float tz)
{
	m[3][0] += (m[0][0] * tx + m[1][0] * ty + m[2][0] * tz);
	m[3][1] += (m[0][1] * tx + m[1][1] * ty + m[2][1] * tz);
	m[3][2] += (m[0][2] * tx + m[1][2] * ty + m[2][2] * tz);
	m[3][3] += (m[0][3] * tx + m[1][3] * ty + m[2][3] * tz);
}

void fr::Matrix::Rotate(float angle, float x, float y, float z)
{
	float sin_angle, cos_angle;
	float mag = sqrtf(x * x + y * y + z * z);
		
	sin_angle = sinf(angle * 3.1415926f / 180.0f);
	cos_angle = cosf(angle * 3.1415926f / 180.0f);
	if(mag > 0.0f)
	{
		float xx, yy, zz, xy, yz, zx, xs, ys, zs;
		float one_minus_cos;
		Matrix rot_mat;
	
		x /= mag;
		y /= mag;
		z /= mag;

		xx = x * x;
		yy = y * y;
		zz = z * z;
		xy = x * y;
		yz = y * z;
		zx = z * x;
		xs = x * sin_angle;
		ys = y * sin_angle;
		zs = z * sin_angle;
		one_minus_cos = 1.0f - cos_angle;

		rot_mat.m[0][0] = (one_minus_cos * xx) + cos_angle;
		rot_mat.m[0][1] = (one_minus_cos * xy) - zs;
		rot_mat.m[0][2] = (one_minus_cos * zx) + ys;
		rot_mat.m[0][3] = 0.0F; 

		rot_mat.m[1][0] = (one_minus_cos * xy) + zs;
		rot_mat.m[1][1] = (one_minus_cos * yy) + cos_angle;
		rot_mat.m[1][2] = (one_minus_cos * yz) - xs;
		rot_mat.m[1][3] = 0.0F;

		rot_mat.m[2][0] = (one_minus_cos * zx) - ys;
		rot_mat.m[2][1] = (one_minus_cos * yz) + xs;
		rot_mat.m[2][2] = (one_minus_cos * zz) + cos_angle;
		rot_mat.m[2][3] = 0.0F; 

		rot_mat.m[3][0] = 0.0F;
		rot_mat.m[3][1] = 0.0F;
		rot_mat.m[3][2] = 0.0F;
		rot_mat.m[3][3] = 1.0F;

		*this = rot_mat * *this;
	}
}

void fr::Matrix::Frustum(float left, float right, float bottom, float top, float near_z, float far_z)
{
	float delta_x = right - left;
	float delta_y = top - bottom;
	float delta_z = far_z - near_z;
	Matrix frust;

	if ((near_z <= 0.0f) || (far_z <= 0.0f) || (delta_x <= 0.0f) || (delta_y <= 0.0f) || (delta_z <= 0.0f))
		 return;

	frust.m[0][0] = 2.0f * near_z / delta_x;
	frust.m[0][1] = frust.m[0][2] = frust.m[0][3] = 0.0f;

	frust.m[1][1] = 2.0f * near_z / delta_y;
	frust.m[1][0] = frust.m[1][2] = frust.m[1][3] = 0.0f;

	frust.m[2][0] = (right + left) / delta_x;
	frust.m[2][1] = (top + bottom) / delta_y;
	frust.m[2][2] = -(near_z + far_z) / delta_z;
	frust.m[2][3] = -1.0f;

	frust.m[3][2] = -2.0f * near_z * far_z / delta_z;
	frust.m[3][0] = frust.m[3][1] = frust.m[3][3] = 0.0f;

	*this = frust * *this;
}

void fr::Matrix::Perspective(float fovy, float aspect, float near_z, float far_z)
{
	float frustum_w, frustum_h;

	frustum_h = tanf(fovy / 360.0f * 3.141592f) * near_z;
	frustum_w = frustum_h * aspect;

	Frustum(-frustum_w, frustum_w, -frustum_h, frustum_h, near_z, far_z);
}

fr::Vector fr::Point::operator-(Point input)
{
	return Vector(x - input.x, y - input.y);
}

fr::Point fr::Point::operator+(fr::Vector input)
{
	return Point(x + input.x, y + input.y);
}

void fr::Triangle::SetRotation(Point load_center, double load_angle)
{
	center = load_center;
	angle = load_angle;
	Vector center2p1 = p1 - center;
	Vector center2p2 = p2 - center;
	Vector center2p3 = p3 - center;

	center2p1 = Vector(center2p1.length, center2p1.angle + load_angle);
	center2p2 = Vector(center2p2.length, center2p2.angle + load_angle);
	center2p3 = Vector(center2p3.length, center2p2.angle + load_angle);
	//Rotation
	current_p1 = center + center2p1;
	current_p2 = center + center2p2;
	current_p3 = center + center2p3;
}

bool fr::Triangle::IsPointOnTriangle(Point input)
{
	Vector point2p1 = current_p1 - input;
	Vector point2p2 = current_p2 - input;
	Vector point2p3 = current_p3 - input;
	int angle_sum = point2p1.GetIntersectingAngle(point2p2) + point2p2.GetIntersectingAngle(point2p3) + point2p3.GetIntersectingAngle(point2p1);
	return angle_sum == 180;
}

fr::Shape::Shape(std::initializer_list<fr::Triangle> load_triangle)
{
	for (auto i = load_triangle.begin(); i != load_triangle.end(); i++)
	{
		m_triangle.push_back(*i);
	}
}

bool fr::Shape::IsPointOnShape(Point input)
{
	bool output = false;
	for (int i = 0; i < m_triangle.size(); i++)
	{
		output = output ? true : m_triangle[i].IsPointOnTriangle(input);
	}
	return output;
}