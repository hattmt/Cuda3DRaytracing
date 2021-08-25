#pragma once
#include<Windows.h>
#include "sphere.h"
#include "sourceslumin.h"
#include "vec3.h"
#include "cuda_test.h"
#define PI 3.14159265359 





typedef union
{
	struct {
		vec3 A;
		vec3 B;
		vec3 C;
		vec3 D;
	};

	vec3 data[4];/*!< accès aux variables de la structure sous la forme d'un tableau. */

}vertex;

typedef struct {
	vertex* vertices;
	int f;
}obj;


class cameras {

	private:
		
	
	

	public:
		double distancefocale ;
		cameras();
		cameras(UINT32* data,UINT32 width, UINT32 height, Vector3D position, double distancefoc);
		~cameras();
		void rendu();
		scene_object* item;
		obj* fichiers_obj;
		int items;
		textur bmp_sol;

		Vector3D position;
		Vector3D direction;
		UINT32* data;
		UINT32 width, height;
		int gravity = 1;;
};

void pixl(UINT32* data, UINT32 width, UINT32 height, UINT32 x, UINT32 y, Vector3D rgb, UINT32 a);