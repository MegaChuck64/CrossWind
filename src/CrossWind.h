struct CrossColor
{
    char r;
    char g;
    char b;
    char a;
};

struct CrossColorPoints
{
    struct CrossColor* colors;
    struct CrossPoint* points;
    int count;
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
    char key[16];
    int keyState;
    struct CrossPoint mousePoint;
    int mouseState;
};

struct CrossInput GetInput();

struct CrossWindow GenerateWindow(struct CrossRect rect, const char* title);

int Update();

void SetColor(struct CrossWindow* window, struct CrossPoint point, struct CrossColor color);

void SetColors(struct CrossWindow* window, struct CrossPoint* points, struct CrossColor* colors, int count);

void SetColorPoints(struct CrossWindow* window, struct CrossColorPoints points);

void SetColorRect(struct CrossWindow* window, struct CrossRect rect, struct CrossColor color);

void ClearWindow(struct CrossWindow* window, struct CrossColor color);

void DisposeWindow(struct CrossWindow* window);

void SendCloseEvent(struct CrossWindow* window);

void DrawString(struct CrossWindow* window, struct CrossPoint point, const char* text, struct CrossColor color, const char* fontName, int fontSize);

char* concat (const char *s1, const char *s2);

void lower_string(char* s, int len);
