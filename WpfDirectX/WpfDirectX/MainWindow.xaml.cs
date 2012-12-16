using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Windows.Interop;


// use D3DImage, or other method to draw 3d result

namespace WpfDirectX
{
    /// <summary>
    /// Interaction logic for MainWindow.xaml
    /// </summary>
    public partial class MainWindow : Window
    {      
        private static ViewModule _view = null;
      
        public MainWindow()
        {
            InitializeComponent();
            CompositionTarget.Rendering += CompositionTarget_Rendering;
        }

        private void CompositionTarget_Rendering(object sender, System.EventArgs e)
        {
       //     RenderingEventArgs args = (RenderingEventArgs)e;

            // It's possible for Rendering to call back twice in the same frame  
            // so only render when we haven't already rendered in this frame. 
            if (d3dimg.IsFrontBufferAvailable)
            {
               
                IntPtr pSurface = IntPtr.Zero;
                pSurface = _view.GetBackBuffer();

                if (pSurface != IntPtr.Zero)
                {
                    d3dimg.Lock();
                    // Repeatedly calling SetBackBuffer with the same IntPtr is  
                    // a no-op. There is no performance penalty.
                    d3dimg.SetBackBuffer(D3DResourceType.IDirect3DSurface9, pSurface);
                    _view.Draw();
                    d3dimg.AddDirtyRect(new Int32Rect(0, 0, d3dimg.PixelWidth, d3dimg.PixelHeight));
                    d3dimg.Unlock();

               //     _lastRender = args.RenderingTime;
                }
            }

            _view.Draw();
        }

        private void Window_Loaded(object sender, RoutedEventArgs e)
        {
            // get the window handle for this window
            IntPtr hwnd = HandleFromVisual(this);

            _view = new ViewModule((uint)imgelt.Width, (uint)imgelt.Height, hwnd);          
        }

        private static IntPtr HandleFromVisual(Visual visual)
        {
            HwndSource hwndSource = PresentationSource.FromVisual(visual) as HwndSource;

            if (null != hwndSource)
                return hwndSource.Handle;

            return IntPtr.Zero;
        }
    }
}
