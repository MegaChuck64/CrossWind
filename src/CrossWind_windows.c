#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "CrossWind.h"

char* concat(const char* s1, const char* s2);

extern struct CrossWindow CreateWindow(struct CrossRect rect, const char* title)
{
    char* append = " - Windows implementation";    
    char* newTitle = concat(title, append);

    struct CrossWindow window;
    window.rect = rect;
    window.title = newTitle;
    return window;
}

extern void SetWindowRect(struct CrossWindow* window, struct CrossRect rect)
{
    window->rect = rect;
}

extern void SetWindowTitle(struct CrossWindow* window, const char* title)
{
    char* append = " - Windows implementation";
    char* newTitle = concat(title, append);
    
    window->title = newTitle;
}

extern void DisposeWindow(struct CrossWindow* window)
{
    free(window->title);
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