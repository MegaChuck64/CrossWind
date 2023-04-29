#include <stdio.h>
#include "../src/CrossWind.h"

int main()
{
    struct CrossRect windowRect = {0, 0, 800, 600};

    struct CrossWindow window;

    window = CreateWindow(windowRect, "CrossWind");

    printf("--------------------\n");
    printf("Window: %s\n", window.title);
    printf("Window Rect: %d, %d, %d, %d\n", window.rect.point.x, window.rect.point.y, window.rect.size.width, window.rect.size.height);

    struct CrossRect newWindowRect = {0, 0, 1024, 768};

    SetWindowRect(&window, newWindowRect);

    printf("--------------------\n");
    printf("Window: %s\n", window.title);
    printf("Window Rect: %d, %d, %d, %d\n", window.rect.point.x, window.rect.point.y, window.rect.size.width, window.rect.size.height);

    SetWindowTitle(&window, "CrossWind 2");

    printf("--------------------\n");
    printf("Window: %s\n", window.title);
    printf("Window Rect: %d, %d, %d, %d\n", window.rect.point.x, window.rect.point.y, window.rect.size.width, window.rect.size.height);

    DisposeWindow(&window);

    return 0;
}