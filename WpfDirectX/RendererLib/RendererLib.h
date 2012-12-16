
#ifndef __directX9Lib
#define __directX9Lib

#include "stdafx.h"
#include <d3d9.h>

using namespace System;
using namespace System::Drawing;


public ref class RendererManager : public IDisposable
{
public:

private:
	// put the real renderer module here.
	IDirect3D9			*_d3dObj;
	IDirect3D9Ex		*_d3dExObj;
	IDirect3DDevice9	*_d3dDevice;
	IDirect3DDevice9Ex	*_d3dDeviceEx;
	IDirect3DSurface9	*_d3dSurface;
	bool				_supportsD3dEx;
	bool				_supportsHardwareVertexProcessing;
	HWND				_windowHandle;


public:
	RendererManager() : _supportsD3dEx(false){};
	~RendererManager() {};

	IntPtr GetImageBackBuffer(unsigned int height, unsigned int width, IntPtr windowHandle);
	void Initialize( unsigned int height, unsigned int width, IntPtr windowHandle );
	void Draw();
	//void Terminate();

private:
	HRESULT CreateD3DObject( unsigned int width, unsigned int height, IntPtr windowHandle );
	bool    CreateD3DDevice();
	
};


#endif // end of def __directX9Lib