
#include "cuda_test.h"
#include "vec3.h"

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <stdio.h>
#include <stdlib.h>
#include "cameras.h"

__device__ double max_min(double a, double b)
{
	if (a >= b)
		return a;
	else
		return b;
}

__device__ double abs_val(double a)
{
	if (a >= 0)
		return a;
	else
		return -a;

}



__device__  test spheretest(vec3 position, vec3 origine, vec3 direction, double r) {
	double b, a, c, delta, x1, x2, X0, Y0, Z0, t;
	test tmp;

	X0 = (origine.x - position.x);
	Y0 = (origine.y - position.y);
	Z0 = (origine.z - position.z);

	a = direction.x * direction.x + direction.y * direction.y + direction.z * direction.z;
	b = 2 * (direction.x * X0 + direction.y * Y0 + direction.z * Z0);
	c = (X0 * X0 + Y0 * Y0 + Z0 * Z0) - (r) * (r);

	delta = b * b - 4 * a * c;

	if (delta < 0)
	{
		tmp.verified = false;
		return tmp;
	}

	x1 = (-b + sqrt(delta)) / (2.0*a);
	x2 = (-b - sqrt(delta)) / (2.0*a);

	
	

		if (x2 >= 0 && x1 >= 0)
		{
			if (x2 > x1)
				t = x1;
			else
				t = x2;

			tmp.verified = true;
			tmp.value = t;
			return tmp;
		}
		


			tmp.verified = false;
			return tmp;
		

}





__device__  test triangle_MOLLER_TRUMBORE(vec3 origine, vec3 direction, vec3 A, vec3 B, vec3 C) {

	vec3 AB = sub3(B, A);//utilisation de l aire du parallelogramme ||u^v|| et des barycentres
	vec3 AC = sub3(C, A);

	float u, v, t;

	test result;

	vec3 pvec = cross3(direction, AC);
	float  det = dot3(AB, pvec);

	if (det < 0.01)
	{
		result.verified = false;
		return result;
	}

	if (abs_val(det) < 0.01)
	{
		result.verified = false;
		return result;
	}

	float invDet = 1 / det;

	vec3 tvec = sub3(origine, A);
	u = dot3(tvec, pvec) * invDet;
	if (u < 0 || u>1)
	{
		result.verified = false;
		return result;
	}

	vec3 qvec = cross3(tvec, AB);
	v = dot3(direction, qvec) * invDet;
	if (v < 0 || u + v>1)
	{
		result.verified = false;
		return result;
	}

	t = dot3(AC, qvec) * invDet;

	result.value = t;
	result.verified = true;
	return result;

}




__device__  test plan_test(vec3 position, vec3 normale, vec3 origine, vec3 direction) {

	test tmp;

	float denom = dot3(normale, direction);

	if (denom > 0.001) {

		tmp.value = dot3(sub3(position, origine), normale) / denom;
		if (tmp.value >= 0)
			tmp.verified = true;
		else
			tmp.verified = false;
	}
	else
		tmp.verified = false;

	tmp.normal = normale;

	return tmp;
}


__device__ test  test_obj_vertex(vec3 origine, vec3 direction, obj objet) {

	test result;
	vec3 A, B, C;
	double min = 1000;
	int obj = -1;
	
	for (int i = 0; i < objet.f; i++)
	{
		result = triangle_MOLLER_TRUMBORE(origine, direction, objet.vertices[i].B, objet.vertices[i].A, objet.vertices[i].C);
		
		
		
		if (result.verified)
		{
			if (result.value < min)
			{
				min = result.value;
				obj = i;
			}		
		}


	}
	if (obj != -1)
	{
		result.value = min;
		result.normal = objet.vertices[obj + objet.f].A;
		result.verified = true;
		return result;

	}
	else {
	result.verified = false;
	return result;
}

}




__device__ vec3 lancer_rayon_cuda(vec3 direction, vec3 origine, obj obj_file,scene_object* objets,int nb_objts,textur sol_bmp,int width,int height)
{
	vec3 rgb = vec3create(0, 0, 0);
	vec3 rgb_sphere = vec3create(255, 0, 0);
	

	vec3 rgb2 = vec3create(0, 0, 0);
	vec3 position_impact, surf_norml, Hv_med;
	double atten,n;
	double spec, diffus;
	float tmp, tmp1;
	vec3 L;
	char lum = 2;
	vec3 Lumiere[2] = { objets[0].position,vec3create(11.46,7.77,-9.7) };


		int obj=-1;
		double min=1000;
		test test_result;


		
	


		for (int i = 0; i < 2; i++)
		{
			for (int j = 1; j < nb_objts; j++)
			{
				if(sol_bmp.height1 == -1)
					test_result = test_obj_vertex(origine, direction, obj_file);
				else {
					if (objets[j].type == 0)
						test_result = spheretest(objets[j].position, origine, direction, objets[j].r);


					if (objets[j].type == 1)
						test_result = plan_test(objets[j].position, objets[j].normal, origine, direction); 
				}

				if (test_result.verified)
				{

					if (test_result.value < min)
					{
						min = test_result.value;
						obj = j;
					}
				}
			}

			if(obj!=-1){

					position_impact = add3(mul3(direction, min), origine);

					if (sol_bmp.height1 == -1) {
						surf_norml = test_result.normal;
					}
					else {
						if(objets[obj].type==0)
							surf_norml = normalize3(sub3(position_impact, objets[obj].position));

						if (objets[obj].type == 1)
							surf_norml = objets[obj].normal;

					}

					for (int j = 0; j < lum; j++) {
						L = sub3(Lumiere[j], position_impact);
						atten = norm3(L);
						Hv_med = normalize3(sub3(L, surf_norml));
						n = 10;
						spec = pow(dot3(surf_norml, Hv_med), n);

						diffus = dot3(surf_norml, L);

						int pixl;

						if (objets[obj].type == 1 && obj==5) {
									pixl = (position_impact.x * (position_impact.z+20)) + (position_impact.y * (position_impact.z - 20))* sol_bmp.width1/80.0;
									
									if (pixl*3 < sol_bmp.height1 * sol_bmp.width1 && pixl >= 0)
									{
										rgb = add3(mul3(vec3create(sol_bmp.pixls1[pixl * 3], sol_bmp.pixls1[pixl * 3 + 1], sol_bmp.pixls1[pixl * 3 + 2]), max_min((objets[obj].material.x + diffus * objets[obj].material.y + objets[obj].material.z * spec) * (1 / atten * 2), 0)), rgb);
									}
						}
						else
							if (objets[obj].type == 1 && obj == 6)
							{	
								tmp = (int)(position_impact.x*135) % ((int)(sol_bmp.width2/3.0));
								tmp1= (int)(position_impact.y*135 ) % ((int)(sol_bmp.height2/3.0));
								
								pixl = (tmp*3 + tmp1 * sol_bmp.width2*3);

								if (pixl  < sol_bmp.height2 * sol_bmp.width2 && pixl >= 0)
								{
									rgb = add3(mul3(vec3create(sol_bmp.pixls2[pixl ], sol_bmp.pixls2[pixl + 1], sol_bmp.pixls2[pixl  + 2]), max_min((objets[obj].material.x + diffus * objets[obj].material.y + objets[obj].material.z * spec) * (1 / atten * 2), 0)), rgb);
								}
									
							}
						else
						rgb = add3(mul3(objets[obj].rgb, max_min((objets[obj].material.x + diffus * objets[obj].material.y + objets[obj].material.z * spec) * (1 / atten), 0)), rgb);
					}

			}
			origine = position_impact;
			direction = normalize3(reflect3(position_impact, surf_norml));
		}
		

		return div3(rgb,4);

}


__device__ void pixl_cuda(UINT32* data, UINT32 width, UINT32 height, UINT32 x, UINT32 y, UINT32* rgb, UINT32 a)//AFFICHER LES PIXELS AVEC WIN32
{

	if (rgb[0] > 255)
		rgb[0] = 255;

	if (rgb[1] > 255)
		rgb[1] = 255;

	if (rgb[2] > 255)
		rgb[2] = 255;

	if (rgb[0] < 0)
		rgb[0] = 0;

	if (rgb[1] < 0)
		rgb[1] = 0;

	if (rgb[2] < 0)
		rgb[2] = 0;

	UINT32 pixl = x + y * width;

	if (height * width > pixl) {
		UINT32 color = (((UINT32)rgb[0]) << 16) + (((UINT32)rgb[1]) << 8) + ((UINT32)rgb[2]) + ((UINT32)a << 24);
		data[pixl] = color;
	}
}



__global__ void Kernel(UINT32* Pd, int width,int height,float ratioH,float ratioW,float ratioecr,vec3 position,vec3 direction,obj fichiers_obj,scene_object* objets,int nbobj,textur sol_bmp) {

	// Calculate the column index of the Pd element, denote by x
	int x = threadIdx.x + blockIdx.x * blockDim.x;
	// Calculate the row index of the Pd element, denote by y
	int y = threadIdx.y + blockIdx.y * blockDim.y;
	vec3 RGB,rayon;
	
	if (x < width && y < height) {
	

		double Rayon_x, Rayon_y;//coordonnées du Rayon en fonction des coordonnees du pixels de l ecran (i,j)

		Rayon_x = (2 * ((x + 0.5) / (float)width) - 1) * ratioW;//on ramene l ecran taille (-1 ,1)(-1,1) et on recupere les vecteurs par rapport a l angle de vision en degrés
		Rayon_y = (1 - 2 * ((y + 0.5) / (float)height) * ratioH);

		rayon = normalize3(add3(vec3create(Rayon_x, Rayon_y, -1),direction));
	

		UINT32 rgb[3] = { 0 };

		RGB= lancer_rayon_cuda( rayon, position, fichiers_obj,objets,nbobj,sol_bmp,width,height);
		
		for (int i = 0; i < 3; i++)
			rgb[i] = RGB.data[i];

		pixl_cuda(Pd, width, height, x, y, rgb, 0);
	}

}







void Camera_cuda(cameras camera,UINT32* Pd) {

	int size = camera.width * camera.height * sizeof(UINT32);
	

	float angle = 90;

	double height = camera.height;
	double width = camera.width;


	Vector3D rgb;

	float ratioecran = camera.width / (float)camera.height;// ratio taille d ecran
	float ratioW = tan(angle / (float)2 * PI / (float)180) * ratioecran;//ratio largeur
	float ratioH = tan(angle / (float)2 * PI / (float)180);//ratio hauteur


	dim3 dimBlock(16, 16);

	dim3 dimGrid((camera.width + dimBlock.x - 1) / dimBlock.x,
			(camera.height + dimBlock.y - 1) / dimBlock.y);


	vec3 position=camera.position.to_vec3cuda();
	vec3 direction = camera.direction.to_vec3cuda();
	obj fichier_obj = *camera.fichiers_obj;
	scene_object *objets_scene = camera.item;
	scene_object* obj_cuda = NULL;
	textur sol_texture = camera.bmp_sol;

	if (camera.gravity) {

		for (int i = 0; i < camera.items; i++) {
			if (objets_scene[i].type == 0) {
				objets_scene[i].speed = objets_scene[i].speed_next;
				objets_scene[i].position = objets_scene[i].position_next;
			}
		}
	}

	cudaMalloc((void**)&obj_cuda, sizeof(scene_object) * camera.items);
	cudaMemcpy(obj_cuda, objets_scene, sizeof(scene_object) * camera.items, cudaMemcpyHostToDevice);


	Kernel << <dimGrid, dimBlock >> > ( Pd, width,  height, ratioH, ratioW, ratioecran,position,direction, fichier_obj, obj_cuda, camera.items,sol_texture);

	// transfer P from device     
	cudaMemcpy(camera.data, Pd, size, cudaMemcpyDeviceToHost);
	cudaFree(obj_cuda);

	

	// destroy events to free memory
	/*cudaEventDestroy(start);
	cudaEventDestroy(stop);*/
}