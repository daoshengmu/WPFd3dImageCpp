using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;



namespace WpfDirectX
{
    class ViewModule : IDisposable
    {
        private RendererManager _renderer;
        private uint _windowWidth = 800;
        private uint _windowHeight = 600;
        private IntPtr _backBuffer;

        public ViewModule(uint width, uint height, IntPtr windowHandle)
        {
            _windowWidth = width;
            _windowHeight = height;
            _renderer = new RendererManager();
            _renderer.Initialize( _windowWidth, _windowHeight, windowHandle );
            _backBuffer = _renderer.GetImageBackBuffer(width, height, windowHandle);
        }

        public void Draw()
        {
            _renderer.Draw();
        }

        public IntPtr GetBackBuffer()
        {
            return _backBuffer;
        }

        public void Dispose()
        {
        	test master1... master1
         //   UninitializeDirectX();
           // _directXManager.Dispose();
        }
               
    }
}
