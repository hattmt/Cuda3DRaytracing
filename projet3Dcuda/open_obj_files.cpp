#include "open_obj_files.h"



vertex ananlyse(char* txt, int mode)
{
	vertex tmp;

	bool number = false;
	int count = 0;
	double a, b;
	int size = strlen(txt);
	char is_found = 0;
	tmp.data[3].x = -1;

	for (int j = 0; txt[j] != '\n' && j < size; j++) {


		if ((txt[j] >= '0' && txt[j] <= '9' || txt[j] == '-') && !number)
		{
			is_found = 0;
			if (mode == 0)
				is_found = sscanf(&txt[j], "%lf", (&tmp.A.data[count]));
			if (mode == 1)
				is_found = sscanf(&txt[j], "%lf/%lf/%lf", (&tmp.data[count].x), (&tmp.data[count].y), (&tmp.data[count].z));
			/*if (mode == 2)
				is_found=sscanf(&txt[j], "%lf/%lf/%lf ", &a, (&tmp.data[count]), &b);
			if (mode == 3)
				is_found=sscanf(&txt[j], "%lf/%lf/%lf ", &a,&b,(&tmp.data[count]));*/

			if (is_found != 0) {
				number = true;
				count++;
			}

		}
		if (txt[j] == ' ')
			number = false;
	}
	return tmp;
}

obj alloccudavertex(obj objet)
{

	obj result;
	result.vertices = NULL;
	result.f = objet.f;

	cudaMalloc((void**)&result.vertices, sizeof(vertex) * objet.f * 3);
	cudaMemcpy(result.vertices, objet.vertices, sizeof(vertex) * objet.f * 3, cudaMemcpyHostToDevice);

	return result;
}



obj open_obj(char* file)
{
	FILE* read;
	obj result;

	int v = 0, vn = 0, ct = 0, size = 0, f = 0;
	int ctv = 0, ctn = 0, ctf = 0;
	vec3 vec3_tmp;
	FILE* read_tmp;

	read = fopen(file, "rb");

	fseek(read, SEEK_SET, SEEK_END);
	size = ftell(read);
	rewind(read);

	char* txt = (char*)malloc(sizeof(char) * size);

	fread(txt, 1, size, read);

	for (int i = 1; i < size; i++)
	{
		if (txt[i - 1] == 'v' && txt[i] == ' ')
			v++;

		if (txt[i - 1] == 'v' && txt[i] == 'n')
			vn++;

		if (txt[i - 1] == 'f' && txt[i] == ' ')
			f++;

	}



	vec3* normales = (vec3*)calloc(sizeof(vec3), vn);
	vec3* vertices = (vec3*)calloc(sizeof(vec3), v);
	vertex* vertex_ = (vertex*)malloc(sizeof(vertex) * (f * 3));


	for (int i = 1; i < size; i++)
	{
		if (txt[i - 1] == 'v' && txt[i] == ' ')
		{

			vertices[ctv] = ananlyse(&txt[i + 1], 0).A;

			std::cout << vertices[ctv].x << " " << vertices[ctv].y << " " << vertices[ctv].z << " " << std::endl;
			ctv++;
		}

		if (txt[i - 1] == 'v' && txt[i] == 'n') {
			normales[ctn] = ananlyse(&txt[i + 1], 0).A;

			std::cout << normales[ctn].x << " " << normales[ctn].y << " " << normales[ctn].z << " " << std::endl;
			ctn++;
		}


		if (txt[i - 1] == 'f' && txt[i] == ' ')
		{
			vertex tmp_vertex = ananlyse(&txt[i + 1], 1);

			for (int j = 0; j < 3; j++) {
				std::cout << (int)tmp_vertex.data[j].x - 1 << " ";
				vertex_[ctf].data[j] = vertices[(int)tmp_vertex.data[j].x - 1];


			}
			if ((int)tmp_vertex.data[3].x != -1)
				vertex_[ctf].data[3] = vertices[(int)tmp_vertex.data[3].x - 1];
			// else
			//	 vertex_[ctf].data[3] = vec3create(-100, -100, -100);
		//	vec3_tmp = ananlyse(&txt[i + 1], 3);

			for (int j = 0; j < 3; j++)
				vertex_[ctf + f].data[j] = normales[(int)tmp_vertex.data[j].z - 1];



			ctf++;
		}

	}

	free(normales);
	free(vertices);
	free(txt);

	for (int i = 0; i < f; i++)
		for (int j = 0; j < 4; j++)
		{
			vertex_[i].data[j].z -= 2;
			vertex_[i].data[j].y += 0.5;
		}

	result.f = f;
	result.vertices = vertex_;
	return result;
}