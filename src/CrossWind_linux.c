#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/Xft/Xft.h>
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
    
    XSelectInput(xdata.display, xdata.window, ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask);

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
    struct CrossInput input = {0};
    KeyCode keycode;
    KeySym keysym;
    int MAX_BUFFER_SIZE = 16;
    char buffer[MAX_BUFFER_SIZE];
    
    switch (xdata.event.type)
    {
        case KeyPress:
            keycode = xdata.event.xkey.keycode;
            keysym = XKeycodeToKeysym(xdata.display, keycode, 0);
            if (keysym != NoSymbol)
            {
                char* key = XKeysymToString(keysym);
                strncpy(buffer, key, MAX_BUFFER_SIZE - 1);
                buffer[MAX_BUFFER_SIZE - 1] = '\0';
                lower_string(buffer, MAX_BUFFER_SIZE);
                strcpy(input.key, buffer);
            }
            input.keyState = 1;
            break;
        case KeyRelease:
            keycode = xdata.event.xkey.keycode;
            keysym = XKeycodeToKeysym(xdata.display, keycode, 0);
            if (keysym != NoSymbol)
            {
                char* key = XKeysymToString(keysym);
                strncpy(buffer, key, MAX_BUFFER_SIZE - 1);
                buffer[MAX_BUFFER_SIZE - 1] = '\0';
                lower_string(buffer, MAX_BUFFER_SIZE);
                strcpy(input.key, buffer);
            }
            input.keyState = 0;
            break;
        case ButtonPress:
            input.mouseState = 1;
            break;
        case ButtonRelease:
            input.mouseState = 0;
            break;
        case MotionNotify:
            input.mousePoint.x = xdata.event.xmotion.x;
            input.mousePoint.y = xdata.event.xmotion.y;
            break;        

    }
    return input;
}

extern void DrawString(struct CrossWindow* window, struct CrossPoint point, const char* text, struct CrossColor color, const char* fontName, int fontSize)
{
    if (!window) {
        printf("Error: Invalid window pointer\n");
        return;
    }

    XftFont *font;
    font = XftFontOpenName(xdata.display, DefaultScreen(xdata.display), fontName);
    font->height = fontSize * 64; // Set font size in pixels

    // Create a color using Xft
    XftColor col;
    XRenderColor xcolor = {color.r << 8, color.g << 8, color.b << 8, 0xFFFF};
    XftColorAllocValue(xdata.display, DefaultVisual(xdata.display, DefaultScreen(xdata.display)), DefaultColormap(xdata.display, DefaultScreen(xdata.display)), &xcolor, &col);

    // Draw the text to the window
    XftDraw *xftdraw = XftDrawCreate(xdata.display, xdata.window, DefaultVisual(xdata.display, DefaultScreen(xdata.display)), DefaultColormap(xdata.display, DefaultScreen(xdata.display)));
    XftDrawStringUtf8(xftdraw, &col, font, point.x, point.y, (XftChar8*)text, strlen(text));

    // Flush changes to the window
    XFlush(xdata.display);

    // Cleanup
    XftDrawDestroy(xftdraw);
    XftColorFree(xdata.display, DefaultVisual(xdata.display, DefaultScreen(xdata.display)), DefaultColormap(xdata.display, DefaultScreen(xdata.display)), &col);
    XftFontClose(xdata.display, font);
}

extern void DisposeWindow(struct CrossWindow* window)
{
    XFreeGC(xdata.display, xdata.gc);
    XDestroyWindow(xdata.display, xdata.window);
    XCloseDisplay(xdata.display);
}


char* concat(const char* s1, const char* s2)
{
    static char result[100];
    if (strlen(s1) + strlen(s2) < 100)
    {
        strcpy(result, s1);
        strcat(result, s2);
        result[strlen(s1) + strlen(s2)] = '\0';
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
    s[len] = '\0';
}