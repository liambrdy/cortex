#include <Windows.h>
#include <gl/GL.h>

#include "cortex_platform.h"

#define WGL_CONTEXT_MAJOR_VERSION_ARB           0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB           0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB            0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB        0x00000001

#define WGL_DRAW_TO_WINDOW_ARB                    0x2001
#define WGL_ACCELERATION_ARB                      0x2003
#define WGL_SUPPORT_OPENGL_ARB                    0x2010
#define WGL_DOUBLE_BUFFER_ARB                     0x2011
#define WGL_PIXEL_TYPE_ARB                        0x2013
#define WGL_COLOR_BITS_ARB                        0x2014
#define WGL_DEPTH_BITS_ARB                        0x2022
#define WGL_STENCIL_BITS_ARB                      0x2023

#define WGL_FULL_ACCELERATION_ARB                 0x2027
#define WGL_TYPE_RGBA_ARB                         0x202B

typedef HGLRC wgl_create_context_attribs_arb(HDC hDC, HGLRC hShareContext, const int *attribList);
global wgl_create_context_attribs_arb *wglCreateContextAttribsARB;

typedef BOOL wgl_choose_pixel_format_arb(HDC hdc, const int *piAttribIList, const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
global wgl_choose_pixel_format_arb *wglChoosePixelFormatARB;

global bool globalRunning;

PLATFORM_GET_OPENGL_FUNCTION(GetOpenGLFunction)
{
    return wglGetProcAddress(proc);
}

struct game_code
{
    HMODULE gameCodeDLL;
    FILETIME dllLastWriteTime;
    game_update *gameUpdate;
};

game_code LoadGameCode(char *path)
{
    game_code result = {};
    
    HMODULE library = LoadLibrary(path);
    if (library)
    {
        result.gameCodeDLL = library;
        result.gameUpdate = (game_update *)GetProcAddress(library, "GameUpdate");
    }

    return result;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    LRESULT result = 0;

    switch (uMsg)
    {
        case WM_CLOSE:
        case WM_DESTROY:
        {
            globalRunning = false;
        } break;

        default:
        {
            result = DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

    return result;
}

void ProcessPendingMessages()
{
    MSG message;
    while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
    {
        switch (message.message)
        {
            case WM_QUIT:
            {
                globalRunning = false;
            } break;

            default:
            {
                TranslateMessage(&message);
                DispatchMessage(&message);
            } break;
        }
    }
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hInstancePrev, PSTR cmdline, int nCmdShow)
{
    WNDCLASS wc = {};
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = "CortexWindowClass";

    RegisterClass(&wc);

    HWND hwnd = CreateWindowEx(0, wc.lpszClassName, "Cortex", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 1280, 720, NULL, NULL, hInstance, NULL);
    if (hwnd)
    {
        ShowWindow(hwnd, nCmdShow);

        HDC dc = GetDC(hwnd);
        PIXELFORMATDESCRIPTOR pfd = {sizeof(pfd), 1};
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.cColorBits = 32;
        pfd.cAlphaBits = 8;
        pfd.iLayerType = PFD_MAIN_PLANE;
        pfd.cDepthBits = 24;
        pfd.cStencilBits = 8;
        int pixelFormat = ChoosePixelFormat(dc, &pfd);
        SetPixelFormat(dc, pixelFormat, &pfd);

        HGLRC wglContext = wglCreateContext(dc);
        wglMakeCurrent(dc, wglContext);

        wglCreateContextAttribsARB = (wgl_create_context_attribs_arb *)wglGetProcAddress("wglCreateContextAttribsARB");
        wglChoosePixelFormatARB = (wgl_choose_pixel_format_arb *)wglGetProcAddress("wglChoosePixelFormatARB");
        if (wglChoosePixelFormatARB)
        {
            int pixel_format_attribs[] = {
                WGL_DRAW_TO_WINDOW_ARB,     GL_TRUE,
                WGL_SUPPORT_OPENGL_ARB,     GL_TRUE,
                WGL_DOUBLE_BUFFER_ARB,      GL_TRUE,
                WGL_ACCELERATION_ARB,       WGL_FULL_ACCELERATION_ARB,
                WGL_PIXEL_TYPE_ARB,         WGL_TYPE_RGBA_ARB,
                WGL_COLOR_BITS_ARB,         32,
                WGL_DEPTH_BITS_ARB,         24,
                WGL_STENCIL_BITS_ARB,       8,
                0
            };

            int pixel_format;
            UINT num_formats;
            wglChoosePixelFormatARB(dc, pixel_format_attribs, 0, 1, &pixel_format, &num_formats);

            PIXELFORMATDESCRIPTOR newPfd;
            DescribePixelFormat(dc, pixel_format, sizeof(newPfd), &newPfd);
            SetPixelFormat(dc, pixel_format, &newPfd);
        }
        if (wglCreateContextAttribsARB)
        {
            int const createAttribs[] = {
                WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
                WGL_CONTEXT_MINOR_VERSION_ARB, 3,
                WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                0,
            };

            HGLRC newContext = wglCreateContextAttribsARB(dc, 0, createAttribs);
            wglMakeCurrent(dc, newContext);
        }

        game_memory gameMemory = {};
        gameMemory.permanentStorageSize = Megabytes(64);
        gameMemory.permanentStorage = VirtualAlloc(0, gameMemory.permanentStorageSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

        RECT dim;
        GetWindowRect(hwnd, &dim);

        game_input gameInput = {};
        gameInput.windowWidth = dim.right - dim.left;
        gameInput.windowHeight = dim.bottom - dim.top;

        gameMemory.getOpenGLFunction = GetOpenGLFunction;

        game_code gameCode = LoadGameCode("cortex.dll");

        globalRunning = true;
        while (globalRunning)
        {
            ProcessPendingMessages();

            if (gameCode.gameUpdate)
            {
                gameCode.gameUpdate(&gameMemory, &gameInput);
            }

            SwapBuffers(dc);
        }
    }
}
