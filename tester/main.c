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
        
        if (input.state == 1 )
        {
            printf("Key: %s\n", input.key);
            if (strncmp(input.key, "Esc", 3) == 0)
            {
                SendCloseEvent(&window);
            }
            else if (strcmp(input.key, "Return") == 0 || strcmp(input.key, "Enter") == 0)
            {
                //draw 8 x 8 black square
                struct CrossPoint points[64];
                struct CrossColor colors[64];
                int i;
                for (i = 0; i < 64; i++)
                {
                    points[i].x = i % 8;
                    points[i].y = i / 8;
                    colors[i].r = 0;
                    colors[i].g = 0;
                    colors[i].b = 0;
                    colors[i].a = 0;
                }
                SetColors(&window, points, colors, 64);
            }
        }

        exit = Update(); 
        
    }
    DisposeWindow(&window);

    return 0;
}