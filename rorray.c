#include <math.h>
#include <stdint.h>

#include "raylib.h"

#include "rorfile.h"

#define NUM_FACTIONS    6
#define RIGHT  11
#define DOWN   13
#define PADDING   1

#define FONTSPACING     2
#define FONTSIZE        font.baseSize*1.0f

#define TARGET_FPS     30

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

__INITRORAPI__ // initializes the API structs


#define SENATORNAMEWIDTH  8

#define COLOR_BLACKCARDBG WHITE
#define COLOR_BUTTON DARKGRAY
#define COLOR_BUTTONCLICKED YELLOW
#define COLOR_BUTTONOUTLINE ORANGE
#define COLOR_FACTIONHEADER DARKGRAY
#define COLOR_MOUSEDRAG  DARKBLUE
#define COLOR_MOUSEHOVER_DRAGABLE  DARKBLUE
#define COLOR_MOUSEHOVER_DROPTARGET  DARKGREEN
#define COLOR_MOUSEHOVER_EDITABLE  ORANGE
#define COLOR_MOUSEHOVER_GAMEMASTER  MAGENTA


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
            position.x += (int)(box.width - textsize.x - FONTSPACING);
            position.y += (int)((box.height - textsize.y)/2);
        } break;
        case TextTopLeft:
        {
            position.x += FONTSPACING;
        } break;
        case TextTopCenter:
        {
            position.x += (int)((box.width - textsize.x)/2);
        } break;
        case TextTopRight:
        {
            position.x += (int)(box.width - textsize.x - FONTSPACING);
        } break;
        case TextLeft:
        default:
            position.x += FONTSPACING;
            position.y += (int)((box.height - textsize.y)/2);
    }
    DrawTextEx(font, text, position, fontSize, spacing, tint);
}



int main(void)
{
    Vector2 cursor;
    Rectangle r;
    char str[99];  // TODO: for debugging

    const int screenWidth = 800;
    const int screenHeight = 600;

    Font font;

    unsigned int rordataLength = SIZE;
    unsigned char *rordata = LoadFileData("scenario.ror", &rordataLength);
 
    Vector2 mouse;    
    int currentGesture = GESTURE_NONE;
    int lastGesture = GESTURE_NONE;

    //Rectangle* rects = (Rectangle *)MemAlloc(header(rordata).elems*sizeof(Rectangle));

    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "RoRRAY");

    SetTargetFPS(TARGET_FPS);

    font = LoadFont("romulus.png");

    int framesCounter = 0;
    int selected = -1;

    while (!WindowShouldClose())
    {
        lastGesture = currentGesture;
        mouse = GetTouchPosition(0);
        currentGesture = GetGestureDetected();

        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);

        // DrawFPS(screenWidth-100, 10); // for debug


        switch (*(A_GAME_PHSE_t*)(val0absaddr(rordata, G_GAME, A_GAME_PHSE)))
        {
            case PHSE_PREP: /* PREPARE TO PLAY */
            {
                switch (*(A_GAME_SPHS_t*)(val0absaddr(rordata, G_GAME, A_GAME_SPHS)))
                {
                    case SPHS_PREP_TAKEFACTIONS: /* 4.1 GAMEBOARD (Take Seets) */
                    {
                        cursor.x = RIGHT;
                        cursor.y = 1* DOWN;
                        DrawTextEx(font, "NAME THE PLAYING FACTIONS OR DELETE NAMES FOR EXCLUSION", cursor, FONTSIZE, FONTSPACING, ORANGE);

                        cursor.x = screenWidth - ceil(0.5 * DOWN) - 12 * RIGHT;
                        cursor.y = floor(0.5 * DOWN);
                        r = rect(cursor, 12, 2);
                        if (CheckCollisionPointRec(mouse, r))
                        {
                            if (currentGesture == GESTURE_TAP)
                            {
                                DrawRectangleRec(r, COLOR_BUTTONCLICKED);
                                *(A_GAME_SPHS_t*)(val0absaddr(rordata, G_GAME, A_GAME_SPHS)) = (A_GAME_SPHS_t)SPHS_PREP_DEALSENATORS;
                                for (int i = 1; i < group(rordata, G_FACT).elems; i++)
                                {
                                    int letterCount = strlen((char*)(valabsaddr(rordata, G_FACT, A_FACT_NAME, i)));
                                    if (letterCount == 0)
                                    {
                                        *(A_FACT_CNGR_t*)valabsaddr(rordata, G_FACT, A_FACT_CNGR, i) = (A_FACT_CNGR_t)(G_NULL);
                                    }
                                }
                                selected = -1;
                                framesCounter = 0;
                            }
                            else
                            {
                                DrawRectangleRec(r, COLOR_BUTTON);
                            }
                        }
                        DrawRectangleLinesEx(r, 2, COLOR_BUTTONOUTLINE);
                        DrawText2(font, "DONE", r, FONTSIZE, FONTSPACING, WHITE, TextCenter);

                        cursor.y = 3 * DOWN;

                        bool anyselected = false;
                        int maxletterCount = (attr(rordata, G_FACT, A_FACT_NAME).type & 0x3FFF)-1;
                        int rectwidth = ceil(((MeasureTextEx(font, "M", 2 * FONTSIZE, FONTSPACING).x + FONTSPACING) * (maxletterCount) + FONTSPACING)/RIGHT);
                        sprintf(str, "%d", rectwidth);
                        DrawText(str, 10, 540, 10, WHITE); 
                        for (int i = 1; i < group(rordata, G_FACT).elems; i++)
                        {
                            cursor.x = RIGHT;

                            r = rect(cursor, rectwidth, 2);
                            if (CheckCollisionPointRec(mouse, r))
                            {
                                anyselected = true;
                                if (i != selected)
                                {
                                    selected = i;
                                    framesCounter = 0;
                                }
                                int letterCount = strlen((char*)(valabsaddr(rordata, G_FACT, A_FACT_NAME, i)));

                                sprintf(str, "%d", letterCount);
                                DrawText(str, 10, 560, 10, WHITE); 
                                sprintf(str, "%d", maxletterCount);
                                DrawText(str, 40, 560, 10, WHITE); 
                                DrawRectangleRec(r, COLOR_MOUSEHOVER_EDITABLE);
                                SetMouseCursor(MOUSE_CURSOR_IBEAM);
                                int key = GetCharPressed();
                                while (key > 0)
                                {
                                    DrawText((char*)&key, 10, 570, 10, WHITE); 
                                    sprintf(str, "%d", key);
                                    DrawText(str, 10, 580, 10, WHITE); 
                                    if (  ( ((key >= 65) && (key <= 90))||((key >= 97) && (key <= 122))||(key == 32) ) && (letterCount < maxletterCount)  )
                                    {
                                        DrawText((char*)&key, 40, 570, 10, WHITE); 
                                        *((char*)(valabsaddr(rordata, G_FACT, A_FACT_NAME, i))+letterCount) = (char)key;
                                        *((char*)(valabsaddr(rordata, G_FACT, A_FACT_NAME, i))+letterCount+1) = '\0'; // Add null terminator at the end of the string.
                                        letterCount++;
                                    }
                                    key = GetCharPressed();  // Check next character in the queue
                                }
                                if (IsKeyPressed(KEY_BACKSPACE))
                                {
                                    letterCount--;
                                    if (letterCount < 0) letterCount = 0;
                                    *((char*)(valabsaddr(rordata, G_FACT, A_FACT_NAME, i))+letterCount) = '\0';
                                } 
                                if ((letterCount < maxletterCount) && (((framesCounter/10)%2) == 0))
                                {
                                    Vector2 textsize = MeasureTextEx(font, (char*)(valabsaddr(rordata, G_FACT, A_FACT_NAME, i)), 2 * FONTSIZE, FONTSPACING);
                                    Rectangle _r = {r.x + textsize.x + FONTSPACING, r.y, r.width - textsize.x - FONTSPACING, r.height};
                                    DrawText2(font, "_", _r, 2 * FONTSIZE, FONTSPACING, WHITE, TextLeft);
                                }
                                framesCounter++;
                            }
                            else
                            {
                                DrawRectangleRec(r, COLOR_FACTIONHEADER);
                            }
                            DrawText2(font, (char*)(valabsaddr(rordata, G_FACT, A_FACT_NAME, i)), r, 2 * FONTSIZE, FONTSPACING, WHITE, TextLeft);
                            cursor.y += 2 * DOWN;
                        }
                        if (!anyselected)
                        {
                            SetMouseCursor(MOUSE_CURSOR_DEFAULT);
                            selected = -1;
                            framesCounter = 0;
                        }
                        sprintf(str, "%i", framesCounter);
                        DrawText(str, 10, 550, 10, WHITE);
                    } break;

                    case SPHS_PREP_DEALSENATORS: /* 4.4 CARDS (Deal Senators) */
                    {
                        Vector2 selectedvector;
                        cursor.x = RIGHT;
                        cursor.y = 1 * DOWN;
                        sprintf(str, "DEAL %i SENATORS TO EACH FACTIONS (DRAG AND DROP)", *val0absaddr(rordata, G_SETT, A_SETT_NSEN));
                        DrawTextEx(font, str, cursor, FONTSIZE, FONTSPACING, ORANGE);

                        cursor.x = screenWidth - ceil(0.5 * DOWN) - 12 * RIGHT;
                        cursor.y = ceil(0.5 * DOWN);
                        r = rect(cursor, 12, 2);
                        if (CheckCollisionPointRec(mouse, r))
                        {
                            if (currentGesture == GESTURE_TAP)
                            {
                                DrawRectangleRec(r, COLOR_BUTTONCLICKED);
                                *(A_GAME_SPHS_t*)(val0absaddr(rordata, G_GAME, A_GAME_SPHS)) = (A_GAME_SPHS_t)SPHS_PREP_SELECTFACTIONLEADERS;
                                selected = -1;
                            }
                            else
                            {
                                DrawRectangleRec(r, COLOR_BUTTON);
                            }
                        }
                        DrawRectangleLinesEx(r, 2, COLOR_BUTTONOUTLINE);
                        DrawText2(font, "DONE", r, FONTSIZE, FONTSPACING, WHITE, TextCenter);

                        cursor.y = 3 * DOWN;

                        for (int i = 1; i < group(rordata, G_FACT).elems; i++)
                        {
                            if (*(A_FACT_CNGR_t*)valabsaddr(rordata, G_FACT, A_FACT_CNGR, i) == (A_FACT_CNGR_t)(G_NULL)) continue;
                            cursor.x = RIGHT;
                            r = rect(cursor, 30, 2);
                            if (selected != -1 && CheckCollisionPointRec(mouse, r))
                            {
                                DrawRectangleRec(r, COLOR_MOUSEHOVER_DROPTARGET);
                                if (currentGesture != lastGesture && currentGesture == GESTURE_NONE)
                                {
                                    *(A_SENA_ALIG_t*)valabsaddr(rordata, G_SENA, A_SENA_ALIG, selected) = (A_SENA_ALIG_t)(i);
                                    *(A_SENA_CNGR_t*)valabsaddr(rordata, G_SENA, A_SENA_CNGR, selected) = (A_SENA_CNGR_t)(G_FORU);
                                    *(A_SENA_CNNR_t*)valabsaddr(rordata, G_SENA, A_SENA_CNNR, selected) = (A_SENA_CNNR_t)(0);
                                    selected = -1;
                                }
                            }
                            else
                            {
                                DrawRectangleRec(r, COLOR_FACTIONHEADER);
                            }
                            DrawText2(font, TextToUpper((char*)(valabsaddr(rordata, G_FACT, A_FACT_NAME, i))), r, FONTSIZE, FONTSPACING, WHITE, TextTopLeft);
                            cursor.y += 1 * DOWN;
                            cursor.x += (2 + 2 + SENATORNAMEWIDTH) * RIGHT;
                            r = rect(cursor, 2, 1);
                            DrawText2(font, "M", r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                            cursor.x += 2 * RIGHT;
                            r = rect(cursor, 2, 1);
                            DrawText2(font, "O", r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                            cursor.x += 2 * RIGHT;
                            r = rect(cursor, 2, 1);
                            DrawText2(font, "L", r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                            cursor.x += 2 * RIGHT;
                            r = rect(cursor, 2, 1);
                            DrawText2(font, "I", rect(cursor, 2, 1), FONTSIZE, FONTSPACING, WHITE, TextCenter);
                            cursor.x += 2 * RIGHT;
                            r = rect(cursor, 2, 1);
                            DrawText2(font, "P", r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                            cursor.y += 1 * DOWN;

                            for (int j = 0; j < group(rordata, G_SENA).elems; j++)
                            {
                                if ( *((A_SENA_ALIG_t*)(val0absaddr(rordata, G_SENA, A_SENA_ALIG))+j) == (A_SENA_ALIG_t)(i) )  // typecast is a must!
                                {
                                    cursor.x = RIGHT;
                                    r = rect(cursor, 30, 1);
                                    if (selected == -1 && CheckCollisionPointRec(mouse, r))
                                    {
                                        if (currentGesture != lastGesture && currentGesture == GESTURE_TAP) // pick up
                                        {
                                            selected = j;
                                            selectedvector.x = mouse.x - r.x;
                                            selectedvector.y = mouse.y - r.y;
                                        }
                                        else
                                        {
                                            DrawRectangleRec(r, COLOR_MOUSEHOVER_DRAGABLE);
                                        }
                                    }
                                    else if (selected != j)
                                    {
                                        DrawRectangleRec(r, COLOR_BLACKCARDBG);
                                    }
                                    cursor.x += 2 * RIGHT;
                                    sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_IDNR, j));
                                    r = rect(cursor, 2, 1);
                                    DrawText2(font, str, r, FONTSIZE, FONTSPACING, BLACK, TextRight);
                                    cursor.x += 2 * RIGHT;
                                    r = rect(cursor, SENATORNAMEWIDTH, 1);
                                    DrawText2(font, (char*)(valabsaddr(rordata, G_SENA, A_SENA_NAME, j)), r, FONTSIZE, FONTSPACING, BLACK, TextLeft);
                                    cursor.x += SENATORNAMEWIDTH * RIGHT;
                                    sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_MIL1, j));
                                    r = rect(cursor, 2, 1);
                                    DrawText2(font, str, r, FONTSIZE, FONTSPACING, BLACK, TextCenter);
                                    cursor.x += 2 * RIGHT;
                                    sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_ORA1, j));
                                    r = rect(cursor, 2, 1);
                                    DrawText2(font, str, r, FONTSIZE, FONTSPACING, BLACK, TextCenter);
                                    cursor.x += 2 * RIGHT;
                                    sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_LOY1, j));
                                    r = rect(cursor, 2, 1);
                                    DrawText2(font, str, r, FONTSIZE, FONTSPACING, BLACK, TextCenter);
                                    cursor.x += 2 * RIGHT;
                                    sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_INF1, j));
                                    r = rect(cursor, 2, 1);
                                    DrawText2(font, str, r, FONTSIZE, FONTSPACING, BLACK, TextCenter);
                                    cursor.x += 2 * RIGHT;
                                    sprintf(str, "%i", (int8_t)(*valabsaddr(rordata, G_SENA, A_SENA_POP1, j)));
                                    r = rect(cursor, 2, 1);
                                    DrawText2(font, str, r, FONTSIZE, FONTSPACING, BLACK, TextCenter);
                                    cursor.y += DOWN;

                                }
                            }
                            cursor.y += DOWN;

                        } 

                        cursor.y = 3 * DOWN;
                        cursor.x = (1+30+2) * RIGHT;
                        r = rect(cursor, 30, 2);
                        if (selected != -1 && CheckCollisionPointRec(mouse, r))
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
                        DrawText2(font, "SENATORS", r, FONTSIZE, FONTSPACING, WHITE, TextTopLeft);
                        cursor.y += 1 * DOWN;
                        cursor.x += (2 + 2 + SENATORNAMEWIDTH) * RIGHT;
                        r = rect(cursor, 2, 1);
                        DrawText2(font, "M", r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                        cursor.x += 2 * RIGHT;
                        r = rect(cursor, 2, 1);
                        DrawText2(font, "O", r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                        cursor.x += 2 * RIGHT;
                        r = rect(cursor, 2, 1);
                        DrawText2(font, "L", r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                        cursor.x += 2 * RIGHT;
                        r = rect(cursor, 2, 1);
                        DrawText2(font, "I", rect(cursor, 2, 1), FONTSIZE, FONTSPACING, WHITE, TextCenter);
                        cursor.x += 2 * RIGHT;
                        r = rect(cursor, 2, 1);
                        DrawText2(font, "P", r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                        cursor.y += 1 * DOWN;

                        for (int j = 0; j < group(rordata, G_SENA).elems; j++)
                        {
                            if (
                                (*(A_SENA_CNGR_t*)valabsaddr(rordata, G_SENA, A_SENA_CNGR, j) != (A_SENA_CNGR_t)(G_DECK))
                                ||
                                (*(A_SENA_CNNR_t*)valabsaddr(rordata, G_SENA, A_SENA_CNNR, j) != (A_SENA_CNNR_t)(0))
                            ) continue;
                            if (j == selected) continue;
                            cursor.x = (1+30+2) * RIGHT;
                            r = rect(cursor, 30, 1);
                            if (selected == -1 && CheckCollisionPointRec(mouse, r))
                            {
                                if (currentGesture != lastGesture && currentGesture == GESTURE_TAP) // pick up
                                {
                                    selected = j;
                                    selectedvector.x = mouse.x - r.x;
                                    selectedvector.y = mouse.y - r.y;
                                }
                                else
                                {
                                    DrawRectangleRec(r, COLOR_MOUSEHOVER_DRAGABLE);
                                }
                            }
                            else if (selected != j)
                            {
                                DrawRectangleRec(r, COLOR_BLACKCARDBG);
                            }
                            cursor.x += 2 * RIGHT;
                            sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_IDNR, j));
                            r = rect(cursor, 2, 1);
                            DrawText2(font, str, r, FONTSIZE, FONTSPACING, BLACK, TextRight);
                            cursor.x += 2 * RIGHT;
                            r = rect(cursor, SENATORNAMEWIDTH, 1);
                            DrawText2(font, (char*)(valabsaddr(rordata, G_SENA, A_SENA_NAME, j)), r, FONTSIZE, FONTSPACING, BLACK, TextLeft);
                            cursor.x += SENATORNAMEWIDTH * RIGHT;
                            sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_MIL1, j));
                            r = rect(cursor, 2, 1);
                            DrawText2(font, str, r, FONTSIZE, FONTSPACING, BLACK, TextCenter);
                            cursor.x += 2 * RIGHT;
                            sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_ORA1, j));
                            r = rect(cursor, 2, 1);
                            DrawText2(font, str, r, FONTSIZE, FONTSPACING, BLACK, TextCenter);
                            cursor.x += 2 * RIGHT;
                            sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_LOY1, j));
                            r = rect(cursor, 2, 1);
                            DrawText2(font, str, r, FONTSIZE, FONTSPACING, BLACK, TextCenter);
                            cursor.x += 2 * RIGHT;
                            sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_INF1, j));
                            r = rect(cursor, 2, 1);
                            DrawText2(font, str, r, FONTSIZE, FONTSPACING, BLACK, TextCenter);
                            cursor.x += 2 * RIGHT;
                            sprintf(str, "%i", (int8_t)(*valabsaddr(rordata, G_SENA, A_SENA_POP1, j)));
                            r = rect(cursor, 2, 1);
                            DrawText2(font, str, r, FONTSIZE, FONTSPACING, BLACK, TextCenter);
                            cursor.y += 1 * DOWN;
                        }
                        if (selected != -1) // draw selected senator on top off all other
                        {
                            int j = selected;
                            DrawText((char*)(valabsaddr(rordata, G_SENA, A_SENA_NAME, j)), 10, 570, 10, WHITE); 
                            cursor.x = mouse.x - selectedvector.x;
                            cursor.y = mouse.y - selectedvector.y;
                            r = rect(cursor, 30, 1);
                            DrawRectangleRec(r, COLOR_MOUSEDRAG);
                            cursor.x += 2 * RIGHT;
                            sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_IDNR, j));
                            r = rect(cursor, 2, 1);
                            DrawText2(font, str, r, FONTSIZE, FONTSPACING, WHITE, TextRight);
                            cursor.x += 2 * RIGHT;
                            r = rect(cursor, SENATORNAMEWIDTH, 1);
                            DrawText2(font, (char*)(valabsaddr(rordata, G_SENA, A_SENA_NAME, j)), r, FONTSIZE, FONTSPACING, WHITE, TextLeft);
                            cursor.x += SENATORNAMEWIDTH * RIGHT;
                            sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_MIL1, j));
                            r = rect(cursor, 2, 1);
                            DrawText2(font, str, r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                            cursor.x += 2 * RIGHT;
                            sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_ORA1, j));
                            r = rect(cursor, 2, 1);
                            DrawText2(font, str, r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                            cursor.x += 2 * RIGHT;
                            sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_LOY1, j));
                            r = rect(cursor, 2, 1);
                            DrawText2(font, str, r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                            cursor.x += 2 * RIGHT;
                            sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_INF1, j));
                            r = rect(cursor, 2, 1);
                            DrawText2(font, str, r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                            cursor.x += 2 * RIGHT;
                            sprintf(str, "%i", (int8_t)(*valabsaddr(rordata, G_SENA, A_SENA_POP1, j)));
                            r = rect(cursor, 2, 1);
                            DrawText2(font, str, r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                            sprintf(str, "%d", *(A_SENA_CNGR_t*)valabsaddr(rordata, G_SENA, A_SENA_CNGR, j));
                            cursor.x += 3 * RIGHT;
                            r = rect(cursor, 3, 1);
                            DrawText2(font, str, r, FONTSIZE, FONTSPACING, MAGENTA, TextLeft);
                        }
                        if (selected != -1)
                        {
                            sprintf(str, "%X", valreladdr(rordata, G_SENA, A_SENA_CNGR, selected));
                            DrawText(str, 10, 550, 10, WHITE);
                            sprintf(str, "%i", *(A_SENA_CNGR_t*)valabsaddr(rordata, G_SENA, A_SENA_CNGR, selected));
                            DrawText(str, 60, 550, 10, WHITE);
                            sprintf(str, "%i", (A_SENA_CNGR_t)(G_DECK));
                            DrawText(str, 110, 550, 10, WHITE);
                            //*(A_SENA_CNGR_t*)valabsaddr(rordata, G_SENA, A_SENA_CNGR, selected) = (A_SENA_CNGR_t)(G_DECK);
                            //sprintf(str, "%i", *(A_SENA_CNGR_t*)valabsaddr(rordata, G_SENA, A_SENA_CNGR, selected));
                            //DrawText(str, 160, 550, 10, WHITE);
                        }
                        if (selected != -1 && currentGesture != lastGesture && currentGesture == GESTURE_NONE) // drop
                        {
                            DrawText("drop", 10, 560, 10, WHITE); 
                            selected = -1;
                        }
                        sprintf(str, "%i", selected);
                        DrawText(str, 10, 580, 10, WHITE); 
                    } break;
                }

            } break;




            case 1:
            {
                cursor.x = RIGHT;
                cursor.y = DOWN;

                for (int i = 0; i < group(rordata, G_FACT).elems; i++)
                {
                    if (*(A_FACT_CNGR_t*)valabsaddr(rordata, G_FACT, A_FACT_CNGR, i) == (A_FACT_CNGR_t)(G_NULL)) continue;
                    cursor.x = RIGHT;

                    r = rect(cursor, 30, 2);
                    DrawRectangleRec(r, COLOR_FACTIONHEADER);
                    DrawText2(font, TextToUpper((char*)(valabsaddr(rordata, G_FACT, A_FACT_NAME, i))), r, FONTSIZE, FONTSPACING, WHITE, TextTopLeft);
                    cursor.y += 1 * DOWN;
                    cursor.x += (2 + 2 + SENATORNAMEWIDTH) * RIGHT;
                    r = rect(cursor, 2, 1);
                    DrawText2(font, "M", r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                    cursor.x += 2 * RIGHT;
                    r = rect(cursor, 2, 1);
                    DrawText2(font, "O", r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                    cursor.x += 2 * RIGHT;
                    r = rect(cursor, 2, 1);
                    DrawText2(font, "L", r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                    cursor.x += 2 * RIGHT;
                    r = rect(cursor, 2, 1);
                    DrawText2(font, "I", rect(cursor, 2, 1), FONTSIZE, FONTSPACING, WHITE, TextCenter);
                    cursor.x += 2 * RIGHT;
                    r = rect(cursor, 2, 1);
                    DrawText2(font, "P", r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                    cursor.y += 1 * DOWN;

                    for (int j = 0; j < group(rordata, G_SENA).elems; j++)
                    {
                        if ( *((A_SENA_ALIG_t*)(val0absaddr(rordata, G_SENA, A_SENA_ALIG))+j) == (A_SENA_ALIG_t)(i) )  // typecast is a must!
                        {
                            cursor.x = RIGHT;
                            cursor.x += 2 * RIGHT;
                            sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_IDNR, j));
                            r = rect(cursor, 2, 1);
                            DrawText2(font, str, r, FONTSIZE, FONTSPACING, WHITE, TextRight);
                            cursor.x += 2 * RIGHT;
                            r = rect(cursor, SENATORNAMEWIDTH, 1);
                            DrawText2(font, (char*)(valabsaddr(rordata, G_SENA, A_SENA_NAME, j)), r, FONTSIZE, FONTSPACING, WHITE, TextLeft);
                            cursor.x += SENATORNAMEWIDTH * RIGHT;
                            sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_MIL1, j));
                            r = rect(cursor, 2, 1);
                            if (CheckCollisionPointRec(mouse, r))
                            {
                                DrawRectangleRec(r, COLOR_MOUSEHOVER_GAMEMASTER);
                                if (IsKeyPressed(KEY_KP_ADD)) increase(rordata, G_SENA, A_SENA_MIL1, j);
                                if (IsKeyPressed(KEY_KP_SUBTRACT)) decrease(rordata, G_SENA, A_SENA_MIL1, j);
                                if (IsKeyPressed(KEY_BACKSPACE)) *valabsaddr(rordata, G_SENA, A_SENA_MIL1, j) = *valabsaddr(rordata, G_SENA, A_SENA_MIL0, j);
                            }
                            DrawText2(font, str, r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                            cursor.x += 2 * RIGHT;
                            sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_ORA1, j));
                            r = rect(cursor, 2, 1);
                            if (CheckCollisionPointRec(mouse, r))
                            {
                                DrawRectangleRec(r, COLOR_MOUSEHOVER_GAMEMASTER);
                                if (IsKeyPressed(KEY_KP_ADD)) increase(rordata, G_SENA, A_SENA_ORA1, j);
                                if (IsKeyPressed(KEY_KP_SUBTRACT)) decrease(rordata, G_SENA, A_SENA_ORA1, j);
                                if (IsKeyPressed(KEY_BACKSPACE)) *valabsaddr(rordata, G_SENA, A_SENA_ORA1, j) = *valabsaddr(rordata, G_SENA, A_SENA_ORA0, j);
                            }
                            DrawText2(font, str, r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                            cursor.x += 2 * RIGHT;
                            sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_LOY1, j));
                            r = rect(cursor, 2, 1);
                            if (CheckCollisionPointRec(mouse, r))
                            {
                                DrawRectangleRec(r, COLOR_MOUSEHOVER_GAMEMASTER);
                                if (IsKeyPressed(KEY_KP_ADD)) increase(rordata, G_SENA, A_SENA_LOY1, j);
                                if (IsKeyPressed(KEY_KP_SUBTRACT)) decrease(rordata, G_SENA, A_SENA_LOY1, j);
                                if (IsKeyPressed(KEY_BACKSPACE)) *valabsaddr(rordata, G_SENA, A_SENA_LOY1, j) = *valabsaddr(rordata, G_SENA, A_SENA_LOY0, j);
                            }
                            DrawText2(font, str, r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                            cursor.x += 2 * RIGHT;
                            sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_INF1, j));
                            r = rect(cursor, 2, 1);
                            if (CheckCollisionPointRec(mouse, r))
                            {
                                DrawRectangleRec(r, COLOR_MOUSEHOVER_GAMEMASTER);
                                if (IsKeyPressed(KEY_KP_ADD)) increase(rordata, G_SENA, A_SENA_INF1, j);
                                if (IsKeyPressed(KEY_KP_SUBTRACT)) decrease(rordata, G_SENA, A_SENA_INF1, j);
                                if (IsKeyPressed(KEY_BACKSPACE)) *valabsaddr(rordata, G_SENA, A_SENA_INF1, j) = *valabsaddr(rordata, G_SENA, A_SENA_INF0, j);
                            }
                            DrawText2(font, str, r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                            cursor.x += 2 * RIGHT;
                            sprintf(str, "%i", (int8_t)(*valabsaddr(rordata, G_SENA, A_SENA_POP1, j)));
                            r = rect(cursor, 2, 1);
                            if (CheckCollisionPointRec(mouse, r))
                            {
                                DrawRectangleRec(r, COLOR_MOUSEHOVER_GAMEMASTER);
                                if (IsKeyPressed(KEY_KP_ADD)) increase(rordata, G_SENA, A_SENA_POP1, j);
                                if (IsKeyPressed(KEY_KP_SUBTRACT)) decrease(rordata, G_SENA, A_SENA_POP1, j);
                                if (IsKeyPressed(KEY_BACKSPACE)) *valabsaddr(rordata, G_SENA, A_SENA_POP1, j) = *valabsaddr(rordata, G_SENA, A_SENA_POP0, j);
                            }
                            DrawText2(font, str, r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
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