struct CrossColor
{
    char r;
    char g;
    char b;
    char a;
};

struct CrossPoint
{
    int x;
    int y;
};

struct CrossSize
{
    int width;
    int height;
};


struct CrossRect
{
    struct CrossPoint point;
    struct CrossSize size;
};


struct CrossWindow 
{
    struct CrossRect rect;
    const char* title;
};


struct CrossWindow CreateWindow(struct CrossRect rect, const char* title);

void SetWindowRect(struct CrossWindow* window, struct CrossRect rect);

void SetWindowTitle(struct CrossWindow* window, const char* title);

void DisposeWindow(struct CrossWindow* window);