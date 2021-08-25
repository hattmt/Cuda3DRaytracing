
#ifndef CUDA_TEST_H
#define CUDA_TEST_H

#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "vec3.h"
#include <stdio.h>
#include <Windows.h>

typedef struct {
	vec3 position;
	vec3 position_next;
	vec3 speed;
	vec3 speed_next;
	double r;
	vec3 rgb;
	vec3 material;
	char type; //0:sphere 1:plan 2:vertex
	vec3 normal;
	int mass;
}scene_object;



typedef struct {
	bool verified;
	double value;
	vec3 normal;
}test;

typedef struct {
	UCHAR* pixls1,*pixls2;
	int width1,width2;
	int height1,height2;
}textur;


__device__  test spheretest(vec3 position, vec3 origine, vec3 direction, double r);
#endif