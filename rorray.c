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
    SetTraceLogLevel(LOG_DEBUG); // TODO: DEBUG
    int debugvar = 0;
    char debugstr[256][256] = {0};

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

    bool rordataLoaded = false;
    unsigned char *rordata;

    RoR_Header_t *header;
    header = MemAlloc(sizeof(RoR_Header_t));
    header->phse = PhPickScenario;

    Vector2 mouse;
    Vector2 mousedelta;
    int currentGesture = GESTURE_NONE;
    int lastGesture = GESTURE_NONE;

    unsigned char randVal = 0;
    int8_t randBitReq = -1;

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