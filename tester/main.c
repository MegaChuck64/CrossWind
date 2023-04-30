#include <stdio.h>
#include <string.h>

#include "../src/CrossWind.h"

int main()
{
    struct CrossRect windowRect = {100, 100, 400, 400};

    struct CrossWindow window;

    window = GenerateWindow(windowRect, "CrossWind");

    int exit;
    struct CrossInput input;
    int offset = 0;
    while (Update() != 1)
    {
        input = GetInput();
        
        if (input.state == 1)
        {
            if (strncmp(input.key, "esc", 3) == 0)
            {
                //close window
                SendCloseEvent(&window);
            }
            else if (strcmp(input.key, "return") == 0 || strcmp(input.key, "enter") == 0)
            {
                //draw black 8x8 squares diagonally across window
                struct CrossRect rect = {offset * 8, offset * 8, 8, 8};
                SetColorRect(&window, rect, (struct CrossColor){0, 0, 0, 255});
                
                offset++;
            }
            else if (strcmp(input.key, "t") == 0)
            {
                //draw text
                char* fontName;
                #ifdef _WIN32
                    fontName = "Courier New";
                #else
                    fontName = "fixed";
                #endif

                DrawString(&window, (struct CrossPoint){20, 20}, "Hello, World!", (struct CrossColor){255, 0, 0, 255}, fontName, 20);
            }
            else if (strcmp(input.key, "space") == 0)
            {
                //clear window with white color
                ClearWindow(&window, (struct CrossColor){255, 255, 255, 255});
                offset = 0;
            }
        }
    }

    DisposeWindow(&window);

    return 0;
}