#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
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
    
    XStoreName(xdata.display, xdata.window, window.title);
    
    XSelectInput(xdata.display, xdata.window, ExposureMask | KeyPressMask | KeyReleaseMask);

    xdata.gc = XCreateGC(xdata.display, xdata.window, 0, NULL);

    XMapWindow(xdata.display, xdata.window);


    return window;
}

extern void SendCloseEvent(struct CrossWindow* window)
{
    xdata.exit = 1;
}


extern int Update()
{
    XNextEvent(xdata.display, &xdata.event);

    return xdata.exit;

}

extern void SetColor(struct CrossWindow* window, struct CrossPoint point, struct CrossColor color)
{
    XSetForeground(xdata.display, xdata.gc, color.r << 16 | color.g << 8 | color.b);
    XDrawPoint(xdata.display, xdata.window, xdata.gc, point.x, point.y);
}

extern void SetColors(struct CrossWindow* window, struct CrossPoint* points, struct CrossColor* colors, int count)
{
    int i;
    for (i = 0; i < count; i++)
    {
        SetColor(window, points[i], colors[i]);
    }
}

extern struct CrossInput GetInput()
{
    struct CrossInput input;
    KeyCode keycode;
    KeySym keysym;
    
    switch (xdata.event.type)
    {
        case KeyPress:
            keycode = xdata.event.xkey.keycode;
            keysym = XKeycodeToKeysym(xdata.display, keycode, 0);
            input.key = XKeysymToString(keysym);
            input.state = 1;
            break;
        case KeyRelease:
            keycode = xdata.event.xkey.keycode;
            keysym = XKeycodeToKeysym(xdata.display, keycode, 0);            
            input.key = XKeysymToString(keysym);
            input.state = 0;
            break;
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