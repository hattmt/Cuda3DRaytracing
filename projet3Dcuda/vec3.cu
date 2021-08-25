#include <stdio.h>
#include <math.h>
#include "vec3.h"

__device__ vec3 vec3create(double x, double y, double z)
{
	vec3 v_tmp;
	v_tmp.x = x;
	v_tmp.y = y;
	v_tmp.z = z;
	return v_tmp;
}

 vec3 vec3create_(double x, double y, double z)
{
	vec3 v_tmp;
	v_tmp.x = x;
	v_tmp.y = y;
	v_tmp.z = z;
	return v_tmp;
}

__device__ vec3 add3(vec3 v1, vec3 v2)
{
	vec3 v_tmp;
	v_tmp.x = v1.x + v2.x;
	v_tmp.y = v1.y + v2.y;
	v_tmp.z = v1.z + v2.z;
	return v_tmp;
}

 vec3 add3_(vec3 v1, vec3 v2)
{
	vec3 v_tmp;
	v_tmp.x = v1.x + v2.x;
	v_tmp.y = v1.y + v2.y;
	v_tmp.z = v1.z + v2.z;
	return v_tmp;
}

__device__ vec3 sub3(vec3 v1, vec3 v2)
{
	vec3 v_tmp2;
	v_tmp2.x = v1.x - v2.x;
	v_tmp2.y = v1.y - v2.y;
	v_tmp2.z = v1.z - v2.z;
	return v_tmp2;
}

vec3 sub3_(vec3 v1, vec3 v2)
{
	vec3 v_tmp2;
	v_tmp2.x = v1.x - v2.x;
	v_tmp2.y = v1.y - v2.y;
	v_tmp2.z = v1.z - v2.z;
	return v_tmp2;
}

__device__ vec3 mul3(vec3 v1, double nmbr)
{
	vec3 v_tmp;

	for (int i = 0; i < 3; i++)
		v_tmp.data[i] = v1.data[i] * nmbr;

	return v_tmp;
}

vec3 mul3_(vec3 v1, double nmbr)
{
	vec3 v_tmp;

	for (int i = 0; i < 3; i++)
		v_tmp.data[i] = v1.data[i] * nmbr;

	return v_tmp;
}

__device__ vec3 mul3_vec(vec3 v1, vec3 v2)
{
	vec3 v_tmp;

	for (int i = 0; i < 3; i++)
		v_tmp.data[i] = v1.data[i] * v2.data[i];

	return v_tmp;
}

__device__ vec3 div3(vec3 v1, double nmbr)
{
	vec3 v_tmp;

	if (!nmbr)
		return v_tmp;

	for (int i = 0; i < 3; i++)
		v_tmp.data[i] = v1.data[i] / nmbr;

	return v_tmp;
}
 vec3 div3_(vec3 v1, double nmbr)
{
	vec3 v_tmp;

	if (!nmbr)
		return v_tmp;

	for (int i = 0; i < 3; i++)
		v_tmp.data[i] = v1.data[i] / nmbr;

	return v_tmp;
}

__device__ double norm3(vec3 v1)
{
	return sqrt(v1.x * v1.x + v1.y * v1.y + v1.z * v1.z);
}

 double norm3_(vec3 v1)
{
	return sqrt(v1.x * v1.x + v1.y * v1.y + v1.z * v1.z);
}


__device__ vec3 normalize3(vec3 v1)
{
	vec3 v_tmp;
	double normv1 = norm3(v1);

	if (!normv1)
		return vec3create(0,0,0);

	for (int i = 0; i < 3; i++)
		v_tmp.data[i] = v1.data[i] / normv1;

	return v_tmp;
}

vec3 normalize3_(vec3 v1)
{
	vec3 v_tmp;
	double normv1 = norm3_(v1);

	if (!normv1)
		return vec3create_(0, 0, 0);

	for (int i = 0; i < 3; i++)
		v_tmp.data[i] = v1.data[i] / normv1;

	return v_tmp;
}

__device__ double distance3(vec3 v1, vec3 v2)
{
	vec3 v_tmp = sub3(v2, v1);
	return norm3(v_tmp);
}

 double distance3_(vec3 v1, vec3 v2)
{
	vec3 v_tmp = sub3_(v2, v1);
	return norm3_(v_tmp);
}

__device__ double dot3(vec3 v1, vec3 v2)
{
	vec3 v_tmp;

	for (int i = 0; i < 3; i++)
		v_tmp.data[i] = v1.data[i] * v2.data[i];

	return v_tmp.x + v_tmp.y + v_tmp.z;
}

double dot3_(vec3 v1, vec3 v2)
{
	vec3 v_tmp;

	for (int i = 0; i < 3; i++)
		v_tmp.data[i] = v1.data[i] * v2.data[i];

	return v_tmp.x + v_tmp.y + v_tmp.z;
}

__device__ vec3 cross3(vec3 v1, vec3 v2)
{
	vec3 v_tmp;
	v_tmp.x = v1.y * v2.z - v1.z * v2.y;
	v_tmp.y = v1.z * v2.x - v1.x * v2.z;
	v_tmp.z = v1.x * v2.y - v1.y * v2.x;
	return v_tmp;
}

__device__ vec3 reflect3(vec3 v, vec3 vnormal)
{
	return sub3(v, mul3(vnormal, dot3(v, vnormal) * 2));
}

vec3 reflect3_(vec3 v, vec3 vnormal)
{
	return sub3_(v, mul3_(vnormal, dot3_(v, vnormal) * 2));
}

__device__ vec3 set3(double x, double y, double z)
{
	vec3 v;
	v.x = x;
	v.y = y;
	v.z = z;
	return v;
}

__device__ vec3 copy3(vec3 v)
{
	vec3 copy;
	copy.x = v.x;
	copy.y = v.y;
	copy.z = v.z;
	return copy;
}


