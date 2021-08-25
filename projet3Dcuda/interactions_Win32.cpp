#include "GestiondeFenetreWin32.h"
#include <cstdio>
#include <cstdlib>
#include "vec3.h"
#include "open_obj_files.h"
#include <windowsx.h>

/*! Coefficient for friction (for the particle) */
#define FRICTION	10.0f


/*! Gravity value (to be applied on Y axis) */
#define GRAVITY		-9.81f
/*! integration step to have same behaviour on different machines */
#define INTEGRATION_STEP	0.05f

//GetSystemTime(&last_time)

void Camera_cuda(cameras camera, UINT32* Pd);
int selector_obj=0;
std::string slctd_obj_name="0. light selected";
std::string str_position="";
Vector3D position= Vector3D(0,0,0);
Vector3D direction = Vector3D(0, 0, 0);
bool buttonlft = false;
double b_step=10;
bool mode_cuda = 1;
bool mode_gravity = 1;
bool mode_help = 0;
bool mode_obj = 0;

void pixl(UINT32* data, UINT32 width, UINT32 height, UINT32 x, UINT32 y, Vector3D rgb, UINT32 a)//AFFICHER LES PIXELS AVEC WIN32
{
	
		UINT32 pixl = x + y * width;
		
		if (height * width>pixl) {
		UINT32 color = (((UINT32)rgb.x) << 16) + (((UINT32)rgb.y) << 8) + ((UINT32)rgb.z) + ((UINT32)a << 24);
		data[pixl] = color;
	}
}



void freeobjts(obj* fichiers_obj, scene_object* objets_scene, int nbobjts)
{
	cudaFree(fichiers_obj->vertices);
	free(fichiers_obj);

}


void mainprog(UINT32 width, UINT32 height, UINT32* data, UINT32* cuda_memory, obj* fichiers_obj, scene_object* objets_scene, int nbobjt, textur bmp_sol)
{
	cameras camera_proj(data, width, height, position, 0);
	camera_proj.position = position;
	camera_proj.direction = direction;

	camera_proj.fichiers_obj = fichiers_obj;
	camera_proj.item = objets_scene;
	camera_proj.items = nbobjt;

	if (mode_obj)
		bmp_sol.height1 = -1;

	camera_proj.bmp_sol = bmp_sol;
	camera_proj.gravity = mode_gravity;


	if (mode_cuda == 0)
		camera_proj.rendu();
	else
		Camera_cuda(camera_proj, cuda_memory);
}


scene_object create_light(vec3 position)
{
	scene_object tmp;
	tmp.position = position;
	tmp.type = 3;
	return tmp;
}

 scene_object create_sphere(vec3 position, vec3 material, vec3 rgb, double r)
{
	scene_object tmp;
	tmp.position = position;
	tmp.material = material;
	tmp.r = r;
	tmp.rgb = rgb;
	tmp.type = 0;
	return tmp;
}

 scene_object create_plan(vec3 position, vec3 normal, vec3 material, vec3 rgb)
{
	scene_object tmp;
	tmp.position = position;
	tmp.material = material;
	tmp.normal = normal;
	tmp.rgb = rgb;
	tmp.type = 1;
	return tmp;
}

void initobjts(obj** fichiers_obj, scene_object** objets_scene,int* nbobjts)
{
 obj tmp=open_obj("cone.obj");
 obj tmp2=alloccudavertex(tmp);
 *fichiers_obj = (obj*)malloc(sizeof(obj));
 (*fichiers_obj)[0] = tmp2;
 free(tmp.vertices);
 
 (*objets_scene) = new scene_object[sizeof(scene_object) * 7];

 (*objets_scene)[0] = create_light(vec3create_(0, 1, -5));
 (*objets_scene)[1] = create_sphere(vec3create_(1.2, 20, 5), vec3create_(0.5, 1, 5), vec3create_(255, 0, 0), 1.1);
 (*objets_scene)[2] = create_sphere(vec3create_(7.48, -0.01, -5.84), vec3create_(0.5, 1, 5), vec3create_(0, 255, 0), 1.0);
 (*objets_scene)[3] = create_sphere(vec3create_(-1.4, -0.012, -3.18), vec3create_(0.5, 1, 5), vec3create_(0, 0, 255), 0.5);
 (*objets_scene)[4] = create_sphere(vec3create_(0.5, -0.012, -4.18), vec3create_(0.5, 1, 5), vec3create_(0, 255, 255), 0.8);
 (*objets_scene)[5] = create_plan(vec3create_(0, 0, -10), vec3create_(0, -1, -1), vec3create_(15, 1, 1), vec3create_(100, 100, 100));
 (*objets_scene)[6] = create_plan(vec3create_(0, 0, -10), vec3create_(0, 0, -1), vec3create_(5, 1, 1), vec3create_(100, 100, 100));
 *nbobjts = 7;

 for (int i = 0; i < (*nbobjts); i++)
 {
	 if ((*objets_scene)[i].type == 0)
		 (*objets_scene)[i].speed = vec3create_(0, 5, 0);
 }
 (*objets_scene)[1].speed.z = 14;
 (*objets_scene)[1].speed.x = -1;
}


void update_objects(int nb_objts, scene_object** objets,double delta)
{
	if (mode_gravity) {
		static float lastingDt = 0.0f;
		delta += lastingDt;
		float masse = 0.2;
		vec3 sommeforces, frict, gravit, acceleration;

		if (delta > INTEGRATION_STEP)
		{


			for (int i = 0; i < nb_objts; i++)
			{
				if ((*objets)[i].type == 0) {

					if ((*objets)[i].position.y < 0)//gere le rebond sur le sol
					{
						(*objets)[i].position.y = 0;
						(*objets)[i].speed.y = -((*objets)[i].speed.y);
						(*objets)[i].speed = mul3_((*objets)[i].speed, 0.95);
					}

					if ((*objets)[i].position.z < -10)//gere le rebond  sur le mur
					{
						(*objets)[i].position.z = -10 + (*objets)[i].r;
						(*objets)[i].speed.z = -((*objets)[i].speed.z);
						if ((*objets)[i].position.y < 0.5)
							(*objets)[i].speed = mul3_((*objets)[i].speed, 0.6);
					}

					if ((*objets)[i].position.z > -1)//gere le rebond  sur le mur
					{
						(*objets)[i].position.z = -1 - (*objets)[i].r;
						(*objets)[i].speed.z = -((*objets)[i].speed.z);
						if ((*objets)[i].position.y < 0.5)
							(*objets)[i].speed = mul3_((*objets)[i].speed, 0.6);
					}


					if ((*objets)[i].position.x > 15)//gere le rebond sur le mur
					{
						(*objets)[i].position.x = 15 - (*objets)[i].r;
						(*objets)[i].speed.x = -((*objets)[i].speed.x);
						if ((*objets)[i].position.y < 0.5)
							(*objets)[i].speed = mul3_((*objets)[i].speed, 0.6);
					}

					if ((*objets)[i].position.x < -15)//gere le rebond sur le mur
					{
						(*objets)[i].position.x = -15 + (*objets)[i].r;
						(*objets)[i].speed.x = -((*objets)[i].speed.x);
						if ((*objets)[i].position.y < 0.5)
							(*objets)[i].speed = mul3_((*objets)[i].speed, 0.6);
					}
					frict = mul3_((*objets)[i].speed, -(FRICTION * 0.001));//friction

					gravit = mul3_(vec3create_(0, GRAVITY, 0), masse);//gravite

					sommeforces = add3_(frict, gravit);
					acceleration = div3_(sommeforces, masse);
					(*objets)[i].speed_next = add3_((*objets)[i].speed, mul3_(acceleration, INTEGRATION_STEP));//integration d euler

					vec3 vecteur = vec3create_(0, 0, 0);

					for (int j = 0; j < nb_objts; j++)
					{
						if ((*objets)[j].type == 0) {


							vecteur = sub3_((*objets)[i].position, (*objets)[j].position);

							if (distance3_((*objets)[j].position, (*objets)[i].position) < (*objets)[j].r + (*objets)[i].r + 0.01)
							{

								(*objets)[j].speed = add3_((*objets)[i].speed_next, (*objets)[j].speed);
								(*objets)[i].speed_next = reflect3_((*objets)[i].speed_next, normalize3_(vecteur));
								(*objets)[i].position = add3_((*objets)[j].position, mul3_(normalize3_(vecteur), (*objets)[j].r + (*objets)[i].r + 0.03));

							}
						}
					}
					(*objets)[i].position_next = add3_((*objets)[i].position, mul3_((*objets)[i].speed_next, INTEGRATION_STEP));


				}
			}
			delta -= INTEGRATION_STEP;
		}

		lastingDt = delta;
	}


}










LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)//pour les interaction de touches onfluent sur la direction et position
{
	SYSTEMTIME   prev_time;
	SYSTEMTIME   new_time;
	double step = 0.1;


	if(objets_scene)
		str_position = "\nposition:(" + std::to_string(objets_scene[selector_obj].position.x) + ',' + std::to_string(objets_scene[selector_obj].position.y) + ',' + std::to_string(objets_scene[selector_obj].position.z) + ')';

	
	if(mode_cuda)
		step = b_step * delta;

	switch (uMsg)
	{
	case WM_CREATE:
		if (FAILED(D2D1CreateFactory(
			D2D1_FACTORY_TYPE_SINGLE_THREADED, &pFactory)))
		{
			return -1;  // Fail CreateWindowEx.
		}
		return 0;


	case WM_LBUTTONDOWN:
		buttonlft = true;
		break;

	case WM_LBUTTONUP:
		buttonlft = false;
		break;

	case WM_MOUSEMOVE:
		if (buttonlft)
		{
			D2D1_SIZE_F size = pRenderTarget->GetSize();
			int boxx = GET_X_LPARAM(lParam);
			int boxy = GET_Y_LPARAM(lParam);
			objets_scene[selector_obj].position.x = ((boxx - size.width / 2) / size.width) * step * 200;
			objets_scene[selector_obj].position.y = ((-boxy + size.height / 2) / size.height)* step * 200;
			objets_scene[selector_obj].speed.x = ((boxx - size.width / 2) / size.width) * step * 500;
			objets_scene[selector_obj].speed.y = ((-boxy + size.height / 2) / size.height) * step * 500;
			objets_scene[selector_obj].speed.z= ((-boxy + size.height / 2) / size.height) * step * 500;
		}
		break;

		case WM_KEYDOWN:
			
			switch (wParam)
			{
				case 0x4F://O
					if (mode_obj)
						mode_obj = 0;
					else
						mode_obj = 1;

					break;
					

				case 0x51://Q
					direction.x -= step;
					
					break;

				case 0x44://D
					direction.x += step;

					break;


				case 0x43://C
					if (mode_cuda == 0)
						mode_cuda = 1;
					else
						mode_cuda = 0;
					break;

				case 0x47:
					if (mode_gravity == 1)
						mode_gravity = 0;
					else
						mode_gravity = 1;
					break;


				case VK_SPACE://ESPACE
					selector_obj++;

					if (selector_obj >= nb_objtscene-2)
						selector_obj = 0;

					slctd_obj_name = "" + std::to_string(selector_obj) + '.';

					if (objets_scene[selector_obj].type == 3)
						slctd_obj_name += "light";

					if (objets_scene[selector_obj].type == 0)
						slctd_obj_name += "sphere";

					if (objets_scene[selector_obj].type == 1)
						slctd_obj_name += "plan";

					slctd_obj_name += " selected";
				break;
				case 0x49://I
					if (objets_scene[selector_obj].type == 0 && mode_gravity)
						objets_scene[selector_obj].speed.z -= step * 10;
					else
						objets_scene[selector_obj].position.z -= step;
				break;

				case 0x4A://J
					if(objets_scene[selector_obj].type==0 && mode_gravity)
						objets_scene[selector_obj].speed.x -= step*10;
					else
						objets_scene[selector_obj].position.x -= step ;

					break;

				case 0x4B://K
				
					if (objets_scene[selector_obj].type == 0&&mode_gravity)
						objets_scene[selector_obj].speed.z += step * 10;
					else
						objets_scene[selector_obj].position.z += step;
				break;

				case 0x4C://L
					if (objets_scene[selector_obj].type == 0 && mode_gravity)
						objets_scene[selector_obj].speed.x += step*10;
					else
						objets_scene[selector_obj].position.x += step ;
				break;

				case 0x52://R

					if (objets_scene[selector_obj].type == 0 && mode_gravity)
						objets_scene[selector_obj].speed.y += step * 10;
					else
						objets_scene[selector_obj].position.y += step;

			break;

				case 0x46://F

					if (objets_scene[selector_obj].type == 0 && mode_gravity)
						objets_scene[selector_obj].speed.y -= step*10;
					else
						objets_scene[selector_obj].position.y -= step;
				break;


				case 0x48://Help
					if (mode_help)
						mode_help = 0;
					else
						mode_help = 1;
				break;
				
				

			case VK_LEFT:

				// Process the LEFT ARROW key. 
				position.x-=step;
				direction = position;
				break;

			case VK_RIGHT:

				// Process the RIGHT ARROW key. 
				position.x+=step;
				direction = position;
				break;

			case VK_UP:
				position.y -= step;
				direction = position;
				// Process the UP ARROW key. 

				break;

			case VK_DOWN:
				
				position.y += step;
				direction = position;
				// Process the DOWN ARROW key. 

				break;

			default:
				
				break;
			}

			return 0;
		
	case WM_DESTROY:
		DiscardGraphicsResources();
		SafeRelease(&pFactory);
		SafeRelease(&m_pDWriteFactory);
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
		QueryPerformanceCounter(&currentTime);
		
		delta = (currentTime.QuadPart - prevTime.QuadPart)/10000000.0;
		text ="FPS: "+std::to_string((int)(1/delta));
		if (mode_cuda)
			text += "\nCuda: On (CPU and GPU)\n";
		else
			text += "\nCuda: Off (CPU only)\n";

		text += slctd_obj_name+"\n";
		text += str_position;
		
		if (!mode_help)
			text += "\n\nPour obtenir de l' aide appuyer sur H";
		else
			text += "\n\nESPACE pour selectionner un objet\n-Cet objet:\n   I,K pour l'avancer ou le reculer\n   J,L pour gauche/droite\n   R,F pour le monter ou le decendre\n\n   Souris pour deplacer les objets\n\n - G :stopper/activer la gravite  (on peut toujours deplacer les objets)\n\n - C:GPU/CPU(keep GPU it's better) \n\n -Q,D et fleches directionnelles pour la camera \n\n O: afficher/enlever un cone en .obj \n\n H: pour cacher l'aide";
		

		OnPaint(delta);
		prevTime = currentTime;
		InvalidateRect(m_hwnd, 0, TRUE);
		return 0;

	case WM_SIZE:
		Resize();
		return 0;
	}
	return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}


