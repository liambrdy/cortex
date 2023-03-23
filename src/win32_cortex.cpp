#include <Windows.h>

#include "cortex_platform.h"

#define WGL_CONTEXT_MAJOR_VERSION_ARB           0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB           0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB            0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB        0x00000001

typedef HGLRC wgl_create_context_attribs_arb(HDC hDC, HGLRC hShareContext, const int *attribList);
global wgl_create_context_attribs_arb *wglCreateContextAttribsARB;

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

    HWND hwnd = CreateWindowEx(0, wc.lpszClassName, "Cortex", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);
    if (hwnd)
    {
        ShowWindow(hwnd, nCmdShow);

        HDC dc = GetDC(hwnd);
        PIXELFORMATDESCRIPTOR pfd = {sizeof(pfd), 1};
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_SUPPORT_COMPOSITION | PFD_DOUBLEBUFFER;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.cColorBits = 32;
        pfd.cAlphaBits = 8;
        pfd.iLayerType = PFD_MAIN_PLANE;
        int pixelFormat = ChoosePixelFormat(dc, &pfd);
        SetPixelFormat(dc, pixelFormat, &pfd);

        HGLRC wglContext = wglCreateContext(dc);
        wglMakeCurrent(dc, wglContext);

        wglCreateContextAttribsARB = (wgl_create_context_attribs_arb *)wglGetProcAddress("wglCreateContextAttribsARB");
        if (wglCreateContextAttribsARB)
        {
            int const createAttribs[] = {
                WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
                WGL_CONTEXT_MINOR_VERSION_ARB, 3,
                WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                0,
            };

            HGLRC newContext = wglCreateContextAttribsARB(dc, wglContext, createAttribs);
            wglMakeCurrent(dc, newContext);
        }

        game_memory gameMemory = {};
        gameMemory.permanentStorageSize = Megabytes(64);
        gameMemory.permanentStorage = VirtualAlloc(0, gameMemory.permanentStorageSize, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

        gameMemory.getOpenGLFunction = GetOpenGLFunction;

        game_code gameCode = LoadGameCode("cortex.dll");

        globalRunning = true;
        while (globalRunning)
        {
            ProcessPendingMessages();

            if (gameCode.gameUpdate)
            {
                gameCode.gameUpdate(&gameMemory);
            }

            SwapBuffers(dc);
        }
    }
}
