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
#define PADDING   1
#define UNIT  15

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

#define COLOR_TITLEBACKGROUND DARKGRAY
#define COLOR_BLACKCARDBG WHITE
#define COLOR_BUTTON DARKGRAY
#define COLOR_BUTTONBACKGROUND BLACK
#define COLOR_BUTTONCLICKED YELLOW
#define COLOR_BUTTONOUTLINE WHITE
#define COLOR_BUTTONTEXT ORANGE
#define COLOR_FACTIONHEADER DARKGRAY
#define COLOR_MOUSEDRAG  ORANGE
#define COLOR_MOUSEDRAGTEXT  BLACK
#define COLOR_MOUSEHOVER_DRAGABLE  DARKBLUE
#define COLOR_MOUSEHOVER_DROPTARGET  DARKGREEN
#define COLOR_MOUSEHOVER_EDITABLE  ORANGE
#define COLOR_MOUSEHOVER_GAMEMASTER  MAGENTA
#define COLOR_MOUSEHOVER_SELECTABLE  DARKGREEN
#define COLOR_OFFICE RED
#define COLOR_TITLETEXT ORANGE

#define Font1H font1.baseSize
#define Font2H font2.baseSize
#define Font3H font3.baseSize
#define Font1Spacing 1
#define Font2Spacing 2
#define Font3Spacing 3
#define Font1PaddingX 1
#define Font2PaddingX 4
#define Font3PaddingX 3
#define DrawFont2(text, box, tint, align) (DrawTextEx2(font2, text, box, Font2H, Font2Spacing, (Vector2){0, 1}, (Vector2){Font2PaddingX, 0}, tint, align))
#define DrawFont3(text, box, tint, align) (DrawTextEx2(font3, text, box, Font3H, Font3Spacing, (Vector2){0, 1}, (Vector2){Font3PaddingX, 0}, tint, align))


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
    Rectangle r = {topleftpoint.x, topleftpoint.y, width_units * RIGHT - PADDING, height_units * DOWN - PADDING};
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

void DrawTextEx2(Font font, const char *text, Rectangle box, float fontSize, float spacing, Vector2 offset, Vector2 padding, Color tint, TextAlignment align)
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
    Rectangle r;
    char str[256];

    uint16_t windowedScreenWidth = 1960;
    uint16_t windowedScreenHeight = 1080;
    uint16_t screenRights = (windowedScreenWidth - 2 * PADDING) / RIGHT;
    uint16_t screenDowns = (windowedScreenHeight - 2 * PADDING) / DOWN;
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

    unsigned char randbits = 0;
    unsigned char newrandbits = 0;
    uint8_t randbitreq = 0;

    uint8_t newentropy;
    uint8_t entropy[256] = {0};
    uint32_t newentropyidx = 0;
    uint8_t newentropynextbit = 0;
    uint32_t entropyidx = 0;
    uint8_t entropynextbit = 0;

    SetTraceLogLevel(LOG_DEBUG);

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

        if (currentGesture == GESTURE_DRAG)
        {
            mousedelta = GetGestureDragVector();
        }
        else
        {
            mousedelta = GetMouseDelta();
        }
        if (mousedelta.x != (float)(0) || mousedelta.y != (float)(0))
        {
            newentropy = (uint8_t)(__rdtsc() & 0x01); // keep last bit
            entropy[(uint8_t)(newentropyidx % 256)] += (newentropy << newentropynextbit);
            newentropynextbit += 1;
            if (newentropynextbit == 8)
            {
                newentropynextbit = 0;
                if (newentropyidx == 255)
                {
                    SaveFileData("entropy.dat", entropy, 256);
                }
                newentropyidx += 1;
            }
        }
        else
        {
            newentropy = 0xFF;
        }

        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawRectangle(0, 0, screenWidth, screenHeight, BLACK);

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
                        Rectangle r_title = {0, 0, screenWidth, 3 * UNIT - 1};
                        DrawRectangleRec(r_title, COLOR_TITLEBACKGROUND);
                        DrawFont3("NAME THE PLAYING FACTIONS OR DELETE NAMES FOR EXCLUSION", r_title, COLOR_TITLETEXT, TextCenter);
                        // FACTIONS
                        uint8_t maxLetterCount = ((attr(rordata, G_FACT, A_FACT_NAME).type & 0x3FFF) - 1); // -1 because of the trailin null character
                        uint16_t numFactions = group(rordata, G_FACT).elems - 1;  // first "faction" (unaligned) is excluded
                        Rectangle r_faction = {0, 0, UNIT + UNITCLAMP((maxLetterCount + 1) * font2Em.x + ((maxLetterCount + 1) - 1) * Font2Spacing + 2 * Font2PaddingX), UNITCLAMP(Font2H)}; // "UNIT +" is required for unknown reason; (maxLetterCount + 1) because of the shown input cursor: "_"
                        r_faction.x = (uint16_t)((screenWidth - r_faction.width) / 2);
                        r_faction.y = 3 * UNIT + (uint16_t)((screenHeight - 2*3*UNIT - numFactions * r_faction.height - (numFactions - 1) * 1) / 2);
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
                                    DrawFont2(str, r, WHITE, TextLeft);
                                }
                                framesCounter++;
                            }
                            else
                            {
                                DrawRectangleRec(r, COLOR_FACTIONHEADER);
                            }
                            DrawFont2(str, r, WHITE, TextLeft);
                            r.y += r.height + 1;
                        }
                        if (!anyselected)
                        {
                            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
                            selected = -1;
                            framesCounter = 0;
                        }
                        // BUTTON
                        uint8_t fontScale = 3;
                        uint16_t buttonRectHeightDowns = 2 * fontScale;
                        uint16_t buttonRectWidthRights = 14 * fontScale;
                        // buttonRectWidthRights = 12 + ceil((PADDING + MeasureTextEx(font1, "NEXT", SFONTSIZE, SFONTSPACING).x) / RIGHT);
                        cursor.y = screenOffsetY + DOWN * (3 * floor(screenDowns/4) + floor((floor(screenDowns/4) - buttonRectHeightDowns) / 2)) ;
                        cursor.x = screenOffsetX + RIGHT * floor((screenRights - buttonRectWidthRights) / 2);
                        r = rect(cursor, buttonRectWidthRights, buttonRectHeightDowns);
                        if (CheckCollisionPointRec(mouse, r))
                        {
                            if (currentGesture == GESTURE_TAP)
                            {
                                DrawRectangleRounded(r, 0.2f, 10, COLOR_BUTTONCLICKED);
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
                            }
                            else
                            {
                                DrawRectangleRounded(r, 0.2f, 10, COLOR_BUTTON);
                            }
                        }
                        DrawRectangleRoundedLines(r, 0.2f, 10, 2, COLOR_BUTTONOUTLINE);
                        DrawText2(font1, "NEXT", r, SFONTSIZE, SFONTSPACING, COLOR_BUTTONTEXT, TextCenter);
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
                        int8_t dealStatus = 0;
                        for (int factidx = 1; factidx < numFactions + 1; factidx++)
                        {
                            for (int senaidx = 0; senaidx < group(rordata, G_SENA).elems; senaidx++)
                            {
                                if ( *((A_SENA_ALIG_t*)(val0absaddr(rordata, G_SENA, A_SENA_ALIG))+senaidx) == (A_SENA_ALIG_t)(factidx) ) factionSenatorCounts[factidx] += 1;
                            }
                            if (factionSenatorCounts[factidx] == 0)
                            {
                                dealStatus &= 0x01;
                            }
                            else if (factionSenatorCounts[factidx] < targetSenatorCount)
                            {
                                dealStatus &= 0x02;
                            }
                            else if (factionSenatorCounts[factidx] == targetSenatorCount)
                            {
                                dealStatus &= 0x04;
                            }
                            else
                            {
                                dealStatus &= 0x08;
                            }
                        }
                        // TITLE
                        cursor.x = screenOffsetX;
                        cursor.y = screenOffsetY;
                        uint8_t fontScale = 3;
                        Rectangle r = rect(cursor, screenRights, 2 * fontScale);
                        sprintf(str, "DEAL %i SENATORS TO EACH FACTIONS (DRAG & DROP OR SHAKE RANDOM)", targetSenatorCount);
                        DrawText2(font1, str, r, fontScale*FONTSIZE, fontScale*FONTSPACING, COLOR_TITLETEXT, TextCenter);
                        cursor.y += r.height + PADDING;
                        // SENATORS AND FACTIONS PREP
                        uint16_t mainCursorY = cursor.y;
                        fontScale = 2;
                        uint16_t screenVCenter = screenOffsetX + RIGHT * floor(screenRights / 2);
                        uint16_t idWidthRights = fontScale * 4;
                        uint16_t namesepWidthRights = fontScale * 1;
                        uint16_t nameWidthRights = fontScale * 19;
                        uint16_t statWidthRights = fontScale * 3;
                        uint16_t rectWidthRights = idWidthRights + namesepWidthRights + nameWidthRights + 5 * statWidthRights;
                        Vector2 selectedvector;
                        // SENATORS
                        cursor.x = screenVCenter - RIGHT * (rectWidthRights + 2);
                        r = rect(cursor, rectWidthRights, fontScale * 2);
                        if (0 <= selected && CheckCollisionPointRec(mouse, r))
                        {
                            DrawRectangleRec(r, COLOR_MOUSEHOVER_DROPTARGET);
                            if (currentGesture != lastGesture && currentGesture == GESTURE_NONE)
                            {
                                *(A_SENA_ALIG_t*)valabsaddr(rordata, G_SENA, A_SENA_ALIG, selected) = (A_SENA_ALIG_t)(-1);
                                *(A_SENA_CNGR_t*)valabsaddr(rordata, G_SENA, A_SENA_CNGR, selected) = (A_SENA_CNGR_t)(G_DECK);
                                *(A_SENA_CNNR_t*)valabsaddr(rordata, G_SENA, A_SENA_CNNR, selected) = (A_SENA_CNNR_t)(0);
                                selected = -1;
                            }
                        }
                        else
                        {
                            DrawRectangleRec(r, COLOR_FACTIONHEADER);
                        }
                        DrawText2(font1, "SENATORS", r, SFONTSIZE, SFONTSPACING, WHITE, TextTopLeft);
                        cursor.y += fontScale * DOWN;
                        cursor.x += (idWidthRights + namesepWidthRights + nameWidthRights) * RIGHT;
                        r = rect(cursor, statWidthRights, fontScale);
                        DrawText2(font1, "M", r, SFONTSIZE, SFONTSPACING, WHITE, TextCenter);
                        cursor.x += statWidthRights * RIGHT;
                        r = rect(cursor, statWidthRights, fontScale);
                        DrawText2(font1, "O", r, SFONTSIZE, SFONTSPACING, WHITE, TextCenter);
                        cursor.x += statWidthRights * RIGHT;
                        r = rect(cursor, statWidthRights, fontScale);
                        DrawText2(font1, "L", r, SFONTSIZE, SFONTSPACING, WHITE, TextCenter);
                        cursor.x += statWidthRights * RIGHT;
                        r = rect(cursor, statWidthRights, fontScale);
                        DrawText2(font1, "I", r, SFONTSIZE, SFONTSPACING, WHITE, TextCenter);
                        cursor.x += statWidthRights * RIGHT;
                        r = rect(cursor, statWidthRights, fontScale);
                        DrawText2(font1, "P", r, SFONTSIZE, SFONTSPACING, WHITE, TextCenter);
                        cursor.y += fontScale * DOWN;
                        for (int senaidx = 0; senaidx < group(rordata, G_SENA).elems; senaidx++)
                        {
                            if (
                                (*(A_SENA_CNGR_t*)valabsaddr(rordata, G_SENA, A_SENA_CNGR, senaidx) != (A_SENA_CNGR_t)(G_DECK))
                                ||
                                (*(A_SENA_CNNR_t*)valabsaddr(rordata, G_SENA, A_SENA_CNNR, senaidx) != (A_SENA_CNNR_t)(0))
                            ) continue;
                            if (senaidx == selected) continue;
                            cursor.x = screenVCenter - RIGHT * (rectWidthRights + 2);
                            r = rect(cursor, rectWidthRights, fontScale);
                            if (selected == -1 && CheckCollisionPointRec(mouse, r))
                            {
                                if (currentGesture != lastGesture && currentGesture == GESTURE_TAP) // pick up
                                {
                                    selected = senaidx;
                                    selectedvector.x = mouse.x - r.x;
                                    selectedvector.y = mouse.y - r.y;
                                }
                                else
                                {
                                    DrawRectangleRec(r, COLOR_MOUSEHOVER_DRAGABLE);
                                }
                            }
                            else if (selected != senaidx)
                            {
                                DrawRectangleRec(r, COLOR_BLACKCARDBG);
                            }
                            sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_IDNR, senaidx));
                            r = rect(cursor, idWidthRights, fontScale);
                            DrawText2(font1, str, r, SFONTSIZE, SFONTSPACING, BLACK, TextRight);
                            cursor.x += (idWidthRights + namesepWidthRights) * RIGHT;
                            r = rect(cursor, nameWidthRights, fontScale);
                            DrawText2(font1, (char*)(valabsaddr(rordata, G_SENA, A_SENA_NAME, senaidx)), r, SFONTSIZE, SFONTSPACING, BLACK, TextLeft);
                            cursor.x += nameWidthRights * RIGHT;
                            sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_MIL1, senaidx));
                            r = rect(cursor, statWidthRights, fontScale);
                            DrawText2(font1, str, r, SFONTSIZE, SFONTSPACING, BLACK, TextCenter);
                            cursor.x += statWidthRights * RIGHT;
                            sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_ORA1, senaidx));
                            r = rect(cursor, statWidthRights, fontScale);
                            DrawText2(font1, str, r, SFONTSIZE, SFONTSPACING, BLACK, TextCenter);
                            cursor.x += statWidthRights * RIGHT;
                            sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_LOY1, senaidx));
                            r = rect(cursor, statWidthRights, fontScale);
                            DrawText2(font1, str, r, SFONTSIZE, SFONTSPACING, BLACK, TextCenter);
                            cursor.x += statWidthRights * RIGHT;
                            sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_INF1, senaidx));
                            r = rect(cursor, statWidthRights, fontScale);
                            DrawText2(font1, str, r, SFONTSIZE, SFONTSPACING, BLACK, TextCenter);
                            cursor.x += statWidthRights * RIGHT;
                            sprintf(str, "%i", (int8_t)(*valabsaddr(rordata, G_SENA, A_SENA_POP1, senaidx)));
                            r = rect(cursor, statWidthRights, fontScale);
                            DrawText2(font1, str, r, SFONTSIZE, SFONTSPACING, BLACK, TextCenter);
                            cursor.y += fontScale * DOWN;
                        }
                        // FACTIONS
                        cursor.y = mainCursorY;
                        for (int factidx = 1; factidx < numFactions + 1; factidx++)
                        {
                            cursor.x = screenVCenter + RIGHT * 2;
                            r = rect(cursor, rectWidthRights, fontScale * 2);
                            if (0 <= selected && CheckCollisionPointRec(mouse, r))
                            {
                                DrawRectangleRec(r, COLOR_MOUSEHOVER_DROPTARGET);
                                if (currentGesture != lastGesture && currentGesture == GESTURE_NONE)
                                {
                                    *(A_SENA_ALIG_t*)valabsaddr(rordata, G_SENA, A_SENA_ALIG, selected) = (A_SENA_ALIG_t)(factidx);
                                    *(A_SENA_CNGR_t*)valabsaddr(rordata, G_SENA, A_SENA_CNGR, selected) = (A_SENA_CNGR_t)(G_FORU);
                                    *(A_SENA_CNNR_t*)valabsaddr(rordata, G_SENA, A_SENA_CNNR, selected) = (A_SENA_CNNR_t)(0);
                                    selected = -1;
                                }
                            }
                            else
                            {
                                DrawRectangleRec(r, COLOR_FACTIONHEADER);
                            }
                            DrawText2(font1, TextToUpper((char*)(valabsaddr(rordata, G_FACT, A_FACT_NAME, factidx))), r, SFONTSIZE, SFONTSPACING, WHITE, TextTopLeft);
                            cursor.y += fontScale * DOWN;
                            cursor.x += (idWidthRights + namesepWidthRights + nameWidthRights) * RIGHT;
                            r = rect(cursor, statWidthRights, fontScale);
                            DrawText2(font1, "M", r, SFONTSIZE, SFONTSPACING, WHITE, TextCenter);
                            cursor.x += statWidthRights * RIGHT;
                            r = rect(cursor, statWidthRights, fontScale);
                            DrawText2(font1, "O", r, SFONTSIZE, SFONTSPACING, WHITE, TextCenter);
                            cursor.x += statWidthRights * RIGHT;
                            r = rect(cursor, statWidthRights, fontScale);
                            DrawText2(font1, "L", r, SFONTSIZE, SFONTSPACING, WHITE, TextCenter);
                            cursor.x += statWidthRights * RIGHT;
                            r = rect(cursor, statWidthRights, fontScale);
                            DrawText2(font1, "I", r, SFONTSIZE, SFONTSPACING, WHITE, TextCenter);
                            cursor.x += statWidthRights * RIGHT;
                            r = rect(cursor, statWidthRights, fontScale);
                            DrawText2(font1, "P", r, SFONTSIZE, SFONTSPACING, WHITE, TextCenter);
                            cursor.y += fontScale * DOWN;

                            for (int senaidx = 0; senaidx < group(rordata, G_SENA).elems; senaidx++)
                            {
                                if ( *((A_SENA_ALIG_t*)(val0absaddr(rordata, G_SENA, A_SENA_ALIG))+senaidx) == (A_SENA_ALIG_t)(factidx) )  // typecast is a must!
                                {
                                    cursor.x = cursor.x = screenVCenter + RIGHT * 2;
                                    r = rect(cursor, rectWidthRights, fontScale);
                                    if (selected == -1 && CheckCollisionPointRec(mouse, r))
                                    {
                                        if (currentGesture != lastGesture && currentGesture == GESTURE_TAP) // pick up
                                        {
                                            selected = senaidx;
                                            selectedvector.x = mouse.x - r.x;
                                            selectedvector.y = mouse.y - r.y;
                                        }
                                        else
                                        {
                                            DrawRectangleRec(r, COLOR_MOUSEHOVER_DRAGABLE);
                                        }
                                    }
                                    else if (selected != senaidx)
                                    {
                                        DrawRectangleRec(r, COLOR_BLACKCARDBG);
                                    }
                                    sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_IDNR, senaidx));
                                    r = rect(cursor, idWidthRights, fontScale);
                                    DrawText2(font1, str, r, SFONTSIZE, SFONTSPACING, BLACK, TextRight);
                                    cursor.x += (idWidthRights + namesepWidthRights) * RIGHT;
                                    r = rect(cursor, nameWidthRights, fontScale);
                                    DrawText2(font1, (char*)(valabsaddr(rordata, G_SENA, A_SENA_NAME, senaidx)), r, SFONTSIZE, SFONTSPACING, BLACK, TextLeft);
                                    cursor.x += nameWidthRights * RIGHT;
                                    sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_MIL1, senaidx));
                                    r = rect(cursor, statWidthRights, fontScale);
                                    DrawText2(font1, str, r, SFONTSIZE, SFONTSPACING, BLACK, TextCenter);
                                    cursor.x += statWidthRights * RIGHT;
                                    sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_ORA1, senaidx));
                                    r = rect(cursor, statWidthRights, fontScale);
                                    DrawText2(font1, str, r, SFONTSIZE, SFONTSPACING, BLACK, TextCenter);
                                    cursor.x += statWidthRights * RIGHT;
                                    sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_LOY1, senaidx));
                                    r = rect(cursor, statWidthRights, fontScale);
                                    DrawText2(font1, str, r, SFONTSIZE, SFONTSPACING, BLACK, TextCenter);
                                    cursor.x += statWidthRights * RIGHT;
                                    sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_INF1, senaidx));
                                    r = rect(cursor, statWidthRights, fontScale);
                                    DrawText2(font1, str, r, SFONTSIZE, SFONTSPACING, BLACK, TextCenter);
                                    cursor.x += statWidthRights * RIGHT;
                                    sprintf(str, "%i", (int8_t)(*valabsaddr(rordata, G_SENA, A_SENA_POP1, senaidx)));
                                    r = rect(cursor, statWidthRights, fontScale);
                                    DrawText2(font1, str, r, SFONTSIZE, SFONTSPACING, BLACK, TextCenter);
                                    cursor.y += fontScale * DOWN;
                                }
                            }
                            cursor.y += DOWN;
                        }
                        // BUTTON PREP
                        fontScale = 3;
                        uint16_t buttonRectHeightDowns = 1 * fontScale;
                        uint16_t buttonRectWidthRights = 14 * fontScale;
                        // BACK BUTTON
                        cursor.y = screenOffsetY + DOWN * (screenDowns - buttonRectHeightDowns - 1);
                        cursor.x = screenVCenter - RIGHT * 6 - RIGHT * buttonRectWidthRights * 2;
                        r = rect(cursor, buttonRectWidthRights, buttonRectHeightDowns);
                        DrawPixel(cursor.x-1, cursor.y-1, MAGENTA);
                        DrawPixel(cursor.x-2, cursor.y-2, MAGENTA);
                        DrawRectangleRounded(r, 0.2f, 10, DARKGRAY);
                        //DrawRectangleRoundedLines(r, 0.2f, 10, 2, COLOR_BUTTONOUTLINE);
                        DrawText2(font1, "@BACK@", r, SFONTSIZE, SFONTSPACING, COLOR_BUTTONTEXT, TextCenter);
                        // NEXT BUTTON
                        cursor.y = screenOffsetY + DOWN * (screenDowns - buttonRectHeightDowns - 1);
                        cursor.x = screenVCenter + RIGHT * 6 + RIGHT * buttonRectWidthRights;
                        r = rect(cursor, buttonRectWidthRights, buttonRectHeightDowns);
                        DrawRectangleRoundedLines(r, 0.2f, 10, 2, COLOR_BUTTONOUTLINE);
                        DrawText2(font1, "NEXT", r, SFONTSIZE, SFONTSPACING, COLOR_BUTTONTEXT, TextCenter);
                        // RESET BUTTON
                        fontScale = 1;
                        buttonRectHeightDowns = 1 * fontScale;
                        buttonRectWidthRights = 14 * fontScale;
                        cursor.y = screenOffsetY + DOWN * (screenDowns - buttonRectHeightDowns - 1);
                        cursor.x = screenVCenter + RIGHT * 2;
                        r = rect(cursor, buttonRectWidthRights, buttonRectHeightDowns);
                        DrawPixel(cursor.x-1, cursor.y-1, MAGENTA);
                        DrawPixel(cursor.x-2, cursor.y-2, MAGENTA);
                        DrawRectangleRounded(r, 0.2f, 10, DARKGRAY);
                        //DrawRectangleRoundedLines(r, 0.2f, 10, 2, COLOR_BUTTONOUTLINE);
                        DrawText2(font1, "@RESET@", r, SFONTSIZE, SFONTSPACING, COLOR_BUTTONTEXT, TextCenter);
                        fontScale = 3;
                        buttonRectHeightDowns = 1 * fontScale;
                        buttonRectWidthRights = 14 * fontScale;
                        // RANDOM BUTTON
                        // buttonRectWidthRights = 12 + ceil((PADDING + MeasureTextEx(font1, "RANDOM", SFONTSIZE, SFONTSPACING).x) / RIGHT);
                        cursor.y = screenOffsetY + DOWN * (screenDowns - buttonRectHeightDowns - 1);
                        cursor.x = screenVCenter - RIGHT * (buttonRectWidthRights + 2);
                        r = rect(cursor, buttonRectWidthRights, buttonRectHeightDowns);
                        if (selected == -501000 && currentGesture != lastGesture && lastGesture == GESTURE_DRAG)
                        {
                            selected = -1;
                        }
                        if (selected == -1 && CheckCollisionPointRec(mouse, r))
                        {
                            if (currentGesture != lastGesture && currentGesture == GESTURE_TAP) // pick up
                            {
                                selected = -501000;
                                selectedvector.x = mouse.x - r.x;
                                selectedvector.y = mouse.y - r.y;
                            }
                            else
                            {
                                DrawRectangleRounded(r, 0.2f, 10, COLOR_MOUSEHOVER_DRAGABLE);
                                DrawRectangleRoundedLines(r, 0.2f, 10, 2, COLOR_BUTTONOUTLINE);
                                DrawText2(font1, "RANDOM", r, SFONTSIZE, SFONTSPACING, COLOR_BUTTONTEXT, TextCenter);
                            }
                        }
                        else if (selected == -501000 && currentGesture == GESTURE_DRAG)
                        {
                            cursor.x = mouse.x - selectedvector.x;
                            cursor.y = mouse.y - selectedvector.y;
                            r = rect(cursor, buttonRectWidthRights, buttonRectHeightDowns);
                            DrawRectangleRounded(r, 0.2f, 10, COLOR_MOUSEDRAG);
                            DrawRectangleRoundedLines(r, 0.2f, 10, 2, COLOR_BUTTONOUTLINE);
                            DrawText2(font1, "RANDOM", r, SFONTSIZE, SFONTSPACING, COLOR_MOUSEDRAGTEXT, TextCenter);
                        }
                        else
                        {
                            DrawRectangleRounded(r, 0.2f, 10, COLOR_BUTTONBACKGROUND);
                            DrawRectangleRoundedLines(r, 0.2f, 10, 2, COLOR_BUTTONOUTLINE);
                            DrawText2(font1, "RANDOM", r, SFONTSIZE, SFONTSPACING, COLOR_BUTTONTEXT, TextCenter);
                        }

                        {
                            // if (CheckCollisionPointRec(mouse, r))
                            // {
                            //       if (0 < deckSize && currentGesture == GESTURE_TAP)
                            //       {
                            //           uint8_t targetSenatorCount = (uint8_t)(*val0absaddr(rordata, G_RULE, A_RULE_NSEN));
                            //           uint8_t factionSenatorCounts[FACT_ELEMCOUNT] = {0};
                            //           bool allfull = true;
                            //           for (int factidx = 1; factidx < *(A_GAME_NFAC_t*)val0absaddr(rordata, G_GAME, A_GAME_NFAC); factidx++)
                            //           {
                            //               for (int senaidx = 0; senaidx < group(rordata, G_SENA).elems; senaidx++)
                            //               {
                            //                   if ( *((A_SENA_ALIG_t*)(val0absaddr(rordata, G_SENA, A_SENA_ALIG))+senaidx) == (A_SENA_ALIG_t)(factidx) ) factionSenatorCounts[factidx] += 1;
                            //               }
                            //               sprintf(str, "FACT: %i; ROUND 1; deck_cards: %i, faction_cards: %i", factidx, deckSize, factionSenatorCounts[factidx]);
                            //               TraceLog(LOG_DEBUG, str);
                            //               if (targetSenatorCount < factionSenatorCounts[factidx])  // more senators than allowed, put them all back to deck
                            //               {
                            //                   allfull = false;
                            //                   for (int senaidx = 0; senaidx < group(rordata, G_SENA).elems; senaidx++)
                            //                   {
                            //                       if ( *((A_SENA_ALIG_t*)(val0absaddr(rordata, G_SENA, A_SENA_ALIG))+senaidx) == (A_SENA_ALIG_t)(factidx) )
                            //                       {
                            //                           *(A_SENA_ALIG_t*)valabsaddr(rordata, G_SENA, A_SENA_ALIG, senaidx) = (A_SENA_ALIG_t)(-1);
                            //                           *(A_SENA_CNGR_t*)valabsaddr(rordata, G_SENA, A_SENA_CNGR, senaidx) = (A_SENA_CNGR_t)(G_DECK);
                            //                           *(A_SENA_CNNR_t*)valabsaddr(rordata, G_SENA, A_SENA_CNNR, senaidx) = (A_SENA_CNNR_t)(0);
                            //                           factionSenatorCounts[factidx] -= 1;
                            //                           deckSize += 1;
                            //                       }
                            //                   }
                            //               }
                            //               else if (targetSenatorCount > factionSenatorCounts[factidx]) allfull = false;
                            //           }
                            //           if (allfull) // perform a full redraw
                            //           {
                            //               for (int senaidx = 0; senaidx < group(rordata, G_SENA).elems; senaidx++)
                            //               {
                            //                   if ( -1 < *((A_SENA_ALIG_t*)(val0absaddr(rordata, G_SENA, A_SENA_ALIG))+senaidx) )
                            //                   {
                            //                       *(A_SENA_ALIG_t*)valabsaddr(rordata, G_SENA, A_SENA_ALIG, senaidx) = (A_SENA_ALIG_t)(-1);
                            //                       *(A_SENA_CNGR_t*)valabsaddr(rordata, G_SENA, A_SENA_CNGR, senaidx) = (A_SENA_CNGR_t)(G_DECK);
                            //                       *(A_SENA_CNNR_t*)valabsaddr(rordata, G_SENA, A_SENA_CNNR, senaidx) = (A_SENA_CNNR_t)(0);
                            //                       deckSize += 1;
                            //                   }
                            //               }
                            //               for (int factidx = 1; factidx < FACT_ELEMCOUNT; factidx++) factionSenatorCounts[factidx] = 0;
                            //           }
                            //           for (int factidx = 1; factidx < *(A_GAME_NFAC_t*)val0absaddr(rordata, G_GAME, A_GAME_NFAC); factidx++)
                            //           {
                            //               while (1 < deckSize && factionSenatorCounts[factidx] < targetSenatorCount)
                            //               {
                            //                   uint8_t k = ((deckSize == 1) ? 0 : 0xFF);
                            //                   int b;
                            //                   for (b = 0; b < 8; b++)
                            //                   {
                            //                       if ( (deckSize - 1) <= (1 << b) ) break;
                            //                   }
                            //                   sprintf(str, "b: %i, k: %i, entropyidx: %i, entropynextbit: %i, newentropyidx: %i, newentropynextbit: %i", b, k, entropyidx, entropynextbit, newentropyidx, newentropynextbit);
                            //                   TraceLog(LOG_DEBUG, str);
                            //                   if (newentropyidx <= entropyidx + 1)
                            //                   {
                            //                       *(A_GAME_SPHS_t*)(val0absaddr(rordata, G_GAME, A_GAME_SPHS)) = (A_GAME_SPHS_t)SPHS_PREP_DEALSENATORS_RANDOM_ENTROPYREQ;
                            //                       //goto LABEL_SPHS_PREP_DEALSENATORS_done;
                            //                   }
                            //                   while (deckSize - 1 < k)
                            //                   {
                            //                       if (k == 0xFF)  // initial value
                            //                       {
                            //                           if (8 < entropynextbit + b)
                            //                           {
                            //                               k = (entropy[(uint8_t)(entropyidx % 256)] >> entropynextbit) + ((((entropy[(uint8_t)((entropyidx+1) % 256)] << (16 - b - entropynextbit)) & 0xFF) >> (8 - b)));
                            //                               entropyidx += 1;
                            //                               entropynextbit = (entropynextbit + b) - 8;
                            //                           }
                            //                           else
                            //                           {
                            //                               k = ( (entropy[(uint8_t)(entropyidx % 256)] << (8 - b - entropynextbit) ) & 0xFF ) >> (8 - b);
                            //                               entropynextbit += b;
                            //                               if (entropynextbit == 8)
                            //                               {
                            //                                   entropyidx += 1;
                            //                                   entropynextbit = 0;
                            //                               }
                            //                           }
                            //                       }
                            //                       sprintf(str, "k: %i, entropyidx: %i, entropynextbit: %i", k, entropyidx, entropynextbit);
                            //                       TraceLog(LOG_DEBUG, str);
                            //                   } 
                            //               uint8_t m = 0;
                            //               for (int senaidx = 0; senaidx < group(rordata, G_SENA).elems; senaidx++)
                            //               {
                            //                   if (
                            //                       (*(A_SENA_CNGR_t*)valabsaddr(rordata, G_SENA, A_SENA_CNGR, senaidx) == (A_SENA_CNGR_t)(G_DECK))
                            //                       &&
                            //                       (*(A_SENA_CNNR_t*)valabsaddr(rordata, G_SENA, A_SENA_CNNR, senaidx) == (A_SENA_CNNR_t)(0))
                            //                   ) 
                            //                   {
                            //                       if (m == k)
                            //                       {
                            //                           *(A_SENA_ALIG_t*)valabsaddr(rordata, G_SENA, A_SENA_ALIG, senaidx) = (A_SENA_ALIG_t)(factidx);
                            //                           *(A_SENA_CNGR_t*)valabsaddr(rordata, G_SENA, A_SENA_CNGR, senaidx) = (A_SENA_CNGR_t)(G_FORU);
                            //                           *(A_SENA_CNNR_t*)valabsaddr(rordata, G_SENA, A_SENA_CNNR, senaidx) = (A_SENA_CNNR_t)(0);
                            //                           factionSenatorCounts[factidx] += 1;
                            //                           deckSize -= 1;
                            //                           break;
                            //                       }
                            //                       m += 1;
                            //                   }
                            //               }
                            //               }
                            //           }
                            //           DrawRectangleRec(r, COLOR_BUTTONCLICKED);
                            //       }
                            //       else
                            //       {
                            //           DrawRectangleRec(r, COLOR_BUTTON);
                            //       }
                            // }
                            // DrawRectangleLinesEx(r, 2, COLOR_BUTTONOUTLINE);
                            // DrawText2(font1, "RANDOM", r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                        }

                        // SELECTION
                        fontScale = 2;
                        if (0 <= selected) // draw selected senator on top off all other
                        {
                            if (currentGesture != lastGesture && currentGesture == GESTURE_NONE)
                            {
                                selected = -1;
                            }
                            else
                            {
                                int senaidx = selected;  // for code coherence
                                cursor.x = mouse.x - selectedvector.x;
                                cursor.y = mouse.y - selectedvector.y;
                                r = rect(cursor, rectWidthRights, fontScale);
                                DrawRectangleRec(r, COLOR_MOUSEDRAG);
                                sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_IDNR, senaidx));
                                r = rect(cursor, idWidthRights, fontScale);
                                DrawText2(font1, str, r, SFONTSIZE, SFONTSPACING, BLACK, TextRight);
                                cursor.x += (idWidthRights + namesepWidthRights) * RIGHT;
                                r = rect(cursor, nameWidthRights, fontScale);
                                DrawText2(font1, (char*)(valabsaddr(rordata, G_SENA, A_SENA_NAME, senaidx)), r, SFONTSIZE, SFONTSPACING, BLACK, TextLeft);
                                cursor.x += nameWidthRights * RIGHT;
                                sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_MIL1, senaidx));
                                r = rect(cursor, statWidthRights, fontScale);
                                DrawText2(font1, str, r, SFONTSIZE, SFONTSPACING, BLACK, TextCenter);
                                cursor.x += statWidthRights * RIGHT;
                                sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_ORA1, senaidx));
                                r = rect(cursor, statWidthRights, fontScale);
                                DrawText2(font1, str, r, SFONTSIZE, SFONTSPACING, BLACK, TextCenter);
                                cursor.x += statWidthRights * RIGHT;
                                sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_LOY1, senaidx));
                                r = rect(cursor, statWidthRights, fontScale);
                                DrawText2(font1, str, r, SFONTSIZE, SFONTSPACING, BLACK, TextCenter);
                                cursor.x += statWidthRights * RIGHT;
                                sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_INF1, senaidx));
                                r = rect(cursor, statWidthRights, fontScale);
                                DrawText2(font1, str, r, SFONTSIZE, SFONTSPACING, BLACK, TextCenter);
                                cursor.x += statWidthRights * RIGHT;
                                sprintf(str, "%i", (int8_t)(*valabsaddr(rordata, G_SENA, A_SENA_POP1, senaidx)));
                                r = rect(cursor, statWidthRights, fontScale);
                                DrawText2(font1, str, r, SFONTSIZE, SFONTSPACING, BLACK, TextCenter);
                                cursor.y += fontScale * DOWN;
                            }
                        }


//                        cursor.x = windowedScreenWidth - ceil(0.5 * DOWN) - 12 * RIGHT;
//                        cursor.y = ceil(0.5 * DOWN);
//                        r = rect(cursor, 12, 2);
//                        if (CheckCollisionPointRec(mouse, r))
//                        {
//                            if (currentGesture == GESTURE_TAP)
//                            {
//                                DrawRectangleRec(r, COLOR_BUTTONCLICKED);
//                                uint8_t temporaryromeconsulflags = *(A_RULE_TERC_t*)(val0absaddr(rordata, G_RULE, A_RULE_TERC));
//                                if (temporaryromeconsulflags == 0xFF || (temporaryromeconsulflags & 0x01)==0x01)
//                                {
//                                    *(A_GAME_SPHS_t*)(val0absaddr(rordata, G_GAME, A_GAME_SPHS)) = (A_GAME_SPHS_t)SPHS_PREP_SELECTFACTIONLEADERS;
//                                }
//                                else
//                                {
//                                    *(A_GAME_SPHS_t*)(val0absaddr(rordata, G_GAME, A_GAME_SPHS)) = (A_GAME_SPHS_t)SPHS_PREP_TEMPORARYROMECONSUL;
//                                }
//                                selected = -1;
//                            }
//                            else
//                            {
//                                DrawRectangleRec(r, COLOR_BUTTON);
//                            }
//                        }
//                        DrawRectangleLinesEx(r, 2, COLOR_BUTTONOUTLINE);
//                        DrawText2(font1, "NEXT", r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
//
//                        LABEL_SPHS_PREP_DEALSENATORS_done:
                    } break;

                    case SPHS_PREP_DEALSENATORS_RANDOM_ENTROPYREQ:
                    {
                        cursor.x = RIGHT;
                        cursor.y = 1 * DOWN;
                        DrawTextEx(font1, "PLEASE GIVE SOME ENTROPY FOR THE DRAW BY MOVING THE POINTER", cursor, FONTSIZE, FONTSPACING, ORANGE);

                        cursor.y = 3 * DOWN;

                        for (int factidx = 1; factidx < *(A_GAME_NFAC_t*)val0absaddr(rordata, G_GAME, A_GAME_NFAC); factidx++)
                        {
                            cursor.x = RIGHT;
                            r = rect(cursor, 30, 2);
                            DrawRectangleRec(r, COLOR_FACTIONHEADER);
                            DrawText2(font1, TextToUpper((char*)(valabsaddr(rordata, G_FACT, A_FACT_NAME, factidx))), r, FONTSIZE, FONTSPACING, WHITE, TextTopLeft);
                            cursor.y += 1 * DOWN;
                            cursor.x += (2 + 2 + SENATORNAMEWIDTH) * RIGHT;
                            r = rect(cursor, 2, 1);
                            DrawText2(font1, "M", r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                            cursor.x += 2 * RIGHT;
                            r = rect(cursor, 2, 1);
                            DrawText2(font1, "O", r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                            cursor.x += 2 * RIGHT;
                            r = rect(cursor, 2, 1);
                            DrawText2(font1, "L", r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                            cursor.x += 2 * RIGHT;
                            r = rect(cursor, 2, 1);
                            DrawText2(font1, "I", rect(cursor, 2, 1), FONTSIZE, FONTSPACING, WHITE, TextCenter);
                            cursor.x += 2 * RIGHT;
                            r = rect(cursor, 2, 1);
                            DrawText2(font1, "P", r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                            cursor.y += 1 * DOWN;

                            for (int senaidx = 0; senaidx < group(rordata, G_SENA).elems; senaidx++)
                            {
                                if ( *((A_SENA_ALIG_t*)(val0absaddr(rordata, G_SENA, A_SENA_ALIG))+senaidx) == (A_SENA_ALIG_t)(factidx) )  // typecast is a must!
                                {
                                    cursor.x = RIGHT;
                                    r = rect(cursor, 30, 1);
                                    DrawRectangleRec(r, COLOR_BLACKCARDBG);
                                    cursor.x += 2 * RIGHT;
                                    sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_IDNR, senaidx));
                                    r = rect(cursor, 2, 1);
                                    DrawText2(font1, str, r, FONTSIZE, FONTSPACING, BLACK, TextRight);
                                    cursor.x += 2 * RIGHT;
                                    r = rect(cursor, SENATORNAMEWIDTH, 1);
                                    DrawText2(font1, (char*)(valabsaddr(rordata, G_SENA, A_SENA_NAME, senaidx)), r, FONTSIZE, FONTSPACING, BLACK, TextLeft);
                                    cursor.x += SENATORNAMEWIDTH * RIGHT;
                                    sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_MIL1, senaidx));
                                    r = rect(cursor, 2, 1);
                                    DrawText2(font1, str, r, FONTSIZE, FONTSPACING, BLACK, TextCenter);
                                    cursor.x += 2 * RIGHT;
                                    sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_ORA1, senaidx));
                                    r = rect(cursor, 2, 1);
                                    DrawText2(font1, str, r, FONTSIZE, FONTSPACING, BLACK, TextCenter);
                                    cursor.x += 2 * RIGHT;
                                    sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_LOY1, senaidx));
                                    r = rect(cursor, 2, 1);
                                    DrawText2(font1, str, r, FONTSIZE, FONTSPACING, BLACK, TextCenter);
                                    cursor.x += 2 * RIGHT;
                                    sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_INF1, senaidx));
                                    r = rect(cursor, 2, 1);
                                    DrawText2(font1, str, r, FONTSIZE, FONTSPACING, BLACK, TextCenter);
                                    cursor.x += 2 * RIGHT;
                                    sprintf(str, "%i", (int8_t)(*valabsaddr(rordata, G_SENA, A_SENA_POP1, senaidx)));
                                    r = rect(cursor, 2, 1);
                                    DrawText2(font1, str, r, FONTSIZE, FONTSPACING, BLACK, TextCenter);
                                    cursor.y += DOWN;
                                }
                            }
                            cursor.y += DOWN;
                        }

                        cursor.y = 3 * DOWN;
                        cursor.x = (1+30+2) * RIGHT;
                        r = rect(cursor, 30, 2);
                        DrawRectangleRec(r, COLOR_FACTIONHEADER);
                        DrawText2(font1, "SENATORS", r, FONTSIZE, FONTSPACING, WHITE, TextTopLeft);
                        cursor.y += 1 * DOWN;
                        cursor.x += (2 + 2 + SENATORNAMEWIDTH) * RIGHT;
                        r = rect(cursor, 2, 1);
                        DrawText2(font1, "M", r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                        cursor.x += 2 * RIGHT;
                        r = rect(cursor, 2, 1);
                        DrawText2(font1, "O", r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                        cursor.x += 2 * RIGHT;
                        r = rect(cursor, 2, 1);
                        DrawText2(font1, "L", r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                        cursor.x += 2 * RIGHT;
                        r = rect(cursor, 2, 1);
                        DrawText2(font1, "I", rect(cursor, 2, 1), FONTSIZE, FONTSPACING, WHITE, TextCenter);
                        cursor.x += 2 * RIGHT;
                        r = rect(cursor, 2, 1);
                        DrawText2(font1, "P", r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                        cursor.y += 1 * DOWN;

                        for (int senaidx = 0; senaidx < group(rordata, G_SENA).elems; senaidx++)
                        {
                            if (
                                (*(A_SENA_CNGR_t*)valabsaddr(rordata, G_SENA, A_SENA_CNGR, senaidx) != (A_SENA_CNGR_t)(G_DECK))
                                ||
                                (*(A_SENA_CNNR_t*)valabsaddr(rordata, G_SENA, A_SENA_CNNR, senaidx) != (A_SENA_CNNR_t)(0))
                            ) continue;
                            cursor.x = (1+30+2) * RIGHT;
                            r = rect(cursor, 30, 1);
                            DrawRectangleRec(r, COLOR_BLACKCARDBG);
                            cursor.x += 2 * RIGHT;
                            sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_IDNR, senaidx));
                            r = rect(cursor, 2, 1);
                            DrawText2(font1, str, r, FONTSIZE, FONTSPACING, BLACK, TextRight);
                            cursor.x += 2 * RIGHT;
                            r = rect(cursor, SENATORNAMEWIDTH, 1);
                            DrawText2(font1, (char*)(valabsaddr(rordata, G_SENA, A_SENA_NAME, senaidx)), r, FONTSIZE, FONTSPACING, BLACK, TextLeft);
                            cursor.x += SENATORNAMEWIDTH * RIGHT;
                            sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_MIL1, senaidx));
                            r = rect(cursor, 2, 1);
                            DrawText2(font1, str, r, FONTSIZE, FONTSPACING, BLACK, TextCenter);
                            cursor.x += 2 * RIGHT;
                            sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_ORA1, senaidx));
                            r = rect(cursor, 2, 1);
                            DrawText2(font1, str, r, FONTSIZE, FONTSPACING, BLACK, TextCenter);
                            cursor.x += 2 * RIGHT;
                            sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_LOY1, senaidx));
                            r = rect(cursor, 2, 1);
                            DrawText2(font1, str, r, FONTSIZE, FONTSPACING, BLACK, TextCenter);
                            cursor.x += 2 * RIGHT;
                            sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_INF1, senaidx));
                            r = rect(cursor, 2, 1);
                            DrawText2(font1, str, r, FONTSIZE, FONTSPACING, BLACK, TextCenter);
                            cursor.x += 2 * RIGHT;
                            sprintf(str, "%i", (int8_t)(*valabsaddr(rordata, G_SENA, A_SENA_POP1, senaidx)));
                            r = rect(cursor, 2, 1);
                            DrawText2(font1, str, r, FONTSIZE, FONTSPACING, BLACK, TextCenter);
                            cursor.y += 1 * DOWN;
                        }

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

                        for (int factidx = 1; factidx < *(A_GAME_NFAC_t*)val0absaddr(rordata, G_GAME, A_GAME_NFAC); factidx++)
                        {
                            for (int senaidx = 0; senaidx < group(rordata, G_SENA).elems; senaidx++)
                            {
                                if ( *((A_SENA_ALIG_t*)(val0absaddr(rordata, G_SENA, A_SENA_ALIG))+senaidx) == (A_SENA_ALIG_t)(factidx) ) factionSenatorCounts[factidx] += 1;
                            }
                        }

                        for (int factidx = 1; factidx < *(A_GAME_NFAC_t*)val0absaddr(rordata, G_GAME, A_GAME_NFAC); factidx++)
                        {
                            while (1 < deckSize && factionSenatorCounts[factidx] < targetSenatorCount)
                            {
                                uint8_t k = ((deckSize == 1) ? 0 : 0xFF);
                                int b;
                                for (b = 0; b < 8; b++)
                                {
                                    if ( (deckSize - 1) <= (1 << b) ) break;
                                }
                                sprintf(str, "b: %i, k: %i, entropyidx: %i, entropynextbit: %i, newentropyidx: %i, newentropynextbit: %i", b, k, entropyidx, entropynextbit, newentropyidx, newentropynextbit);
                                TraceLog(LOG_DEBUG, str);
                                if (newentropyidx <= entropyidx + 1)
                                {
                                    *(A_GAME_SPHS_t*)(val0absaddr(rordata, G_GAME, A_GAME_SPHS)) = (A_GAME_SPHS_t)SPHS_PREP_DEALSENATORS_RANDOM_ENTROPYREQ;
//                                    goto LABEL_SPHS_PREP_DEALSENATORS_done;
                                }
                                while (deckSize - 1 < k)
                                {
                                    if (k == 0xFF)
                                    {
                                        if (8 < entropynextbit + b)
                                        {
                                            k = entropy[(uint8_t)((entropyidx+1) % 256)];
                                            k = k & (0xFF >> (16 - (entropynextbit + b)));
                                            k += entropy[(uint8_t)(entropyidx % 256)] >> entropynextbit;
                                            entropyidx += 1;
                                            entropynextbit = (entropynextbit + b) - 8;
                                        }
                                        else
                                        {
                                            k = (entropy[(uint8_t)(entropyidx % 256)] << (8 - (entropynextbit + b)) ) >> (entropynextbit + b);
                                            entropynextbit += b;
                                            if (entropynextbit == 8)
                                            {
                                                entropyidx += 1;
                                                entropynextbit = 0;
                                            }
                                        }
                                    }
                                    else
                                    {
                                        k = (k << 1) & (0xFF >> (8 - b));
                                        k += entropy[(uint8_t)(entropyidx % 256)] >> entropynextbit;
                                        entropynextbit += 1;
                                        if (entropynextbit == 8)
                                        {
                                            entropyidx += 1;
                                            entropynextbit = 0;
                                        }
                                    }
                                    sprintf(str, "k: %i, entropyidx: %i, entropynextbit: %i", k, entropyidx, entropynextbit);
                                    TraceLog(LOG_DEBUG, str);
                                } 
                            uint8_t m = 0;
                            for (int senaidx = 0; senaidx < group(rordata, G_SENA).elems; senaidx++)
                            {
                                if (
                                    (*(A_SENA_CNGR_t*)valabsaddr(rordata, G_SENA, A_SENA_CNGR, senaidx) == (A_SENA_CNGR_t)(G_DECK))
                                    &&
                                    (*(A_SENA_CNNR_t*)valabsaddr(rordata, G_SENA, A_SENA_CNNR, senaidx) == (A_SENA_CNNR_t)(0))
                                ) 
                                {
                                    if (m == k)
                                    {
                                        *(A_SENA_ALIG_t*)valabsaddr(rordata, G_SENA, A_SENA_ALIG, senaidx) = (A_SENA_ALIG_t)(factidx);
                                        *(A_SENA_CNGR_t*)valabsaddr(rordata, G_SENA, A_SENA_CNGR, senaidx) = (A_SENA_CNGR_t)(G_FORU);
                                        *(A_SENA_CNNR_t*)valabsaddr(rordata, G_SENA, A_SENA_CNNR, senaidx) = (A_SENA_CNNR_t)(0);
                                        factionSenatorCounts[factidx] += 1;
                                        deckSize -= 1;
                                        break;
                                    }
                                    m += 1;
                                }
                            }
                            }
                        }
                        *(A_GAME_SPHS_t*)(val0absaddr(rordata, G_GAME, A_GAME_SPHS)) = (A_GAME_SPHS_t)SPHS_PREP_DEALSENATORS;
                    } break;

                    case SPHS_PREP_TEMPORARYROMECONSUL:
                    {
                        cursor.x = RIGHT;
                        cursor.y = 1 * DOWN;
                        DrawTextEx(font1, "TEMPORARY ROME CONSUL", cursor, FONTSIZE, FONTSPACING, ORANGE);

                        uint8_t senator_count = 0;  // determine population
                        for (int senaidx = 0; senaidx < group(rordata, G_SENA).elems; senaidx++)
                        {
                            if (0 <= *(A_SENA_ALIG_t*)valabsaddr(rordata, G_SENA, A_SENA_ALIG, senaidx)) senator_count += 1;
                        }
                        sprintf(str, "%i", senator_count);
                        DrawText(str, windowedScreenWidth-30, 50, 10, MAGENTA);

                        uint8_t m = 0;
                        if (senator_count == 1) goto LABEL_SPHS_PREP_TEMPORARYROMECONSUL_POSTRANDOM;

                        uint8_t* p_temporaryromeconsulflags = (A_RULE_TERC_t*)(val0absaddr(rordata, G_RULE, A_RULE_TERC));

                        sprintf(str, "0x%X", (((*p_temporaryromeconsulflags) & 0x82) == 0x00));
                        DrawText(str, windowedScreenWidth-30, 80, 10, MAGENTA);

                        cursor.x = windowedScreenWidth - ceil(0.5 * DOWN) - (12+1+12) * RIGHT;
                        cursor.y = ceil(0.5 * DOWN);
                        r = rect(cursor, 12, 2);
                        bool hovering = CheckCollisionPointRec(mouse, r);
                        if (((*p_temporaryromeconsulflags) & 0x82) == 0x02)
                        {
                            A_SENA_IDNR_t lowest_idnr = 0xFF;
                            for (int senaidx = 0; senaidx < group(rordata, G_SENA).elems; senaidx++)
                            {
                                if (0 <= *(A_SENA_ALIG_t*)valabsaddr(rordata, G_SENA, A_SENA_ALIG, senaidx))
                                {
                                    A_SENA_IDNR_t idnr = *(A_SENA_IDNR_t*)valabsaddr(rordata, G_SENA, A_SENA_IDNR, senaidx);
                                    lowest_idnr = min(lowest_idnr, idnr);
                                }
                            }
                            for (int senaidx = 0; senaidx < group(rordata, G_SENA).elems; senaidx++)
                            {
                                if (0 <= *(A_SENA_ALIG_t*)valabsaddr(rordata, G_SENA, A_SENA_ALIG, senaidx))
                                {
                                    A_SENA_IDNR_t idnr = *(A_SENA_IDNR_t*)valabsaddr(rordata, G_SENA, A_SENA_IDNR, senaidx);
                                    if (idnr == lowest_idnr)
                                    {
                                        *(A_SENA_INF1_t*)valabsaddr(rordata, G_SENA, A_SENA_INF1, senaidx) += *(A_OFFI_INFL_t*)valabsaddr(rordata, G_OFFI, A_OFFI_INFL, OFFICE_ROME_CONSUL);
                                        *(A_SENA_PRCO_t*)valabsaddr(rordata, G_SENA, A_SENA_PRCO, senaidx) =  (A_SENA_PRCO_t)(1);
                                        *p_temporaryromeconsulflags = *p_temporaryromeconsulflags | 0x80;
                                    }
                                }
                            }
                        }
                        else if (((*p_temporaryromeconsulflags) & 0x82) == 0x00 || (hovering && currentGesture == GESTURE_TAP))
                        {
                            int b;  // bitcount of population
                            for (b = 0; b < 8; b++)
                            {
                                if ( (senator_count - 1) <= (1 << b) ) break;
                            }

                            sprintf(str, "TRC; b: %i", b);
                            TraceLog(LOG_DEBUG, str);

                            if (newentropyidx <= entropyidx + 1)
                            {
                                *(A_GAME_SPHS_t*)(val0absaddr(rordata, G_GAME, A_GAME_SPHS)) = (A_GAME_SPHS_t)SPHS_PREP_TEMPORARYROMECONSUL_RANDOM_ENTROPYREQ;
                                goto LABEL_GAME_TREE;
                            }

                            uint8_t k = 0xFF;
                            while (senator_count - 1 < k)
                            {
                                sprintf(str, "TRC; entropyidx: %i, entropynextbit: %i; %X %X", entropyidx, entropynextbit, entropy[(uint8_t)(entropyidx % 256)], entropy[(uint8_t)((entropyidx+1) % 256)]);
                                TraceLog(LOG_DEBUG, str);
                                if (k == 0xFF)  // initial value
                                {
                                        if (8 < entropynextbit + b)
                                        {
                                            k = (entropy[(uint8_t)(entropyidx % 256)] >> entropynextbit) + ((((entropy[(uint8_t)((entropyidx+1) % 256)] << (16 - b - entropynextbit)) & 0xFF) >> (8 - b)));
                                            entropyidx += 1;
                                            entropynextbit = (entropynextbit + b) - 8;
                                            sprintf(str, "TRC; 2 bytes; k: %i", k);
                                        }
                                        else
                                        {
                                            k = ( (entropy[(uint8_t)(entropyidx % 256)] << (8 - b - entropynextbit) ) & 0xFF ) >> (8 - b);
                                            entropynextbit += b;
                                            if (entropynextbit == 8)
                                            {
                                                entropyidx += 1;
                                                entropynextbit = 0;
                                            }
                                            sprintf(str, "TRC; 1 byte; k: %i", k);
                                        }
                                }
                                TraceLog(LOG_DEBUG, str);
                            }

                            for (int senaidx = 0; senaidx < group(rordata, G_SENA).elems; senaidx++)
                            {
                                A_SENA_OFFI_t* p_current_office = (A_SENA_OFFI_t*)valabsaddr(rordata, G_SENA, A_SENA_OFFI, senaidx);
                                if (*p_current_office == (A_SENA_OFFI_t)(OFFICE_ROME_CONSUL))
                                {
                                    *p_current_office = (A_SENA_OFFI_t)(0);
                                    *(A_SENA_INF1_t*)valabsaddr(rordata, G_SENA, A_SENA_INF1, senaidx) -= *(A_OFFI_INFL_t*)valabsaddr(rordata, G_OFFI, A_OFFI_INFL, OFFICE_ROME_CONSUL);
                                    *(A_SENA_PRCO_t*)valabsaddr(rordata, G_SENA, A_SENA_PRCO, senaidx) =  (A_SENA_PRCO_t)(0);
                                }
                            }

                            for (int senaidx = 0; senaidx < group(rordata, G_SENA).elems; senaidx++)
                            {
                                if (0 <= *(A_SENA_ALIG_t*)valabsaddr(rordata, G_SENA, A_SENA_ALIG, senaidx))
                                {
                                    A_SENA_OFFI_t* p_current_office = (A_SENA_OFFI_t*)valabsaddr(rordata, G_SENA, A_SENA_OFFI, senaidx);
                                    if (m == k)
                                    {
                                        *p_current_office = (A_SENA_OFFI_t)(OFFICE_ROME_CONSUL);
                                        *(A_SENA_INF1_t*)valabsaddr(rordata, G_SENA, A_SENA_INF1, senaidx) += *(A_OFFI_INFL_t*)valabsaddr(rordata, G_OFFI, A_OFFI_INFL, OFFICE_ROME_CONSUL);
                                        *(A_SENA_PRCO_t*)valabsaddr(rordata, G_SENA, A_SENA_PRCO, senaidx) =  (A_SENA_PRCO_t)(1);
                                        *p_temporaryromeconsulflags = *p_temporaryromeconsulflags | 0x80;
                                        break;
                                    }
                                    m += 1;
                                }
                            }
                            if (hovering && currentGesture == GESTURE_TAP)
                            {
                                DrawRectangleRec(r, COLOR_BUTTONCLICKED);
                            }
                            else if (hovering)
                            {
                                DrawRectangleRec(r, COLOR_BUTTON);
                            }
                        }
                        DrawRectangleLinesEx(r, 2, COLOR_BUTTONOUTLINE);
                        DrawText2(font1, "RANDOM", r, FONTSIZE, FONTSPACING, WHITE, TextCenter);

                        LABEL_SPHS_PREP_TEMPORARYROMECONSUL_POSTRANDOM:

                        cursor.y = 3 * DOWN;

                        for (int factidx = 1; factidx < *(A_GAME_NFAC_t*)val0absaddr(rordata, G_GAME, A_GAME_NFAC); factidx++)
                        {
                            cursor.x = RIGHT;
                            r = rect(cursor, 30, 2);
                            DrawRectangleRec(r, COLOR_FACTIONHEADER);
                            DrawText2(font1, TextToUpper((char*)(valabsaddr(rordata, G_FACT, A_FACT_NAME, factidx))), r, FONTSIZE, FONTSPACING, WHITE, TextTopLeft);
                            cursor.y += 1 * DOWN;
                            cursor.x += (2 + 2 + SENATORNAMEWIDTH) * RIGHT;
                            r = rect(cursor, 2, 1);
                            DrawText2(font1, "M", r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                            cursor.x += 2 * RIGHT;
                            r = rect(cursor, 2, 1);
                            DrawText2(font1, "O", r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                            cursor.x += 2 * RIGHT;
                            r = rect(cursor, 2, 1);
                            DrawText2(font1, "L", r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                            cursor.x += 2 * RIGHT;
                            r = rect(cursor, 2, 1);
                            DrawText2(font1, "I", rect(cursor, 2, 1), FONTSIZE, FONTSPACING, WHITE, TextCenter);
                            cursor.x += 2 * RIGHT;
                            r = rect(cursor, 2, 1);
                            DrawText2(font1, "P", r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                            cursor.y += 1 * DOWN;

                            for (int senaidx = 0; senaidx < group(rordata, G_SENA).elems; senaidx++)
                            {
                                if ( *((A_SENA_ALIG_t*)(val0absaddr(rordata, G_SENA, A_SENA_ALIG))+senaidx) == (A_SENA_ALIG_t)(factidx) )  // typecast is a must!
                                {
                                    cursor.x = RIGHT;
                                    r = rect(cursor, 30, 1);
                                    if (CheckCollisionPointRec(mouse, r))
                                    {
                                        if (currentGesture != lastGesture && currentGesture == GESTURE_TAP) // select manually
                                        {

                                            for (int senaidx = 0; senaidx < group(rordata, G_SENA).elems; senaidx++)
                                            {
                                                A_SENA_OFFI_t* p_current_office = (A_SENA_OFFI_t*)valabsaddr(rordata, G_SENA, A_SENA_OFFI, senaidx);
                                                if (*p_current_office == (A_SENA_OFFI_t)(OFFICE_ROME_CONSUL))
                                                {
                                                    *p_current_office = (A_SENA_OFFI_t)(0);
                                                    *(A_SENA_INF1_t*)valabsaddr(rordata, G_SENA, A_SENA_INF1, senaidx) -= *(A_OFFI_INFL_t*)valabsaddr(rordata, G_OFFI, A_OFFI_INFL, OFFICE_ROME_CONSUL);
                                                    *(A_SENA_PRCO_t*)valabsaddr(rordata, G_SENA, A_SENA_PRCO, senaidx) =  (A_SENA_PRCO_t)(0);
                                                }
                                            }
                                            *(A_SENA_OFFI_t*)valabsaddr(rordata, G_SENA, A_SENA_OFFI, senaidx) = (A_SENA_OFFI_t)(OFFICE_ROME_CONSUL);
                                            *(A_SENA_INF1_t*)valabsaddr(rordata, G_SENA, A_SENA_INF1, senaidx) += *(A_OFFI_INFL_t*)valabsaddr(rordata, G_OFFI, A_OFFI_INFL, OFFICE_ROME_CONSUL);
                                            *(A_SENA_PRCO_t*)valabsaddr(rordata, G_SENA, A_SENA_PRCO, senaidx) =  (A_SENA_PRCO_t)(1);
                                            *p_temporaryromeconsulflags = *p_temporaryromeconsulflags | 0x80;
                                        }
                                        else
                                        {
                                            DrawRectangleRec(r, COLOR_MOUSEHOVER_SELECTABLE);
                                        }
                                    }
                                    // else if (selected != senaidx)
                                    // {
                                    //     DrawRectangleRec(r, COLOR_BLACKCARDBG);
                                    // }
                                    if (*(A_SENA_OFFI_t*)valabsaddr(rordata, G_SENA, A_SENA_OFFI, senaidx) == (A_SENA_OFFI_t)(OFFICE_ROME_CONSUL))
                                    {
                                        r = rect(cursor, 2, 1);
                                        DrawRectangleRec(r, COLOR_OFFICE);
                                        DrawText2(font1, (char*)(valabsaddr(rordata, G_OFFI, A_OFFI_SNAM, OFFICE_ROME_CONSUL)), r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                                    }
                                    cursor.x += 2 * RIGHT;
                                    sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_IDNR, senaidx));
                                    r = rect(cursor, 2, 1);
                                    DrawText2(font1, str, r, FONTSIZE, FONTSPACING, WHITE, TextRight);
                                    cursor.x += 2 * RIGHT;
                                    r = rect(cursor, SENATORNAMEWIDTH, 1);
                                    DrawText2(font1, (char*)(valabsaddr(rordata, G_SENA, A_SENA_NAME, senaidx)), r, FONTSIZE, FONTSPACING, WHITE, TextLeft);
                                    cursor.x += SENATORNAMEWIDTH * RIGHT;
                                    sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_MIL1, senaidx));
                                    r = rect(cursor, 2, 1);
                                    DrawText2(font1, str, r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                                    cursor.x += 2 * RIGHT;
                                    sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_ORA1, senaidx));
                                    r = rect(cursor, 2, 1);
                                    DrawText2(font1, str, r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                                    cursor.x += 2 * RIGHT;
                                    sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_LOY1, senaidx));
                                    r = rect(cursor, 2, 1);
                                    DrawText2(font1, str, r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                                    cursor.x += 2 * RIGHT;
                                    sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_INF1, senaidx));
                                    r = rect(cursor, 2, 1);
                                    DrawText2(font1, str, r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                                    cursor.x += 2 * RIGHT;
                                    sprintf(str, "%i", (int8_t)(*valabsaddr(rordata, G_SENA, A_SENA_POP1, senaidx)));
                                    r = rect(cursor, 2, 1);
                                    DrawText2(font1, str, r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                                    cursor.y += DOWN;
                                }
                            }
                            cursor.y += DOWN;
                        } 
                    }
                }
            } break;




            case 1:
            {
                cursor.x = RIGHT;
                cursor.y = DOWN;

                for (int factidx = 0; factidx < *(A_GAME_NFAC_t*)val0absaddr(rordata, G_GAME, A_GAME_NFAC); factidx++)
                {
                    cursor.x = RIGHT;

                    r = rect(cursor, 30, 2);
                    DrawRectangleRec(r, COLOR_FACTIONHEADER);
                    DrawText2(font1, TextToUpper((char*)(valabsaddr(rordata, G_FACT, A_FACT_NAME, factidx))), r, FONTSIZE, FONTSPACING, WHITE, TextTopLeft);
                    cursor.y += 1 * DOWN;
                    cursor.x += (2 + 2 + SENATORNAMEWIDTH) * RIGHT;
                    r = rect(cursor, 2, 1);
                    DrawText2(font1, "M", r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                    cursor.x += 2 * RIGHT;
                    r = rect(cursor, 2, 1);
                    DrawText2(font1, "O", r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                    cursor.x += 2 * RIGHT;
                    r = rect(cursor, 2, 1);
                    DrawText2(font1, "L", r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                    cursor.x += 2 * RIGHT;
                    r = rect(cursor, 2, 1);
                    DrawText2(font1, "I", rect(cursor, 2, 1), FONTSIZE, FONTSPACING, WHITE, TextCenter);
                    cursor.x += 2 * RIGHT;
                    r = rect(cursor, 2, 1);
                    DrawText2(font1, "P", r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                    cursor.y += 1 * DOWN;

                    for (int senaidx = 0; senaidx < group(rordata, G_SENA).elems; senaidx++)
                    {
                        if ( *((A_SENA_ALIG_t*)(val0absaddr(rordata, G_SENA, A_SENA_ALIG))+senaidx) == (A_SENA_ALIG_t)(factidx) )  // typecast is a must!
                        {
                            cursor.x = RIGHT;
                            cursor.x += 2 * RIGHT;
                            sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_IDNR, senaidx));
                            r = rect(cursor, 2, 1);
                            DrawText2(font1, str, r, FONTSIZE, FONTSPACING, WHITE, TextRight);
                            cursor.x += 2 * RIGHT;
                            r = rect(cursor, SENATORNAMEWIDTH, 1);
                            DrawText2(font1, (char*)(valabsaddr(rordata, G_SENA, A_SENA_NAME, senaidx)), r, FONTSIZE, FONTSPACING, WHITE, TextLeft);
                            cursor.x += SENATORNAMEWIDTH * RIGHT;
                            sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_MIL1, senaidx));
                            r = rect(cursor, 2, 1);
                            if (CheckCollisionPointRec(mouse, r))
                            {
                                DrawRectangleRec(r, COLOR_MOUSEHOVER_GAMEMASTER);
                                if (IsKeyPressed(KEY_KP_ADD)) increase(rordata, G_SENA, A_SENA_MIL1, senaidx);
                                if (IsKeyPressed(KEY_KP_SUBTRACT)) decrease(rordata, G_SENA, A_SENA_MIL1, senaidx);
                                if (IsKeyPressed(KEY_BACKSPACE)) *valabsaddr(rordata, G_SENA, A_SENA_MIL1, senaidx) = *valabsaddr(rordata, G_SENA, A_SENA_MIL0, senaidx);
                            }
                            DrawText2(font1, str, r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                            cursor.x += 2 * RIGHT;
                            sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_ORA1, senaidx));
                            r = rect(cursor, 2, 1);
                            if (CheckCollisionPointRec(mouse, r))
                            {
                                DrawRectangleRec(r, COLOR_MOUSEHOVER_GAMEMASTER);
                                if (IsKeyPressed(KEY_KP_ADD)) increase(rordata, G_SENA, A_SENA_ORA1, senaidx);
                                if (IsKeyPressed(KEY_KP_SUBTRACT)) decrease(rordata, G_SENA, A_SENA_ORA1, senaidx);
                                if (IsKeyPressed(KEY_BACKSPACE)) *valabsaddr(rordata, G_SENA, A_SENA_ORA1, senaidx) = *valabsaddr(rordata, G_SENA, A_SENA_ORA0, senaidx);
                            }
                            DrawText2(font1, str, r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                            cursor.x += 2 * RIGHT;
                            sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_LOY1, senaidx));
                            r = rect(cursor, 2, 1);
                            if (CheckCollisionPointRec(mouse, r))
                            {
                                DrawRectangleRec(r, COLOR_MOUSEHOVER_GAMEMASTER);
                                if (IsKeyPressed(KEY_KP_ADD)) increase(rordata, G_SENA, A_SENA_LOY1, senaidx);
                                if (IsKeyPressed(KEY_KP_SUBTRACT)) decrease(rordata, G_SENA, A_SENA_LOY1, senaidx);
                                if (IsKeyPressed(KEY_BACKSPACE)) *valabsaddr(rordata, G_SENA, A_SENA_LOY1, senaidx) = *valabsaddr(rordata, G_SENA, A_SENA_LOY0, senaidx);
                            }
                            DrawText2(font1, str, r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                            cursor.x += 2 * RIGHT;
                            sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_INF1, senaidx));
                            r = rect(cursor, 2, 1);
                            if (CheckCollisionPointRec(mouse, r))
                            {
                                DrawRectangleRec(r, COLOR_MOUSEHOVER_GAMEMASTER);
                                if (IsKeyPressed(KEY_KP_ADD)) increase(rordata, G_SENA, A_SENA_INF1, senaidx);
                                if (IsKeyPressed(KEY_KP_SUBTRACT)) decrease(rordata, G_SENA, A_SENA_INF1, senaidx);
                                if (IsKeyPressed(KEY_BACKSPACE)) *valabsaddr(rordata, G_SENA, A_SENA_INF1, senaidx) = *valabsaddr(rordata, G_SENA, A_SENA_INF0, senaidx);
                            }
                            DrawText2(font1, str, r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                            cursor.x += 2 * RIGHT;
                            sprintf(str, "%i", (int8_t)(*valabsaddr(rordata, G_SENA, A_SENA_POP1, senaidx)));
                            r = rect(cursor, 2, 1);
                            if (CheckCollisionPointRec(mouse, r))
                            {
                                DrawRectangleRec(r, COLOR_MOUSEHOVER_GAMEMASTER);
                                if (IsKeyPressed(KEY_KP_ADD)) increase(rordata, G_SENA, A_SENA_POP1, senaidx);
                                if (IsKeyPressed(KEY_KP_SUBTRACT)) decrease(rordata, G_SENA, A_SENA_POP1, senaidx);
                                if (IsKeyPressed(KEY_BACKSPACE)) *valabsaddr(rordata, G_SENA, A_SENA_POP1, senaidx) = *valabsaddr(rordata, G_SENA, A_SENA_POP0, senaidx);
                            }
                            DrawText2(font1, str, r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                            cursor.y += DOWN;
                        }
                    }
                    cursor.y += DOWN;
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