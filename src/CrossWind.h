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

struct CrossInput
{
    char* key;
    int state;
};

struct CrossInput GetInput();

struct CrossWindow GenerateWindow(struct CrossRect rect, const char* title);

int Update();

void SetColor(struct CrossWindow* window, struct CrossPoint point, struct CrossColor color);

void SetColors(struct CrossWindow* window, struct CrossPoint* points, struct CrossColor* colors, int count);

void DisposeWindow(struct CrossWindow* window);

void SendCloseEvent(struct CrossWindow* window);
