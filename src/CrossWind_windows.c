#include <stdio.h>
#include <ctype.h>
#include <windows.h>
#include "CrossWind.h"

struct wData
{
    HWND windowHandle;
    MSG msg;
    int exit;
};

struct wData wdata;
struct CrossInput win_input;
char *concat(const char *s1, const char *s2);
char *lower_string(char *string);

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

extern struct CrossInput GetInput()
{
    struct CrossInput temp_input;
    temp_input.key = NULL;
    temp_input.state = win_input.state;

    if (win_input.key != NULL)
    {
        temp_input.key = lower_string(win_input.key);
    }

    win_input.state = 0;
    return temp_input;
}

extern struct CrossWindow GenerateWindow(struct CrossRect rect, const char *title)
{
    char *append = " - Windows implementation";
    char *newTitle = concat(title, append);

    struct CrossWindow window;
    window.rect = rect;
    window.title = newTitle;

    // FreeConsole();
    WNDCLASS wc = {0};
    wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hInstance = NULL;
    wc.lpfnWndProc = WndProc;
    wc.lpszClassName = "CrossWind";
    if (!RegisterClass(&wc))
    {
        fprintf(stderr, "Error: RegisterClass failed.\n");
        exit(EXIT_FAILURE);
    }
    HWND windowHandle = CreateWindow(
        wc.lpszClassName,
        window.title,
        WS_OVERLAPPEDWINDOW,
        rect.point.x,
        rect.point.y,
        rect.size.width,
        rect.size.height,
        NULL,
        NULL,
        NULL,
        NULL);

    ShowWindow(windowHandle, SW_SHOWDEFAULT);

    wdata.windowHandle = windowHandle;
    return window;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    UINT scancode;
    char keyName[256];
    switch (uMsg)
    {
    case WM_CREATE:
        // Force a WM_PAINT message by invalidating the entire client area
        InvalidateRect(hwnd, NULL, TRUE);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_KEYDOWN:
        scancode = MapVirtualKey(wParam, MAPVK_VK_TO_VSC);
        if (GetKeyNameTextA(scancode << 16, keyName, sizeof(keyName)) != 0)
        {
            win_input.key = keyName;
            win_input.state = 1;
        }

        return 0;
    case WM_KEYUP:
        scancode = MapVirtualKey(wParam, MAPVK_VK_TO_VSC);
        if (GetKeyNameTextA(scancode << 16, keyName, sizeof(keyName)) != 0)
        {
            win_input.key = keyName;
            win_input.state = 0;
        }
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}

extern void SendCloseEvent(struct CrossWindow *window)
{
    wdata.exit = 1;
    PostQuitMessage(0);
}

extern int Update()
{

    int res;
    res = GetMessage(&wdata.msg, NULL, 0, 0);

    TranslateMessage(&wdata.msg);
    DispatchMessage(&wdata.msg);

    if (res < 1)
    {
        wdata.exit = 1;
    }

    return wdata.exit;
}

extern void SetColor(struct CrossWindow *window, struct CrossPoint point, struct CrossColor color)
{
    HDC hdc = GetDC(wdata.windowHandle);

    COLORREF c = RGB(color.r, color.g, color.b);
    SetPixel(hdc, point.x, point.y, c);

    ReleaseDC(wdata.windowHandle, hdc);
}

extern void SetColors(struct CrossWindow *window, struct CrossPoint *points, struct CrossColor *colors, int count)
{
    HDC hdc = GetDC(wdata.windowHandle);

    for (int i = 0; i < count; i++)
    {
        COLORREF c = RGB(colors[i].r, colors[i].g, colors[i].b);
        SetPixel(hdc, points[i].x, points[i].y, c);
    }

    ReleaseDC(wdata.windowHandle, hdc);
}

extern void SetColorPoints(struct CrossWindow* window, struct CrossColorPoints points)
{
    SetColors(window, points.points, points.colors, points.count);
}

extern void SetColorRect(struct CrossWindow* window, struct CrossRect rect, struct CrossColor color)
{
    HDC hdc = GetDC(wdata.windowHandle);

    RECT r;
    r.left = rect.point.x;
    r.top = rect.point.y;
    r.right = rect.point.x + rect.size.width;
    r.bottom = rect.point.y + rect.size.height;

    HBRUSH brush = CreateSolidBrush(RGB(color.r, color.g, color.b));
    FillRect(hdc, &r, brush);
    DeleteObject(brush);

    ReleaseDC(wdata.windowHandle, hdc);
}

extern void ClearWindow(struct CrossWindow *window, struct CrossColor color)
{
    HDC hdc = GetDC(wdata.windowHandle);

    RECT rect;
    GetClientRect(wdata.windowHandle, &rect);
    HBRUSH brush = CreateSolidBrush(RGB(color.r, color.g, color.b));
    FillRect(hdc, &rect, brush);
    DeleteObject(brush);

    ReleaseDC(wdata.windowHandle, hdc);
}

extern void DisposeWindow(struct CrossWindow *window)
{
    free(window->title);
    DeleteObject(wdata.windowHandle);
    window->title = NULL;
}

char *concat(const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1) + strlen(s2) + 1); //+1 for the zero-terminator
    if (result == NULL)
    {
        fprintf(stderr, "Error: malloc failed in concat.\n");
        exit(EXIT_FAILURE);
    }
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

char *lower_string(char *string)
{
    int i = 0;
    while (string[i])
    {
        string[i] = tolower(string[i]);
        i++;
    }
    return string;
}