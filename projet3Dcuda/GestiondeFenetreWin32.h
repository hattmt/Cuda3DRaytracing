#pragma once
#ifndef UNICODE
#define UNICODE
#endif 

#include <windows.h>
#include "cameras.h"
#include <d2d1.h>
#include <dwrite.h>
#include <string>


#pragma comment(lib, "d2d1")


void mainprog(UINT32 width, UINT32 height, UINT32* data, UINT32* cuda_memory, obj* fichiers_obj, scene_object* objets_scene, int nbobjt,textur bmp_sol); //CREATION DE LA CAMERA
void initobjts(obj** fichiers_obj, scene_object** objets_scene, int* nbobjts);
void freeobjts(obj* fichiers_obj, scene_object* objets_scene, int nbobjts);

template <class DERIVED_TYPE>
class BaseWindow
{
public:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		DERIVED_TYPE* pThis = NULL;

		if (uMsg == WM_NCCREATE)
		{
			CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
			pThis = (DERIVED_TYPE*)pCreate->lpCreateParams;
			SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

			pThis->m_hwnd = hwnd;
		}
		
		else
		{
			pThis = (DERIVED_TYPE*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		}
		if (pThis)
		{
			return pThis->HandleMessage(uMsg, wParam, lParam);
		}
		else
		{
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
		}
	}

	BaseWindow() : m_hwnd(NULL) { }

	BOOL Create(
		PCWSTR lpWindowName,
		DWORD dwStyle,
		DWORD dwExStyle = 0,
		int x = CW_USEDEFAULT,
		int y = CW_USEDEFAULT,
		int nWidth = CW_USEDEFAULT,
		int nHeight = CW_USEDEFAULT,
		HWND hWndParent = 0,
		HMENU hMenu = 0
	)
	{
		WNDCLASS wc = { 0 };

		wc.lpfnWndProc = DERIVED_TYPE::WindowProc;
		wc.hInstance = GetModuleHandle(NULL);
		wc.lpszClassName = L"Fenetre";

		RegisterClass(&wc);

		m_hwnd = CreateWindowEx(
			dwExStyle, L"Fenetre", lpWindowName, dwStyle, x, y,
			nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this
		);

		return (m_hwnd ? TRUE : FALSE);
	}

	HWND Window() const { return m_hwnd; }
	
protected:

	virtual PCWSTR  ClassName() const = 0;
	virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

	HWND m_hwnd;
};



template <class T> void SafeRelease(T** ppT)
{
	if (*ppT)
	{
		(*ppT)->Release();
		*ppT = NULL;
	}
}

class MainWindow : public BaseWindow<MainWindow>
{
	UINT32* Pd;
	textur bmp_textr;
	obj* fichiers_obj=NULL;
	scene_object*objets_scene=NULL;
	int nb_objtscene=6;
	ID2D1Factory* pFactory; 
	IDWriteFactory* m_pDWriteFactory;
	ID2D1HwndRenderTarget* pRenderTarget;
	ID2D1SolidColorBrush* pBrush;
	D2D1_ELLIPSE            ellipse;
	
	std::string text="";
	double delta;
	LARGE_INTEGER perfFreq;
	LARGE_INTEGER currentTime;
	LARGE_INTEGER prevTime;

	void    CalculateLayout();
	HRESULT CreateGraphicsResources();
	void    DiscardGraphicsResources();
	void    OnPaint(double delta);
	void    Resize();

public:

	MainWindow() : pFactory(NULL), pRenderTarget(NULL), pBrush(NULL), m_pDWriteFactory(NULL)
	{
	}

	PCWSTR  ClassName() const { return L"Circle2 Window Class"; }
	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

void update_objects(int nb_objts, scene_object** objets, double delta);