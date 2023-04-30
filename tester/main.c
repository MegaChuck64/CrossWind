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
    int offset = 0;
    while (exit != 1)
    {
        input = GetInput();
        
        if (input.state == 1 )
        {
            printf("Key: %s\n", input.key);
            if (strncmp(input.key, "esc", 3) == 0)
            {
                SendCloseEvent(&window);
            }
            else if (strcmp(input.key, "return") == 0 || strcmp(input.key, "enter") == 0)
            {
                //draw 8 x 8 black square using SetColorRect
                struct CrossRect rect = {offset * 8, offset * 8, 8, 8};
                SetColorRect(&window, rect, (struct CrossColor){0, 0, 0, 255});
                
                offset++;
            }
            else if (strcmp(input.key, "space") == 0)
            {
                ClearWindow(&window, (struct CrossColor){255, 255, 255, 255});
                offset = 0;
            }
        }

        exit = Update(); 
        
    }
    DisposeWindow(&window);

    return 0;
}