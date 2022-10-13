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

__INITRORAPI__ // initializes the API structs


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
#define COLOR_BLACKCARDBACKGROUND COLOR_BrightGray
#define COLOR_BLACKCARDTEXT COLOR_ChineseBlack
#define COLOR_BUTTONBACKGROUND COLOR_BrightGray
#define COLOR_BUTTONOUTLINE COLOR_ChineseBlack
#define COLOR_BUTTONTEXT COLOR_ChineseBlack
#define COLOR_CLICKED COLOR_CookiesAndCream
#define COLOR_ERAHEADER COLOR_CrayolaTan
#define COLOR_ERAHEADERTEXT COLOR_ChineseBlack
#define COLOR_FACTION COLOR_TitaniumYellow
#define COLOR_FACTIONHEADER COLOR_LightTaupe
#define COLOR_FACTIONHEADERTEXT COLOR_ChineseBlack
#define COLOR_FACTIONTEXT COLOR_ChineseBlack
#define COLOR_HRAO COLOR_LightTaupe
#define COLOR_MOUSEDRAG  COLOR_CookiesAndCream
#define COLOR_MOUSEDRAGTEXT  COLOR_ChineseBlack
#define COLOR_MOUSEHOVER_CLICKABLE COLOR_CookiesAndCream
#define COLOR_MOUSEHOVER_DRAGABLE  COLOR_CookiesAndCream
#define COLOR_MOUSEHOVER_DROPTARGET  COLOR_CookiesAndCream
#define COLOR_MOUSEHOVER_EDITABLE  COLOR_CookiesAndCream
#define COLOR_MOUSEHOVER_GAMEMASTER  MAGENTA
#define COLOR_MOUSEHOVER_SELECTABLE  COLOR_CookiesAndCream
#define COLOR_OFFICE COLOR_ImperialRed
#define COLOR_OFFICETEXT COLOR_ChineseBlack
#define COLOR_STATESMANBACKGROUND COLOR_BrightGray
#define COLOR_STATESMANTEXT COLOR_Red
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


void decrease(unsigned char *rordata, uint16_t groupidx, uint16_t attridx, uint16_t elemidx)
{
    Attr attr = attr(rordata, groupidx, attridx);
    switch (attr.type)
    {
        case T_INT8:
        case T_UINT8:
        {
            (*valabsaddr(rordata, groupidx, attridx, elemidx))--;
        } break;
    }
}

void increase(unsigned char *rordata, uint16_t groupidx, uint16_t attridx, uint16_t elemidx)
{
    Attr attr = attr(rordata, groupidx, attridx);
    switch (attr.type)
    {
        case T_INT8:
        case T_UINT8:
        {
            (*valabsaddr(rordata, groupidx, attridx, elemidx))++;
        } break;
    }
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
void DrawText2(Font font, const char *text, Rectangle box, float fontSize, float spacing, Color tint, TextAlignment align)
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
}

void DrawTextEx2(Font font, const char *text, Rectangle box, float fontSize, float spacing, Vector2 padding, Color tint, TextAlignment align, Vector2 offset)
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
}


int main(void)
{
    Vector2 cursor;
    char str[256];

    uint16_t windowedScreenWidth = 1960;
    uint16_t windowedScreenHeight = 1080;
    uint16_t screenRights = (windowedScreenWidth - 2 * PAD) / RIGHT;
    uint16_t screenDowns = (windowedScreenHeight - 2 * PAD) / DOWN;
    uint16_t screenOffsetX = (windowedScreenWidth - RIGHT * screenRights) / 2;
    uint16_t screenOffsetY = (windowedScreenHeight - DOWN * screenDowns) / 2;
    uint16_t screenWidth = windowedScreenWidth;
    uint16_t screenHeight = windowedScreenHeight;

    Font font1, font2, font3;

    //unsigned int rordataLength = SIZE;
    //unsigned char *rordata = LoadFileData("scenario.ror", &rordataLength);
    uint8_t scene = 0;
    bool rordataLoaded = false;
    unsigned char *rordata;

    Game *game;

    Vector2 mouse;
    Vector2 mousedelta;
    int currentGesture = GESTURE_NONE;
    int lastGesture = GESTURE_NONE;

    unsigned char randVal = 0;
    int8_t randBitReq = -1;

    SetTraceLogLevel(LOG_DEBUG); // TODO: DEBUG
    int debugvar = 0;
    char debugstr[256][256] = {0};

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

        switch (scene)
        {
            case 0:
            {
                // PREP
                static bool searched = false;
                static int count = 0;
                static FilePathList files;
                static char **descriptions;
                static unsigned int readLength = sizeof(Header);
                if (!searched)
                {
                    TraceLog(LOG_DEBUG, "Hello");
                    files = LoadDirectoryFiles(".");
                    descriptions = (char**)MemAlloc(files.count * sizeof(char *));
                    for(int i=0; i<files.count; i++ ){
                        if (IsPathFile(files.paths[i]) == false) continue;
                        if (!TextIsEqual(GetFileExtension(files.paths[i]), ".ror")) continue;
                        unsigned char *data = LoadFileData(files.paths[i], &readLength);
                        descriptions[count] = (char*)MemAlloc(44);
                        strcpy(descriptions[count], ((Header*)(data))->dsc);
                        if (i != count) strcpy(files.paths[count], files.paths[i]);
                        //TraceLog(LOG_DEBUG, descriptions[count]);
                        UnloadFileData(data);
                        count++;
                    }
                    searched = true;
                }
                // TITLE
                Rectangle r_title = {0, 0, screenWidth, TITLEHEIGHT};
                DrawRectangleRec(r_title, COLOR_TITLEBACKGROUND);
                DrawTitle("SCENARIO", r_title, COLOR_TITLETEXT, TextLeft);
                // LIST
                for (int i=0; i<count; i++ ){
                    Rectangle r = {4 * UNIT, (r_title.height + PAD) + 3 * UNIT + (Font2RectH + PAD) * i, 60 * Font2HUnit, Font2RectH};
                    if (CheckCollisionPointRec(mouse, r))
                    {
                        if (currentGesture == GESTURE_TAP)
                        {
                            unsigned int rordataLength = GetFileLength(files.paths[i]);
                            rordata = LoadFileData(files.paths[i], &rordataLength);
                            game = val0absaddr(rordata, G_GAME, 0);
                            game->phse = PHSE_PREP;
                            game->sphs = SPHS_PREP_TAKEFACTIONS;
                            rordataLoaded = true;
                            scene = 1;
                            UnloadDirectoryFiles(files);
                            for (int j=0; j<count; j++ ) MemFree(descriptions[count]);
                            MemFree(descriptions);
                            searched = false;
                            break;
                        }
                    }
                    DrawRectangleRec(r, COLOR_FACTIONHEADER);
                    sprintf(str, "[%s] %s", files.paths[i], descriptions[i]);
                    DrawFont2(str, r, WHITE, TextLeft, ((Vector2){0, 0}));
                }
            } break;

            default:
        {
        switch (*(A_GAME_PHSE_t*)(val0absaddr(rordata, G_GAME, A_GAME_PHSE)))
        {
            case PHSE_PREP: /* PREPARE TO PLAY */
            {
                switch (*(A_GAME_SPHS_t*)(val0absaddr(rordata, G_GAME, A_GAME_SPHS)))
                {
                    case 0:
                    case SPHS_PREP_TAKEFACTIONS: /* 3.01.2 [4.1] GAMEBOARD (Take Seets) */
                    {
                        // TITLE
                        Rectangle r_title = {0, 0, screenWidth, TITLEHEIGHT};
                        DrawRectangleRec(r_title, COLOR_TITLEBACKGROUND);
                        DrawTitle("NAME THE PLAYING FACTIONS OR DELETE NAMES FOR EXCLUSION", r_title, COLOR_TITLETEXT, TextLeft);
                        // FACTIONS
                        uint8_t maxLetterCount = ((attr(rordata, G_FACT, A_FACT_NAME).type & 0x3FFF) - 1); // -1 because of the trailin null character
                        uint16_t numFactions = group(rordata, G_FACT).elems - 1;  // first "faction" (unaligned) is excluded
                        Rectangle r_faction = {0, 0, UNIT + UNITCLAMP((maxLetterCount + 1) * font2Em.x + ((maxLetterCount + 1) - 1) * Font2Spacing + 2 * Font2PaddingX), Font2RectH}; // "UNIT +" is required for unknown reason; (maxLetterCount + 1) because of the shown input cursor: "_"
                        r_faction.x = (uint16_t)((screenWidth - r_faction.width) / 2);
                        r_faction.y = (uint16_t)((screenHeight - numFactions * r_faction.height - (numFactions - 1) * PAD) / 2);
                        bool anyselected = false;
                        Rectangle r = r_faction;
                        for (int factidx = 1; factidx < numFactions + 1; factidx++)
                        {
                            strcpy(str, (char*)(valabsaddr(rordata, G_FACT, A_FACT_NAME, factidx)));
                            if (CheckCollisionPointRec(mouse, r))
                            {
                                anyselected = true;
                                if (factidx != selected)
                                {
                                    selected = factidx;
                                    framesCounter = 0;
                                }
                                int letterCount = strlen((char*)(valabsaddr(rordata, G_FACT, A_FACT_NAME, factidx)));
                                DrawRectangleRec(r, COLOR_MOUSEHOVER_EDITABLE);
                                SetMouseCursor(MOUSE_CURSOR_IBEAM);
                                int key = GetCharPressed();
                                while (key > 0)
                                {
                                    // if (32 <= key && key <= 255 && letterCount < maxLetterCount)
                                    if (  ( ((key >= 65) && (key <= 90))||((key >= 97) && (key <= 122))||(key == 32) ) && (letterCount < maxLetterCount)  )
                                    {
                                        *((char*)(valabsaddr(rordata, G_FACT, A_FACT_NAME, factidx))+letterCount) = (char)key;
                                        *((char*)(valabsaddr(rordata, G_FACT, A_FACT_NAME, factidx))+letterCount+1) = '\0'; // Add null terminator at the end of the string.
                                        letterCount++;
                                    }
                                    key = GetCharPressed();  // Check next character in the queue
                                }
                                if (IsKeyPressed(KEY_BACKSPACE))
                                {
                                    letterCount--;
                                    if (letterCount < 0) letterCount = 0;
                                    *((char*)(valabsaddr(rordata, G_FACT, A_FACT_NAME, factidx))+letterCount) = '\0';
                                }
                                if ((letterCount <= maxLetterCount) && (((framesCounter/10)%2) == 0))
                                {
                                    uint8_t factionNameLength = strlen((char*)(valabsaddr(rordata, G_FACT, A_FACT_NAME, factidx)));
                                    str[factionNameLength] = '_';
                                    str[factionNameLength+1] = '\0';
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
                        if (!anyselected)
                        {
                            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
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
                                *(A_GAME_SPHS_t*)(val0absaddr(rordata, G_GAME, A_GAME_SPHS)) = (A_GAME_SPHS_t)SPHS_PREP_DEALSENATORS;
                                int nfac = 1; // first "faction" (unaligned) is counted as well
                                for (int factidx = 1; factidx < group(rordata, G_FACT).elems; factidx++)
                                {
                                    int letterCount = strlen((char*)(valabsaddr(rordata, G_FACT, A_FACT_NAME, factidx)));
                                    if (letterCount == 0)
                                    {
                                        *(A_FACT_CNGR_t*)valabsaddr(rordata, G_FACT, A_FACT_CNGR, factidx) = (A_FACT_CNGR_t)(G_NULL);
                                    }
                                    else
                                    {
                                        if (factidx != nfac) // eliminate seat gaps
                                        {
                                            for (uint16_t a = 0; a < group(rordata, G_FACT).attrs; a++)
                                            {
                                                Attr attr_ = attr(rordata, G_FACT, a);
                                                void *addr0 = valabsaddr(rordata, G_FACT, a, factidx);
                                                void *addr1 = valabsaddr(rordata, G_FACT, a, nfac);
                                                uint16_t size = valsize(rordata, G_FACT, a);
                                                memcpy(addr1, addr0, size);  // copy
                                                memset(addr0, 0, size);  // zero
                                            }
                                        }
                                        nfac += 1;
                                    }
                                }
                                *(A_GAME_NFAC_t*)val0absaddr(rordata, G_GAME, A_GAME_NFAC) = (A_GAME_NFAC_t)(nfac);
                                selected = -1;
                                framesCounter = 0;
                                randVal = 0;
                                randBitReq = -1;
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

                    case SPHS_PREP_DEALSENATORS: /* 3.01.4 B. [4.4] CARDS (Deal Senators) */
                    {
                        // PREP
                        uint16_t numFactions = *(A_GAME_NFAC_t*)val0absaddr(rordata, G_GAME, A_GAME_NFAC) - 1;  // first "faction" (unaligned) is excluded
                        uint8_t deckSize = 0;  // deck size
                        for (int senaidx = 0; senaidx < group(rordata, G_SENA).elems; senaidx++)
                        {
                            if (
                                (*(A_SENA_CNGR_t*)valabsaddr(rordata, G_SENA, A_SENA_CNGR, senaidx) == (A_SENA_CNGR_t)(G_DECK))
                                &&
                                (*(A_SENA_CNNR_t*)valabsaddr(rordata, G_SENA, A_SENA_CNNR, senaidx) == (A_SENA_CNNR_t)(0))
                            ) deckSize += 1;
                        }
                        uint8_t targetSenatorCount = (uint8_t)(*valabsaddr(rordata, G_ERAR, A_ERAR_NSEN, game->erai));
                        uint8_t factionSenatorCounts[FACT_ELEMCOUNT] = {0};
                        uint8_t minFactionSenatorCount = 255;
                        uint8_t dealTargetFactionIdx = 255;
                        int8_t dealStatus = 0;
                        for (int factidx = 1; factidx < numFactions + 1; factidx++)
                        {
                            for (int senaidx = 0; senaidx < group(rordata, G_SENA).elems; senaidx++)
                            {
                                if ( *((A_SENA_ALIG_t*)(val0absaddr(rordata, G_SENA, A_SENA_ALIG))+senaidx) == (A_SENA_ALIG_t)(factidx) ) factionSenatorCounts[factidx] += 1;
                            }
                        }
                        for (int factidx = 1; factidx < numFactions + 1; factidx++)
                        {
                            if (factionSenatorCounts[factidx] == 0)
                            {
                                dealStatus |= 0x01;
                            }
                            else if (factionSenatorCounts[factidx] < targetSenatorCount)
                            {
                                dealStatus |= 0x02;
                            }
                            else if (factionSenatorCounts[factidx] == targetSenatorCount)
                            {
                                dealStatus |= 0x04;
                            }
                            else
                            {
                                dealStatus |= 0x08;
                            }
                        }
                        for (int factidx = 1; factidx < numFactions + 1; factidx++)
                        {
                            minFactionSenatorCount = min(minFactionSenatorCount, factionSenatorCounts[factidx]);
                        }
                        if (0 < dealStatus & 0x03)
                        {
                            for (int factidx = 1; factidx < numFactions + 1; factidx++)
                            {
                                if (factionSenatorCounts[factidx] == minFactionSenatorCount)
                                {
                                    dealTargetFactionIdx = factidx;
                                    break;
                                }
                            }
                        }
                        // TITLE
                        Rectangle r_title = {0, 0, screenWidth, TITLEHEIGHT};
                        DrawRectangleRec(r_title, COLOR_TITLEBACKGROUND);
                        sprintf(str, "DEAL %i SENATORS TO EACH FACTION", targetSenatorCount);
                        DrawTitle(str, r_title, COLOR_TITLETEXT, TextLeft);
                        // SENATORS AND FACTIONS PREP
                        Vector2 selectedvector;
                        Rectangle r_header = {0, 0, RECT_SEN_WIDTH, Font2RectH};
                        Rectangle r_senator = {0, 0, r_header.width, Font2RectH};
                        // SENATORS
                        r_header.x = UNITCLAMP(screenWidth / 2) - (r_header.width + PAD) - 2 * UNIT;
                        r_header.y = (r_title.height + PAD) + 3 * UNIT;
                        Rectangle r_seantorarea = {r_header.x, r_header.y, r_header.width, screenHeight - r_header.y};
                        // DrawRectangleRec(((Rectangle){r_seantorarea.x, r_seantorarea.y + (Font2RectH + PAD) + 2 * UNIT, r_seantorarea.width, r_seantorarea.height - (Font2RectH + PAD) - 4 * UNIT}), COLOR_BACKGROUNDAREA);
                        // L> looks better without this
                        if (0 <= selected && CheckCollisionPointRec(mouse, r_seantorarea))
                        {
                            DrawRectangleRec(r_header, COLOR_MOUSEHOVER_DROPTARGET);
                            if (currentGesture != lastGesture && currentGesture == GESTURE_NONE)
                            {
                                int senaidx = selected; // for code coherence
                                *(A_SENA_ALIG_t*)valabsaddr(rordata, G_SENA, A_SENA_ALIG, senaidx) = (A_SENA_ALIG_t)(-1);
                                *(A_SENA_CNGR_t*)valabsaddr(rordata, G_SENA, A_SENA_CNGR, senaidx) = (A_SENA_CNGR_t)(G_DECK);
                                *(A_SENA_CNNR_t*)valabsaddr(rordata, G_SENA, A_SENA_CNNR, senaidx) = (A_SENA_CNNR_t)(0);
                                selected = -1;
                            }
                        }
                        else
                        {
                            DrawRectangleRec(r_header, COLOR_FACTIONHEADER);
                        }
                        DrawFont2("FAMILIES", r_header, COLOR_FACTIONHEADERTEXT, TextLeft, ((Vector2){0, 0}));
                        DrawFont2("M", ((Rectangle){r_header.x + RECT_SEN_M_X, r_header.y, RECT_SEN_M_WIDTH, r_header.height}), COLOR_FACTIONHEADERTEXT, TextCenter, ((Vector2){0, 0}));
                        DrawFont2("O", ((Rectangle){r_header.x + RECT_SEN_O_X, r_header.y, RECT_SEN_O_WIDTH, r_header.height}), COLOR_FACTIONHEADERTEXT, TextCenter, ((Vector2){0, 0}));
                        DrawFont2("L", ((Rectangle){r_header.x + RECT_SEN_L_X, r_header.y, RECT_SEN_L_WIDTH, r_header.height}), COLOR_FACTIONHEADERTEXT, TextCenter, ((Vector2){0, 0}));
                        DrawFont2("I", ((Rectangle){r_header.x + RECT_SEN_I_X, r_header.y, RECT_SEN_I_WIDTH, r_header.height}), COLOR_FACTIONHEADERTEXT, TextCenter, ((Vector2){0, 0}));
                        DrawFont2("P", ((Rectangle){r_header.x + RECT_SEN_P_X, r_header.y, RECT_SEN_P_WIDTH, r_header.height}), COLOR_FACTIONHEADERTEXT, TextCenter, ((Vector2){0, 0}));
                        r_senator.x = r_header.x;
                        r_senator.y = r_header.y + 1 * (Font2RectH + PAD) + 2 * UNIT;
                        for (int senaidx = 0; senaidx < group(rordata, G_SENA).elems; senaidx++)
                        {
                            if (
                                (*(A_SENA_CNGR_t*)valabsaddr(rordata, G_SENA, A_SENA_CNGR, senaidx) != (A_SENA_CNGR_t)(G_DECK))
                                ||
                                (*(A_SENA_CNNR_t*)valabsaddr(rordata, G_SENA, A_SENA_CNNR, senaidx) != (A_SENA_CNNR_t)(0))
                            ) continue;
                            // if ( *((A_SENA_ALIG_t*)(val0absaddr(rordata, G_SENA, A_SENA_ALIG))+senaidx) == (A_SENA_ALIG_t)(-1) ) continue;  // must be aligned
                            if (senaidx == selected) continue;
                            if (selected == -1 && CheckCollisionPointRec(mouse, r_senator))
                            {
                                if (currentGesture != lastGesture && currentGesture == GESTURE_TAP) // pick up
                                {
                                    selected = senaidx;
                                    selectedvector.x = mouse.x - r_senator.x;
                                    selectedvector.y = mouse.y - r_senator.y;
                                }
                                else
                                {
                                    DrawRectangleRec(r_senator, COLOR_MOUSEHOVER_DRAGABLE);
                                }
                            }
                            else if (selected != senaidx)
                            {
                                DrawRectangleRec(r_senator, COLOR_BLACKCARDBACKGROUND);
                            }
                            sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_IDNR, senaidx));
                            DrawFont1(str, ((Rectangle){r_senator.x + RECT_SEN_ID_X, r_senator.y + Font2RectH - Font1RectH, RECT_SEN_ID_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextRight, ((Vector2){1, -8}));
                            DrawFont2((char*)(valabsaddr(rordata, G_SENA, A_SENA_NAME, senaidx)), ((Rectangle){r_senator.x + RECT_SEN_NAME_X, r_senator.y, RECT_SEN_NAME_WIDTH, Font2RectH}), COLOR_BLACKCARDTEXT, TextLeft, ((Vector2){0, 0}));
                            sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_MIL1, senaidx));
                            DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_M_X, r_senator.y, RECT_SEN_M_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                            sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_ORA1, senaidx));
                            DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_O_X, r_senator.y, RECT_SEN_O_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                            sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_LOY1, senaidx));
                            DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_L_X, r_senator.y, RECT_SEN_L_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                            sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_INF1, senaidx));
                            DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_I_X, r_senator.y, RECT_SEN_I_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                            A_SENA_POP1_t pop = *valabsaddr(rordata, G_SENA, A_SENA_POP1, senaidx);
                            if (pop != 0) {sprintf(str, "%i", pop); DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_P_X, r_senator.y, RECT_SEN_P_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));}
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
                        for (int factidx = 1; factidx < numFactions + 1; factidx++)
                        {
                            Rectangle r_faction = {r_senator.x, r_senator.y, r_senator.width, r_senator.height + (Font2RectH + PAD) * max(targetSenatorCount, factionSenatorCounts[factidx])};
                            DrawRectangleRec(r_faction, COLOR_BACKGROUNDAREA);
                            if (0 <= selected && CheckCollisionPointRec(mouse, r_faction))
                            {
                                DrawRectangleRec(r_senator, COLOR_MOUSEHOVER_DROPTARGET);
                                if
                                (
                                    (currentGesture == GESTURE_NONE || currentGesture == GESTURE_SWIPE_RIGHT || currentGesture == GESTURE_SWIPE_LEFT || currentGesture == GESTURE_SWIPE_UP || currentGesture == GESTURE_SWIPE_DOWN)
                                    &&
                                    (lastGesture == GESTURE_HOLD || lastGesture == GESTURE_DRAG || lastGesture == GESTURE_TAP)
                                )
                                {
                                    int senaidx = selected; // for code coherence
                                    *(A_SENA_ALIG_t*)valabsaddr(rordata, G_SENA, A_SENA_ALIG, senaidx) = (A_SENA_ALIG_t)(factidx);
                                    *(A_SENA_CNGR_t*)valabsaddr(rordata, G_SENA, A_SENA_CNGR, senaidx) = (A_SENA_CNGR_t)(G_FORU);
                                    *(A_SENA_CNNR_t*)valabsaddr(rordata, G_SENA, A_SENA_CNNR, senaidx) = (A_SENA_CNNR_t)(0);
                                    selected = -1;
                                }
                            }
                            else
                            {
                                DrawRectangleRec(r_senator, COLOR_FACTION);
                            }
                            DrawFont2(TextToUpper((char*)(valabsaddr(rordata, G_FACT, A_FACT_NAME, factidx))), r_senator, COLOR_FACTIONTEXT, TextLeft, ((Vector2){0, 0}));
                            for (int senaidx = 0; senaidx < group(rordata, G_SENA).elems; senaidx++)
                            {
                                if (selected == senaidx) continue;  // handled elsewhere
                                if ( *((A_SENA_ALIG_t*)(val0absaddr(rordata, G_SENA, A_SENA_ALIG))+senaidx) != (A_SENA_ALIG_t)(factidx) ) continue;  // typecast is a must!; not in this faction
                                r_senator.y += (Font2RectH + PAD);
                                if (selected == -1 && CheckCollisionPointRec(mouse, r_senator))
                                {
                                    if (currentGesture != lastGesture && currentGesture == GESTURE_TAP) // pick up
                                    {
                                        selected = senaidx;
                                        selectedvector.x = mouse.x - r_senator.x;
                                        selectedvector.y = mouse.y - r_senator.y;
                                    }
                                    else
                                    {
                                        DrawRectangleRec(r_senator, COLOR_MOUSEHOVER_DRAGABLE);
                                    }
                                }
                                else
                                {
                                    DrawRectangleRec(r_senator, COLOR_BLACKCARDBACKGROUND);
                                }
                                sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_IDNR, senaidx));
                                DrawFont1(str, ((Rectangle){r_senator.x + RECT_SEN_ID_X, r_senator.y + Font2RectH - Font1RectH, RECT_SEN_ID_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextRight, ((Vector2){1, -8}));
                                DrawFont2((char*)(valabsaddr(rordata, G_SENA, A_SENA_NAME, senaidx)), ((Rectangle){r_senator.x + RECT_SEN_NAME_X, r_senator.y, RECT_SEN_NAME_WIDTH, Font2RectH}), COLOR_BLACKCARDTEXT, TextLeft, ((Vector2){0, 0}));
                                sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_MIL1, senaidx));
                                DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_M_X, r_senator.y, RECT_SEN_M_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                                sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_ORA1, senaidx));
                                DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_O_X, r_senator.y, RECT_SEN_O_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                                sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_LOY1, senaidx));
                                DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_L_X, r_senator.y, RECT_SEN_L_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                                sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_INF1, senaidx));
                                DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_I_X, r_senator.y, RECT_SEN_I_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                                A_SENA_POP1_t pop = *valabsaddr(rordata, G_SENA, A_SENA_POP1, senaidx);
                                if (pop != 0) {sprintf(str, "%i", pop); DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_P_X, r_senator.y, RECT_SEN_P_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));}
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
                                uint8_t temporaryromeconsulflags = *(A_ERAR_TERC_t*)(valabsaddr(rordata, G_ERAR, A_ERAR_TERC, game->erai));
                                if ((temporaryromeconsulflags & 0x05) == 0x00)
                                {
                                    *(A_GAME_SPHS_t*)(val0absaddr(rordata, G_GAME, A_GAME_SPHS)) = (A_GAME_SPHS_t)SPHS_PREP_TEMPORARYROMECONSUL;
                                }
                                else
                                {
                                    *(A_GAME_SPHS_t*)(val0absaddr(rordata, G_GAME, A_GAME_SPHS)) = (A_GAME_SPHS_t)SPHS_PREP_SELECTFACTIONLEADERS;
                                }
                                selected = -1;
                                framesCounter = 0;
                            }
                            else
                            {
                                DrawRectangleRounded(r_button, 0.2f, 10, COLOR_MOUSEHOVER_CLICKABLE);
                            }
                        }
                        else DrawRectangleRounded(r_button, 0.2f, 10, COLOR_BUTTONBACKGROUND);
                        DrawRectangleRoundedLines(r_button, 0.2f, 10, 2, COLOR_BUTTONOUTLINE);
                        DrawFont3("NEXT", r_button, COLOR_BUTTONTEXT, TextCenter, ((Vector2){0, 1}));
                        // RANDOM BUTTON
                        if (0 < deckSize && 0 < (dealStatus & 0x03))
                        {
                            r_button.width = UNITCLAMP(6 * Font3HUnit);
                            r_button.x -= 2 * UNIT + r_button.width;
                            if (selected == -501000)
                            {
                                if
                                (
                                    (currentGesture == GESTURE_NONE || currentGesture == GESTURE_SWIPE_RIGHT || currentGesture == GESTURE_SWIPE_LEFT || currentGesture == GESTURE_SWIPE_UP || currentGesture == GESTURE_SWIPE_DOWN)
                                    &&
                                    (lastGesture == GESTURE_HOLD || lastGesture == GESTURE_DRAG || lastGesture == GESTURE_TAP)
                                )
                                {
                                    selected = -1;
                                }
                            }
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
                                    strcpy(str, "drag and shake it");
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
                                int factidx = dealTargetFactionIdx; // for code coherence
                                uint8_t populationSize = deckSize; // for code coherence
                                r_button.x = mouse.x - selectedvector.x;
                                r_button.y = mouse.y - selectedvector.y;
                                DrawRectangleRounded(r_button, 0.2f, 10, COLOR_MOUSEDRAG);
                                DrawRectangleRoundedLines(r_button, 0.2f, 10, 2, COLOR_BUTTONOUTLINE);
                                DrawFont3("RANDOM", r_button, COLOR_MOUSEDRAGTEXT, TextCenter, ((Vector2){0, 1}));
                                if (currentGesture == GESTURE_DRAG) mousedelta = GetGestureDragVector();
                                else mousedelta = GetMouseDelta();
                                if (mousedelta.x != (float)(0) || mousedelta.y != (float)(0))
                                {
                                    uint8_t randSize = clp2(populationSize);
                                    uint8_t randBitReqBase = ffs(randSize) - 1;
                                    uint8_t randSizeHigh = flp2(randSize - populationSize);
                                    if (populationSize == 1)
                                    {
                                        randVal = 0;
                                        randBitReq = 0;
                                    }
                                    else
                                    {
                                        if (randBitReq == -1) randBitReq = randBitReqBase;
                                        uint8_t bit = __rdtsc() & 0x01; // keep LSB (least significant bit)
                                        randVal = (randVal << 1) + bit;
                                        randBitReq -= 1;
                                    }
                                    if (randBitReq == 0)
                                    {
                                        if (randVal < populationSize)  // random result within bound
                                        {
                                            int counter = 0;
                                            for (int senaidx = 0; senaidx < group(rordata, G_SENA).elems; senaidx++)
                                            {
                                                if (
                                                    (*(A_SENA_CNGR_t*)valabsaddr(rordata, G_SENA, A_SENA_CNGR, senaidx) != (A_SENA_CNGR_t)(G_DECK))
                                                    ||
                                                    (*(A_SENA_CNNR_t*)valabsaddr(rordata, G_SENA, A_SENA_CNNR, senaidx) != (A_SENA_CNNR_t)(0))
                                                ) continue;
                                                if (counter == randVal)
                                                {

                                                    *(A_SENA_ALIG_t*)valabsaddr(rordata, G_SENA, A_SENA_ALIG, senaidx) = (A_SENA_ALIG_t)(factidx);
                                                    *(A_SENA_CNGR_t*)valabsaddr(rordata, G_SENA, A_SENA_CNGR, senaidx) = (A_SENA_CNGR_t)(G_FORU);
                                                    *(A_SENA_CNNR_t*)valabsaddr(rordata, G_SENA, A_SENA_CNNR, senaidx) = (A_SENA_CNNR_t)(0);
                                                    break;
                                                }
                                                counter += 1;
                                            }
                                            randVal = 0;
                                            randBitReq -= 1;
                                        }
                                        else if ((0 < randSizeHigh) && ((randSize - randSizeHigh) <= randVal))  // keep some bits for next attempt if possible
                                        {
                                            randVal -= (randSize - randSizeHigh);
                                            uint8_t randBitReqBaseHigh = ffs(randSizeHigh) - 1;
                                            randBitReq = randBitReqBase - randBitReqBaseHigh;
                                        }
                                        else
                                        {
                                            randVal = 0;
                                            randBitReq -= 1;
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
                        else if (selected == -501000)
                        {
                            selected = -1;
                        }
                        // SELECTION
                        if (0 <= selected) // draw selected senator on top off all other
                        {
                            if
                            (
                                (currentGesture == GESTURE_NONE || currentGesture == GESTURE_SWIPE_RIGHT || currentGesture == GESTURE_SWIPE_LEFT || currentGesture == GESTURE_SWIPE_UP || currentGesture == GESTURE_SWIPE_DOWN)
                                &&
                                (lastGesture == GESTURE_HOLD || lastGesture == GESTURE_DRAG || lastGesture == GESTURE_TAP)
                            )
                            {
                                selected = -1;
                            }
                            else
                            {
                                int senaidx = selected;  // for code coherence
                                r_senator.x = mouse.x - selectedvector.x;
                                r_senator.y = mouse.y - selectedvector.y;
                                DrawRectangleRec(r_senator, COLOR_MOUSEDRAG);
                                sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_IDNR, senaidx));
                                DrawFont1(str, ((Rectangle){r_senator.x + RECT_SEN_ID_X, r_senator.y + Font2RectH - Font1RectH, RECT_SEN_ID_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextRight, ((Vector2){1, -8}));
                                DrawFont2((char*)(valabsaddr(rordata, G_SENA, A_SENA_NAME, senaidx)), ((Rectangle){r_senator.x + RECT_SEN_NAME_X, r_senator.y, RECT_SEN_NAME_WIDTH, Font2RectH}), COLOR_BLACKCARDTEXT, TextLeft, ((Vector2){0, 0}));
                                sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_MIL1, senaidx));
                                DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_M_X, r_senator.y, RECT_SEN_M_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                                sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_ORA1, senaidx));
                                DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_O_X, r_senator.y, RECT_SEN_O_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                                sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_LOY1, senaidx));
                                DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_L_X, r_senator.y, RECT_SEN_L_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                                sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_INF1, senaidx));
                                DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_I_X, r_senator.y, RECT_SEN_I_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                                A_SENA_POP1_t pop = *valabsaddr(rordata, G_SENA, A_SENA_POP1, senaidx);
                                if (pop != 0) {sprintf(str, "%i", pop); DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_P_X, r_senator.y, RECT_SEN_P_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));}
                            }
                        }
                    } break;

                    case SPHS_PREP_TEMPORARYROMECONSUL:
                    {
                        // PREP
                        uint8_t* p_temporaryRomeConsulFlags = (A_ERAR_TERC_t*)(valabsaddr(rordata, G_ERAR, A_ERAR_TERC, game->erai));
                        uint16_t numFactions = *(A_GAME_NFAC_t*)val0absaddr(rordata, G_GAME, A_GAME_NFAC) - 1;  // first "faction" (unaligned) is excluded
                        uint8_t factionSenatorCounts[FACT_ELEMCOUNT] = {0};
                        for (int factidx = 1; factidx < numFactions + 1; factidx++)
                        {
                            for (int senaidx = 0; senaidx < group(rordata, G_SENA).elems; senaidx++)
                            {
                                if ( *((A_SENA_ALIG_t*)(val0absaddr(rordata, G_SENA, A_SENA_ALIG))+senaidx) == (A_SENA_ALIG_t)(factidx) ) factionSenatorCounts[factidx] += 1;
                            }
                        }
                        uint8_t senatorCount = 0;
                        for (int factidx = 0; factidx < FACT_ELEMCOUNT; factidx++) senatorCount += factionSenatorCounts[factidx];
                        if (((*p_temporaryRomeConsulFlags) & 0x82) == 0x02)
                        {
                            A_SENA_IDNR_t senatorLowestId = 255;
                            for (int senaidx = 0; senaidx < group(rordata, G_SENA).elems; senaidx++)
                            {
                                if (0 <= *(A_SENA_ALIG_t*)valabsaddr(rordata, G_SENA, A_SENA_ALIG, senaidx))
                                {
                                    A_SENA_IDNR_t senatorId = *(A_SENA_IDNR_t*)valabsaddr(rordata, G_SENA, A_SENA_IDNR, senaidx);
                                    senatorLowestId = min(senatorLowestId, senatorId);
                                }
                            }
                            for (int senaidx = 0; senaidx < group(rordata, G_SENA).elems; senaidx++)
                            {
                                if (0 <= *(A_SENA_ALIG_t*)valabsaddr(rordata, G_SENA, A_SENA_ALIG, senaidx))
                                {
                                    A_SENA_IDNR_t senatorId = *(A_SENA_IDNR_t*)valabsaddr(rordata, G_SENA, A_SENA_IDNR, senaidx);
                                    if (senatorId == senatorLowestId)
                                    {
                                        A_SENA_OFFI_t* p_senatorOffice = (A_SENA_OFFI_t*)valabsaddr(rordata, G_SENA, A_SENA_OFFI, senaidx);
                                        *p_senatorOffice = (A_SENA_OFFI_t)(OFFICE_ROME_CONSUL);
                                        *(A_SENA_INF1_t*)valabsaddr(rordata, G_SENA, A_SENA_INF1, senaidx) += *(A_OFFI_INFL_t*)valabsaddr(rordata, G_OFFI, A_OFFI_INFL, OFFICE_ROME_CONSUL);
                                        *(A_SENA_PRIC_t*)valabsaddr(rordata, G_SENA, A_SENA_PRIC, senaidx) = (A_SENA_PRIC_t)(1);
                                        *p_temporaryRomeConsulFlags = *p_temporaryRomeConsulFlags | 0x80;
                                        *(A_HRAO_CNGR_t*)val0absaddr(rordata, G_HRAO, A_HRAO_CNGR) = (A_HRAO_CNGR_t)(G_SENA);
                                        *(A_HRAO_CNNR_t*)val0absaddr(rordata, G_HRAO, A_HRAO_CNNR) = (A_HRAO_CNNR_t)(senaidx);
                                    }
                                }
                            }
                        }
                        int temporaryRomeConsulIdx = -1;
                        for (int senaidx = 0; senaidx < group(rordata, G_SENA).elems; senaidx++)
                        {
                            A_SENA_OFFI_t* p_senatorOffice = (A_SENA_OFFI_t*)valabsaddr(rordata, G_SENA, A_SENA_OFFI, senaidx);
                            if (*p_senatorOffice == (A_SENA_OFFI_t)(OFFICE_ROME_CONSUL)) {temporaryRomeConsulIdx = senaidx; break;}
                        }
                        int hraoIdx = -1;
                        if (0 <= temporaryRomeConsulIdx) hraoIdx = temporaryRomeConsulIdx;
                        else if (*(A_HRAO_CNGR_t*)val0absaddr(rordata, G_HRAO, A_HRAO_CNGR) == (A_HRAO_CNGR_t)(G_SENA)) hraoIdx = *(A_HRAO_CNNR_t*)val0absaddr(rordata, G_HRAO, A_HRAO_CNNR);
                        else
                        {
                            uint32_t maxWeight = 0;
                            for (int senaidx = 0; senaidx < group(rordata, G_SENA).elems; senaidx++)
                            {
                                if ( *((A_SENA_ALIG_t*)(val0absaddr(rordata, G_SENA, A_SENA_ALIG))+senaidx) == (A_SENA_ALIG_t)(-1) ) continue;  // must be aligned
                                // must be in rome
                                uint32_t weight = 100000 * (*valabsaddr(rordata, G_SENA, A_SENA_INF1, senaidx)) + 1000 * (*valabsaddr(rordata, G_SENA, A_SENA_ORA1, senaidx)) + 500 - (*valabsaddr(rordata, G_SENA, A_SENA_IDNR, senaidx));
                                if (maxWeight < weight) {hraoIdx = senaidx; maxWeight = weight;}
                            }
                            *(A_HRAO_CNGR_t*)val0absaddr(rordata, G_HRAO, A_HRAO_CNGR) = (A_HRAO_CNGR_t)(G_SENA);
                            *(A_HRAO_CNNR_t*)val0absaddr(rordata, G_HRAO, A_HRAO_CNNR) = (A_HRAO_CNNR_t)(hraoIdx);
                        }
                        // TITLE
                        Rectangle r_title = {0, 0, screenWidth, TITLEHEIGHT};
                        DrawRectangleRec(r_title, COLOR_TITLEBACKGROUND);
                        DrawTitle("TEMPORARY ROME CONSUL", r_title, COLOR_TITLETEXT, TextLeft);
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
                        for (int factidx = 1; factidx < numFactions + 1; factidx++)
                        {
                            A_FACT_LEGR_t leadergroup = *valabsaddr(rordata, G_FACT, A_FACT_LEGR, factidx);
                            A_FACT_LENR_t leaderidx = *valabsaddr(rordata, G_FACT, A_FACT_LENR, factidx);
                            Rectangle r_faction = {r_senator.x, r_senator.y, r_senator.width, r_senator.height + (Font2RectH + PAD) * factionSenatorCounts[factidx]};
                            DrawRectangleRec(r_faction, COLOR_BACKGROUNDAREA);
                            DrawRectangleRec(r_senator, COLOR_FACTION);
                            DrawFont2(TextToUpper((char*)(valabsaddr(rordata, G_FACT, A_FACT_NAME, factidx))), r_senator, COLOR_FACTIONTEXT, TextLeft, ((Vector2){0, 0}));
                            for (int senaidx = 0; senaidx < group(rordata, G_SENA).elems; senaidx++)
                            {
                                if ( *((A_SENA_ALIG_t*)(val0absaddr(rordata, G_SENA, A_SENA_ALIG))+senaidx) != (A_SENA_ALIG_t)(factidx) ) continue;  // typecast is a must!; not in this faction
                                A_SENA_OFFI_t* p_senatorOffice = (A_SENA_OFFI_t*)valabsaddr(rordata, G_SENA, A_SENA_OFFI, senaidx);
                                r_senator.y += (Font2RectH + PAD);
                                if ((CheckCollisionPointRec(mouse, r_senator)))
                                {
                                    if (currentGesture != lastGesture && currentGesture == GESTURE_TAP)
                                    {
                                        bool assign = (senaidx != temporaryRomeConsulIdx);
                                        if (0 <= temporaryRomeConsulIdx)
                                        {
                                            *(A_SENA_OFFI_t*)valabsaddr(rordata, G_SENA, A_SENA_OFFI, temporaryRomeConsulIdx) = (A_SENA_OFFI_t)(0);
                                            *(A_SENA_INF1_t*)valabsaddr(rordata, G_SENA, A_SENA_INF1, temporaryRomeConsulIdx) -= *(A_OFFI_INFL_t*)valabsaddr(rordata, G_OFFI, A_OFFI_INFL, OFFICE_ROME_CONSUL);
                                            *(A_SENA_PRIC_t*)valabsaddr(rordata, G_SENA, A_SENA_PRIC, temporaryRomeConsulIdx) =  (A_SENA_PRIC_t)(0);
                                            *p_temporaryRomeConsulFlags = *p_temporaryRomeConsulFlags & 0x7F;
                                            uint32_t maxWeight = 0;
                                            for (int senaidx = 0; senaidx < group(rordata, G_SENA).elems; senaidx++)
                                            {
                                                if ( *((A_SENA_ALIG_t*)(val0absaddr(rordata, G_SENA, A_SENA_ALIG))+senaidx) == (A_SENA_ALIG_t)(-1) ) continue;  // must be aligned
                                                // must be in rome
                                                uint32_t weight = 100000 * (*valabsaddr(rordata, G_SENA, A_SENA_INF1, senaidx)) + 1000 * (*valabsaddr(rordata, G_SENA, A_SENA_ORA1, senaidx)) + 500 - (*valabsaddr(rordata, G_SENA, A_SENA_IDNR, senaidx));
                                                if (maxWeight < weight) {hraoIdx = senaidx; maxWeight = weight;}
                                            }
                                            *(A_HRAO_CNGR_t*)val0absaddr(rordata, G_HRAO, A_HRAO_CNGR) = (A_HRAO_CNGR_t)(G_SENA);
                                            *(A_HRAO_CNNR_t*)val0absaddr(rordata, G_HRAO, A_HRAO_CNNR) = (A_HRAO_CNNR_t)(hraoIdx);
                                        }
                                        if (assign)
                                        {
                                            *p_senatorOffice = (A_SENA_OFFI_t)(OFFICE_ROME_CONSUL);
                                            *(A_SENA_INF1_t*)valabsaddr(rordata, G_SENA, A_SENA_INF1, senaidx) += *(A_OFFI_INFL_t*)valabsaddr(rordata, G_OFFI, A_OFFI_INFL, OFFICE_ROME_CONSUL);
                                            *(A_SENA_PRIC_t*)valabsaddr(rordata, G_SENA, A_SENA_PRIC, senaidx) = (A_SENA_PRIC_t)(1);
                                            temporaryRomeConsulIdx = senaidx;
                                            *p_temporaryRomeConsulFlags = *p_temporaryRomeConsulFlags | 0x80;
                                            *(A_HRAO_CNGR_t*)val0absaddr(rordata, G_HRAO, A_HRAO_CNGR) = (A_HRAO_CNGR_t)(G_SENA);
                                            *(A_HRAO_CNNR_t*)val0absaddr(rordata, G_HRAO, A_HRAO_CNNR) = (A_HRAO_CNNR_t)(senaidx);
                                        }
                                        DrawRectangleRec(r_senator, COLOR_CLICKED);
                                    }
                                    else
                                    {
                                        DrawRectangleRec(r_senator, COLOR_MOUSEHOVER_CLICKABLE);
                                    }
                                }
                                else
                                {
                                    DrawRectangleRec(r_senator, COLOR_BLACKCARDBACKGROUND);
                                }
                                if (senaidx == hraoIdx)
                                {
                                    Rectangle r_hrao = {r_senator.x + RECT_SEN_HRAO_X, r_senator.y, RECT_SEN_HRAO_WIDTH, r_senator.height};
                                    DrawRectangleRec(r_hrao, COLOR_HRAO);
                                    DrawFont2("H", r_hrao, COLOR_OFFICETEXT, TextCenter, ((Vector2){1, 0}));
                                }
                                if ((leadergroup == G_SENA && leaderidx == senaidx))
                                {
                                    DrawRectangleRec(((Rectangle){r_senator.x + RECT_SEN_ID_X, r_senator.y, RECT_SEN_ID_WIDTH + 2, r_senator.height}), COLOR_FACTION);
                                    DrawRectangleRec(((Rectangle){r_senator.x + RECT_SEN_ID_X + 1, r_senator.y + r_senator.height - 4, RECT_SEN_ID_WIDTH, 3}), COLOR_BLACKCARDTEXT);
                                }
                                sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_IDNR, senaidx));
                                DrawFont1(str, ((Rectangle){r_senator.x + RECT_SEN_ID_X, r_senator.y + Font2RectH - Font1RectH, RECT_SEN_ID_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextRight, ((Vector2){1, -8}));
                                DrawFont2((char*)(valabsaddr(rordata, G_SENA, A_SENA_NAME, senaidx)), ((Rectangle){r_senator.x + RECT_SEN_NAME_X, r_senator.y, RECT_SEN_NAME_WIDTH, Font2RectH}), COLOR_BLACKCARDTEXT, TextLeft, ((Vector2){0, 0}));
                                sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_MIL1, senaidx));
                                if (*p_senatorOffice == (A_SENA_OFFI_t)(OFFICE_ROME_CONSUL))
                                {
                                    Rectangle r_office = {r_senator.x + RECT_SEN_OFF_X, r_senator.y, RECT_SEN_OFF_WIDTH, r_senator.height};
                                    DrawRectangleRec(r_office, COLOR_OFFICE);
                                    DrawFont2((char*)(valabsaddr(rordata, G_OFFI, A_OFFI_SNAM, OFFICE_ROME_CONSUL)), r_office, COLOR_OFFICETEXT, TextCenter, ((Vector2){0, 0}));
                                }
                                DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_M_X, r_senator.y, RECT_SEN_M_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                                sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_ORA1, senaidx));
                                DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_O_X, r_senator.y, RECT_SEN_O_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                                sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_LOY1, senaidx));
                                DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_L_X, r_senator.y, RECT_SEN_L_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                                sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_INF1, senaidx));
                                DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_I_X, r_senator.y, RECT_SEN_I_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                                A_SENA_POP1_t pop = *valabsaddr(rordata, G_SENA, A_SENA_POP1, senaidx);
                                if (pop != 0) {sprintf(str, "%i", pop); DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_P_X, r_senator.y, RECT_SEN_P_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));}
                                if (0 < *valabsaddr(rordata, G_SENA, A_SENA_PRIC, senaidx)) DrawFont2("PC", ((Rectangle){r_senator.x + RECT_SEN_PRIC_X, r_senator.y, RECT_SEN_PRIC_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
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
                                uint8_t temporaryromeconsulflags = *(A_ERAR_TERC_t*)(valabsaddr(rordata, G_ERAR, A_ERAR_TERC, game->erai));
                                if ((temporaryromeconsulflags & 0x05) == 0x00)
                                {
                                    *(A_GAME_SPHS_t*)(val0absaddr(rordata, G_GAME, A_GAME_SPHS)) = (A_GAME_SPHS_t)SPHS_PREP_SELECTFACTIONLEADERS;
                                }
                                else
                                {
                                    *(A_GAME_SPHS_t*)(val0absaddr(rordata, G_GAME, A_GAME_SPHS)) = (A_GAME_SPHS_t)SPHS_PREP_INITIALFACTIONPHASE;
                                }
                                selected = -1;
                                framesCounter = 0;
                            }
                            else
                            {
                                DrawRectangleRounded(r_button, 0.2f, 10, COLOR_MOUSEHOVER_CLICKABLE);
                            }
                        }
                        else DrawRectangleRounded(r_button, 0.2f, 10, COLOR_BUTTONBACKGROUND);
                        DrawRectangleRoundedLines(r_button, 0.2f, 10, 2, COLOR_BUTTONOUTLINE);
                        DrawFont3("NEXT", r_button, COLOR_BUTTONTEXT, TextCenter, ((Vector2){0, 1}));
                        // RANDOM BUTTON
                        if (((*p_temporaryRomeConsulFlags) & 0x82) == 0x00)
                        {
                            r_button.width = UNITCLAMP(6 * Font3HUnit);
                            r_button.x -= 2 * UNIT + r_button.width;
                            if (selected == -501000)
                            {
                                if
                                (
                                    (currentGesture == GESTURE_NONE || currentGesture == GESTURE_SWIPE_RIGHT || currentGesture == GESTURE_SWIPE_LEFT || currentGesture == GESTURE_SWIPE_UP || currentGesture == GESTURE_SWIPE_DOWN)
                                    &&
                                    (lastGesture == GESTURE_HOLD || lastGesture == GESTURE_DRAG || lastGesture == GESTURE_TAP)
                                )
                                {
                                    selected = -1;
                                }
                            }
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
                                    strcpy(str, "drag and shake it");
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
                                uint8_t populationSize = senatorCount; // for code coherence
                                r_button.x = mouse.x - selectedvector.x;
                                r_button.y = mouse.y - selectedvector.y;
                                DrawRectangleRounded(r_button, 0.2f, 10, COLOR_MOUSEDRAG);
                                DrawRectangleRoundedLines(r_button, 0.2f, 10, 2, COLOR_BUTTONOUTLINE);
                                DrawFont3("RANDOM", r_button, COLOR_MOUSEDRAGTEXT, TextCenter, ((Vector2){0, 1}));
                                if (currentGesture == GESTURE_DRAG) mousedelta = GetGestureDragVector();
                                else mousedelta = GetMouseDelta();
                                if (mousedelta.x != (float)(0) || mousedelta.y != (float)(0))
                                {
                                    uint8_t randSize = clp2(populationSize);
                                    uint8_t randBitReqBase = ffs(randSize) - 1;
                                    uint8_t randSizeHigh = flp2(randSize - populationSize);
                                    if (populationSize == 1)
                                    {
                                        randVal = 0;
                                        randBitReq = 0;
                                    }
                                    else
                                    {
                                        if (randBitReq == -1) randBitReq = randBitReqBase;
                                        uint8_t bit = __rdtsc() & 0x01; // keep LSB (least significant bit)
                                        randVal = (randVal << 1) + bit;
                                        randBitReq -= 1;
                                    }
                                    if (randBitReq == 0)
                                    {
                                        if (randVal < populationSize)  // random result within bound
                                        {
                                            int counter = 0;
                                            for (int senaidx = 0; senaidx < group(rordata, G_SENA).elems; senaidx++)
                                            {
                                                if (0 <= *(A_SENA_ALIG_t*)valabsaddr(rordata, G_SENA, A_SENA_ALIG, senaidx))
                                                {
                                                    if (counter == randVal)
                                                    {
                                                        if (0 <= temporaryRomeConsulIdx)
                                                        {
                                                            *(A_SENA_OFFI_t*)valabsaddr(rordata, G_SENA, A_SENA_OFFI, temporaryRomeConsulIdx) = (A_SENA_OFFI_t)(0);
                                                            *(A_SENA_INF1_t*)valabsaddr(rordata, G_SENA, A_SENA_INF1, temporaryRomeConsulIdx) -= *(A_OFFI_INFL_t*)valabsaddr(rordata, G_OFFI, A_OFFI_INFL, OFFICE_ROME_CONSUL);
                                                            *(A_SENA_PRIC_t*)valabsaddr(rordata, G_SENA, A_SENA_PRIC, temporaryRomeConsulIdx) =  (A_SENA_PRIC_t)(0);
                                                        }
                                                        A_SENA_OFFI_t* p_senatorOffice = (A_SENA_OFFI_t*)valabsaddr(rordata, G_SENA, A_SENA_OFFI, senaidx);
                                                        *p_senatorOffice = (A_SENA_OFFI_t)(OFFICE_ROME_CONSUL);
                                                        *(A_SENA_INF1_t*)valabsaddr(rordata, G_SENA, A_SENA_INF1, senaidx) += *(A_OFFI_INFL_t*)valabsaddr(rordata, G_OFFI, A_OFFI_INFL, OFFICE_ROME_CONSUL);
                                                        *(A_SENA_PRIC_t*)valabsaddr(rordata, G_SENA, A_SENA_PRIC, senaidx) = (A_SENA_PRIC_t)(1);
                                                        temporaryRomeConsulIdx = senaidx;
                                                        *p_temporaryRomeConsulFlags = *p_temporaryRomeConsulFlags | 0x80;
                                                        break;
                                                    }
                                                    counter += 1;
                                                }

                                            }
                                            randVal = 0;
                                            randBitReq -= 1;
                                        }
                                        else if ((0 < randSizeHigh) && ((randSize - randSizeHigh) <= randVal))  // keep some bits for next attempt if possible
                                        {
                                            randVal -= (randSize - randSizeHigh);
                                            uint8_t randBitReqBaseHigh = ffs(randSizeHigh) - 1;
                                            randBitReq = randBitReqBase - randBitReqBaseHigh;
                                        }
                                        else
                                        {
                                            randVal = 0;
                                            randBitReq -= 1;
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
                        else if (selected == -501000)
                        {
                            selected = -1;
                        }
                    } break;

                    case SPHS_PREP_SELECTFACTIONLEADERS:
                    {
                        // PREP
                        uint8_t* p_temporaryRomeConsulFlags = (A_ERAR_TERC_t*)(valabsaddr(rordata, G_ERAR, A_ERAR_TERC, game->erai));
                        uint16_t numFactions = *(A_GAME_NFAC_t*)val0absaddr(rordata, G_GAME, A_GAME_NFAC) - 1;  // first "faction" (unaligned) is excluded
                        uint8_t factionSenatorCounts[FACT_ELEMCOUNT] = {0};
                        for (int factidx = 1; factidx < numFactions + 1; factidx++)
                        {
                            for (int senaidx = 0; senaidx < group(rordata, G_SENA).elems; senaidx++)
                            {
                                if ( *((A_SENA_ALIG_t*)(val0absaddr(rordata, G_SENA, A_SENA_ALIG))+senaidx) == (A_SENA_ALIG_t)(factidx) ) factionSenatorCounts[factidx] += 1;
                            }
                        }
                        uint8_t factionLeaderCounts[FACT_ELEMCOUNT] = {0};
                        for (int factidx = 1; factidx < numFactions + 1; factidx++)
                        {
                            A_FACT_LEGR_t leadergroup = *valabsaddr(rordata, G_FACT, A_FACT_LEGR, factidx);
                            A_FACT_LENR_t leaderidx = *valabsaddr(rordata, G_FACT, A_FACT_LENR, factidx);
                            for (int senaidx = 0; senaidx < group(rordata, G_SENA).elems; senaidx++)
                            {
                                if ((leadergroup == G_SENA && leaderidx == senaidx)) factionLeaderCounts[factidx] += 1;
                            }
                        }
                        uint8_t resolved = 0;
                        for (int factidx = 1; factidx < numFactions + 1; factidx++)
                        {
                            if (factionLeaderCounts[factidx] == 1 || factionSenatorCounts[factidx] == 0) resolved += 1;
                        }
                        int temporaryRomeConsulIdx = -1;
                        for (int senaidx = 0; senaidx < group(rordata, G_SENA).elems; senaidx++)
                        {
                            A_SENA_OFFI_t* p_senatorOffice = (A_SENA_OFFI_t*)valabsaddr(rordata, G_SENA, A_SENA_OFFI, senaidx);
                            if (*p_senatorOffice == (A_SENA_OFFI_t)(OFFICE_ROME_CONSUL)) {temporaryRomeConsulIdx = senaidx; break;}
                        }
                        int hraoIdx = -1;
                        if (0 <= temporaryRomeConsulIdx) hraoIdx = temporaryRomeConsulIdx;
                        else if (*(A_HRAO_CNGR_t*)val0absaddr(rordata, G_HRAO, A_HRAO_CNGR) == (A_HRAO_CNGR_t)(G_SENA)) hraoIdx = *(A_HRAO_CNNR_t*)val0absaddr(rordata, G_HRAO, A_HRAO_CNNR);
                        else
                        {
                            uint32_t maxWeight = 0;
                            for (int senaidx = 0; senaidx < group(rordata, G_SENA).elems; senaidx++)
                            {
                                if ( *((A_SENA_ALIG_t*)(val0absaddr(rordata, G_SENA, A_SENA_ALIG))+senaidx) == (A_SENA_ALIG_t)(-1) ) continue;  // must be aligned
                                // must be in rome
                                uint32_t weight = 100000 * (*valabsaddr(rordata, G_SENA, A_SENA_INF1, senaidx)) + 1000 * (*valabsaddr(rordata, G_SENA, A_SENA_ORA1, senaidx)) + 500 - (*valabsaddr(rordata, G_SENA, A_SENA_IDNR, senaidx));
                                if (maxWeight < weight) {hraoIdx = senaidx; maxWeight = weight;}
                            }
                            *(A_HRAO_CNGR_t*)val0absaddr(rordata, G_HRAO, A_HRAO_CNGR) = (A_HRAO_CNGR_t)(G_SENA);
                            *(A_HRAO_CNNR_t*)val0absaddr(rordata, G_HRAO, A_HRAO_CNNR) = (A_HRAO_CNNR_t)(hraoIdx);
                        }
                        // TITLE
                        Rectangle r_title = {0, 0, screenWidth, TITLEHEIGHT};
                        DrawRectangleRec(r_title, COLOR_TITLEBACKGROUND);
                        DrawTitle("NOMINATE LEADERS FOR EACH FACTION", r_title, COLOR_TITLETEXT, TextLeft);
                        // FACTIONS
                        Rectangle r_header = {0, 0, RECT_SEN_WIDTH2, Font2RectH};
                        Rectangle r_senator = {0, 0, r_header.width, Font2RectH};
                        r_header.x = UNITCLAMP((screenWidth - (RECT_SEN_HRAO_WIDTH + RECT_SEN_WIDTH2 + PAD + 4 * UNIT + RECT_SEN_WIDTH + PAD - (RECT_SEN_OFF_WIDTH + PAD))) / 2 + RECT_SEN_HRAO_WIDTH);
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
                        for (int factidx = 1; factidx < numFactions + 1; factidx++)
                        {
                            A_FACT_LEGR_t leadergroup = *valabsaddr(rordata, G_FACT, A_FACT_LEGR, factidx);
                            A_FACT_LENR_t leaderidx = *valabsaddr(rordata, G_FACT, A_FACT_LENR, factidx);
                            Rectangle r_faction = {r_senator.x, r_senator.y, r_senator.width, r_senator.height + (Font2RectH + PAD) * factionSenatorCounts[factidx]};
                            DrawRectangleRec(r_faction, COLOR_BACKGROUNDAREA);
                            DrawRectangleRec(r_senator, COLOR_FACTION);
                            DrawFont2(TextToUpper((char*)(valabsaddr(rordata, G_FACT, A_FACT_NAME, factidx))), r_senator, COLOR_FACTIONTEXT, TextLeft, ((Vector2){0, 0}));
                            for (int senaidx = 0; senaidx < group(rordata, G_SENA).elems; senaidx++)
                            {
                                if ( *((A_SENA_ALIG_t*)(val0absaddr(rordata, G_SENA, A_SENA_ALIG))+senaidx) != (A_SENA_ALIG_t)(factidx) ) continue;  // typecast is a must!; not in this faction
                                A_SENA_OFFI_t* p_senatorOffice = (A_SENA_OFFI_t*)valabsaddr(rordata, G_SENA, A_SENA_OFFI, senaidx);
                                r_senator.y += (Font2RectH + PAD);
                                if ((leadergroup != G_SENA || leaderidx != senaidx) && (CheckCollisionPointRec(mouse, r_senator)))
                                {
                                    if (currentGesture != lastGesture && currentGesture == GESTURE_TAP)
                                    {
                                        leadergroup = G_SENA;
                                        leaderidx = senaidx;
                                        *(A_FACT_LEGR_t*)valabsaddr(rordata, G_FACT, A_FACT_LEGR, factidx) = (A_FACT_LEGR_t)(leadergroup);
                                        *(A_FACT_LENR_t*)valabsaddr(rordata, G_FACT, A_FACT_LENR, factidx) = (A_FACT_LENR_t)(leaderidx);
                                        DrawRectangleRec(r_senator, COLOR_CLICKED);
                                    }
                                    else
                                    {
                                        DrawRectangleRec(r_senator, COLOR_MOUSEHOVER_CLICKABLE);
                                    }
                                }
                                else
                                {
                                    DrawRectangleRec(r_senator, COLOR_BLACKCARDBACKGROUND);
                                }
                                if (senaidx == hraoIdx)
                                {
                                    Rectangle r_hrao = {r_senator.x + RECT_SEN_HRAO_X, r_senator.y, RECT_SEN_HRAO_WIDTH, r_senator.height};
                                    DrawRectangleRec(r_hrao, COLOR_HRAO);
                                    DrawFont2("H", r_hrao, COLOR_OFFICETEXT, TextCenter, ((Vector2){1, 0}));
                                }
                                if ((leadergroup == G_SENA && leaderidx == senaidx))
                                {
                                    DrawRectangleRec(((Rectangle){r_senator.x + RECT_SEN_ID_X, r_senator.y, RECT_SEN_ID_WIDTH + 2, r_senator.height}), COLOR_FACTION);
                                    DrawRectangleRec(((Rectangle){r_senator.x + RECT_SEN_ID_X + 1, r_senator.y + r_senator.height - 4, RECT_SEN_ID_WIDTH, 3}), COLOR_BLACKCARDTEXT);
                                }
                                sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_IDNR, senaidx));
                                DrawFont1(str, ((Rectangle){r_senator.x + RECT_SEN_ID_X, r_senator.y + Font2RectH - Font1RectH, RECT_SEN_ID_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextRight, ((Vector2){1, -8}));
                                DrawFont2((char*)(valabsaddr(rordata, G_SENA, A_SENA_NAME, senaidx)), ((Rectangle){r_senator.x + RECT_SEN_NAME_X, r_senator.y, RECT_SEN_NAME_WIDTH, Font2RectH}), COLOR_BLACKCARDTEXT, TextLeft, ((Vector2){0, 0}));
                                sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_MIL1, senaidx));
                                if (*p_senatorOffice == (A_SENA_OFFI_t)(OFFICE_ROME_CONSUL))
                                {
                                    Rectangle r_office = {r_senator.x + RECT_SEN_OFF_X, r_senator.y, RECT_SEN_OFF_WIDTH, r_senator.height};
                                    DrawRectangleRec(r_office, COLOR_OFFICE);
                                    DrawFont2((char*)(valabsaddr(rordata, G_OFFI, A_OFFI_SNAM, OFFICE_ROME_CONSUL)), r_office, COLOR_OFFICETEXT, TextCenter, ((Vector2){0, 0}));
                                }
                                DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_M_X, r_senator.y, RECT_SEN_M_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                                sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_ORA1, senaidx));
                                DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_O_X, r_senator.y, RECT_SEN_O_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                                sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_LOY1, senaidx));
                                DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_L_X, r_senator.y, RECT_SEN_L_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                                sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_INF1, senaidx));
                                DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_I_X, r_senator.y, RECT_SEN_I_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                                A_SENA_POP1_t pop = *valabsaddr(rordata, G_SENA, A_SENA_POP1, senaidx);
                                if (pop != 0) {sprintf(str, "%i", pop); DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_P_X, r_senator.y, RECT_SEN_P_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));}
                                if (0 < *valabsaddr(rordata, G_SENA, A_SENA_PRIC, senaidx)) DrawFont2("PC", ((Rectangle){r_senator.x + RECT_SEN_PRIC_X, r_senator.y, RECT_SEN_PRIC_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                            }
                            r_senator.y = r_faction.y + r_faction.height + 2 * UNIT + PAD;
                        }
                        // STATESMEN
                        Rectangle r_tooltip = r_header;
                        r_header.x += r_header.width + PAD + 4 * UNIT;
                        r_header.y = (r_title.height + PAD) + 3 * UNIT;
                        r_header.width = RECT_SEN_WIDTH - (RECT_SEN_OFF_WIDTH + PAD);
                        r_header.height = Font2RectH;
                        r_senator.x = r_header.x;
                        r_senator.y = r_header.y + 1 * (Font2RectH + PAD) + 2 * UNIT;
                        r_senator.width = r_header.width;
                        r_senator.height = Font2RectH;
                        r_tooltip.width += 4 * UNIT + r_header.width + PAD;
                        r_tooltip.height = 4 * (Font2RectH + PAD) - PAD;
                        r_tooltip.x -= RECT_SEN_HRAO_WIDTH;
                        r_tooltip.width += RECT_SEN_HRAO_WIDTH;
                        DrawRectangleRec(r_header, COLOR_FACTIONHEADER);
                        DrawFont2("STATESMEN", r_header, COLOR_FACTIONHEADERTEXT, TextLeft, ((Vector2){0, 0}));
                        r_header.x -= (RECT_SEN_OFF_WIDTH + PAD);  // statesmen list does not have office column, so I applied this dirty shift trick
                        DrawFont2("M", ((Rectangle){r_header.x + RECT_SEN_M_X, r_header.y, RECT_SEN_M_WIDTH, r_header.height}), COLOR_FACTIONHEADERTEXT, TextCenter, ((Vector2){0, 0}));
                        DrawFont2("O", ((Rectangle){r_header.x + RECT_SEN_O_X, r_header.y, RECT_SEN_O_WIDTH, r_header.height}), COLOR_FACTIONHEADERTEXT, TextCenter, ((Vector2){0, 0}));
                        DrawFont2("L", ((Rectangle){r_header.x + RECT_SEN_L_X, r_header.y, RECT_SEN_L_WIDTH, r_header.height}), COLOR_FACTIONHEADERTEXT, TextCenter, ((Vector2){0, 0}));
                        DrawFont2("I", ((Rectangle){r_header.x + RECT_SEN_I_X, r_header.y, RECT_SEN_I_WIDTH, r_header.height}), COLOR_FACTIONHEADERTEXT, TextCenter, ((Vector2){0, 0}));
                        DrawFont2("P", ((Rectangle){r_header.x + RECT_SEN_P_X, r_header.y, RECT_SEN_P_WIDTH, r_header.height}), COLOR_FACTIONHEADERTEXT, TextCenter, ((Vector2){0, 0}));
                        r_header.x = r_senator.x;
                        for (int era = 0; era < group(rordata, G_ERAR).elems; era++)
                        {
                            r_senator.x = r_header.x;
                            DrawRectangleRec(r_senator, COLOR_ERAHEADER);
                            DrawFont2(TextToUpper((char*)(valabsaddr(rordata, G_ERAR, A_ERAR_NAME, era))), r_senator, COLOR_ERAHEADERTEXT, TextLeft, ((Vector2){0, 0}));
                            for (int smanidx = 0; smanidx < group(rordata, G_SMAN).elems; smanidx++)
                            {
                                if ( *((A_SMAN_ERAI_t*)(valabsaddr(rordata, G_SMAN, A_SMAN_ERAI, smanidx))) != (A_SMAN_ERAI_t)(era) ) continue;  // typecast is a must!; not for this era
                                bool loy0 = ( (0 <= *((A_SMAN_L0W1_t*)(valabsaddr(rordata, G_SMAN, A_SMAN_L0W1, smanidx)))) || (0 <= *((A_SMAN_L0W2_t*)(valabsaddr(rordata, G_SMAN, A_SMAN_L0W2, smanidx)))) || (0 <= *((A_SMAN_L0W3_t*)(valabsaddr(rordata, G_SMAN, A_SMAN_L0W3, smanidx)))) || (0 <= *((A_SMAN_L0N1_t*)(valabsaddr(rordata, G_SMAN, A_SMAN_L0N1, smanidx)))) );
                                r_senator.x = r_header.x;
                                r_senator.y += (Font2RectH + PAD);
                                DrawRectangleRec(r_senator, COLOR_STATESMANBACKGROUND);
                                sprintf(str, "%d", *valabsaddr(rordata, G_SMAN, A_SMAN_IDNR, smanidx));
                                DrawFont1(str, ((Rectangle){r_senator.x + RECT_SEN_ID_X, r_senator.y + Font2RectH - Font1RectH, RECT_SEN_ID_WIDTH, r_senator.height}), COLOR_STATESMANTEXT, TextRight, ((Vector2){1, -8}));
                                DrawFont1(valabsaddr(rordata, G_SMAN, A_SMAN_IDCH, smanidx), ((Rectangle){r_senator.x + RECT_SEN_ID_X, r_senator.y, RECT_SEN_ID_WIDTH, r_senator.height}), COLOR_STATESMANTEXT, TextRight, ((Vector2){1, -4}));
                                Rectangle r_name = {r_senator.x + RECT_SEN_NAME_X, r_senator.y, RECT_SEN_NAME_WIDTH, Font2RectH};
                                DrawFont2((char*)(valabsaddr(rordata, G_SMAN, A_SMAN_SNAM, smanidx)), r_name, COLOR_STATESMANTEXT, TextLeft, ((Vector2){0, 0}));
                                if (CheckCollisionPointRec(mouse, r_name))
                                {
                                    r_tooltip.y = r_senator.y - 4 * (Font2RectH + PAD);
                                    DrawRectangleRec(r_tooltip, COLOR_TOOLTIPBACKGROUND);
                                    DrawRectangleLinesEx(((Rectangle){r_tooltip.x - 1, r_tooltip.y - 1, r_tooltip.width + 2, r_tooltip.height + 2}), 1.0f, COLOR_TOOLTIPOUTLINE);
                                    DrawFont2(valabsaddr(rordata, G_SMAN, A_SMAN_NAME, smanidx), ((Rectangle){r_tooltip.x, r_tooltip.y, r_tooltip.width, Font2RectH}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                                    DrawFont2(valabsaddr(rordata, G_SMAN, A_SMAN_TIME, smanidx), ((Rectangle){r_tooltip.x, r_tooltip.y + 1 * (Font2RectH + PAD), r_tooltip.width, Font2RectH}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                                    DrawTextBoxed(font2, valabsaddr(rordata, G_SMAN, A_SMAN_SPEC, smanidx), ((Rectangle){r_tooltip.x + Font2PaddingX, r_tooltip.y + 2 * (Font2RectH + PAD) - PAD, r_tooltip.width - Font2PaddingX, 3 * (Font2RectH + PAD) - PAD}), Font2H, Font2Spacing, true, COLOR_TOOLTIPTEXT, Font2H - 2);
                                }
                                r_senator.x -= (RECT_SEN_OFF_WIDTH + PAD);  // statesmen list does not have office column, so I applied this dirty shift trick
                                A_SMAN_MIL0_t mil = *valabsaddr(rordata, G_SMAN, A_SMAN_MIL0, smanidx);
                                if (0x10 <= mil)
                                {
                                    if (1 < (mil >> 4)) sprintf(str, "%dd6", (mil >> 4));
                                    else strcpy(str, "d6");
                                    DrawFont1(str, ((Rectangle){r_senator.x + RECT_SEN_M_X, r_senator.y, RECT_SEN_M_WIDTH, r_senator.height}), COLOR_STATESMANTEXT, TextCenter, ((Vector2){0, -4}));
                                    sprintf(str, "+%d", (mil & 0x0F));
                                    DrawFont1(str, ((Rectangle){r_senator.x + RECT_SEN_M_X, r_senator.y, RECT_SEN_M_WIDTH, r_senator.height}), COLOR_STATESMANTEXT, TextCenter, ((Vector2){0, 4}));
                                }
                                else
                                {
                                    sprintf(str, "%d", mil);
                                    DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_M_X, r_senator.y, RECT_SEN_M_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                                }
                                sprintf(str, "%d", *valabsaddr(rordata, G_SMAN, A_SMAN_ORA0, smanidx));
                                DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_O_X, r_senator.y, RECT_SEN_O_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                                sprintf(str, "%d", *valabsaddr(rordata, G_SMAN, A_SMAN_LOY0, smanidx));
                                DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_L_X, r_senator.y, RECT_SEN_L_WIDTH, r_senator.height}), (loy0) ? COLOR_STATESMANTEXT : COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                                sprintf(str, "%d", *valabsaddr(rordata, G_SMAN, A_SMAN_INF0, smanidx));
                                DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_I_X, r_senator.y, RECT_SEN_I_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));
                                A_SMAN_POP1_t pop = *valabsaddr(rordata, G_SMAN, A_SMAN_POP0, smanidx);
                                if (pop != 0) {sprintf(str, "%i", pop); DrawFont2(str, ((Rectangle){r_senator.x + RECT_SEN_P_X, r_senator.y, RECT_SEN_P_WIDTH, r_senator.height}), COLOR_BLACKCARDTEXT, TextCenter, ((Vector2){0, 0}));}
                            }
                            r_senator.y += 1 * (Font2RectH + PAD) + 2 * UNIT;
                        }
                        // NEXT BUTTON
                        if (resolved == numFactions)
                        {
                            Rectangle r_button = {screenWidth - 4 * UNIT - 4 * Font3HUnit, 2 * UNIT, UNITCLAMP(4 * Font3HUnit), TITLEHEIGHT - 4 * UNIT};
                            if (selected == -1 && CheckCollisionPointRec(mouse, r_button))
                            {
                                if (currentGesture == GESTURE_TAP)
                                {
                                    DrawRectangleRounded(r_button, 0.2f, 10, COLOR_CLICKED);
                                    uint8_t temporaryromeconsulflags = *(A_ERAR_TERC_t*)(valabsaddr(rordata, G_ERAR, A_ERAR_TERC, game->erai));
                                    if ((temporaryromeconsulflags & 0x05) == 0x04)
                                    {
                                        *(A_GAME_SPHS_t*)(val0absaddr(rordata, G_GAME, A_GAME_SPHS)) = (A_GAME_SPHS_t)SPHS_PREP_TEMPORARYROMECONSUL;
                                    }
                                    else
                                    {
                                        *(A_GAME_SPHS_t*)(val0absaddr(rordata, G_GAME, A_GAME_SPHS)) = (A_GAME_SPHS_t)SPHS_PREP_INITIALFACTIONPHASE;
                                    }
                                    selected = -1;
                                    framesCounter = 0;
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
                }
            } break;
        }
        }
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