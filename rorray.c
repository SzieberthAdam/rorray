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


#define COLOR_BrownYellow     ((Color){0xCC, 0x99, 0x66, 0xFF})
#define COLOR_Yellow          ((Color){0xFF, 0xFF, 0x00, 0xFF})
#define COLOR_DarkCharcoal    ((Color){0x33, 0x33, 0x33, 0xFF})
#define COLOR_CopperRose      ((Color){0x99, 0x66, 0x66, 0xFF})
#define COLOR_Black           ((Color){0x00, 0x00, 0x00, 0xFF})
#define COLOR_ChineseSilver   ((Color){0xCC, 0xCC, 0xCC, 0xFF})
#define COLOR_DarkVanilla     ((Color){0xCC, 0xCC, 0x99, 0xFF})
#define COLOR_PeachOrange     ((Color){0xFF, 0xCC, 0x99, 0xFF})
#define COLOR_GraniteGray     ((Color){0x66, 0x66, 0x66, 0xFF})
#define COLOR_White           ((Color){0xFF, 0xFF, 0xFF, 0xFF})
#define COLOR_SpanishGray     ((Color){0x99, 0x99, 0x99, 0xFF})
#define COLOR_DeepCarminePink ((Color){0xFF, 0x33, 0x33, 0xFF})
#define COLOR_MediumVioletRed ((Color){0xCC, 0x00, 0x99, 0xFF})
#define COLOR_FrenchLilac     ((Color){0x99, 0x66, 0x99, 0xFF})
#define COLOR_SheenGreen      ((Color){0x99, 0xCC, 0x00, 0xFF})


#define COLOR_BIRREN_SCARLET ((Color){209, 33, 33, 255})
#define COLOR_BIRREN_IVORY_WHITE ((Color){234, 229, 224, 255})
#define COLOR_BIRREN_POMPEII_ROSE ((Color){192, 95, 79, 255})
#define COLOR_BIRREN_RICH_GOLD ((Color){210, 146, 48, 255})
#define COLOR_BIRREN_RICH_CORAL ((Color){231, 142, 112, 255})
#define COLOR_BIRREN_BRIGHT_COPPER ((Color){154, 48, 32, 255})
#define COLOR_BIRREN_NUBIAN_BROWN ((Color){100, 43, 36, 255})
#define COLOR_BIRREN_LIGHT_ROMAN_BLUE ((Color){136, 208, 232, 255})
#define COLOR_BIRREN_MEDIUM_ROMAN_BLUE ((Color){78, 148, 197, 255})
#define COLOR_BIRREN_DEEP_COBALT ((Color){26, 88, 163, 255})
#define COLOR_BIRREN_LIGHT_POMPEII_GREEN ((Color){164, 218, 194, 255})
#define COLOR_BIRREN_MEDIUM_POMPEII_GREEN ((Color){90, 161, 129, 255})
#define COLOR_BIRREN_DEEP_POMPEII_GREEN ((Color){40, 108, 67, 255})
#define COLOR_BIRREN_BONE_BLACK ((Color){32, 32, 32, 255})


#define COLOR_SCARLET ((Color){255, 39, 0, 255})
#define COLOR_IVORY_WHITE ((Color){242, 239, 222, 255})
#define COLOR_RICH_GOLD ((Color){236, 190, 7, 255})

#define COLOR_ANTIQUE_RUBY ((Color){127, 33, 34, 255})
#define COLOR_OLD_LACE ((Color){229, 219, 183, 255})

#define COLOR_BACKGROUND COLOR_DarkCharcoal
#define COLOR_BACKGROUNDAREA COLOR_Black
#define COLOR_BLACKCARDBACKGROUND COLOR_White
#define COLOR_BLACKCARDTEXT COLOR_Black
#define COLOR_BUTTONBACKGROUND COLOR_White
#define COLOR_CLICKED COLOR_PeachOrange
#define COLOR_BUTTONOUTLINE COLOR_Black
#define COLOR_BUTTONTEXT COLOR_Black
#define COLOR_FACTION COLOR_Yellow
#define COLOR_FACTIONHEADER COLOR_SpanishGray
#define COLOR_FACTIONHEADERTEXT COLOR_Black
#define COLOR_FACTIONTEXT COLOR_Black
#define COLOR_MOUSEDRAG  COLOR_BIRREN_LIGHT_ROMAN_BLUE
#define COLOR_MOUSEDRAGTEXT  COLOR_Black
#define COLOR_MOUSEHOVER_CLICKABLE COLOR_SheenGreen
#define COLOR_MOUSEHOVER_DRAGABLE  COLOR_FrenchLilac
#define COLOR_MOUSEHOVER_DROPTARGET  COLOR_MediumVioletRed
#define COLOR_MOUSEHOVER_EDITABLE  COLOR_CopperRose
#define COLOR_MOUSEHOVER_GAMEMASTER  MAGENTA
#define COLOR_MOUSEHOVER_SELECTABLE  COLOR_DarkVanilla
#define COLOR_OFFICE COLOR_DeepCarminePink
#define COLOR_HRAO COLOR_FrenchLilac
#define COLOR_OFFICETEXT COLOR_Black
#define COLOR_TITLEBACKGROUND COLOR_BrownYellow
#define COLOR_TITLETEXT COLOR_Black
#define COLOR_TOOLTIPBACKGROUND COLOR_PeachOrange
#define COLOR_TOOLTIPOUTLINE COLOR_Black
#define COLOR_TOOLTIPTEXT COLOR_Black

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

    unsigned int rordataLength = SIZE;
    unsigned char *rordata = LoadFileData("scenario.ror", &rordataLength);

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

        LABEL_GAME_TREE:
        switch (*(A_GAME_PHSE_t*)(val0absaddr(rordata, G_GAME, A_GAME_PHSE)))
        {
            // *(A_GAME_PHSE_t*)(val0absaddr(rordata, G_GAME, A_GAME_PHSE)) = (A_GAME_PHSE_t)1;
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
                        // BUTTON
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
                        uint8_t targetSenatorCount = (uint8_t)(*val0absaddr(rordata, G_RULE, A_RULE_NSEN));
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
                                uint8_t temporaryromeconsulflags = *(A_RULE_TERC_t*)(val0absaddr(rordata, G_RULE, A_RULE_TERC));
                                if (temporaryromeconsulflags == 0xFF || (temporaryromeconsulflags & 0x01)==0x01)
                                {
                                    *(A_GAME_SPHS_t*)(val0absaddr(rordata, G_GAME, A_GAME_SPHS)) = (A_GAME_SPHS_t)SPHS_PREP_SELECTFACTIONLEADERS;
                                }
                                else
                                {
                                    *(A_GAME_SPHS_t*)(val0absaddr(rordata, G_GAME, A_GAME_SPHS)) = (A_GAME_SPHS_t)SPHS_PREP_TEMPORARYROMECONSUL;
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
                        uint8_t* p_temporaryRomeConsulFlags = (A_RULE_TERC_t*)(val0absaddr(rordata, G_RULE, A_RULE_TERC));
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
                                uint8_t temporaryromeconsulflags = *(A_RULE_TERC_t*)(val0absaddr(rordata, G_RULE, A_RULE_TERC));
                                if (temporaryromeconsulflags != 0xFF && (temporaryromeconsulflags & 0x01)==0x01)
                                {
                                    *(A_GAME_SPHS_t*)(val0absaddr(rordata, G_GAME, A_GAME_SPHS)) = (A_GAME_SPHS_t)SPHS_PREP_INITIALFACTIONPHASE;
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
                        uint8_t* p_temporaryRomeConsulFlags = (A_RULE_TERC_t*)(val0absaddr(rordata, G_RULE, A_RULE_TERC));
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
                        // NEXT BUTTON
                        if (resolved == numFactions)
                        {
                            Rectangle r_button = {screenWidth - 4 * UNIT - 4 * Font3HUnit, 2 * UNIT, UNITCLAMP(4 * Font3HUnit), TITLEHEIGHT - 4 * UNIT};
                            if (selected == -1 && CheckCollisionPointRec(mouse, r_button))
                            {
                                if (currentGesture == GESTURE_TAP)
                                {
                                    DrawRectangleRounded(r_button, 0.2f, 10, COLOR_CLICKED);
                                    uint8_t temporaryromeconsulflags = *(A_RULE_TERC_t*)(val0absaddr(rordata, G_RULE, A_RULE_TERC));
                                    if (temporaryromeconsulflags != 0xFF && (temporaryromeconsulflags & 0x01)==0x01)
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