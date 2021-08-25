#pragma once
#include "vector3D.h"

class sourceslumin
{

private:
	Vector3D p_origine;
	Vector3D direction;

public:
	 sourceslumin(Vector3D p_origine,Vector3D direction);
	 sourceslumin();
	 ~sourceslumin();
	 Vector3D getsourceslumin(double t);


};
