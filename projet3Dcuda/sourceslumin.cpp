#include "sourceslumin.h"

 sourceslumin::sourceslumin(Vector3D p_origine, Vector3D direction)
{
	this->direction = direction;
	this->p_origine = p_origine;
}



 sourceslumin::sourceslumin()
{
}

 sourceslumin::~sourceslumin()
{
}

 Vector3D sourceslumin::getsourceslumin(double t)
{
	return p_origine +  direction*t;

}
