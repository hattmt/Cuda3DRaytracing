#include "sphere.h"

 sphere::sphere()
{
}

 sphere::~sphere()
{
}

 sphere::sphere(Vector3D position, double r, Vector3D materiel, Vector3D rgb )
{
	this->materiel = materiel;
	this->rgb = rgb;
	this->position = position;
	this->r = r;
}





 bool sphere::test(Vector3D direction,Vector3D origine)
{

	double b, a, c, delta, x1, x2,X0,Y0,Z0,t;

	X0 = (origine.x - this->position.x);
	Y0 = (origine.y - this->position.y);
	Z0 = (origine.z - this->position.z);

	a = direction.x * direction.x + direction.y * direction.y + direction.z * direction.z;
	b = 2 * (direction.x * X0 + direction.y * Y0 + direction.z * Z0);
	c = (X0 * X0 + Y0 * Y0 + Z0 * Z0) - (this->r) * (this->r);

	delta = b * b - 4 * a * c;

	if (delta > 0)
	{
		x1 = (-b + sqrt(delta)) / 2.0;
		x2 = (-b - sqrt(delta)) / 2.0;

		if (x1 > 0) {
			t = x1;
		}

		if (x2 > 0) {
			t = x1;
		}

		this->impactdist =  t;
		return true;
	}
	else
		return false;

}

 void sphere::createcube()
{
	vertices = new float[24]{
		// front
		-1.0, -1.0,  1.0,
		 1.0, -1.0,  1.0,
		 1.0,  1.0,  1.0,
		-1.0,  1.0,  1.0,
		// back
		-1.0, -1.0, -1.0,
		 1.0, -1.0, -1.0,
		 1.0,  1.0, -1.0,
		-1.0,  1.0, -1.0
	};

}


