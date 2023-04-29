#include <stdio.h>
#include "../src/CrossWind.h"

int main()
{
    struct CrossRect windowRect = {0, 0, 400, 400};

    struct CrossWindow window;

    window = GenerateWindow(windowRect, "CrossWind");

    printf("--------------------\n");
    printf("Window: %s\n", window.title);
    printf("Window Rect: %d, %d, %d, %d\n", window.rect.point.x, window.rect.point.y, window.rect.size.width, window.rect.size.height);

    int exit;
    struct CrossInput input;
    while (exit != 1)
    {
        input = GetInput();
        //check if input is escape key
        printf("Key: %d\n", input.key);
        if (input.state == 1)
        {
            SendCloseEvent(&window);
        }

        exit = Update(); 
    }
    DisposeWindow(&window);

    return 0;
}