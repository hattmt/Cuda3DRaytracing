
#include "vector3D.h"

#pragma once


class sphere
{
private:
	float* vertices;
	
	

public:
	double r;
	 sphere();
	 ~sphere();
	 sphere(Vector3D position, double r, Vector3D materiel, Vector3D rgb);
	 bool test(Vector3D direction, Vector3D origine);
	 void createcube();
	double impactdist;
	
	Vector3D position,rgb,materiel;

};

