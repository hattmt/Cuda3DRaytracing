#include "cameras.h"
#include <vector>
#include <algorithm>


void Camera(cameras camera);


Vector3D lancer_rayon(Vector3D direction, Vector3D origine, sphere* objets)
{
	Vector3D avance_rayon, rayon_incid, surf_norml, L, R, Hv_med;
	Vector3D Lumiere(3, 5, -6), rgb = Vector3D(0, 0, 0);
	int n = 6;

	int count = -1;


	double spec, diffus, coeffatten = 0.2, k, diff_tmp,coefspec,coefamb,coefdiff;
	double coef = 0.7, coef2 = 0;
	int obj=-1;
	float attenuation, min = 10000;

	double b, a, c, delta, x1, x2;

	for (int j = 0; j < 1; j++)
	{
		//obj = -1;
		count = -1;
		min = 1000;

		for (int i = 0; i < 2; i++) {
			if (objets[i].test(direction, origine))
				if (objets[i].impactdist < min) {
					min = objets[i].impactdist;
					obj = i;
				}
		}

		/*if (obj == -1)
			return Vector3D(-1, 0, 0);*/

		avance_rayon = direction * min + origine;
		surf_norml = (avance_rayon - objets[obj].position).normalize();
		//surf_norml = 

		L = Lumiere - objets[obj].position;
		attenuation = L.norme();
		//	origine = avance_rayon;//reflexion de la lumiere
		//	direction = avance_rayon.reflex(surf_norml);

		//Beep(10000, 10000);

			//k = (L-objets[i].position).norme()-(L-avance_rayon).norme();
		Hv_med = (L - surf_norml).normalize();
		//spec = pow(, 1);

		n = 10;
		spec = max(pow(surf_norml.dot(Hv_med),n), 0);
		diffus = max(surf_norml.dot(L), 0) ;

	//	spec = max((direction*(-1) ).dot(((L).reflex(surf_norml))), 0);

		//rgb =   (objets[obj].rgb*(diffus*0.6+spec*0.8  ))*0.6 + rgb;
		
		coefamb = 0.8;
		coefspec = 0.015;
		coefdiff = 0.2;
		rgb = (objets[obj].rgb*coefamb+objets[obj].rgb* (Vector3D(1, 1, 1) * spec * 255)*coefspec + (objets[obj].rgb * diffus *coefdiff)) * 1/attenuation+ rgb;

	//	rgb = objets[obj].rgb * diffus + rgb;
		//rgb = objets[obj].rgb * spec + rgb;



		//rgb = min(objets[i].rgb * 0.7 + rgb,0);

	//	rgb = objets[i].rgb * spec *0.1 + rgb;
		///rgb =   rgb- Vector3D(1,1,1)* (200/t );

		//return  ( objets[i].rgb * ((min(attenuation * diffus, 1))+ max(spec * 255, 0)*2.75 )/700 ) ;
		//objets[i].rgb*(min(attenuation * diffus, 1))+ 

		//rgb = rgb *0.2;
		origine = avance_rayon;
		direction = avance_rayon.reflex(surf_norml);
		
	}
	//rgb = rgb / 2.0;
	if (rgb.x >= 255)
		rgb.x = 255;
	if (rgb.y >= 255)
		rgb.y = 255;
	if (rgb.z >= 255)
		rgb.z = 255;

	return rgb;
}
	
	



cameras::cameras()
{
}

cameras::cameras(UINT32* data, UINT32 width, UINT32 height, Vector3D position, double distancefoc)
{
	this->width = width;
	this->height = height;
	this->data = data;
	this->distancefocale=distancefoc;
}



cameras::~cameras()
{
}

void cameras::rendu()
{

	
	float angle = 90;
	sphere objarray[2];

	objarray[0] = sphere(Vector3D(2, 1, -3), 1, Vector3D(255, 10, 10), Vector3D(255,10, 10));
	objarray[1] = sphere(Vector3D(0, 1, -3), 1, Vector3D(10, 255, 10), Vector3D(10, 255, 10));

	Vector3D rgb;
	
	float ratioecran = width / (float)height;// ratio taille d ecran
	float ratioW = tan(angle / (float)2 * PI / (float)180) * ratioecran;//ratio largeur
	float ratioH = tan(angle / (float)2 * PI / (float)180);//ratio hauteur

	
	float Rayon_x, Rayon_y;//coordonnées du Rayon en fonction des coordonnees du pixels de l ecran (i,j)

		for (UINT32 i = 0; i < width; i++)
			for (UINT32 j = 0; j < height; j++)
			{
				Rayon_x = (2 * ((i + 0.5) / (float)width) - 1) * ratioW;//on ramene l ecran taille (-1 ,1)(-1,1) et on recupere les vecteurs par rapport a l angle de vision en degrés
				Rayon_y = (1 - 2 * ((j + 0.5) / (float)height) * ratioH);

				rgb = lancer_rayon(((Vector3D(Rayon_x, Rayon_y, -1) + direction).normalize()), position, objarray);//lancé du rayon

				if (rgb.x != -1)
					pixl(data, width, height, i, j, rgb, 0);
				

			}


	
		

}


