#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include "CrossWind.h"

struct xData {
    Display* display;
    Window window;
    int screen;
    GC gc;
    XEvent event;
    int exit;
};

struct xData xdata;

char* concat(const char* s1, const char* s2);

extern struct CrossWindow GenerateWindow(struct CrossRect rect, const char* title)
{
    char* append = " - Linux implementation";    
    char* newTitle = concat(title, append);

    struct CrossWindow window;
    window.rect = rect;
    window.title = newTitle;

    xdata.display = XOpenDisplay(NULL);
    if(xdata.display == NULL)
    {
        fprintf(stderr, "Error: XOpenDisplay failed.\n");
        exit(EXIT_FAILURE);
    }

    xdata.screen = DefaultScreen(xdata.display);

    xdata.window = XCreateSimpleWindow(xdata.display, RootWindow(xdata.display, xdata.screen), rect.point.x, rect.point.y, rect.size.width, rect.size.height, 1, BlackPixel(xdata.display, xdata.screen), WhitePixel(xdata.display, xdata.screen));

    XSelectInput(xdata.display, xdata.window, ExposureMask | KeyPressMask);

    xdata.gc = XCreateGC(xdata.display, xdata.window, 0, NULL);

    XMapWindow(xdata.display, xdata.window);


    return window;
}

extern void SendCloseEvent(struct CrossWindow* window)
{
    xdata.exit = 1;
}

extern void SetWindowRect(struct CrossWindow* window, struct CrossRect rect)
{
    window->rect = rect;
}

extern void SetWindowTitle(struct CrossWindow* window, const char* title)
{
    char* append = " - Linux implementation";
    char* newTitle = concat(title, append);

    window->title = newTitle;
}

extern int Update()
{
    XNextEvent(xdata.display, &xdata.event);

    return xdata.exit;

}

extern struct CrossInput GetInput()
{
    struct CrossInput input;

    if(xdata.event.type == KeyPress)
    {
        input.key = xdata.event.xkey.keycode;
        input.state = 1;
    }
    else
    {
        input.key = 0;
        input.state = 0;
    }

    return input;
}

extern void DisposeWindow(struct CrossWindow* window)
{
    free(window->title);

    XFreeGC(xdata.display, xdata.gc);
    XDestroyWindow(xdata.display, xdata.window);
    XCloseDisplay(xdata.display);


    window->title = NULL;
}

char* concat (const char *s1, const char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);//+1 for the zero-terminator
    if(result == NULL)
    {
        fprintf(stderr, "Error: malloc failed in concat.\n");
        exit(EXIT_FAILURE);
    }
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}