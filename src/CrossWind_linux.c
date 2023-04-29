#include "CrossWind.h"

extern struct CrossWindow CreateWindow(struct CrossRect rect, const char* title)
{
    struct CrossWindow window;
    window.rect = rect;
    window.title = title;
    return window;
}

extern void SetWindowRect(struct CrossWindow* window, struct CrossRect rect)
{
    window->rect = rect;
}

extern void SetWindowTitle(struct CrossWindow* window, const char* title)
{
    window->title = title;
}