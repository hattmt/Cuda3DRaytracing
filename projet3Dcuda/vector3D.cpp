#include "vector3D.h"

#include <iostream>
#include <string>

Vector3D::Vector3D()
{
}

Vector3D::Vector3D(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;

}

Vector3D::~Vector3D()
{
}

Vector3D Vector3D::operator+(Vector3D vect2)
{
	Vector3D result;
	result.x = vect2.x + this->x;
	result.y = vect2.y + this->y;
	result.z = vect2.z + this->z;
	return result;
}

Vector3D Vector3D::operator-(Vector3D vect2)
{
	Vector3D result;
	result.x =  this->x - vect2.x;
	result.y = this->y - vect2.y;
	result.z = this->z - vect2.z;
	return result;
}

Vector3D Vector3D::operator*(Vector3D vect2)
{
	Vector3D result;
	result.x = this->x * vect2.x;
	result.y = this->y * vect2.y;
	result.z = this->z * vect2.z;
	return result;
}

Vector3D Vector3D::operator*(double scalar)
{
	Vector3D result;
	result.x = this->x * scalar;
	result.y = this->y * scalar;
	result.z = this->z * scalar;
	return result;
}

bool Vector3D::operator==(Vector3D vect)
{
	if (x == vect.x && y == vect.y && z == vect.z)
		return true;
	else 
		return false;
}

bool Vector3D::iscolinear(Vector3D vect)
{
	if (x * vect.y == y * vect.x && x * vect.z == z * vect.x)
		return true;
	else
		return false;
}

Vector3D Vector3D::operator/(double div)
{
	Vector3D result;
	result.x = this->x /div;
	result.y = this->y / div;
	result.z = this->z / div;
	return result;
}

Vector3D Vector3D::reflex(Vector3D normal)//−(2(n . v) n − v)
{
	double dot3d = normal.dot((*this));
	Vector3D calc= normal * (dot3d * 2);

	return (*this)-calc;
}

Vector3D Vector3D::cross(Vector3D vect)
{
	Vector3D v_tmp;
	v_tmp.x = y * vect.z - z * vect.y;
	v_tmp.y = z * vect.x - x * vect.z;
	v_tmp.z = x * vect.y - y * vect.x;
	return v_tmp;
}

double Vector3D::dot(Vector3D vec)
{
	Vector3D tmp = vec * (*this);
	return tmp.x+tmp.y+tmp.z;
}

vec3 Vector3D::to_vec3cuda()
{
	vec3 vector;
	vector.x = this->x;
	vector.y = this->y;
	vector.z = this->z;
	
	return vector;
}

std::string Vector3D::print()
{
	return "vector: ("+ std::to_string(x)+ " ,"+ std::to_string(y)+" ,"+ std::to_string(z)+")\n" ;
}

double Vector3D::norme()
{
	return sqrt(x * x + y * y + z * z);
}

Vector3D Vector3D::normalize()
{
	double normtmp = this->norme();
	if (normtmp ==0)
		return (*this);
	else
		return (*this) /(double) normtmp;

}



