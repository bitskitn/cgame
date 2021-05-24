// This definition is necessary to override the definition of WINGDIAPI
// so the compiler doesn't try to import the wgl_ functions. This way,
// we don't have to link against opengl32.dll at compile time.
#define _GDI32_

#include <windows.h>

#include "graphics-gl.h"
#include "objects.h"

extern void render_gdi(void);
extern void render_gl(void);

HGLRC (*wglCreateContextProc)(HDC);
WINBOOL (*wglDeleteContextProc)(HGLRC);
WINBOOL (*wglMakeCurrentProc)(HDC, HGLRC);
PROC (*wglGetProcAddressProc)(LPCSTR);

WINAPI WINGDIAPI HGLRC
wglCreateContext(HDC hDC)
{
    return wglCreateContextProc(hDC);
}

WINAPI WINGDIAPI WINBOOL
wglDeleteContext(HGLRC hGLRC)
{
    return wglDeleteContextProc(hGLRC);
}

WINAPI WINGDIAPI WINBOOL
wglMakeCurrent(HDC hDC, HGLRC hGLRC)
{
    return wglMakeCurrentProc(hDC, hGLRC);
}

static VOIDPROC
loadWGLFunction(void *loader, const char *name)
{
    return (VOIDPROC)wglGetProcAddressProc(name);
}

static LRESULT CALLBACK
WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static HDC hDC = NULL;
    static HGLRC hGLRC = NULL;
    static HMODULE hLibOpenGL32;
    switch (uMsg)
    {
        case WM_CREATE:
        {
            // Load the OpenGL32 library
            {
                hLibOpenGL32 = LoadLibrary(TEXT("opengl32.dll"));
                if (hLibOpenGL32 == NULL)
                {
                    MessageBox
                    (
                        /* .hWnd = */ NULL,
                        /* .lpText = */ TEXT("Failed to load opengl32.dll."),
                        /* .lpCaption = */ TEXT("Error"),
                        /* .uType = */ MB_ICONERROR
                    );
                    return -1;
                }
            }
            // Load the functions for working with rendering contexts
            {
                wglCreateContextProc = (HGLRC (*)(HDC))
                    GetProcAddress(hLibOpenGL32, "wglCreateContext");
                wglDeleteContextProc = (WINBOOL (*)(HGLRC))
                    GetProcAddress(hLibOpenGL32, "wglDeleteContext");
                wglMakeCurrentProc = (WINBOOL (*)(HDC, HGLRC))
                    GetProcAddress(hLibOpenGL32, "wglMakeCurrent");
                wglGetProcAddressProc = (PROC (*)(LPCSTR))
                    GetProcAddress(hLibOpenGL32, "wglGetProcAddress");
            }
            // Get the device context
            {
                hDC = GetDC(hWnd);
                if (hDC == NULL)
                {
                    MessageBox
                    (
                        /* .hWnd = */ NULL,
                        /* .lpText = */ TEXT("Failed to get device context."),
                        /* .lpCaption = */ TEXT("Error"),
                        /* .uType = */ MB_ICONERROR
                    );
                    FreeLibrary(hLibOpenGL32);
                    return -1;
                }
            }
            // Set the pixel format
            {
                PIXELFORMATDESCRIPTOR pfd =
                {
                    .nSize = sizeof(PIXELFORMATDESCRIPTOR),
                    .nVersion = 1,
                    .dwFlags = PFD_DRAW_TO_WINDOW
                             | PFD_DOUBLEBUFFER
                             | PFD_SUPPORT_OPENGL,
                    .cColorBits = 32,
                    .cDepthBits = 24,
                    .cStencilBits = 8,
                    .iLayerType = PFD_MAIN_PLANE
                };
                int pf = ChoosePixelFormat(hDC, &pfd);
                if (!(pf && SetPixelFormat(hDC, pf, &pfd)))
                {
                    MessageBox
                    (
                        /* .hWnd = */ NULL,
                        /* .lpText = */ TEXT("Failed to set pixel format."),
                        /* .lpCaption = */ TEXT("Error"),
                        /* .uType = */ MB_ICONERROR
                    );
                    ReleaseDC(hWnd, hDC);
                    FreeLibrary(hLibOpenGL32);
                    return -1;
                }
            }
            // Get the rendering context
            {
                hGLRC = wglCreateContext(hDC);
                if (hGLRC == NULL)
                {
                    MessageBox
                    (
                        /* .hWnd = */ NULL,
                        /* .lpText = */ TEXT("Failed to get rendering context."),
                        /* .lpCaption = */ TEXT("Error"),
                        /* .uType = */ MB_ICONERROR
                    );
                    ReleaseDC(hWnd, hDC);
                    FreeLibrary(hLibOpenGL32);
                    return -1;
                }
                wglMakeCurrent(hDC, hGLRC);
            }
            // Load the OpenGL functions used by the engine
            {
                FUNCTIONLOADER loaders[] =
                {
                    { loadWGLFunction, NULL },
                    { (PFNLOADFUNCTIONPROC) GetProcAddress, hLibOpenGL32 }
                };
                loadGLFunctions(sizeof(loaders) / sizeof(*loaders), loaders);
            }
            return 0;
        }
        case WM_DESTROY:
        {
            if (hGLRC)
            {
                wglMakeCurrent(hDC, NULL);
                wglDeleteContext(hGLRC);
                hGLRC = NULL;
            }
            if (hDC)
            {
                ReleaseDC(hWnd, hDC);
                hDC = NULL;
            }
            FreeLibrary(hLibOpenGL32);
            PostQuitMessage(0);
            return 0;
        }
        case WM_CLOSE:
        {
            DestroyWindow(hWnd);
            return 0;
        }
        case WM_PAINT:
        {
            render_gl();
            SwapBuffers(hDC);
            return 0;
        }
        default:
        {
            return DefWindowProc(hWnd, uMsg, wParam, lParam);
        }
    }
}

INT WINAPI
WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,
        INT nShowCmd)
{
    LPSTR wndClassName = TEXT("Game Window");
    DWORD dwStyle = WS_OVERLAPPEDWINDOW;
    DWORD dwExStyle = WS_EX_CLIENTEDGE;
    HWND hWnd;
    MSG msg;
    INT exitCode = EXIT_SUCCESS;
    // Register the window class
    {
        WNDCLASSEX wndClassEx =
        {
            .cbSize = sizeof(WNDCLASSEX),
            .style = CS_HREDRAW | CS_VREDRAW,
            .lpfnWndProc = WindowProc,
            .hInstance = hInstance,
            .hIcon = LoadIcon(NULL, IDI_APPLICATION),
            .hCursor = LoadCursor(NULL, IDC_ARROW),
            .hbrBackground = (HBRUSH)(COLOR_WINDOW + 1),
            .lpszMenuName = NULL,
            .lpszClassName = wndClassName,
            .hIconSm = LoadIcon(NULL, IDI_APPLICATION)
        };
        if (!RegisterClassEx(&wndClassEx))
        {
            MessageBox
            (
                /* .hWnd = */ NULL,
                /* .lpText = */ TEXT("Failed to register window class."),
                /* .lpCaption = */ TEXT("Error"),
                /* .uType = */ MB_ICONERROR
            );
            ExitProcess(EXIT_FAILURE);
        }
    }
    // Create the window
    {
        hWnd = CreateWindowEx
        (
            /* .dwExStyle = */ dwExStyle,
            /* .lpClassName = */ wndClassName,
            /* .lpWindowName = */ "Game",
            /* .dwStyle = */ dwStyle,
            /* .X = */ CW_USEDEFAULT,
            /* .Y = */ CW_USEDEFAULT,
            /* .nWidth = */ CW_USEDEFAULT,
            /* .nHeight = */ CW_USEDEFAULT,
            /* .hWndParent = */ NULL,
            /* .hMenu = */ NULL,
            /* .hInstance = */ hInstance,
            /* .lpParam = */ NULL
        );
        if (!hWnd)
        {
            UnregisterClass(wndClassName, hInstance);
            MessageBox
            (
                /* .hWnd = */ NULL,
                /* .lpText = */ TEXT("Failed to create window."),
                /* .lpCaption = */ TEXT("Error"),
                /* .uType = */ MB_ICONERROR
            );
            ExitProcess(EXIT_FAILURE);
        }
        ShowWindow(hWnd, nShowCmd);
        UpdateWindow(hWnd);
    }
    while (PeekMessage(&msg, hWnd, 0, 0, PM_REMOVE))
    {
        if (msg.message == WM_QUIT)
        {
            exitCode = msg.wParam;
            break;
        }
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    UnregisterClass(wndClassName, hInstance);
    return exitCode;
}
