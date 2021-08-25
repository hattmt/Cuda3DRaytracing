#include "GestiondeFenetreWin32.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"
#include "cuda_test.h"

#pragma comment(lib, "Dwrite")

UINT32* data = NULL;

// Recalculate drawing layout when the size of the window changes.



void MainWindow::CalculateLayout()
{
	if (pRenderTarget != NULL)
	{
		D2D1_SIZE_F size = pRenderTarget->GetSize();
		const float x = size.width / 2;
		const float y = size.height / 2;
		const float radius = min(x, y);
		ellipse = D2D1::Ellipse(D2D1::Point2F(x, y), radius, radius);
	}
}

unsigned char* readBMP(char* filename, int* width, int* height)
{
	int i;
	FILE* f = fopen(filename, "rb");
	unsigned char info[54];
	fread(info, sizeof(unsigned char), 54, f); // read the 54-byte header

	// extract image height and width from header
	*width = *(int*)&info[18];
	*height = *(int*)&info[22];

	int size = 3 * (*width) * (*height);
	unsigned char* data = new unsigned char[size]; // allocate 3 bytes per pixel
	fread(data, sizeof(unsigned char), size, f); // read the rest of the data at once
	fclose(f);

	for (i = 0; i < size; i += 3)
	{
		unsigned char tmp = data[i];
		data[i] = data[i + 2];
		data[i + 2] = tmp;
	}

	return data;
}


HRESULT MainWindow::CreateGraphicsResources()
{
	HRESULT hr = S_OK;

	if (pRenderTarget == NULL)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

		data= new UINT32[size.height*size.width];//creation du buffer de pixels

		hr = pFactory->CreateHwndRenderTarget(
			D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(m_hwnd, size),
			&pRenderTarget);

		int width = 0, height = 0,height1=0,width1=0;
		UCHAR* bmp = readBMP("texture.bmp", &height, &width);
		cudaMalloc((void**)&bmp_textr.pixls1, width * height * sizeof(UCHAR));
		cudaMemcpy(bmp_textr.pixls1, bmp, width * height * sizeof(UCHAR), cudaMemcpyHostToDevice);
		delete bmp;
		bmp_textr.height1 = height;
		bmp_textr.width1 = width;


		UCHAR* bmp2 = readBMP("texture2.bmp", &height1, &width1);
		cudaMalloc((void**)&bmp_textr.pixls2, height1 * width1 * sizeof(UCHAR));
		cudaMemcpy(bmp_textr.pixls2, bmp2, height1 * width1 * sizeof(UCHAR), cudaMemcpyHostToDevice);
		delete bmp2;
		bmp_textr.height2 = height1;
		bmp_textr.width2 = width1;
		

		cudaMalloc((void**)&Pd, size.height*size.width *sizeof(UINT32));//memoire cuda partagee
		initobjts(&fichiers_obj,&objets_scene,&nb_objtscene);

		if (SUCCEEDED(hr))
		{
			const D2D1_COLOR_F color = D2D1::ColorF(1.0f, 1.0f, 1.0f);
			hr = pRenderTarget->CreateSolidColorBrush(color, &pBrush);
			if (SUCCEEDED(hr))
			{
				CalculateLayout();
			}

		
			hr =DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED,
				__uuidof(IDWriteFactory),
				reinterpret_cast<IUnknown**>(&m_pDWriteFactory));

		}
	}
	return hr;
}

void MainWindow::DiscardGraphicsResources()
{
	freeobjts(fichiers_obj,objets_scene,nb_objtscene);
	cudaFree(Pd);
	SafeRelease(&pRenderTarget);
	SafeRelease(&pBrush);
	delete data;
}

void MainWindow::OnPaint(double delta)
{
	HRESULT hr = CreateGraphicsResources();
	if (SUCCEEDED(hr))
	{
		PAINTSTRUCT ps;
		ID2D1Bitmap* bitmap = NULL;

		HDC hdc=BeginPaint(m_hwnd, &ps);



		D2D1_SIZE_F renderTargetSize = pRenderTarget->GetSize();
		UINT32 size_r = renderTargetSize.width * renderTargetSize.height;


		D2D1_PIXEL_FORMAT pixelFormat = D2D1::PixelFormat(//format UINT32 4 a,r,g,b: alpha=0XFF000000 ,rouge=0X00FF0000,vert=0X0000FF00,bleu=0X000000FF
			DXGI_FORMAT_B8G8R8A8_UNORM,
			D2D1_ALPHA_MODE_IGNORE
		);

		D2D1_BITMAP_PROPERTIES properties = D2D1::BitmapProperties();//dpi bitmap et format de pixels
		pRenderTarget->GetDpi(&properties.dpiX, &properties.dpiY);
		properties.pixelFormat = pixelFormat;

		
		update_objects(nb_objtscene, &objets_scene,delta);
		mainprog(renderTargetSize.width, renderTargetSize.height, data,Pd,fichiers_obj,objets_scene,nb_objtscene, bmp_textr);//modification du buffer de la bitmap

		// Retrieve the size of the bitmap.
		hr = pRenderTarget->CreateBitmap(D2D1::SizeU(renderTargetSize.width, renderTargetSize.height), data, renderTargetSize.width * 4, properties, &bitmap);//creation de la bitmap a partir du buffer data (pixels)

		pRenderTarget->BeginDraw();
		pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::White));//clear l ecran
		pRenderTarget->DrawBitmap( //dessiner la bitmap
			bitmap,
		D2D1::RectF(0, 0, renderTargetSize.width,
				renderTargetSize.height));
		if(bitmap)
		bitmap->Release();

		IDWriteTextFormat* textformat;



		m_pDWriteFactory->CreateTextFormat(
			L"Arial",
			NULL,
			DWRITE_FONT_WEIGHT_REGULAR,
			DWRITE_FONT_STYLE_NORMAL,
			DWRITE_FONT_STRETCH_NORMAL,
			10.0f,
			L"",
			&textformat
		);
		
		WCHAR txt[1000];

		int size=mbstowcs((WCHAR * )txt, text.c_str(), text.length());
			
		pRenderTarget->DrawText(
			txt,
			size,
			textformat,
			D2D1::RectF(5, 5, 180, 100),
			pBrush
		);

		textformat->Release();

		hr = pRenderTarget->EndDraw();

		if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
		{
			DiscardGraphicsResources();
		}
		EndPaint(m_hwnd, &ps);
	}
}



void MainWindow::Resize()
{
	if (pRenderTarget != NULL)
	{
		RECT rc;
		GetClientRect(m_hwnd, &rc);

		D2D1_SIZE_U size = D2D1::SizeU(rc.right, rc.bottom);

		pRenderTarget->Resize(size);
		CalculateLayout();
		InvalidateRect(m_hwnd, NULL, FALSE);
	}
}






int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow)
{
	MainWindow win;

	if (!win.Create(L"Ray Tracing 3D", WS_OVERLAPPEDWINDOW))
	{
		return 0;
	}

	ShowWindow(win.Window(), nCmdShow);

	// Run the message loop.

	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;
}