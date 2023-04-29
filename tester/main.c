#include <stdio.h>
#include <string.h>

#include "../src/CrossWind.h"

int main()
{
    struct CrossRect windowRect = {100, 100, 400, 400};

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
        printf("Key: %s\n", input.key);
        if (input.state == 1 )
        {
            if (strcmp(input.key, "Escape") == 0 || strcmp(input.key, "Esc") == 0)
            {
                SendCloseEvent(&window);
            }
        }

        exit = Update(); 
    }
    DisposeWindow(&window);

    return 0;
}