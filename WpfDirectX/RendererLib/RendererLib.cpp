// directX9Lib.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "RendererLib.h"

typedef HRESULT (WINAPI *DIRECT3DCREATE9EXFUNCTION)(UINT SDKVersion, IDirect3D9Ex**);

void RendererManager::Initialize( unsigned int height, unsigned int width, IntPtr windowHandle )
{
	HRESULT hr;

	hr = CreateD3DObject( height, width, windowHandle );

	if ( hr == S_OK )
	{

	}
	else
	{

	}

}

HRESULT RendererManager::CreateD3DObject( unsigned int width, unsigned int height, IntPtr windowHandle )
{
	HRESULT hr = S_OK;
	HMODULE hD3D = NULL;

	// first load the d3d library
	hD3D = LoadLibrary(TEXT("d3d9.dll"));

	_windowHandle = (HWND)windowHandle.ToPointer();

	if (hD3D)
	{
		// see if the function to create a d3dEx object is there
		DIRECT3DCREATE9EXFUNCTION pfnCreate9Ex = (DIRECT3DCREATE9EXFUNCTION)GetProcAddress(hD3D, "Direct3DCreate9Ex");

		if (pfnCreate9Ex)
		{
			_supportsD3dEx = true;
			pin_ptr<IDirect3D9Ex*> pinnedD3dEx = &_d3dExObj;
			// the function exists, so create a d3dEx object
			hr = (*pfnCreate9Ex)(D3D_SDK_VERSION, pinnedD3dEx);
			if (hr == S_OK)
			{
				pin_ptr<IDirect3D9*> pinnedD3d = &_d3dObj;
				hr = _d3dExObj->QueryInterface(__uuidof(IDirect3D9), reinterpret_cast<void **>(pinnedD3d));
			}
		}
		else
		{
			_supportsD3dEx = false;
			// the function was not found, so just create a d3d object
			_d3dObj = Direct3DCreate9(D3D_SDK_VERSION);
			if (!_d3dObj) 
			{
				hr = E_FAIL;
			}
		}

		D3DCAPS9 caps;

		hr = _d3dObj->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &caps);
		if ((caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT) == D3DDEVCAPS_HWTRANSFORMANDLIGHT)
			_supportsHardwareVertexProcessing = true;
		else
			_supportsHardwareVertexProcessing = false;

		FreeLibrary(hD3D);
	}
	else
		hr = E_FAIL;

	CreateD3DDevice();

	return hr;
}

bool RendererManager::CreateD3DDevice()
{
	HRESULT hr;
	bool result = false;

	// determine what type of vertex processing to use based on the device capabilities
	DWORD vertexProcessing;

	if (_supportsHardwareVertexProcessing)
		vertexProcessing = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vertexProcessing = D3DCREATE_SOFTWARE_VERTEXPROCESSING;;

	// present parameters to create the D3DDevice
	D3DPRESENT_PARAMETERS presentParameters;

	ZeroMemory(&presentParameters, sizeof(presentParameters));

	presentParameters.Windowed = TRUE;
	presentParameters.BackBufferHeight = 1;
	presentParameters.BackBufferWidth = 1;
	presentParameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	presentParameters.BackBufferFormat = D3DFMT_UNKNOWN;

	if (_d3dExObj)
	{
		pin_ptr<IDirect3DDevice9Ex*> pinnedDeviceEx = &_d3dDeviceEx;
		
		// create the D3D device using the Ex function
		hr = _d3dExObj->CreateDeviceEx(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, _windowHandle,
			vertexProcessing | D3DCREATE_MULTITHREADED | D3DCREATE_FPU_PRESERVE,
			&presentParameters, NULL, pinnedDeviceEx);

		if (hr == S_OK)
		{
			pin_ptr<IDirect3DDevice9*> pinnedDevice = &_d3dDevice;

			// obtain the standard D3D device interface
			hr = _d3dDeviceEx->QueryInterface(__uuidof(IDirect3DDevice9), reinterpret_cast<void **>(pinnedDevice));
		}
	}
	else
	{
		pin_ptr<IDirect3DDevice9*> pinnedDevice = &_d3dDevice;

		// create the D3D device
		hr = _d3dObj->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, _windowHandle,
			vertexProcessing | D3DCREATE_MULTITHREADED | D3DCREATE_FPU_PRESERVE,
			&presentParameters, pinnedDevice);
	}

	if (hr == S_OK)
		result = true;

	return result;
}

void RendererManager::Draw()
{
	//if ( _d3dDevice )
	{
		_d3dDevice->BeginScene();
		_d3dDevice->Clear(  0,
			NULL,
			D3DCLEAR_TARGET,
			D3DCOLOR_ARGB(255, 255, 0, 0),  
			1.0f,
			0
			);

		_d3dDevice->EndScene();
	}
	
}

System::IntPtr RendererManager::GetImageBackBuffer( unsigned int height, unsigned int width, IntPtr windowHandle )
{
	IntPtr backBuffer = IntPtr::Zero;
	HRESULT hr;

	pin_ptr<IDirect3DSurface9*> pinnedSurface = &_d3dSurface;

	// create and set the render target surface
	// it should be lockable on XP and nonlockable on Vista
	hr = _d3dDevice->CreateRenderTarget(width, height, 
		D3DFMT_A8R8G8B8, D3DMULTISAMPLE_NONE, 0, 
		!_supportsD3dEx, // lockable
		pinnedSurface, NULL);

	if (hr == S_OK)
	{
		// set the device to render to the surface
		hr = _d3dDevice->SetRenderTarget(0, _d3dSurface);

		if (hr == S_OK)
		{
			
			if (hr == S_OK)
			{
				if (hr == S_OK)				
					backBuffer = IntPtr(_d3dSurface);
			}
		}			
	}

	return backBuffer;
}
