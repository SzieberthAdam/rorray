#include <math.h>
#include <stdint.h>

// TSC clock
// https://stackoverflow.com/questions/13772567/how-to-get-the-cpu-cycle-count-in-x86-64-from-c
#ifdef _MSC_VER
# include <intrin.h>
#else
# include <x86intrin.h>
#endif

#include "raylib.h"
static void DrawTextBoxed(Font font, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint, float lineHeight);   // Draw text using font inside rectangle limits; taken from raylib example text_rectangle_bounds.c


#include "bitbit.h"
#include "rorfile.h"

#define NUM_FACTIONS    6
#define RIGHT  6
#define DOWN   12
#define PAD   1
#define UNIT  4

#define UNITCLAMP(a) (uint16_t)(UNIT * ceil((float)(a) / UNIT))

#define FONTSPACING     2
#define FONTSIZE        font1.baseSize*1.0f
#define SFONTSPACING    fontScale * FONTSPACING
#define SFONTSIZE       fontScale * FONTSIZE

#define TARGET_FPS     30

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

// https://stackoverflow.com/a/3553321/2334951
#define MEMBER_SIZE(type, member) sizeof(((type *)0)->member)

#define SENATORNAMEWIDTH  8

#define COLOR_AfricanViolet   ((Color){0xAA, 0x88, 0xBB, 0xFF}) // Allegiance Marker
#define COLOR_BrightGray      ((Color){0xEE, 0xEE, 0xEE, 0xFF}) // Board Text
#define COLOR_CarrotOrange    ((Color){0xEE, 0x88, 0x22, 0xFF}) // Exile Marker
#define COLOR_ChineseBlack    ((Color){0x11, 0x11, 0x11, 0xFF}) // Board Black
#define COLOR_ChineseSilver   ((Color){0xCC, 0xCC, 0xCC, 0xFF}) // Middle Republic card
#define COLOR_Cinnabar        ((Color){0xEE, 0x44, 0x33, 0xFF}) // Priest Marker; Rebel Marker
#define COLOR_ColumbiaBlue    ((Color){0xCC, 0xDD, 0xEE, 0xFF}) // Map See
#define COLOR_CookiesAndCream ((Color){0xEE, 0xDD, 0xAA, 0xFF}) // Map Land
#define COLOR_CrayolaSkyBlue  ((Color){0x66, 0xCC, 0xEE, 0xFF}) // Unrest Level Marker; Captive Marker
#define COLOR_CrayolaTan      ((Color){0xDD, 0x99, 0x77, 0xFF}) // Board Blocks
#define COLOR_Goldenrod       ((Color){0xDD, 0x99, 0x22, 0xFF}) // Talent
#define COLOR_ImperialRed     ((Color){0xEE, 0x22, 0x33, 0xFF}) // Legion and Fleet Markers; Office Marker; Manpower Shortage Marker; etc.
#define COLOR_LightTaupe      ((Color){0xBB, 0x88, 0x66, 0xFF}) // Board Blocks Dark
#define COLOR_Melon           ((Color){0xFF, 0xAA, 0xAA, 0xFF}) // Late Republic card
#define COLOR_RaisinBlack     ((Color){0x22, 0x22, 0x22, 0xFF}) // Marker Black Light
#define COLOR_Red             ((Color){0xFF, 0x22, 0x22, 0xFF}) // Card red
#define COLOR_TitaniumYellow  ((Color){0xEE, 0xDD, 0x00, 0xFF}) // Faction Leader Markers
#define COLOR_VivdLimeGreen   ((Color){0xAA, 0xCC, 0x00, 0xFF}) // Prior Consul Marker; Faction Dominance Marker

#define COLOR_BACKGOUNDSECTIONTEXT COLOR_BrightGray
#define COLOR_BACKGOUNDTEXT COLOR_BrightGray
#define COLOR_BACKGROUND COLOR_ChineseBlack
#define COLOR_BACKGROUNDAREA COLOR_RaisinBlack
#define COLOR_BEINGEDITED  COLOR_CarrotOrange
#define COLOR_BEINGEDITEDTEXT  COLOR_ChineseBlack
#define COLOR_BLACKCARDBACKGROUND COLOR_BrightGray
#define COLOR_BLACKCARDTEXT COLOR_ChineseBlack
#define COLOR_BUTTONBACKGROUND COLOR_BrightGray
#define COLOR_BUTTONOUTLINE COLOR_ChineseBlack
#define COLOR_BUTTONTEXT COLOR_ChineseBlack
#define COLOR_CLICKED COLOR_CookiesAndCream
#define COLOR_CLICKEDTEXT COLOR_ChineseBlack
#define COLOR_EDITABLETEXT  COLOR_Goldenrod
#define COLOR_CLICKABLE COLOR_RaisinBlack
#define COLOR_CLICKABLETEXT COLOR_Goldenrod
#define COLOR_ERROR COLOR_Red
#define COLOR_FACTION COLOR_TitaniumYellow
#define COLOR_FACTIONHEADER COLOR_LightTaupe
#define COLOR_FACTIONHEADERTEXT COLOR_ChineseBlack
#define COLOR_FACTIONTEXT COLOR_ChineseBlack
#define COLOR_HRAO COLOR_LightTaupe
#define COLOR_MOUSEDRAG  COLOR_CookiesAndCream
#define COLOR_MOUSEDRAGTEXT  COLOR_ChineseBlack
#define COLOR_MOUSEHOVER_CLICKABLE COLOR_CookiesAndCream
#define COLOR_MOUSEHOVER_CLICKABLETEXT COLOR_ChineseBlack
#define COLOR_MOUSEHOVER_DRAGABLE  COLOR_CookiesAndCream
#define COLOR_MOUSEHOVER_DROPTARGET  COLOR_CookiesAndCream
#define COLOR_MOUSEHOVER_EDITABLE  COLOR_CookiesAndCream
#define COLOR_MOUSEHOVER_EDITABLETEXT  COLOR_ChineseBlack
#define COLOR_MOUSEHOVER_GAMEMASTER  MAGENTA
#define COLOR_MOUSEHOVER_SELECTABLE  COLOR_CookiesAndCream
#define COLOR_OFFICE COLOR_ImperialRed
#define COLOR_OFFICETEXT COLOR_ChineseBlack
#define COLOR_ERAHEADER COLOR_CrayolaTan
#define COLOR_ERAHEADERTEXT COLOR_ChineseBlack
#define COLOR_STATESMANBACKGROUND COLOR_BrightGray
#define COLOR_STATESMANTEXT COLOR_Red
#define COLOR_SUBTITLEBACKGROUND COLOR_RaisinBlack
#define COLOR_SUBTITLETEXT COLOR_ChineseSilver
#define COLOR_TITLEBACKGROUND COLOR_Goldenrod
#define COLOR_TITLETEXT COLOR_ChineseBlack
#define COLOR_TOGGLEOUTLINE COLOR_BrightGray
#define COLOR_TOGGLESET COLOR_Goldenrod
#define COLOR_TOGGLEUNSET COLOR_RaisinBlack
#define COLOR_TOOLTIPBACKGROUND COLOR_CookiesAndCream
#define COLOR_TOOLTIPOUTLINE COLOR_ChineseBlack
#define COLOR_TOOLTIPTEXT COLOR_ChineseBlack

#define COLOR_DEBUGTEXT WHITE


#define Font1H font1.baseSize
#define Font2H font2.baseSize
#define Font3H font3.baseSize
#define Font1RectH (12 - PAD)
#define Font2RectH (24 - PAD)
#define Font3RectH (36 - PAD)
#define Font1HUnit (2 * UNIT)
#define Font2HUnit (5 * UNIT)
#define Font3HUnit (8 * UNIT)
#define Font1Spacing 1
#define Font2Spacing 2
#define Font3Spacing 3
#define Font1PaddingX 1
#define Font2PaddingX 4
#define Font3PaddingX 7
#define DrawFont1(text, box, tint, align, offset) (DrawTextEx2(font1, text, box, Font1H, Font1Spacing, ((Vector2){Font1PaddingX, 0}), tint, align, offset))
#define DrawFont2(text, box, tint, align, offset) (DrawTextEx2(font2, text, box, Font2H, Font2Spacing, ((Vector2){Font2PaddingX, 0}), tint, align, offset))
#define DrawFont3(text, box, tint, align, offset) (DrawTextEx2(font3, text, box, Font3H, Font3Spacing, ((Vector2){Font3PaddingX, 0}), tint, align, offset))
#define DrawTitle(text, box, tint, align) (DrawTextEx2(font3, text, box, Font3H, Font3Spacing, ((Vector2){Font3PaddingX + 2 * UNIT, 0}), tint, align, ((Vector2){0, 1})))
#define TITLEHEIGHT (Font3RectH + 4 * UNIT)


#define RECT_SEN_HRAO_X (-6 * UNIT)
#define RECT_SEN_ID_X (0 * Font2HUnit)
#define RECT_SEN_NAME_X (2 * Font1HUnit)
#define RECT_SEN_OFF_X (10 * Font2HUnit)
#define RECT_SEN_M_X (13 * Font2HUnit)
#define RECT_SEN_O_X (15 * Font2HUnit)
#define RECT_SEN_L_X (17 * Font2HUnit)
#define RECT_SEN_I_X (19 * Font2HUnit)
#define RECT_SEN_P_X (21 * Font2HUnit)
#define RECT_SEN_PRIC_X (23 * Font2HUnit)

#define RECT_SEN_WIDTH (23 * Font2HUnit - PAD)
#define RECT_SEN_WIDTH2 (25 * Font2HUnit - PAD)

#define RECT_SEN_HRAO_WIDTH (6 * UNIT)
#define RECT_SEN_ID_WIDTH (2 * Font1HUnit)
#define RECT_SEN_NAME_WIDTH (10 * Font2HUnit - 2 * Font1HUnit - PAD)
#define RECT_SEN_OFF_WIDTH (3 * Font2HUnit - PAD)
#define RECT_SEN_M_WIDTH (2 * Font2HUnit - PAD)
#define RECT_SEN_O_WIDTH (2 * Font2HUnit - PAD)
#define RECT_SEN_L_WIDTH (2 * Font2HUnit - PAD)
#define RECT_SEN_I_WIDTH (2 * Font2HUnit - PAD)
#define RECT_SEN_P_WIDTH (2 * Font2HUnit - PAD)
#define RECT_SEN_PRIC_WIDTH (2 * Font2HUnit - PAD)

#ifndef MAX_TEXT_BUFFER_LENGTH
    #define MAX_TEXT_BUFFER_LENGTH              1024        // Size of internal static buffers used on some functions:
                                                            // TextFormat(), TextSubtext(), TextToUpper(), TextToLower(), TextToPascal(), TextSplit()
#endif

const char *TextToUpper2(const char *text)
{
    static char buffer[MAX_TEXT_BUFFER_LENGTH] = { 0 };
    memset(buffer, 0, MAX_TEXT_BUFFER_LENGTH);

    for (int i = 0; i < MAX_TEXT_BUFFER_LENGTH; i++)
    {
        if (text[i] != '\0')
        {
            buffer[i] = (char)toupper(text[i]);
            //if ((text[i] >= 'a') && (text[i] <= 'z')) buffer[i] = text[i] - 32;

            // TODO: Support UTF-8 diacritics to upper-case
            //if ((text[i] >= 'à') && (text[i] <= 'ý')) buffer[i] = text[i] - 32;
        }
        else { buffer[i] = '\0'; break; }
    }

    return buffer;
}

// Greatest power of 2 less than or equal to x. Hacker's Delight, Figure 3-1.
unsigned flp2(unsigned x)
{
    x = x | (x >> 1);
    x = x | (x >> 2);
    x = x | (x >> 4);
    x = x | (x >> 8);
    x = x | (x >> 16);
    return x - (x >> 1);
}

// Least power of 2 greater than or equal to x. Hacker's Delight, Figure 3-3.
unsigned clp2(unsigned x)
{
    x = x - 1;
    x = x | (x >> 1);
    x = x | (x >> 2);
    x = x | (x >> 4);
    x = x | (x >> 8);
    x = x | (x >> 16);
    return x + 1;
}

// Find first set bit in an integer.
unsigned ffs(int n)
{
    return log2(n & -n) + 1;
}

// Use the following formula to turn off the rightmost 1-bit in a word, producing 0 if none (e.g., 01011000 ⇒ 01010000)
// This can be used to determine if an unsigned integer is a power of 2 or is 0: apply the formula followed by a 0-test on the result.
// Hacker's Delight, 2-1.
inline unsigned urmb(unsigned x)
{
    return x & (x - 1);
}

// Determine if an unsigned integer is a power of 2.
bool ispow2(unsigned x)
{
    if (x==0) return 0;
    else return (urmb(x) == 0);
}


Rectangle rect(Vector2 topleftpoint, int width_units, int height_units)
{
    Rectangle r = {topleftpoint.x, topleftpoint.y, width_units * RIGHT - PAD, height_units * DOWN - PAD};
    return r;
}


typedef enum {
    TextLeft = 0,
    TextCenter = 1,
    TextRight = 2,
    TextTopLeft = 3,
    TextTopCenter = 4,
    TextTopRight = 5
} TextAlignment;

// Temporary
Rectangle DrawText2(Font font, const char *text, Rectangle box, float fontSize, float spacing, Color tint, TextAlignment align)
{
    Vector2 textsize = MeasureTextEx(font, text, fontSize, spacing);
    Vector2 position = *(Vector2 *)(&box);
    switch (align)
    {
        case TextCenter:
        {
            position.x += (int)((box.width - textsize.x)/2);
            position.y += (int)((box.height - textsize.y)/2);
        } break;
        case TextRight:
        {
            position.x += (int)(box.width - textsize.x - spacing);
            position.y += (int)((box.height - textsize.y)/2);
        } break;
        case TextTopLeft:
        {
            position.x += spacing;
        } break;
        case TextTopCenter:
        {
            position.x += (int)((box.width - textsize.x)/2);
        } break;
        case TextTopRight:
        {
            position.x += (int)(box.width - textsize.x - spacing);
        } break;
        case TextLeft:
        default:
            position.x += spacing;
            position.y += (int)((box.height - textsize.y)/2);
    }
    DrawTextEx(font, text, position, fontSize, spacing, tint);
    return ((Rectangle){position.x, position.y, textsize.x, textsize.y});
}

Rectangle DrawTextEx2(Font font, const char *text, Rectangle box, float fontSize, float spacing, Vector2 padding, Color tint, TextAlignment align, Vector2 offset)
{
    Vector2 textsize = MeasureTextEx(font, text, fontSize, spacing);
    Vector2 position = *(Vector2 *)(&box);
    switch (align)
    {
        case TextCenter:
        {
            position.x += (int)((box.width - textsize.x)/2);
            position.y += (int)((box.height - textsize.y)/2);
        } break;
        case TextRight:
        {
            position.x += (int)(box.width - textsize.x - padding.x);
            position.y += (int)((box.height - textsize.y)/2);
        } break;
        case TextTopLeft:
        {
            position.x += padding.x;
        } break;
        case TextTopCenter:
        {
            position.x += (int)((box.width - textsize.x)/2);
        } break;
        case TextTopRight:
        {
            position.x += (int)(box.width - textsize.x - padding.x);
        } break;
        case TextLeft:
        default:
            position.x += padding.x;
            position.y += (int)((box.height - textsize.y)/2);
    }
    position.x += offset.x;
    position.y += offset.y;
    DrawTextEx(font, text, position, fontSize, spacing, tint);
    return ((Rectangle){position.x, position.y, textsize.x, textsize.y});
}


bool save(rordata, length)
{
    char fileName[64];
    sprintf(fileName, "saved/%s %03d-%d-%d.ror", p_HEADER(rordata)->name, p_HEADER(rordata)->turn, (p_HEADER(rordata)->phse >> 28), (p_HEADER(rordata)->phse & 0x0FFFFFFF));
    return SaveFileData(fileName, rordata, length);
}


int main(void)
{
    SetTraceLogLevel(LOG_DEBUG); // TODO: DEBUG
    int debugvar = 0;
    char debugstr[256][256] = {0};

    Vector2 cursor;
    char str[1024];

    uint16_t windowedScreenWidth = 1960;
    uint16_t windowedScreenHeight = 1080;
    uint16_t screenRights = (windowedScreenWidth - 2 * PAD) / RIGHT;
    uint16_t screenDowns = (windowedScreenHeight - 2 * PAD) / DOWN;
    uint16_t screenOffsetX = (windowedScreenWidth - RIGHT * screenRights) / 2;
    uint16_t screenOffsetY = (windowedScreenHeight - DOWN * screenDowns) / 2;
    uint16_t screenWidth = windowedScreenWidth;
    uint16_t screenHeight = windowedScreenHeight;

    Font font1, font2, font3;

    bool rordataLoaded = false;
    unsigned int rordataLength = 0;
    unsigned char *rordata;

    RoR_Header_t *header;
    header = MemAlloc(sizeof(RoR_Header_t));
    header->phse = PhPickScenario;

    Vector2 mouse;
    Vector2 mousedelta;
    int currentGesture = GESTURE_NONE;
    int lastGesture = GESTURE_NONE;

    unsigned char randval = 0;
    int8_t randbitreq = -1;

    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);
    InitWindow(windowedScreenWidth, windowedScreenHeight, "RoRRAY");
    Vector2 windowPos = GetWindowPosition();

    SetTargetFPS(TARGET_FPS);

    font1 = LoadFontEx("Romulus.ttf", 13, 0, 250);
    Vector2 font1Em = MeasureTextEx(font1, "M", font1.baseSize, 0);
    Vector2 font1Ex = MeasureTextEx(font1, "x", font1.baseSize, 0);
    font2 = LoadFontEx("Romulus.ttf", 26, 0, 250);
    Vector2 font2Em = MeasureTextEx(font1, "M", font2.baseSize, 0);
    Vector2 font2Ex = MeasureTextEx(font1, "x", font2.baseSize, 0);
    Vector2 font2E0 = MeasureTextEx(font1, "0", font2.baseSize, 0);
    Vector2 font2E_ = MeasureTextEx(font1, " ", font2.baseSize, 0);
    font3 = LoadFontEx("Romulus.ttf", 39, 0, 250);
    Vector2 font3Em = MeasureTextEx(font1, "M", font3.baseSize, 0);
    Vector2 font3Ex = MeasureTextEx(font1, "x", font3.baseSize, 0);

    int framesCounter = 0;
    int selected = -1;

    int display = GetCurrentMonitor(); // see what display we are on right now

    while (!WindowShouldClose())
    {
        // check for alt + enter
        if (IsKeyPressed(KEY_ENTER) && (IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT)))
        {
            // instead of the true fullscreen which would be the following...
            // if (IsWindowFullscreen()) {ToggleFullscreen(); SetWindowSize(windowedScreenWidth, windowedScreenHeight);}
            // else {SetWindowSize(GetMonitorWidth(display), GetMonitorHeight(display)); ToggleFullscreen();}

            // ... I go for fake fullscreen, similar to SDL_WINDOW_FULLSCREEN_DESKTOP.
            if (IsWindowState(FLAG_WINDOW_UNDECORATED))
            {
                ClearWindowState(FLAG_WINDOW_UNDECORATED);
                screenWidth = windowedScreenWidth;
                screenHeight = windowedScreenHeight;
                SetWindowSize(screenWidth, screenHeight);
                SetWindowPosition(windowPos.x, windowPos.y);
            }
            else
            {
                windowPos = GetWindowPosition();
                windowedScreenWidth = GetScreenWidth();
                windowedScreenHeight = GetScreenHeight();
                screenWidth = GetMonitorWidth(display);
                screenHeight = GetMonitorHeight(display);
                SetWindowState(FLAG_WINDOW_UNDECORATED);
                SetWindowSize(screenWidth, screenHeight);
                SetWindowPosition(0, 0);
            }
        }
        screenWidth = GetScreenWidth();
        screenHeight = GetScreenHeight();

        mouse = GetTouchPosition(0);
        lastGesture = currentGesture;
        currentGesture = GetGestureDetected();

        // DEBUG
        // sprintf(str, "lastGesture: %i; currentGesture: %i", lastGesture, currentGesture);
        // TraceLog(LOG_DEBUG, str);

        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawRectangle(0, 0, screenWidth, screenHeight, COLOR_BACKGROUND);

        // DrawFPS(windowedScreenWidth-100, 10); // for debug

        switch (header->phse)
        {
            case PhPickScenario:
            {
                // PREP
                static bool searched = false;
                static int count = 0;
                static FilePathList files;
                static char **descriptions;
                static unsigned int readLength = sizeof(RoR_Header_t);
                if (IsFileDropped())
                {
                    FilePathList droppedfiles = LoadDroppedFiles();
                    if (droppedfiles.count == 1 && TextIsEqual(GetFileExtension(droppedfiles.paths[0]), ".ror"))
                    {
                        RoR_Header_t *header2 = LoadFileData(droppedfiles.paths[0], &readLength);
                        strncpy(str, &(header2->sign), 3);
                        str[3] = '\0';
                        if ((strcmp(str, RORFILESIGN) == 0) && (header2->vers == RORFILEVERS))
                        {
                            rordataLength = GetFileLength(droppedfiles.paths[0]);
                            rordata = LoadFileData(droppedfiles.paths[0], &rordataLength);
                            MemFree(header);
                            header = p_HEADER(rordata);
                            rordataLoaded = true;
                            DrawRectangle(0, 0, screenWidth, screenHeight, COLOR_BACKGROUND);
                        }
                        else DrawRectangle(0, 0, screenWidth, screenHeight, COLOR_ERROR);
                        UnloadFileData(header2);
                    }
                    else DrawRectangle(0, 0, screenWidth, screenHeight, COLOR_ERROR);

                    UnloadDroppedFiles(droppedfiles);
                }
                else
                {
                    if (!searched)
                    {
                        files = LoadDirectoryFiles(".");
                        descriptions = (char**)MemAlloc(files.count * sizeof(char *));
                        for(int i=0; i<files.count; i++)
                        {
                            if (IsPathFile(files.paths[i]) == false) continue;
                            if (!TextIsEqual(GetFileExtension(files.paths[i]), ".ror")) continue;
                            RoR_Header_t *header2 = LoadFileData(files.paths[i], &readLength);
                            if (0 < header2->phse) continue;
                            descriptions[count] = (char*)MemAlloc(MEMBER_SIZE(RoR_Header_t, desc));
                            strcpy(descriptions[count], header2->desc);
                            if (i != count) strcpy(files.paths[count], files.paths[i]);
                            UnloadFileData(header2);
                            count++;
                        }
                        searched = true;
                    }
                    // TITLE
                    Rectangle r_title = {0, 0, screenWidth, TITLEHEIGHT};
                    DrawRectangleRec(r_title, COLOR_TITLEBACKGROUND);
                    DrawTitle("SCENARIO", r_title, COLOR_TITLETEXT, TextLeft);
                    Rectangle r_subtitle ={r_title.x, r_title.y + r_title.height + PAD + 1 * UNIT, r_title.width, Font2RectH};
                    DrawFont2("NOTE: Drag & drop a .ror file onto this window to load", r_subtitle, COLOR_SUBTITLETEXT, TextLeft, ((Vector2){Font3PaddingX + 1* UNIT, 0}));
                    // LIST
                    Rectangle r_centerlistelem = {0, 0, 60 * Font2HUnit, Font2RectH};
                    r_centerlistelem.x = (uint16_t)(UNITCLAMP((screenWidth - r_centerlistelem.width) / 2));
                    r_centerlistelem.y = (uint16_t)(UNITCLAMP((screenHeight - count * r_centerlistelem.height - (count - 1) * PAD) / 2));
                    Rectangle r = r_centerlistelem;
                    for (int i=0; i<count; i++ )
                    {
                        if (CheckCollisionPointRec(mouse, r))
                        {
                            if (currentGesture == GESTURE_TAP)
                            {
                                rordataLength = GetFileLength(files.paths[i]);
                                rordata = LoadFileData(files.paths[i], &rordataLength);
                                MemFree(header);
                                header = p_HEADER(rordata);
                                rordataLoaded = true;
                                UnloadDirectoryFiles(files);
                                for (int j=0; j<count; j++ ) MemFree(descriptions[count]);
                                MemFree(descriptions);
                                searched = false;
                                DrawRectangleRec(r, COLOR_CLICKED);
                            }
                            else DrawRectangleRec(r, COLOR_MOUSEHOVER_CLICKABLE);
                        }
                        else DrawRectangleRec(r, COLOR_FACTIONHEADER);
                        sprintf(str, "[%s] %s", GetFileName(files.paths[i]), descriptions[i]);
                        DrawFont2(str, r, WHITE, TextLeft, ((Vector2){0, 0}));
                        r.y += Font2RectH + PAD;
                    }
                }
            } break;

            case 0:
            case PhTakeFactions:
            {
                // TITLE
                Rectangle r_title = {0, 0, screenWidth, TITLEHEIGHT};
                DrawRectangleRec(r_title, COLOR_TITLEBACKGROUND);
                DrawTitle("NAME THE PLAYING FACTIONS OR DELETE NAMES FOR EXCLUSION", r_title, COLOR_TITLETEXT, TextLeft);
                // FACTIONS
                int factclicked = -1;
                uint8_t maxLetterCount = MEMBER_SIZE(RoR_FactionItem_t, name) - 1; // -1 because of the trailin null character
                uint8_t fcnt = 0;
                for (uint8_t f = 1; f <= ITEMCOUNT(rordata, Faction); f++) if ((FACTION(f).type & FactionNeutral) == 0) fcnt++;
                Rectangle r_centerlistelem = {0, 0, UNIT + UNITCLAMP((maxLetterCount + 1) * font2Em.x + ((maxLetterCount + 1) - 1) * Font2Spacing + 2 * Font2PaddingX), Font2RectH}; // "UNIT +" is required for unknown reason; (maxLetterCount + 1) because of the shown input cursor: "_"
                r_centerlistelem.x = (uint16_t)((screenWidth - r_centerlistelem.width) / 2);
                r_centerlistelem.y = (uint16_t)((screenHeight - fcnt * r_centerlistelem.height - (fcnt - 1) * PAD) / 2);
                Rectangle r = r_centerlistelem;
                for (uint8_t f = 1; f <= ITEMCOUNT(rordata, Faction); f++)
                {
                    if ((FACTION(f).type & FactionNeutral) != 0) continue;
                    strcpy(str, FACTION(f).name);  // displayed cursor character will be added to str
                    if (CheckCollisionPointRec(mouse, r) && f != selected)
                    {
                        if (currentGesture == GESTURE_TAP)
                        {
                            factclicked = f;
                            selected = f;
                            framesCounter = 0;
                            DrawRectangleRec(r, COLOR_CLICKED);
                        }
                        else DrawRectangleRec(r, COLOR_MOUSEHOVER_EDITABLE);
                    }
                    else if (f == selected)
                    {
                        int letterCount = strlen(str);
                        DrawRectangleRec(r, COLOR_MOUSEHOVER_EDITABLE);
                        int key = GetCharPressed();
                        while (key > 0)
                        {
                            if
                            (
                                (letterCount < maxLetterCount)
                                &&
                                (
                                    (('A' <= key) && (key <= 'Z'))
                                    ||
                                    (('a' <= key) && (key <= 'z'))
                                    ||
                                    (key == ' ')
                                )
                            )
                            {
                                (FACTION(f).name)[letterCount] = (char)key;
                                (FACTION(f).name)[letterCount+1] = '\0'; // Add null terminator at the end of the string.
                                letterCount++;
                            }
                            key = GetCharPressed();  // Check next character in the queue
                        }
                        if (IsKeyPressed(KEY_BACKSPACE))
                        {
                            letterCount--;
                            if (letterCount < 0) letterCount = 0;
                            (FACTION(f).name)[letterCount] = '\0';
                        }
                        if ((letterCount <= maxLetterCount) && (((framesCounter/10)%2) == 0))
                        {
                            letterCount = strlen(FACTION(f).name);
                            str[letterCount] = '_';
                            str[letterCount+1] = '\0';
                            DrawFont2(str, r, COLOR_FACTIONTEXT, TextLeft, ((Vector2){0, 0}));
                        }
                        framesCounter++;
                    }
                    else
                    {
                        DrawRectangleRec(r, COLOR_FACTIONHEADER);
                    }
                    DrawFont2(str, r, COLOR_FACTIONTEXT, TextLeft, ((Vector2){0, 0}));
                    r.y += r.height + 1;
                }
                if (factclicked == -1 && currentGesture == GESTURE_TAP)
                {
                    selected = -1;
                    framesCounter = 0;
                }
                // NEXT BUTTON
                Rectangle r_button = {screenWidth - 4 * UNIT - 4 * Font3HUnit, 2 * UNIT, UNITCLAMP(4 * Font3HUnit), TITLEHEIGHT - 4 * UNIT};
                if (CheckCollisionPointRec(mouse, r_button))
                {
                    if (currentGesture == GESTURE_TAP)
                    {
                        DrawRectangleRounded(r_button, 0.2f, 10, COLOR_CLICKED);
                        uint8_t pcnt = 0;
                        for (uint8_t f = 1; f <= ITEMCOUNT(rordata, Faction); f++) if ((FACTION(f).name[0] != '\0') && ((FACTION(f).type & FactionNeutral) == 0)) pcnt++;
                        uint8_t assign = 0;
                        switch (pcnt)
                        {
                            case 0: break;
                            case 1: assign = SOLITAIRE_FCNT - pcnt; // fallthrough
                            case 2: assign = TWOPLAYERS_FCNT - pcnt; // fallthrough
                            default:
                            {
                                // assign neutral factions
                                for (uint8_t a = 1; a <= assign; a++)
                                {
                                    uint8_t f;
                                    for (f = 1; (f <= ITEMCOUNT(rordata, Faction)) && ((FACTION(f).name[0] != '\0') || ((FACTION(f).type & FactionNeutral) != 0)); f++);
                                    uint8_t fa;
                                    for (fa = 1; (fa < (ITEMCOUNT(rordata, Faction)) && !(((FACTION(fa).type & FactionNeutral) != 0) && (FACTION(fa).asor == a)) ); fa++);
                                    memcpy(&FACTION(f), &FACTION(fa), sizeof(RoR_FactionItem_t));  // copy
                                    FACTION(f).type = FactionUsed | FactionNeutral;
                                }
                                // set type flags
                                for (uint8_t f = 1; (f <= ITEMCOUNT(rordata, Faction)); f++)
                                {
                                    if (FACTION(f).type == FactionNeutral) FACTION(f).type = FactionSet | FactionNeutral;
                                    else if ((FACTION(f).type & FactionNeutral) == 0)
                                    {
                                        if (FACTION(f).name[0] != '\0') FACTION(f).type = FactionSet | FactionUsed;
                                        else FACTION(f).type = FactionSet;
                                    }
                                    // note that we skip the already assigned Neutrals which has the FActionUsed flag set
                                }
                                // eliminate gaps
                                bool done = false;
                                while (!done)
                                {
                                    uint8_t ffirstunused;
                                    for (ffirstunused = 1; (ffirstunused <= ITEMCOUNT(rordata, Faction)) && ((FACTION(ffirstunused).type & FactionUsed) != 0); ffirstunused++);
                                    uint8_t fnextused;
                                    for (fnextused = ffirstunused + 1; (fnextused <= ITEMCOUNT(rordata, Faction)) &&((FACTION(fnextused).type & FactionUsed) == 0); fnextused++);
                                    if (fnextused <= ITEMCOUNT(rordata, Faction))
                                    {
                                        RoR_FactionItem_t faction;
                                        memcpy(&faction, &FACTION(fnextused), sizeof(RoR_FactionItem_t));
                                        memcpy(&FACTION(fnextused), &FACTION(ffirstunused), sizeof(RoR_FactionItem_t));
                                        memcpy(&FACTION(ffirstunused), &faction, sizeof(RoR_FactionItem_t));
                                    }
                                    else done = true;
                                }
                            }
                        }
                        selected = -1;
                        framesCounter = 0;
                        randval = 0;
                        randbitreq = -1;
                        header->phse = PhSetGamename;
                    }
                    else
                    {
                        DrawRectangleRounded(r_button, 0.2f, 10, COLOR_MOUSEHOVER_CLICKABLE);
                    }
                }
                else DrawRectangleRounded(r_button, 0.2f, 10, COLOR_BUTTONBACKGROUND);
                DrawRectangleRoundedLines(r_button, 0.2f, 10, 2, COLOR_BUTTONOUTLINE);
                DrawFont3("NEXT", r_button, COLOR_BUTTONTEXT, TextCenter, ((Vector2){0, 1}));
            } break;

            case PhSetGamename:
            {
                // TITLE
                Rectangle r_title = {0, 0, screenWidth, TITLEHEIGHT};
                DrawRectangleRec(r_title, COLOR_TITLEBACKGROUND);
                DrawTitle("GAMENAME", r_title, COLOR_TITLETEXT, TextLeft);
                // INPUT
                uint8_t maxLetterCount = MEMBER_SIZE(RoR_Header_t, name) - 1; // -1 because of the trailin null character
                uint16_t numLines = 1;
                Rectangle r_centerlistelem = {0, 0, UNIT + UNITCLAMP((maxLetterCount + 1) * font3Em.x + ((maxLetterCount + 1) - 1) * Font3Spacing + 2 * Font3PaddingX + Font3PaddingX), Font3RectH}; // "UNIT +" is required for unknown reason; (maxLetterCount + 1) because of the shown input cursor: "_"
                r_centerlistelem.x = (uint16_t)((screenWidth - r_centerlistelem.width) / 2);
                r_centerlistelem.y = (uint16_t)((screenHeight - numLines * r_centerlistelem.height - (numLines - 1) * PAD) / 2);
                Rectangle r = r_centerlistelem;
                strcpy(str, header->name);  // displayed cursor character will be added to str
                int letterCount = strlen(str);
                DrawRectangleRec(r, COLOR_BEINGEDITED);
                int key = GetCharPressed();
                while (key > 0)
                {
                    // if (32 <= key && key <= 255 && letterCount < maxLetterCount)
                    if
                    (
                        (letterCount < maxLetterCount)
                        &&
                        (
                            (('A' <= key) && (key <= 'Z'))
                            ||
                            (('a' <= key) && (key <= 'z'))
                            ||
                            (('0' <= key) && (key <= '9'))
                            ||
                            (key == '-')
                            ||
                            (key == '_')
                        )
                    )
                    {
                        (header->name)[letterCount] = (char)key;
                        (header->name)[letterCount+1] = '\0'; // Add null terminator at the end of the string.
                        letterCount++;
                    }
                    key = GetCharPressed();  // Check next character in the queue
                }
                if (IsKeyPressed(KEY_BACKSPACE))
                {
                    letterCount--;
                    if (letterCount < 0) letterCount = 0;
                    (header->name)[letterCount] = '\0';
                }
                if ((letterCount <= maxLetterCount) && (((framesCounter/10)%2) == 0))
                {
                    letterCount = strlen(header->name);
                    str[letterCount] = '_';
                    str[letterCount+1] = '\0';
                }
                DrawFont3(str, r, COLOR_BEINGEDITEDTEXT, TextLeft, ((Vector2){0, 0}));
                framesCounter++;
                // NEXT
                if ((header->name)[0] != '\0')
                {
                    Rectangle r_button = {screenWidth - 4 * UNIT - 4 * Font3HUnit, 2 * UNIT, UNITCLAMP(4 * Font3HUnit), TITLEHEIGHT - 4 * UNIT};
                    if (CheckCollisionPointRec(mouse, r_button))
                    {
                        if (currentGesture == GESTURE_TAP)
                        {
                            DrawRectangleRounded(r_button, 0.2f, 10, COLOR_CLICKED);
                            framesCounter = 0;
                            header->phse = PhSetupRules;
                            save(rordata, rordataLength);
                        }
                        else
                        {
                            DrawRectangleRounded(r_button, 0.2f, 10, COLOR_MOUSEHOVER_CLICKABLE);
                        }
                    }
                    else DrawRectangleRounded(r_button, 0.2f, 10, COLOR_BUTTONBACKGROUND);
                    DrawRectangleRoundedLines(r_button, 0.2f, 10, 2, COLOR_BUTTONOUTLINE);
                    DrawFont3("NEXT", r_button, COLOR_BUTTONTEXT, TextCenter, ((Vector2){0, 1}));
                }
            } break;

            case PhSetupRules:
            {
                // TITLE
                Rectangle r_title = {0, 0, screenWidth, TITLEHEIGHT};
                DrawRectangleRec(r_title, COLOR_TITLEBACKGROUND);
                DrawTitle("RULES", r_title, COLOR_TITLETEXT, TextLeft);
                // ERA
                uint8_t ecnt = ITEMCOUNT(rordata, Era);
                uint8_t *pe = (uint8_t*)(p_TEMP(rordata)+0);
                if (*pe < 1 || ecnt < *pe) *pe = 1; // ensure range
                Rectangle r_subtitle ={r_title.x, r_title.y + r_title.height + PAD, r_title.width, Font3RectH};
                if (*pe == 1) sprintf(str, "%s (first era)", TextToUpper(ERA(*pe).name));
                else sprintf(str, "%s (after %s)", TextToUpper((ERA(*pe).name)), ERA(*pe-1).name); // Subsequent TextToUpper(); results should be cached first.
                if (CheckCollisionPointRec(mouse, r_subtitle))
                {
                    if (currentGesture == GESTURE_TAP)
                    {
                        *pe += 1;
                        selected = -1;
                    }
                    else
                    {
                        DrawRectangleRec(r_subtitle, COLOR_MOUSEHOVER_CLICKABLE);
                        DrawFont3(str, r_subtitle, COLOR_MOUSEHOVER_CLICKABLETEXT, TextCenter, ((Vector2){0, 0}));
                    }
                }
                else
                {
                    DrawRectangleRec(r_subtitle, COLOR_SUBTITLEBACKGROUND);
                    DrawFont3(str, r_subtitle, COLOR_SUBTITLETEXT, TextCenter, ((Vector2){0, 0}));
                }
                // RULES
                int current = 1;
                int clicked = -1;
                uint8_t maxLetterCount;
                Rectangle r;
                Rectangle r_toggle;
                Rectangle r_text;
                Rectangle r_section = {2 * UNIT, r_subtitle.y + (r_subtitle.height + PAD) + 3 * UNIT, UNITCLAMP(screenWidth/2 - 2 * UNIT), Font3RectH};
                DrawFont3("ERA START TRIGGERS (Era Ends card can force it at all times)", r_section, COLOR_BACKGOUNDSECTIONTEXT, TextLeft, ((Vector2){0, 0}));
                r.x = r_section.x + Font3PaddingX - Font2PaddingX;
                r.width = r_section.width - (Font3PaddingX - Font2PaddingX);
                r.height = Font2RectH;
                r.y = r_section.y + r_section.height + PAD + 2 * UNIT;
                r_text = DrawFont2("AT TURN ", r, COLOR_BACKGOUNDTEXT, TextLeft, ((Vector2){0, 0}));
                r.x = r_text.x + r_text.width + Font2Spacing - Font2PaddingX;
                r.width = 4 * font2E0.x + 3 * Font2Spacing + 2 * Font2PaddingX + 2; // HACKY: + 2
                sprintf(str, "%d", (ERA(*pe).trig & 0x0FFF));
                if (CheckCollisionPointRec(mouse, r) && current != selected)
                {
                    if (currentGesture == GESTURE_TAP)
                    {
                        clicked = current;
                        selected = current;
                        framesCounter = 0;
                        DrawRectangleRec(r, COLOR_CLICKED);
                        DrawFont2(str, r, COLOR_CLICKEDTEXT, TextCenter, ((Vector2){1, 0}));
                    }
                    else DrawRectangleRec(r, COLOR_MOUSEHOVER_EDITABLE);
                    DrawFont2(str, r, COLOR_MOUSEHOVER_EDITABLETEXT, TextCenter, ((Vector2){1, 0}));
                }
                else if (current == selected)
                {
                    int letterCount = 1;
                    DrawRectangleRec(r, COLOR_BEINGEDITED);
                    DrawFont2(str, r, COLOR_BEINGEDITEDTEXT, TextCenter, ((Vector2){1, 0}));
                    int key = GetCharPressed();
                    while (key > 0)
                    {
                        if (('0' <= key) && (key <= '9'))
                        {
                            ERA(*pe).trig = min(0x0FFF, ERA(*pe).trig * 10 + (key - '0'));
                            save(rordata, rordataLength);
                        }
                        key = GetCharPressed();  // Check next character in the queue
                    }
                    if (IsKeyPressed(KEY_BACKSPACE))
                    {
                        ERA(*pe).trig = ERA(*pe).trig / 10;
                        save(rordata, rordataLength);
                    }
                }
                else
                {
                    DrawRectangleRec(r, COLOR_BACKGROUNDAREA);
                    DrawFont2(str, r, COLOR_EDITABLETEXT, TextCenter, ((Vector2){1, 0}));
                }
                current += 1;
                r.x += r.width + font2E_.x + 2 * Font2Spacing - Font2PaddingX;
                r.width = MeasureTextEx(font2, "AND", Font2H, Font2Spacing).x + 2 * Font2PaddingX - 1; // HACKY: - 1
                if ((ERA(*pe).trig & 0x1000) == 0) strcpy(str, "OR");
                else strcpy(str, "AND");
                if (CheckCollisionPointRec(mouse, r))
                {
                    if (currentGesture == GESTURE_TAP)
                    {
                        ERA(*pe).trig = BIT_FLIP(ERA(*pe).trig, 12);
                        save(rordata, rordataLength);
                        selected = -1;
                        DrawRectangleRec(r, COLOR_CLICKED);
                        DrawFont2(str, r, COLOR_CLICKEDTEXT, TextCenter, ((Vector2){1, 0}));
                    }
                    else DrawRectangleRec(r, COLOR_MOUSEHOVER_CLICKABLE);
                    DrawFont2(str, r, COLOR_MOUSEHOVER_CLICKABLETEXT, TextCenter, ((Vector2){1, 0}));
                }
                else
                {
                    DrawRectangleRec(r, COLOR_BACKGROUNDAREA);
                    DrawFont2(str, r, COLOR_CLICKABLETEXT, TextCenter, ((Vector2){1, 0}));
                }
                current += 1;
                r.x += r.width + font2E_.x + 2 * Font2Spacing - Font2PaddingX;
                r.width = MeasureTextEx(font2, "CARD WITH THIS ERA IS DRAWN FROM DECK WITH THIS ERA", Font2H, Font2Spacing).x + 2 * Font2PaddingX - 1; // HACKY: - 1
                switch (ERA(*pe).trig >> 13)
                {
                    case 0: {strcpy(str, "NEVER");} break;
                    case 1: {strcpy(str, "DECK WITH THIS ERA COMES NEXT");} break;
                    case 5: {strcpy(str, "CARD IS DRAWN FROM DECK WITH THIS ERA");} break;
                    case 6: {strcpy(str, "CARD WITH THIS ERA IS DRAWN");} break;
                    case 7: {strcpy(str, "CARD WITH THIS ERA IS DRAWN FROM DECK WITH THIS ERA");} break;
                }
                if (CheckCollisionPointRec(mouse, r))
                {
                    if (currentGesture == GESTURE_TAP)
                    {
                        switch (ERA(*pe).trig >> 13)
                        {
                            case 0: {ERA(*pe).trig = (ERA(*pe).trig & 0x1FFF) + (1 << 13);} break;
                            case 1: {ERA(*pe).trig = (ERA(*pe).trig & 0x1FFF) + (5 << 13);} break;
                            case 5: {ERA(*pe).trig = (ERA(*pe).trig & 0x1FFF) + (6 << 13);} break;
                            case 6: {ERA(*pe).trig = (ERA(*pe).trig & 0x1FFF) + (7 << 13);} break;
                            case 7: {ERA(*pe).trig = (ERA(*pe).trig & 0x1FFF) + (0 << 13);} break;
                        }
                        save(rordata, rordataLength);
                        selected = -1;
                        DrawRectangleRec(r, COLOR_CLICKED);
                        DrawFont2(str, r, COLOR_CLICKEDTEXT, TextLeft, ((Vector2){0, 0}));
                    }
                    else DrawRectangleRec(r, COLOR_MOUSEHOVER_CLICKABLE);
                    DrawFont2(str, r, COLOR_MOUSEHOVER_CLICKABLETEXT, TextLeft, ((Vector2){0, 0}));
                }
                else
                {
                    DrawRectangleRec(r, COLOR_BACKGROUNDAREA);
                    DrawFont2(str, r, COLOR_CLICKABLETEXT, TextLeft, ((Vector2){0, 0}));
                }
                current += 1;
                r_section.y = r.y + (r.height + PAD) + 6 * UNIT;



                DrawFont3("ERA START DRAW OF FAMILY CARDS", r_section, COLOR_BACKGOUNDSECTIONTEXT, TextLeft, ((Vector2){0, 0}));
                r.x = r_section.x + Font3PaddingX - Font2PaddingX;
                r.width = r_section.width - (Font3PaddingX - Font2PaddingX);
                r.height = Font2RectH;
                r.y = r_section.y + r_section.height + PAD + 2 * UNIT;
                r_text = DrawFont2("FACTION MINIMUM NUMBER OF SENATORS: ", r, COLOR_BACKGOUNDTEXT, TextLeft, ((Vector2){0, 0}));
                r.x = r_text.x + r_text.width + Font2Spacing - Font2PaddingX;
                r.width = font2E0.x + 2 * Font2PaddingX;
                sprintf(str, "%d", ERA(*pe).nsen);
                if (CheckCollisionPointRec(mouse, r) && current != selected)
                {
                    if (currentGesture == GESTURE_TAP)
                    {
                        clicked = current;
                        selected = current;
                        framesCounter = 0;
                        DrawRectangleRec(r, COLOR_CLICKED);
                        DrawFont2(str, r, COLOR_CLICKEDTEXT, TextCenter, ((Vector2){1, 0}));
                    }
                    else DrawRectangleRec(r, COLOR_MOUSEHOVER_EDITABLE);
                    DrawFont2(str, r, COLOR_MOUSEHOVER_EDITABLETEXT, TextCenter, ((Vector2){1, 0}));
                }
                else if (current == selected)
                {
                    int letterCount = 1;
                    DrawRectangleRec(r, COLOR_BEINGEDITED);
                    DrawFont2(str, r, COLOR_BEINGEDITEDTEXT, TextCenter, ((Vector2){1, 0}));
                    int key = GetCharPressed();
                    while (key > 0)
                    {
                        if (('0' <= key) && (key <= '9'))
                        {
                            ERA(*pe).nsen = key - '0';
                            save(rordata, rordataLength);
                        }
                        key = GetCharPressed();  // Check next character in the queue
                    }
                }
                else
                {
                    DrawRectangleRec(r, COLOR_BACKGROUNDAREA);
                    DrawFont2(str, r, COLOR_EDITABLETEXT, TextCenter, ((Vector2){1, 0}));
                }
                current += 1;
                r_section.y = r.y + (r.height + PAD) + 6 * UNIT;

                DrawFont3("ERA START (TEMPORARY) ROME CONSUL", r_section, COLOR_BACKGOUNDSECTIONTEXT, TextLeft, ((Vector2){0, 0}));
                //A_ERAR_TERC_t* p = (A_ERAR_TERC_t*)(valabsaddr(rordata, G_ERAR, A_ERAR_TERC, game->erai));
                r_toggle.x = r_section.x + 7;
                r.x = r_toggle.x + Font2RectH;
                r.width = r_section.width - Font2RectH - 7;
                r.height = Font2RectH;
                r.y = r_section.y + r_section.height + PAD + 2 * UNIT;
                r_toggle.y = r.y + 1 * UNIT;
                r_toggle.height = Font2RectH - 2 * UNIT - PAD;
                r_toggle.width = r_toggle.height;

                if (currentGesture == GESTURE_TAP && CheckCollisionPointRec(mouse, r_toggle)) BIT_FLIP(ERA(*pe).terc,0);
                if ((ERA(*pe).terc & 0x01) == 0x00) DrawRectangleRec(r_toggle, COLOR_TOGGLESET);
                else DrawRectangleRec(r_toggle, COLOR_TOGGLEUNSET);
                DrawRectangleLinesEx(r_toggle, 1.0f, COLOR_TOGGLEOUTLINE);
                DrawFont2("NO (TEMPORARY) ROME CONSUL", r, COLOR_BACKGOUNDTEXT, TextLeft, ((Vector2){0, 0}));

                r.y += Font2RectH + PAD;
                r_toggle.y += Font2RectH + PAD;

                if ((ERA(*pe).terc & 0x01) != 0x00){
                    if (currentGesture == GESTURE_TAP && CheckCollisionPointRec(mouse, r_toggle))
                    {
                        BIT_FLIP(ERA(*pe).terc,1);
                        save(rordata, rordataLength);
                    }
                    if ((ERA(*pe).terc & 0x02) == 0x00) DrawRectangleRec(r_toggle, COLOR_TOGGLESET);
                    else DrawRectangleRec(r_toggle, COLOR_TOGGLEUNSET);
                    DrawRectangleLinesEx(r_toggle, 1.0f, COLOR_TOGGLEOUTLINE);
                }
                DrawFont2("RANDOM DRAW (VG)", r, COLOR_BACKGOUNDTEXT, TextLeft, ((Vector2){0, 0}));

                r.y += Font2RectH + PAD;
                r_toggle.y += Font2RectH + PAD;

                if ((ERA(*pe).terc & 0x01) != 0x00){
                    if (currentGesture == GESTURE_TAP && CheckCollisionPointRec(mouse, r_toggle))
                    {
                        BIT_FLIP(ERA(*pe).terc,1);
                        save(rordata, rordataLength);
                    }
                    if ((ERA(*pe).terc & 0x02) == 0x02) DrawRectangleRec(r_toggle, COLOR_TOGGLESET);
                    else DrawRectangleRec(r_toggle, COLOR_TOGGLEUNSET);
                    DrawRectangleLinesEx(r_toggle, 1.0f, COLOR_TOGGLEOUTLINE);
                }
                DrawFont2("LOWEST ID (AH)", r, COLOR_BACKGOUNDTEXT, TextLeft, ((Vector2){0, 0}));

                r.y += Font2RectH + PAD;
                r_toggle.y += Font2RectH + PAD;

                if ((ERA(*pe).terc & 0x01) != 0x00){
                    if (currentGesture == GESTURE_TAP && CheckCollisionPointRec(mouse, r_toggle))
                    {
                        BIT_FLIP(ERA(*pe).terc,2);
                        save(rordata, rordataLength);
                    }
                    if ((ERA(*pe).terc & 0x04) == 0x00) DrawRectangleRec(r_toggle, COLOR_TOGGLESET);
                    else DrawRectangleRec(r_toggle, COLOR_TOGGLEUNSET);
                    DrawRectangleLinesEx(r_toggle, 1.0f, COLOR_TOGGLEOUTLINE);
                }
                DrawFont2("BEFORE FACTION LEADERS (VG)", r, COLOR_BACKGOUNDTEXT, TextLeft, ((Vector2){0, 0}));

                r.y += Font2RectH + PAD;
                r_toggle.y += Font2RectH + PAD;

                if ((ERA(*pe).terc & 0x01) != 0x00){
                    if (currentGesture == GESTURE_TAP && CheckCollisionPointRec(mouse, r_toggle))
                    {
                        BIT_FLIP(ERA(*pe).terc,2);
                        save(rordata, rordataLength);
                    }
                    if ((ERA(*pe).terc & 0x04) == 0x04) DrawRectangleRec(r_toggle, COLOR_TOGGLESET);
                    else DrawRectangleRec(r_toggle, COLOR_TOGGLEUNSET);
                    DrawRectangleLinesEx(r_toggle, 1.0f, COLOR_TOGGLEOUTLINE);
                }
                DrawFont2("AFTER FACTION LEADERS (AH)", r, COLOR_BACKGOUNDTEXT, TextLeft, ((Vector2){0, 0}));

                r.y += Font2RectH + PAD;
                r_toggle.y += Font2RectH + PAD;

                if ((ERA(*pe).terc & 0x01) != 0x00){
                    if (currentGesture == GESTURE_TAP && CheckCollisionPointRec(mouse, r_toggle))
                    {
                        BIT_FLIP(ERA(*pe).terc,3);
                        save(rordata, rordataLength);
                    }
                    if ((ERA(*pe).terc & 0x08) == 0x08) DrawRectangleRec(r_toggle, COLOR_TOGGLESET);
                    else DrawRectangleRec(r_toggle, COLOR_TOGGLEUNSET);
                    DrawRectangleLinesEx(r_toggle, 1.0f, COLOR_TOGGLEOUTLINE);
                }
                DrawFont2("REPEAT IF DIES IN FIRST MORTALITY PHASE (AH-LRB)", r, COLOR_BACKGOUNDTEXT, TextLeft, ((Vector2){0, 0}));
                if (clicked == -1 && currentGesture == GESTURE_TAP)
                {
                    selected = -1;
                    framesCounter = 0;
                }
                // NEXT BUTTON
                Rectangle r_button = {screenWidth - 4 * UNIT - 4 * Font3HUnit, 2 * UNIT, UNITCLAMP(4 * Font3HUnit), TITLEHEIGHT - 4 * UNIT};
                if (CheckCollisionPointRec(mouse, r_button))
                {
                    if (currentGesture == GESTURE_TAP)
                    {
                        DrawRectangleRounded(r_button, 0.2f, 10, COLOR_CLICKED);
                        header->phse = PhDealSenators;
                        memset(p_TEMP(rordata), 0, TEMPSIZE);
                        save(rordata, rordataLength);
                        selected = -1;
                        framesCounter = 0;
                    }
                    else DrawRectangleRounded(r_button, 0.2f, 10, COLOR_MOUSEHOVER_CLICKABLE);
                }
                else DrawRectangleRounded(r_button, 0.2f, 10, COLOR_BUTTONBACKGROUND);
                DrawRectangleRoundedLines(r_button, 0.2f, 10, 2, COLOR_BUTTONOUTLINE);
                DrawFont3("NEXT", r_button, COLOR_BUTTONTEXT, TextCenter, ((Vector2){0, 1}));
            } break;

            case PhDealSenators:
            {
                // PREP
                uint8_t *stage = (uint8_t*)(p_TEMP(rordata)+0);
                uint8_t *change = (uint8_t*)(p_TEMP(rordata)+1);
                *change = 0;
                uint16_t dcnt = ITEMCOUNT(rordata, Deck);
                uint16_t d;
                for (d = dcnt; d <= 1 && DECK(d).eran != HEADER.eran || DECK(d).type != EraStartSenatorPool; d--);
                uint8_t *sat = MemAlloc(ITEMCOUNT(rordata, Senator));
                memset(sat, 0, ITEMCOUNT(rordata, Senator) + 1);  // zero
                uint8_t *factsenacnt = MemAlloc(ITEMCOUNT(rordata, Faction) + 1);
                memset(factsenacnt, 0, ITEMCOUNT(rordata, Faction) + 1);  // zero
                uint8_t decksenacnt = 0;
                for (uint8_t s = 1; s <= ITEMCOUNT(rordata, Senator); s++)
                {
                    if (SENATOR(s).loit == DeckItem && DECK(SENATOR(s).lonr).eran == HEADER.eran && DECK(SENATOR(s).lonr).type == EraStartSenatorPool)
                    {
                        sat[s-1] = (1 << 7) + (SENATOR(s).lonr & 0x7F);
                        decksenacnt++;
                    }
                    else if (SENATOR(s).loit == LocationItem && SENATOR(s).lonr == Forum)
                    {
                        sat[s-1] = SENATOR(s).fact & 0x3F;
                        factsenacnt[sat[s-1]]++;
                        sat[s-1] += 1 << 6;
                    }
                    else sat[s-1] = 0;
                }
                uint8_t dealstatus = 0;
                uint8_t minfactsenacnt = 0xFF;
                for (uint8_t f = 1; (f <= ITEMCOUNT(rordata, Faction)) && ((FACTION(f).type & FactionUsed) != 0); f++)
                {
                    uint8_t fdealstatus = 0;
                    if (factsenacnt[f] == 0) fdealstatus |= 0x01;
                    if (factsenacnt[f] < ERA(HEADER.eran).nsen + FACTION(f).xsen)
                    {
                        fdealstatus |= 0x02;
                        minfactsenacnt = min(minfactsenacnt, factsenacnt[f]);
                    }
                    else if (factsenacnt[f] == ERA(HEADER.eran).nsen + FACTION(f).xsen) fdealstatus |= 0x04;
                    if (factsenacnt[f] < ERA(HEADER.eran).nsen) fdealstatus |= 0x08;
                    else if (factsenacnt[f] == ERA(HEADER.eran).nsen) fdealstatus |= 0x10;
                    dealstatus |= fdealstatus;
                    if (fdealstatus == 0) dealstatus |= 0x80;
                }
                for (uint8_t f = 1; (f <= ITEMCOUNT(rordata, Faction)); f++)
                {
                    // memset((&FACTION(f))->tmil, 0, 10); // clear M O L I P, all 16 bit (u)integers //TODO: FOR SOME REASON IT CRASHES
                    FACTION(f).tmil = 0;
                    FACTION(f).tora = 0;
                    FACTION(f).tloy = 0;
                    FACTION(f).tinf = 0;
                    FACTION(f).tpop = 0;
                }
                for (uint8_t s = 1; s <= ITEMCOUNT(rordata, Senator); s++)
                {
                    uint8_t f = SENATOR(s).fact;
                    FACTION(f).tmil += SENATOR(s).mil1;
                    FACTION(f).tora += SENATOR(s).ora1;
                    FACTION(f).tloy += SENATOR(s).loy1;
                    FACTION(f).tinf += SENATOR(s).inf1;
                    FACTION(f).tpop += SENATOR(s).pop1;
                }
                uint8_t targetf = 0;
                if (0 < (dealstatus & 0x03)) for (targetf = 1; (targetf <= ITEMCOUNT(rordata, Faction)) && ((FACTION(targetf).type & FactionUsed) != 0) && ((factsenacnt[targetf] != minfactsenacnt) || ((factsenacnt[targetf] >= ERA(HEADER.eran).nsen + FACTION(targetf).xsen))); targetf++);
                // TITLE
                Rectangle r_title = {0, 0, screenWidth, TITLEHEIGHT};
                DrawRectangleRec(r_title, COLOR_TITLEBACKGROUND);
                if (*stage == 0) sprintf(str, "DEAL %i SENATORS TO EACH FACTION", ERA(HEADER.eran).nsen);
                else strcpy(str, "DEAL THE EXTRA SENATORS");
                DrawTitle(str, r_title, COLOR_TITLETEXT, TextLeft);
                // SENATORS AND FACTIONS PREP
                Vector2 selectedvector;
                Rectangle r_header = {0, 0, RECT_SEN_WIDTH, Font2RectH};
                Rectangle r_senator = {0, 0, r_header.width, Font2RectH};
                // SENATORS
                r_header.x = UNITCLAMP(screenWidth / 2) - (r_header.width + PAD) - 2 * UNIT;
                r_header.y = (r_title.height + PAD) + 3 * UNIT;
                Rectangle r_seantorarea = {r_header.x, r_header.y, r_header.width, screenHeight - r_header.y};
                if (0 <= selected && CheckCollisionPointRec(mouse, r_seantorarea))
                {
                    DrawRectangleRec(r_header, COLOR_MOUSEHOVER_DROPTARGET);
                    if (currentGesture != lastGesture && currentGesture == GESTURE_NONE)
                    {
                        uint8_t s = selected; // for code coherence
                        uint8_t f0 = SENATOR(s).fact;
                        if (f0 != 0)
                        {
                            FACTION(f0).tmil -= SENATOR(s).mil1;
                            FACTION(f0).tora -= SENATOR(s).ora1;
                            FACTION(f0).tloy -= SENATOR(s).loy1;
                            FACTION(f0).tinf -= SENATOR(s).inf1;
                            FACTION(f0).tpop -= SENATOR(s).pop1;
                            factsenacnt[f0]--;
                        }
                        SENATOR(s).fact = 0;
                        SENATOR(s).loit = DeckItem;
                        SENATOR(s).lonr = d;
                        sat[s-1] = (1 << 7) + (SENATOR(s).lonr & 0x7F);
                        decksenacnt++;
                        *change = 1;
                        save(rordata, rordataLength);
                        selected = -1;
                    }
                }
                else DrawRectangleRec(r_header, COLOR_FACTIONHEADER);
                DrawFont2("FAMILIES", r_header, COLOR_FACTIONHEADERTEXT, TextLeft, ((Vector2){0, 0}));
                DrawFont2("M", ((Rectangle){r_header.x + RECT_SEN_M_X, r_header.y, RECT_SEN_M_WIDTH, r_header.height}), COLOR_FACTIONHEADERTEXT, TextCenter, ((Vector2){0, 0}));
                DrawFont2("O", ((Rectangle){r_header.x + RECT_SEN_O_X, r_header.y, RECT_SEN_O_WIDTH, r_header.height}), COLOR_FACTIONHEADERTEXT, TextCenter, ((Vector2){0, 0}));
                DrawFont2("L", ((Rectangle){r_header.x + RECT_SEN_L_X, r_header.y, RECT_SEN_L_WIDTH, r_header.height}), COLOR_FACTIONHEADERTEXT, TextCenter, ((Vector2){0, 0}));
                DrawFont2("I", ((Rectangle){r_header.x + RECT_SEN_I_X, r_header.y, RECT_SEN_I_WIDTH, r_header.height}), COLOR_FACTIONHEADERTEXT, TextCenter, ((Vector2){0, 0}));
                DrawFont2("P", ((Rectangle){r_header.x + RECT_SEN_P_X, r_header.y, RECT_SEN_P_WIDTH, r_header.height}), COLOR_FACTIONHEADERTEXT, TextCenter, ((Vector2){0, 0}));
                r_senator.x = r_header.x;
                r_senator.y = r_header.y + 1 * (Font2RectH + PAD) + 2 * UNIT;
                for(uint8_t s = 1; s <= ITEMCOUNT(rordata, Senator); s++)
                {
                    if (sat[s-1] < (1 << 7)) continue;
                    if (s == selected) continue;
                    if (selected == -1 && CheckCollisionPointRec(mouse, r_senator))
                    {
                        if (currentGesture != lastGesture && currentGesture == GESTURE_TAP) // pick up
                        {
                            selected = s;
                            selectedvector.x = mouse.x - r_senator.x;
                            selectedvector.y = mouse.y - r_senator.y;
                        }
                        else DrawRectangleRec(r_senator, COLOR_MOUSEHOVER_DRAGABLE);
                    }
                    else if (selected != s) DrawRectangleRec(r_senator, COLOR_BLACKCARDBACKGROUND);
                    sprintf(str, "%d", SENATOR(s).idnr);
                    DrawFont1(str, ((Rectangle){r_senator.x + RECT_SEN_ID_X, r_senator.y + Font2RectH - Font1RectH, RECT_SEN_ID_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextRight, ((Vector2){1, -8}));
                    DrawFont2(SENATOR(s).name, ((Rectangle){r_senator.x + RECT_SEN_NAME_X, r_senator.y, RECT_SEN_NAME_WIDTH, Font2RectH}), COLOR_BLACKCARDTEXT, TextLeft, ((Vector2){0, 0}));
                    sprintf(str, "%d", SENATOR(s).mil0);
                    DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_M_X, r_senator.y, RECT_SEN_M_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                    sprintf(str, "%d", SENATOR(s).ora0);
                    DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_O_X, r_senator.y, RECT_SEN_O_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                    sprintf(str, "%d", SENATOR(s).loy0);
                    DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_L_X, r_senator.y, RECT_SEN_L_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                    sprintf(str, "%d", SENATOR(s).inf0);
                    DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_I_X, r_senator.y, RECT_SEN_I_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                    if (SENATOR(s).pop0 != 0) {sprintf(str, "%i", SENATOR(s).pop0); DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_P_X, r_senator.y, RECT_SEN_P_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));}
                    r_senator.y += (Font2RectH + PAD);
                }
                // FACTIONS
                r_header.x = UNITCLAMP(screenWidth / 2) + 2 * UNIT;
                DrawRectangleRec(r_header, COLOR_FACTIONHEADER);
                DrawFont2("SENATE", r_header, COLOR_FACTIONHEADERTEXT, TextLeft, ((Vector2){0, 0}));
                DrawFont2("M", ((Rectangle){r_header.x + RECT_SEN_M_X, r_header.y, RECT_SEN_M_WIDTH, r_header.height}), COLOR_FACTIONHEADERTEXT, TextCenter, ((Vector2){0, 0}));
                DrawFont2("O", ((Rectangle){r_header.x + RECT_SEN_O_X, r_header.y, RECT_SEN_O_WIDTH, r_header.height}), COLOR_FACTIONHEADERTEXT, TextCenter, ((Vector2){0, 0}));
                DrawFont2("L", ((Rectangle){r_header.x + RECT_SEN_L_X, r_header.y, RECT_SEN_L_WIDTH, r_header.height}), COLOR_FACTIONHEADERTEXT, TextCenter, ((Vector2){0, 0}));
                DrawFont2("I", ((Rectangle){r_header.x + RECT_SEN_I_X, r_header.y, RECT_SEN_I_WIDTH, r_header.height}), COLOR_FACTIONHEADERTEXT, TextCenter, ((Vector2){0, 0}));
                DrawFont2("P", ((Rectangle){r_header.x + RECT_SEN_P_X, r_header.y, RECT_SEN_P_WIDTH, r_header.height}), COLOR_FACTIONHEADERTEXT, TextCenter, ((Vector2){0, 0}));
                r_senator.x = r_header.x;
                r_senator.y = r_header.y + 1 * (Font2RectH + PAD) + 2 * UNIT;
                for (uint8_t f = 1; f <= ITEMCOUNT(rordata, Faction); f++)
                {
                    if ((FACTION(f).type & FactionUsed) == 0) continue;
                    Rectangle r_faction = {r_senator.x, r_senator.y, r_senator.width, r_senator.height + (Font2RectH + PAD) * max(ERA(HEADER.eran).nsen + ((*stage == 0) ? 0 : FACTION(f).xsen), factsenacnt[f])};
                    DrawRectangleRec(r_faction, COLOR_BACKGROUNDAREA);
                    if (0 <= selected && CheckCollisionPointRec(mouse, r_faction))
                    {
                        DrawRectangleRec(r_senator, COLOR_MOUSEHOVER_DROPTARGET);
                        if ((currentGesture == GESTURE_NONE || currentGesture == GESTURE_SWIPE_RIGHT || currentGesture == GESTURE_SWIPE_LEFT || currentGesture == GESTURE_SWIPE_UP || currentGesture == GESTURE_SWIPE_DOWN) && (lastGesture == GESTURE_HOLD || lastGesture == GESTURE_DRAG || lastGesture == GESTURE_TAP))
                        {
                            uint8_t s = selected; // for code coherence
                            uint8_t f0 = SENATOR(s).fact;
                            if (f0 != 0)
                            {
                                FACTION(f0).tmil -= SENATOR(s).mil1;
                                FACTION(f0).tora -= SENATOR(s).ora1;
                                FACTION(f0).tloy -= SENATOR(s).loy1;
                                FACTION(f0).tinf -= SENATOR(s).inf1;
                                FACTION(f0).tpop -= SENATOR(s).pop1;
                                factsenacnt[f0]--;
                            }
                            else if (SENATOR(s).loit == DeckItem)
                            {
                                decksenacnt--;
                            }
                            SENATOR(s).fact = f;
                            SENATOR(s).loit = LocationItem;
                            SENATOR(s).lonr = Forum;
                            FACTION(f).tmil += SENATOR(s).mil1;
                            FACTION(f).tora += SENATOR(s).ora1;
                            FACTION(f).tloy += SENATOR(s).loy1;
                            FACTION(f).tinf += SENATOR(s).inf1;
                            FACTION(f).tpop += SENATOR(s).pop1;
                            sat[s-1] = (1 << 6) + (f & 0x3F);
                            factsenacnt[f]++;
                            *change = 1;
                            save(rordata, rordataLength);
                            selected = -1;
                        }
                    }
                    else DrawRectangleRec(r_senator, COLOR_FACTION);
                    DrawFont2(TextToUpper(FACTION(f).name), r_senator, COLOR_FACTIONTEXT, TextLeft, ((Vector2){0, 0}));
                    sprintf(str, "%d", FACTION(f).tmil);
                    DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_M_X, r_senator.y, RECT_SEN_M_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                    sprintf(str, "%d", FACTION(f).tora);
                    DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_O_X, r_senator.y, RECT_SEN_O_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                    sprintf(str, "%d", FACTION(f).tloy);
                    DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_L_X, r_senator.y, RECT_SEN_L_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                    sprintf(str, "%d", FACTION(f).tinf);
                    DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_I_X, r_senator.y, RECT_SEN_I_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                    if (FACTION(f).tpop != 0) {sprintf(str, "%i", FACTION(f).tpop); DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_P_X, r_senator.y, RECT_SEN_P_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));}
                    for(uint8_t s = 1; s <= ITEMCOUNT(rordata, Senator); s++)
                    {
                        if (s == selected) continue;  // handled elsewhere
                        if ((sat[s-1] & 0x3F) != f) continue;  // not in this faction
                        r_senator.y += (Font2RectH + PAD);
                        if (selected == -1 && CheckCollisionPointRec(mouse, r_senator))
                        {
                            if (currentGesture != lastGesture && currentGesture == GESTURE_TAP) // pick up
                            {
                                selected = s;
                                selectedvector.x = mouse.x - r_senator.x;
                                selectedvector.y = mouse.y - r_senator.y;
                            }
                            else DrawRectangleRec(r_senator, COLOR_MOUSEHOVER_DRAGABLE);
                        }
                        else DrawRectangleRec(r_senator, COLOR_BLACKCARDBACKGROUND);
                        sprintf(str, "%d", SENATOR(s).idnr);
                        DrawFont1(str, ((Rectangle){r_senator.x + RECT_SEN_ID_X, r_senator.y + Font2RectH - Font1RectH, RECT_SEN_ID_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextRight, ((Vector2){1, -8}));
                        DrawFont2(SENATOR(s).name, ((Rectangle){r_senator.x + RECT_SEN_NAME_X, r_senator.y, RECT_SEN_NAME_WIDTH, Font2RectH}), COLOR_BLACKCARDTEXT, TextLeft, ((Vector2){0, 0}));
                        sprintf(str, "%d", SENATOR(s).mil0);
                        DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_M_X, r_senator.y, RECT_SEN_M_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                        sprintf(str, "%d", SENATOR(s).ora0);
                        DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_O_X, r_senator.y, RECT_SEN_O_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                        sprintf(str, "%d", SENATOR(s).loy0);
                        DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_L_X, r_senator.y, RECT_SEN_L_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                        sprintf(str, "%d", SENATOR(s).inf0);
                        DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_I_X, r_senator.y, RECT_SEN_I_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                        if (SENATOR(s).pop0 != 0) {sprintf(str, "%i", SENATOR(s).pop0); DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_P_X, r_senator.y, RECT_SEN_P_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));}
                    }
                    r_senator.y = r_faction.y + r_faction.height + 2 * UNIT + PAD;
                }
                // NEXT BUTTON
                Rectangle r_button = {screenWidth - 4 * UNIT - 4 * Font3HUnit, 2 * UNIT, UNITCLAMP(4 * Font3HUnit), TITLEHEIGHT - 4 * UNIT};
                if (selected == -1 && CheckCollisionPointRec(mouse, r_button))
                {
                    if (currentGesture == GESTURE_TAP)
                    {
                        DrawRectangleRounded(r_button, 0.2f, 10, COLOR_CLICKED);
                        if ((ERA(HEADER.eran).terc & 0x05) == 0x01) header->phse = PhTemporaryRomeConsul;
                        else header->phse = PhSelectFactionLeaders;
                        memset(p_TEMP(rordata), 0, TEMPSIZE);
                        save(rordata, rordataLength);
                        MemFree(sat);
                        MemFree(factsenacnt);
                        selected = -1;
                        framesCounter = 0;
                    }
                    else DrawRectangleRounded(r_button, 0.2f, 10, COLOR_MOUSEHOVER_CLICKABLE);
                }
                else DrawRectangleRounded(r_button, 0.2f, 10, COLOR_BUTTONBACKGROUND);
                DrawRectangleRoundedLines(r_button, 0.2f, 10, 2, COLOR_BUTTONOUTLINE);
                DrawFont3("NEXT", r_button, COLOR_BUTTONTEXT, TextCenter, ((Vector2){0, 1}));
                // RANDOM BUTTON
                if (0 < decksenacnt && 0 < (dealstatus & 0x0B))
                {
                    r_button.width = UNITCLAMP(6 * Font3HUnit);
                    r_button.x -= 2 * UNIT + r_button.width;
                    if (selected == -501000 && ((currentGesture == GESTURE_NONE || currentGesture == GESTURE_SWIPE_RIGHT || currentGesture == GESTURE_SWIPE_LEFT || currentGesture == GESTURE_SWIPE_UP || currentGesture == GESTURE_SWIPE_DOWN) && (lastGesture == GESTURE_HOLD || lastGesture == GESTURE_DRAG || lastGesture == GESTURE_TAP))) selected = -1;
                    if (selected == -1 && CheckCollisionPointRec(mouse, r_button))
                    {
                        if (currentGesture != lastGesture && currentGesture == GESTURE_TAP) // pick up
                        {
                            selected = -501000;
                            selectedvector.x = mouse.x - r_button.x;
                            selectedvector.y = mouse.y - r_button.y;
                        }
                        else
                        {
                            DrawRectangleRounded(r_button, 0.2f, 10, COLOR_MOUSEHOVER_DRAGABLE);
                            DrawRectangleRoundedLines(r_button, 0.2f, 10, 2, COLOR_BUTTONOUTLINE);
                            DrawFont3("RANDOM", r_button, COLOR_BUTTONTEXT, TextCenter, ((Vector2){0, 1}));
                            strcpy(str, "drag this for random draw");
                            Vector2 textsize = MeasureTextEx(font2, str, Font2H, Font2Spacing);
                            textsize.x += 4 * Font2Spacing;
                            Rectangle r_tooltip = {UNITCLAMP(r_button.x + r_button.width / 2 - textsize.x / 2), r_button.y + r_button.height + 1 * UNIT, UNITCLAMP(textsize.x), Font2RectH};
                            DrawRectangleRec(r_tooltip, COLOR_TOOLTIPBACKGROUND);
                            DrawRectangleLinesEx(r_tooltip, 1.0f, COLOR_TOOLTIPOUTLINE);
                            DrawFont2(str, r_tooltip, COLOR_TOOLTIPTEXT, TextCenter, ((Vector2){0, -1}));
                        }
                    }
                    else if (selected == -501000 && (currentGesture == GESTURE_DRAG || currentGesture == GESTURE_HOLD))
                    {
                        uint8_t f = targetf; // for code coherence
                        uint8_t populationsize = decksenacnt; // for code coherence
                        r_button.x = mouse.x - selectedvector.x;
                        r_button.y = mouse.y - selectedvector.y;
                        DrawRectangleRounded(r_button, 0.2f, 10, COLOR_MOUSEDRAG);
                        DrawRectangleRoundedLines(r_button, 0.2f, 10, 2, COLOR_BUTTONOUTLINE);
                        DrawFont3("RANDOM", r_button, COLOR_MOUSEDRAGTEXT, TextCenter, ((Vector2){0, 1}));
                        if (currentGesture == GESTURE_DRAG) mousedelta = GetGestureDragVector();
                        else mousedelta = GetMouseDelta();
                        if (mousedelta.x != (float)(0) || mousedelta.y != (float)(0))
                        {
                            uint8_t randsize = clp2(populationsize);
                            uint8_t randbitreqbase = ffs(randsize) - 1;
                            uint8_t randsizehigh = flp2(randsize - populationsize);
                            if (populationsize == 1)
                            {
                                randval = 0;
                                randbitreq = 0;
                            }
                            else
                            {
                                if (randbitreq == -1) randbitreq = randbitreqbase;
                                uint8_t bit = __rdtsc() & 0x01; // keep LSB (least significant bit)
                                randval = (randval << 1) + bit;
                                randbitreq -= 1;
                            }
                            if (randbitreq == 0)
                            {
                                if (randval < populationsize)  // random result within bound
                                {
                                    int counter = 0;
                                    for(uint8_t s = 1; s <= ITEMCOUNT(rordata, Senator); s++)
                                    {
                                        if (sat[s-1] >= (1 << 7)) counter++;
                                        if (counter == randval + 1)
                                        {
                                            SENATOR(s).fact = f;
                                            SENATOR(s).loit = LocationItem;
                                            SENATOR(s).lonr = Forum;
                                            FACTION(f).tmil += SENATOR(s).mil1;
                                            FACTION(f).tora += SENATOR(s).ora1;
                                            FACTION(f).tloy += SENATOR(s).loy1;
                                            FACTION(f).tinf += SENATOR(s).inf1;
                                            FACTION(f).tpop += SENATOR(s).pop1;
                                            decksenacnt--;
                                            sat[s-1] = (1 << 6) + (f & 0x3F);
                                            factsenacnt[f]++;
                                            *change = 1;
                                            save(rordata, rordataLength);
                                            break;
                                        }
                                    }
                                    randval = 0;
                                    randbitreq -= 1;
                                }
                                else if ((0 < randsizehigh) && ((randsize - randsizehigh) <= randval))  // keep some bits for next attempt if possible
                                {
                                    randval -= (randsize - randsizehigh);
                                    uint8_t randbitreqbaseHigh = ffs(randsizehigh) - 1;
                                    randbitreq = randbitreqbase - randbitreqbaseHigh;
                                }
                                else
                                {
                                    randval = 0;
                                    randbitreq -= 1;
                                }
                            }
                        }
                    }
                    else
                    {
                        DrawRectangleRounded(r_button, 0.2f, 10, COLOR_BUTTONBACKGROUND);
                        DrawRectangleRoundedLines(r_button, 0.2f, 10, 2, COLOR_BUTTONOUTLINE);
                        DrawFont3("RANDOM", r_button, COLOR_BUTTONTEXT, TextCenter, ((Vector2){0, 1}));
                    }
                }
                else if (selected == -501000) selected = -1;
                // SELECTION
                if (0 <= selected) // draw selected senator on top off all other
                {
                    if ((currentGesture == GESTURE_NONE || currentGesture == GESTURE_SWIPE_RIGHT || currentGesture == GESTURE_SWIPE_LEFT || currentGesture == GESTURE_SWIPE_UP || currentGesture == GESTURE_SWIPE_DOWN) && (lastGesture == GESTURE_HOLD || lastGesture == GESTURE_DRAG || lastGesture == GESTURE_TAP)) selected = -1;
                    else
                    {
                        uint8_t s = selected;  // for code coherence
                        r_senator.x = mouse.x - selectedvector.x;
                        r_senator.y = mouse.y - selectedvector.y;
                        DrawRectangleRec(r_senator, COLOR_MOUSEDRAG);
                        sprintf(str, "%d", SENATOR(s).idnr);
                        DrawFont1(str, ((Rectangle){r_senator.x + RECT_SEN_ID_X, r_senator.y + Font2RectH - Font1RectH, RECT_SEN_ID_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextRight, ((Vector2){1, -8}));
                        DrawFont2(SENATOR(s).name, ((Rectangle){r_senator.x + RECT_SEN_NAME_X, r_senator.y, RECT_SEN_NAME_WIDTH, Font2RectH}), COLOR_BLACKCARDTEXT, TextLeft, ((Vector2){0, 0}));
                        sprintf(str, "%d", SENATOR(s).mil0);
                        DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_M_X, r_senator.y, RECT_SEN_M_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                        sprintf(str, "%d", SENATOR(s).ora0);
                        DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_O_X, r_senator.y, RECT_SEN_O_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                        sprintf(str, "%d", SENATOR(s).loy0);
                        DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_L_X, r_senator.y, RECT_SEN_L_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                        sprintf(str, "%d", SENATOR(s).inf0);
                        DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_I_X, r_senator.y, RECT_SEN_I_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                        if (SENATOR(s).pop0 != 0) {sprintf(str, "%i", SENATOR(s).pop0); DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_P_X, r_senator.y, RECT_SEN_P_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));}
                    }
                }
                // UPDATE dealstatus
                if (*change == 1)
                {
                    dealstatus = 0;
                    minfactsenacnt = 0xFF;
                    for (uint8_t f = 1; (f <= ITEMCOUNT(rordata, Faction)) && ((FACTION(f).type & FactionUsed) != 0); f++)
                    {
                        uint8_t fdealstatus = 0;
                        if (factsenacnt[f] == 0) fdealstatus |= 0x01;
                        if (factsenacnt[f] < ERA(HEADER.eran).nsen + FACTION(f).xsen)
                        {
                            fdealstatus |= 0x02;
                            minfactsenacnt = min(minfactsenacnt, factsenacnt[f]);
                        }
                        else if (factsenacnt[f] == ERA(HEADER.eran).nsen + FACTION(f).xsen) fdealstatus |= 0x04;
                        if (factsenacnt[f] < ERA(HEADER.eran).nsen) fdealstatus |= 0x08;
                        else if (factsenacnt[f] == ERA(HEADER.eran).nsen) fdealstatus |= 0x10;
                        dealstatus |= fdealstatus;
                        if (fdealstatus == 0) dealstatus |= 0x80;
                    }
                }
                // Update stage drop now so I can reallocate based on that
                if (((dealstatus & 0x09) != 0) && (*stage != 0))
                {
                    *stage = 0;
                    save(rordata, rordataLength);
                }
                // REALLOCATE NEUTRAL FACTIONS
                if (*stage == 0 && *change == 1)
                {
                    uint8_t assign = 0;
                    uint8_t pcnt = 0;
                    for (uint8_t f = 1; (f <= ITEMCOUNT(rordata, Faction)) && ((FACTION(f).type & FactionUsed) != 0); f++) if ((FACTION(f).name[0] != '\0') && ((FACTION(f).type & FactionNeutral) == 0)) pcnt++;
                    switch (pcnt)
                    {
                        case 0: break;
                        case 1: assign = SOLITAIRE_FCNT - pcnt; // fallthrough
                        case 2: assign = TWOPLAYERS_FCNT - pcnt; // fallthrough
                        default:
                        {
                            // assign neutral factions
                            int32_t maxfactweight;
                            int32_t factweight;
                            uint8_t fa;
                            for (uint8_t a = 1; a <= assign; a++)
                            {
                                for (fa = 1; (fa < (ITEMCOUNT(rordata, Faction)) && ((FACTION(fa).type & FactionUsed) != 0) && !(((FACTION(fa).type & FactionNeutral) != 0) && (FACTION(fa).asor == a)) ); fa++);
                                if (!(((FACTION(fa).type & FactionNeutral) != 0) && (FACTION(fa).asor == a))) continue;
                                void *p = &(((RoR_FactionItem_t_unordered *)p_ITEM(rordata, Faction) + (fa-1))->ast1);
                                int8_t fmax = -1;
                                maxfactweight = -2147483648;
                                for (uint8_t f = 1; (f <= ITEMCOUNT(rordata, Faction)) && ((FACTION(f).type & FactionUsed) != 0); f++)
                                {
                                    if ((FACTION(f).type & FactionNeutral) == 0) continue;
                                    if (FACTION(f).asor < a) continue;
                                    factweight = 0;
                                    for (uint8_t i=0; i <= 3; i++)
                                    {
                                        uint8_t ast = *(uint8_t*)(p+i);
                                        int32_t mul = (1 << (7 * (3-i)));
                                        if (0 < (ast & 0x80)) factweight += mul * FACTION(f).tmil;
                                        if (0 < (ast & 0x40)) factweight += mul * FACTION(f).tora;
                                        if (0 < (ast & 0x20)) factweight += mul * FACTION(f).tloy;
                                        if (0 < (ast & 0x10)) factweight += mul * FACTION(f).tinf;
                                        if (0 < (ast & 0x08)) factweight -= mul * FACTION(f).tmil;
                                        if (0 < (ast & 0x04)) factweight -= mul * FACTION(f).tora;
                                        if (0 < (ast & 0x02)) factweight -= mul * FACTION(f).tloy;
                                        if (0 < (ast & 0x01)) factweight -= mul * FACTION(f).tinf;
                                    }
                                    if (maxfactweight < factweight)
                                    {
                                        fmax = f;
                                        maxfactweight = factweight;
                                    }
                                }
                                if ((0 <= fmax) && ((uint8_t)fmax != fa))
                                {
                                    RoR_FactionItem_t faction;
                                    memcpy(&faction, &FACTION(fa), sizeof(RoR_FactionItem_t));
                                    faction.tmil = FACTION(fmax).tmil;
                                    faction.tora = FACTION(fmax).tora;
                                    faction.tloy = FACTION(fmax).tloy;
                                    faction.tinf = FACTION(fmax).tinf;
                                    faction.tpop = FACTION(fmax).tpop;
                                    FACTION(fmax).tmil = FACTION(fa).tmil;
                                    FACTION(fmax).tora = FACTION(fa).tora;
                                    FACTION(fmax).tloy = FACTION(fa).tloy;
                                    FACTION(fmax).tinf = FACTION(fa).tinf;
                                    FACTION(fmax).tpop = FACTION(fa).tpop;
                                    memcpy(&FACTION(fa), &FACTION(fmax), sizeof(RoR_FactionItem_t));
                                    memcpy(&FACTION(fmax), &faction, sizeof(RoR_FactionItem_t));
                                    save(rordata, rordataLength);
                                }
                            }
                        }
                    }
                }
                // Stage promotion though goes after a final reallocation of neutral factions
                if (((dealstatus & 0x09) == 0) && (*stage == 0))
                {
                    selected = -1;
                    *stage = 1;
                    save(rordata, rordataLength);
                }
                // Now that I handled the change in this fram, I can set it back
                if (*change == 1)
                {
                    *change = 0;
                    save(rordata, rordataLength);
                }
            } break;

            case PhTemporaryRomeConsul:
            {
                // PREP
                uint8_t o;
                for(o = 1; o <= ITEMCOUNT(rordata, Office) && OFFICE(o).type != RomeConsul; o++);  // find rome consul
                uint8_t src = 0;
                uint8_t *sat = MemAlloc(ITEMCOUNT(rordata, Senator));
                uint8_t *factsenacnt = MemAlloc(ITEMCOUNT(rordata, Faction) + 1);
                memset(factsenacnt, 0, ITEMCOUNT(rordata, Faction) + 1);  // zero
                uint8_t senacnt = 0;
                uint8_t minsenaidnr = 255;
                for(uint8_t s = 1; s <= ITEMCOUNT(rordata, Senator); s++)
                {
                    if (SENATOR(s).loit == LocationItem && SENATOR(s).lonr == Forum)
                    {
                        senacnt++;
                        sat[s-1] = SENATOR(s).fact & 0x3F;
                        factsenacnt[sat[s-1]]++;
                        sat[s-1] += 1 << 6;
                        if (0 < SENATOR(s).fact) minsenaidnr = min(minsenaidnr, SENATOR(s).idnr);
                        if (SENATOR(s).offi == o) src = s;
                    }
                    else sat[s-1] = 0;
                }
                if ((ERA(HEADER.eran).terc & 0x82) == 0x02)
                {
                    uint8_t s;
                    for(s = 1; s <= ITEMCOUNT(rordata, Senator) && ((SENATOR(s).fact == 0) || (SENATOR(s).idnr != minsenaidnr)); s++);
                    SENATOR(s).offi = o;
                    SENATOR(s).inf1 += OFFICE(o).infl;
                    SENATOR(s).inf2 = SENATOR(s).inf1;
                    SENATOR(s).pric += 1;
                    MAGISTRATE(HRAO).owit = SenatorItem;
                    MAGISTRATE(HRAO).ownr = s;
                    ERA(HEADER.eran).terc |= 0x80;  // resolved
                    save(rordata, rordataLength);
                }
                if (MAGISTRATE(HRAO).owit == 0 || MAGISTRATE(HRAO).ownr == 0)
                {
                    MAGISTRATE(HRAO).owit = SenatorItem;
                    uint32_t maxweight = 0;
                    for(uint8_t s = 1; s <= ITEMCOUNT(rordata, Senator); s++)
                    {
                        if (sat[s-1] <= (1 << 6)) continue; // must be on the forum and aligned
                        uint32_t weight = 100000 * SENATOR(s).inf2 + 1000 * SENATOR(s).ora2 + 500 - SENATOR(s).idnr;
                        if (maxweight < weight){
                            MAGISTRATE(HRAO).ownr = s;
                            maxweight = weight;
                        }
                    }
                    save(rordata, rordataLength);
                }
                // TITLE
                Rectangle r_title = {0, 0, screenWidth, TITLEHEIGHT};
                DrawRectangleRec(r_title, COLOR_TITLEBACKGROUND);
                DrawTitle("(TEMPORARY) ROME CONSUL", r_title, COLOR_TITLETEXT, TextLeft);
                // FACTIONS PREP
                Vector2 selectedvector;
                Rectangle r_header = {0, 0, RECT_SEN_WIDTH2, Font2RectH};
                Rectangle r_senator = {0, 0, r_header.width, Font2RectH};
                r_header.x = UNITCLAMP((screenWidth - r_header.width) / 2);
                r_header.y = (r_title.height + PAD) + 3 * UNIT;
                DrawRectangleRec(r_header, COLOR_FACTIONHEADER);
                DrawFont2("SENATE", r_header, COLOR_FACTIONHEADERTEXT, TextLeft, ((Vector2){0, 0}));
                DrawFont2("M", ((Rectangle){r_header.x + RECT_SEN_M_X, r_header.y, RECT_SEN_M_WIDTH, r_header.height}), COLOR_FACTIONHEADERTEXT, TextCenter, ((Vector2){0, 0}));
                DrawFont2("O", ((Rectangle){r_header.x + RECT_SEN_O_X, r_header.y, RECT_SEN_O_WIDTH, r_header.height}), COLOR_FACTIONHEADERTEXT, TextCenter, ((Vector2){0, 0}));
                DrawFont2("L", ((Rectangle){r_header.x + RECT_SEN_L_X, r_header.y, RECT_SEN_L_WIDTH, r_header.height}), COLOR_FACTIONHEADERTEXT, TextCenter, ((Vector2){0, 0}));
                DrawFont2("I", ((Rectangle){r_header.x + RECT_SEN_I_X, r_header.y, RECT_SEN_I_WIDTH, r_header.height}), COLOR_FACTIONHEADERTEXT, TextCenter, ((Vector2){0, 0}));
                DrawFont2("P", ((Rectangle){r_header.x + RECT_SEN_P_X, r_header.y, RECT_SEN_P_WIDTH, r_header.height}), COLOR_FACTIONHEADERTEXT, TextCenter, ((Vector2){0, 0}));
                DrawFont2("PC", ((Rectangle){r_header.x + RECT_SEN_PRIC_X, r_header.y, RECT_SEN_PRIC_WIDTH, r_header.height}), COLOR_FACTIONHEADERTEXT, TextCenter, ((Vector2){0, 0}));
                r_senator.x = r_header.x;
                r_senator.y = r_header.y + 1 * (Font2RectH + PAD) + 2 * UNIT;
                for (uint8_t f = 1; f <= ITEMCOUNT(rordata, Faction); f++)
                {
                    Rectangle r_faction = {r_senator.x, r_senator.y, r_senator.width, r_senator.height + (Font2RectH + PAD) * factsenacnt[f]};
                    DrawRectangleRec(r_faction, COLOR_BACKGROUNDAREA);
                    DrawRectangleRec(r_senator, COLOR_FACTION);
                    DrawFont2(TextToUpper(FACTION(f).name), r_senator, COLOR_FACTIONTEXT, TextLeft, ((Vector2){0, 0}));
                    for(uint8_t s = 1; s <= ITEMCOUNT(rordata, Senator); s++)
                    {
                        if ((sat[s-1] & 0x3F) != f) continue;  // not in this faction
                        r_senator.y += (Font2RectH + PAD);
                        if ((CheckCollisionPointRec(mouse, r_senator)))
                        {
                            if (currentGesture != lastGesture && currentGesture == GESTURE_TAP)
                            {
                                if (0 < src)
                                {
                                    SENATOR(src).offi = 0;
                                    SENATOR(src).inf1 -= OFFICE(o).infl;
                                    SENATOR(src).inf2 = SENATOR(src).inf1;
                                    SENATOR(src).pric -= 1;
                                    if (MAGISTRATE(HRAO).owit == SenatorItem && MAGISTRATE(HRAO).ownr == src)
                                    {
                                        MAGISTRATE(HRAO).owit = 0;
                                        MAGISTRATE(HRAO).ownr = 0;
                                    }
                                    ERA(HEADER.eran).terc &= 0x7F;  // unresolved
                                }
                                if (s != src)
                                {
                                    SENATOR(s).offi = o;
                                    SENATOR(s).inf1 += OFFICE(o).infl;
                                    SENATOR(s).inf2 = SENATOR(s).inf1;
                                    SENATOR(s).pric += 1;
                                    MAGISTRATE(HRAO).owit = SenatorItem;
                                    MAGISTRATE(HRAO).ownr = s;
                                    ERA(HEADER.eran).terc |= 0x80;  // resolved
                                }
                                else if (MAGISTRATE(HRAO).owit == 0 || MAGISTRATE(HRAO).ownr == 0)
                                {
                                    MAGISTRATE(HRAO).owit = SenatorItem;
                                    uint32_t maxweight = 0;
                                    for(uint8_t s = 1; s <= ITEMCOUNT(rordata, Senator); s++)
                                    {
                                        if (sat[s-1] <= (1 << 6)) continue; // must be on the forum and aligned
                                        uint32_t weight = 100000 * SENATOR(s).inf2 + 1000 * SENATOR(s).ora2 + 500 - SENATOR(s).idnr;
                                        if (maxweight < weight){
                                            MAGISTRATE(HRAO).ownr = s;
                                            maxweight = weight;
                                        }
                                    }
                                }
                                save(rordata, rordataLength);
                                DrawRectangleRec(r_senator, COLOR_CLICKED);
                            }
                            else DrawRectangleRec(r_senator, COLOR_MOUSEHOVER_CLICKABLE);
                        }
                        else DrawRectangleRec(r_senator, COLOR_BLACKCARDBACKGROUND);
                        if (MAGISTRATE(HRAO).owit == SenatorItem && MAGISTRATE(HRAO).ownr == s)
                        {
                            Rectangle r_hrao = {r_senator.x + RECT_SEN_HRAO_X, r_senator.y, RECT_SEN_HRAO_WIDTH, r_senator.height};
                            DrawRectangleRec(r_hrao, COLOR_HRAO);
                            DrawFont2("H", r_hrao, COLOR_OFFICETEXT, TextCenter, ((Vector2){1, 0}));
                        }
                        if (FACTION(f).leit == SenatorItem && FACTION(f).lenr == s)
                        {
                            DrawRectangleRec(((Rectangle){r_senator.x + RECT_SEN_ID_X, r_senator.y, RECT_SEN_ID_WIDTH + 2, r_senator.height}), COLOR_FACTION);
                            DrawRectangleRec(((Rectangle){r_senator.x + RECT_SEN_ID_X + 1, r_senator.y + r_senator.height - 4, RECT_SEN_ID_WIDTH, 3}), COLOR_BLACKCARDTEXT);
                        }
                        sprintf(str, "%d", SENATOR(s).idnr);
                        DrawFont1(str, ((Rectangle){r_senator.x + RECT_SEN_ID_X, r_senator.y + Font2RectH - Font1RectH, RECT_SEN_ID_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextRight, ((Vector2){1, -8}));
                        DrawFont2(SENATOR(s).name, ((Rectangle){r_senator.x + RECT_SEN_NAME_X, r_senator.y, RECT_SEN_NAME_WIDTH, Font2RectH}), COLOR_BLACKCARDTEXT, TextLeft, ((Vector2){0, 0}));
                        if (0 < SENATOR(s).offi)
                        {
                            Rectangle r_office = {r_senator.x + RECT_SEN_OFF_X, r_senator.y, RECT_SEN_OFF_WIDTH, r_senator.height};
                            DrawRectangleRec(r_office, COLOR_OFFICE);
                            DrawFont2(OFFICE(SENATOR(s).offi).snam, r_office, COLOR_OFFICETEXT, TextCenter, ((Vector2){0, 0}));
                        }
                        sprintf(str, "%d", SENATOR(s).mil2);
                        DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_M_X, r_senator.y, RECT_SEN_M_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                        sprintf(str, "%d", SENATOR(s).ora2);
                        DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_O_X, r_senator.y, RECT_SEN_O_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                        sprintf(str, "%d", SENATOR(s).loy2);
                        DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_L_X, r_senator.y, RECT_SEN_L_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                        sprintf(str, "%d", SENATOR(s).inf2);
                        DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_I_X, r_senator.y, RECT_SEN_I_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                        if (SENATOR(s).pop2 != 0) {sprintf(str, "%i", SENATOR(s).pop2); DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_P_X, r_senator.y, RECT_SEN_P_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));}
                        if (0 < SENATOR(s).pric) DrawFont2("PC", ((Rectangle){r_senator.x + RECT_SEN_PRIC_X, r_senator.y, RECT_SEN_PRIC_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                    }
                    r_senator.y = r_faction.y + r_faction.height + 2 * UNIT + PAD;
                }
                // NEXT BUTTON
                Rectangle r_button = {screenWidth - 4 * UNIT - 4 * Font3HUnit, 2 * UNIT, UNITCLAMP(4 * Font3HUnit), TITLEHEIGHT - 4 * UNIT};
                if (selected == -1 && CheckCollisionPointRec(mouse, r_button))
                {
                    if (currentGesture == GESTURE_TAP)
                    {
                        DrawRectangleRounded(r_button, 0.2f, 10, COLOR_CLICKED);
                        if ((ERA(HEADER.eran).terc & 0x05) == 0x01) header->phse = PhSelectFactionLeaders;
                        else header->phse = PhInitialFactionPhase;
                        save(rordata, rordataLength);
                        MemFree(sat);
                        MemFree(factsenacnt);
                        selected = -1;
                        framesCounter = 0;
                    }
                    else DrawRectangleRounded(r_button, 0.2f, 10, COLOR_MOUSEHOVER_CLICKABLE);
                }
                else DrawRectangleRounded(r_button, 0.2f, 10, COLOR_BUTTONBACKGROUND);
                DrawRectangleRoundedLines(r_button, 0.2f, 10, 2, COLOR_BUTTONOUTLINE);
                DrawFont3("NEXT", r_button, COLOR_BUTTONTEXT, TextCenter, ((Vector2){0, 1}));
                // RANDOM BUTTON
                if ((ERA(HEADER.eran).terc & 0x82) == 0x00)
                {
                    r_button.width = UNITCLAMP(6 * Font3HUnit);
                    r_button.x -= 2 * UNIT + r_button.width;
                    if (selected == -501000 && ((currentGesture == GESTURE_NONE || currentGesture == GESTURE_SWIPE_RIGHT || currentGesture == GESTURE_SWIPE_LEFT || currentGesture == GESTURE_SWIPE_UP || currentGesture == GESTURE_SWIPE_DOWN) && (lastGesture == GESTURE_HOLD || lastGesture == GESTURE_DRAG || lastGesture == GESTURE_TAP))) selected = -1;
                    if (selected == -1 && CheckCollisionPointRec(mouse, r_button))
                    {
                        if (currentGesture != lastGesture && currentGesture == GESTURE_TAP) // pick up
                        {
                            selected = -501000;
                            selectedvector.x = mouse.x - r_button.x;
                            selectedvector.y = mouse.y - r_button.y;
                        }
                        else
                        {
                            DrawRectangleRounded(r_button, 0.2f, 10, COLOR_MOUSEHOVER_DRAGABLE);
                            DrawRectangleRoundedLines(r_button, 0.2f, 10, 2, COLOR_BUTTONOUTLINE);
                            DrawFont3("RANDOM", r_button, COLOR_BUTTONTEXT, TextCenter, ((Vector2){0, 1}));
                            strcpy(str, "drag this for random draw");
                            Vector2 textsize = MeasureTextEx(font2, str, Font2H, Font2Spacing);
                            textsize.x += 4 * Font2Spacing;
                            Rectangle r_tooltip = {UNITCLAMP(r_button.x + r_button.width / 2 - textsize.x / 2), r_button.y + r_button.height + 1 * UNIT, UNITCLAMP(textsize.x), Font2RectH};
                            DrawRectangleRec(r_tooltip, COLOR_TOOLTIPBACKGROUND);
                            DrawRectangleLinesEx(r_tooltip, 1.0f, COLOR_TOOLTIPOUTLINE);
                            DrawFont2(str, r_tooltip, COLOR_TOOLTIPTEXT, TextCenter, ((Vector2){0, -1}));
                        }
                    }
                    else if (selected == -501000 && (currentGesture == GESTURE_DRAG || currentGesture == GESTURE_HOLD))
                    {
                        uint8_t populationsize = senacnt; // for code coherence
                        r_button.x = mouse.x - selectedvector.x;
                        r_button.y = mouse.y - selectedvector.y;
                        DrawRectangleRounded(r_button, 0.2f, 10, COLOR_MOUSEDRAG);
                        DrawRectangleRoundedLines(r_button, 0.2f, 10, 2, COLOR_BUTTONOUTLINE);
                        DrawFont3("RANDOM", r_button, COLOR_MOUSEDRAGTEXT, TextCenter, ((Vector2){0, 1}));
                        if (currentGesture == GESTURE_DRAG) mousedelta = GetGestureDragVector();
                        else mousedelta = GetMouseDelta();
                        if (mousedelta.x != (float)(0) || mousedelta.y != (float)(0))
                        {
                            uint8_t randsize = clp2(populationsize);
                            uint8_t randbitreqbase = ffs(randsize) - 1;
                            uint8_t randsizehigh = flp2(randsize - populationsize);
                            if (populationsize == 1)
                            {
                                randval = 0;
                                randbitreq = 0;
                            }
                            else
                            {
                                if (randbitreq == -1) randbitreq = randbitreqbase;
                                uint8_t bit = __rdtsc() & 0x01; // keep LSB (least significant bit)
                                randval = (randval << 1) + bit;
                                randbitreq -= 1;
                            }
                            if (randbitreq == 0)
                            {
                                if (randval < populationsize)  // random result within bound
                                {
                                    int counter = 0;
                                    for(uint8_t s = 1; s <= ITEMCOUNT(rordata, Senator); s++)
                                    {
                                        if (sat[s-1] > (1 << 6)) counter++;  // must be on the forum and aligned
                                        if (counter == randval + 1)
                                        {
                                            if (0 < src)
                                            {
                                                SENATOR(src).offi = 0;
                                                SENATOR(src).inf1 -= OFFICE(o).infl;
                                                SENATOR(src).inf2 = SENATOR(src).inf1;
                                                SENATOR(src).pric -= 1;
                                            }
                                            SENATOR(s).offi = o;
                                            SENATOR(s).inf1 += OFFICE(o).infl;
                                            SENATOR(s).inf2 = SENATOR(s).inf1;
                                            SENATOR(s).pric += 1;
                                            MAGISTRATE(HRAO).owit = SenatorItem;
                                            MAGISTRATE(HRAO).ownr = s;
                                            ERA(HEADER.eran).terc |= 0x80;  // resolved
                                            save(rordata, rordataLength);
                                            break;
                                        }
                                    }
                                    randval = 0;
                                    randbitreq -= 1;
                                }
                                else if ((0 < randsizehigh) && ((randsize - randsizehigh) <= randval))  // keep some bits for next attempt if possible
                                {
                                    randval -= (randsize - randsizehigh);
                                    uint8_t randbitreqbaseHigh = ffs(randsizehigh) - 1;
                                    randbitreq = randbitreqbase - randbitreqbaseHigh;
                                }
                                else
                                {
                                    randval = 0;
                                    randbitreq -= 1;
                                }
                            }
                        }
                    }
                    else
                    {
                        DrawRectangleRounded(r_button, 0.2f, 10, COLOR_BUTTONBACKGROUND);
                        DrawRectangleRoundedLines(r_button, 0.2f, 10, 2, COLOR_BUTTONOUTLINE);
                        DrawFont3("RANDOM", r_button, COLOR_BUTTONTEXT, TextCenter, ((Vector2){0, 1}));
                    }
                }
                else if (selected == -501000) selected = -1;
            } break;

        }

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadFileData(rordata);      // Free memory from loaded file
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}



// Draw text using font inside rectangle limits
static void DrawTextBoxed(Font font, const char *text, Rectangle rec, float fontSize, float spacing, bool wordWrap, Color tint, float lineHeight)
{
    int length = TextLength(text);  // Total length in bytes of the text, scanned by codepoints in loop

    float textOffsetY = 0;          // Offset between lines (on line break '\n')
    float textOffsetX = 0.0f;       // Offset X to next character to draw

    float scaleFactor = fontSize/(float)font.baseSize;     // Character rectangle scaling factor

    // Word/character wrapping mechanism variables
    enum { MEASURE_STATE = 0, DRAW_STATE = 1 };
    int state = wordWrap? MEASURE_STATE : DRAW_STATE;

    int startLine = -1;         // Index where to begin drawing (where a line begins)
    int endLine = -1;           // Index where to stop drawing (where a line ends)
    int lastk = -1;             // Holds last value of the character position

    if (lineHeight == 0) lineHeight = (font.baseSize + font.baseSize/2)*scaleFactor;  // Apply default if 0 lineHeight

    for (int i = 0, k = 0; i < length; i++, k++)
    {
        // Get next codepoint from byte string and glyph index in font
        int codepointByteCount = 0;
        int codepoint = GetCodepoint(&text[i], &codepointByteCount);
        int index = GetGlyphIndex(font, codepoint);

        // NOTE: Normally we exit the decoding sequence as soon as a bad byte is found (and return 0x3f)
        // but we need to draw all of the bad bytes using the '?' symbol moving one byte
        if (codepoint == 0x3f) codepointByteCount = 1;
        i += (codepointByteCount - 1);

        float glyphWidth = 0;
        if (codepoint != '\n')
        {
            glyphWidth = (font.glyphs[index].advanceX == 0) ? font.recs[index].width*scaleFactor : font.glyphs[index].advanceX*scaleFactor;

            if (i + 1 < length) glyphWidth = glyphWidth + spacing;
        }

        // NOTE: When wordWrap is ON we first measure how much of the text we can draw before going outside of the rec container
        // We store this info in startLine and endLine, then we change states, draw the text between those two variables
        // and change states again and again recursively until the end of the text (or until we get outside of the container).
        // When wordWrap is OFF we don't need the measure state so we go to the drawing state immediately
        // and begin drawing on the next line before we can get outside the container.
        if (state == MEASURE_STATE)
        {
            // TODO: There are multiple types of spaces in UNICODE, maybe it's a good idea to add support for more
            // Ref: http://jkorpela.fi/chars/spaces.html
            if ((codepoint == ' ') || (codepoint == '\t') || (codepoint == '\n')) endLine = i;

            if ((textOffsetX + glyphWidth) > rec.width)
            {
                endLine = (endLine < 1)? i : endLine;
                if (i == endLine) endLine -= codepointByteCount;
                if ((startLine + codepointByteCount) == endLine) endLine = (i - codepointByteCount);

                state = !state;
            }
            else if ((i + 1) == length)
            {
                endLine = i;
                state = !state;
            }
            else if (codepoint == '\n') state = !state;

            if (state == DRAW_STATE)
            {
                textOffsetX = 0;
                i = startLine;
                glyphWidth = 0;

                // Save character position when we switch states
                int tmp = lastk;
                lastk = k - 1;
                k = tmp;
            }
        }
        else
        {
            if (codepoint == '\n')
            {
                if (!wordWrap)
                {
                    textOffsetY += lineHeight;
                    textOffsetX = 0;
                }
            }
            else
            {
                if (!wordWrap && ((textOffsetX + glyphWidth) > rec.width))
                {
                    textOffsetY += lineHeight;
                    textOffsetX = 0;
                }

                // When text overflows rectangle height limit, just stop drawing
                // if ((textOffsetY + font.baseSize*scaleFactor) > rec.height) break;

                // Draw current character glyph
                if ((codepoint != ' ') && (codepoint != '\t'))
                {
                    DrawTextCodepoint(font, codepoint, (Vector2){ rec.x + textOffsetX, rec.y + textOffsetY}, fontSize, tint);
                }
            }

            if (wordWrap && (i == endLine))
            {
                textOffsetY += lineHeight;
                textOffsetX = 0;
                startLine = endLine;
                endLine = -1;
                glyphWidth = 0;
                k = lastk;

                state = !state;
            }
        }

        if ((textOffsetX != 0) || (codepoint != ' ')) textOffsetX += glyphWidth; // avoid leading spaces
    }
}