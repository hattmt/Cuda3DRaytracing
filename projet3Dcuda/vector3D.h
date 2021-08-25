#include <iostream>
#pragma once
#include "vec3.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"


class Vector3D {
private:
	

public:
	double x, y, z;
	 Vector3D();
	 Vector3D(double x, double y, double z);
	 ~Vector3D();
	 Vector3D operator+(Vector3D vect2);
	 Vector3D operator-(Vector3D vect2);
	 Vector3D operator*(Vector3D vect2);
	 Vector3D operator*(double scalar);
	 bool operator==(Vector3D vect);
	 bool iscolinear(Vector3D vect);
	 Vector3D operator/(double div);
	 Vector3D reflex(Vector3D normal);
	 Vector3D cross(Vector3D vect);
	 double dot(Vector3D vec);
	 vec3 to_vec3cuda();

	 std::string print();
	 double norme();
	 Vector3D normalize();

};