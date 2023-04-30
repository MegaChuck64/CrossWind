#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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

void SetColorPoints(struct CrossWindow* window, struct CrossColorPoints points)
{
    SetColors(window, points.points, points.colors, points.count);
}

void SetColorRect(struct CrossWindow* window, struct CrossRect rect, struct CrossColor color)
{
    XSetForeground(xdata.display, xdata.gc, color.r << 16 | color.g << 8 | color.b);
    XFillRectangle(xdata.display, xdata.window, xdata.gc, rect.point.x, rect.point.y, rect.size.width, rect.size.height);
}

void ClearWindow(struct CrossWindow* window, struct CrossColor color)
{
    struct CrossRect rect;
    rect.point.x = 0;
    rect.point.y = 0;
    rect.size.width = window->rect.size.width;
    rect.size.height = window->rect.size.height;

    SetColorRect(window, rect, color);
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
            if (keysym != NULL)
            {
                char* key = XKeysymToString(keysym);
                char buffer[20];
                strcpy(buffer, key);
                lower_string(buffer, 20);
                input.key = buffer;
            }
            input.state = 1;
            break;
        case KeyRelease:
            keycode = xdata.event.xkey.keycode;
            keysym = XKeycodeToKeysym(xdata.display, keycode, 0);    
            if (keysym != NULL)
            {
                char* key = XKeysymToString(keysym);
                char buffer[20];
                strcpy(buffer, key);
                lower_string(buffer, 20);
                input.key = buffer;
            }
            input.state = 0;
            break;
    }
    return input;
}

extern void DrawString(struct CrossWindow* window, struct CrossPoint point, const char* text, struct CrossColor color, const char* fontName)
{
    XFontStruct* fontInfo;
    fontInfo = XLoadQueryFont(xdata.display, fontName);
    char error_string[255];
    if (!fontInfo) {
        sprintf(error_string, "Error: XLoadQueryFont - failed loading font: %s\n", fontName);
        fprintf(stderr, error_string);
        return;
    }
    XSetFont(xdata.display, xdata.gc, fontInfo->fid);
    XSetForeground(xdata.display, xdata.gc, color.r << 16 | color.g << 8 | color.b);
    XDrawString(xdata.display, xdata.window, xdata.gc, point.x, point.y, text, strlen(text));    
}

extern void DisposeWindow(struct CrossWindow* window)
{
    XFreeGC(xdata.display, xdata.gc);
    XDestroyWindow(xdata.display, xdata.window);
    XCloseDisplay(xdata.display);
}


char* concat (const char *s1, const char *s2)
{
    static char result[100];
    if (strlen(s1) + strlen(s2) < 100)
    {
        strcpy(result, s1);
        strcat(result, s2);
    }
    else
    {
        printf("Error: concat() result too long\n");
    }

    return result;    
}

void lower_string(char* s, int len)
{
    int i;
    for (i = 0; i < len; i++)
    {        
        s[i] = tolower(s[i]);
    }
}