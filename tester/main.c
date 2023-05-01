#include <stdio.h>
#include <string.h>

#include "../src/CrossWind.h"



void game_loop(struct CrossInput input, struct CrossWindow window)
{
    static int x = 0;
    static int y = 0;

    ClearWindow(&window, (struct CrossColor){255, 255, 255, 255});

    if (input.keyState == 1)
    {
        if (strcmp(input.key, "w") == 0)
        {
            y -= 1;
        }
        if (strcmp(input.key, "s") == 0)
        {
            y += 1;
        }
        if (strcmp(input.key, "a") == 0)
        {
            x -= 1;
        }
        if (strcmp(input.key, "d") == 0)
        {
            x += 1;
        }
        if (strcmp(input.key, "esc") == 0)
        {
            SendCloseEvent(&window);
        }
    }

    if (input.mouseState == 1)
    {
        x = input.mousePoint.x;
        y = input.mousePoint.y;        
    }

    SetColorRect(&window, (struct CrossRect){x, y, 64, 64}, (struct CrossColor){0, 0, 0, 255});

    //draw text
    char* fontName;
    #ifdef _WIN32
        fontName = "Courier New";
    #else
        fontName = "fixed";
    #endif

    DrawString(&window, (struct CrossPoint){170, 20}, "Hello, World!", (struct CrossColor){255, 0, 0, 255}, fontName, 20);

}


int main()
{
    struct CrossWindow window = GenerateWindow((struct CrossRect){ 40, 40, 800, 600 } , "CrossWind");

    while (Update() != 1)
    {
        game_loop(GetInput(), window); 
    }

    DisposeWindow(&window);

    return 0;
}

