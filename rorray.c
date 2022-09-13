#include <stdint.h>

#include "raylib.h"

#include "rorfile.h"

#define NUM_FACTIONS    6
#define W   13
#define H   13
#define L   1

#define FONTSPACING     2

__INITRORAPI__ // initializes the API structs


int main(void)
{
    Vector2 v;
    Vector2 touchPosition = { 0, 0 };
    //Rectangle r;
    char str[9999];

    const int screenWidth = 800;
    const int screenHeight = 600;

    Font font;

    int currentGesture = GESTURE_NONE;
    int lastGesture = GESTURE_NONE;

    unsigned int rordataLength = SIZE;
    unsigned char *rordata = LoadFileData("scenario.ror", &rordataLength);
 
    Rectangle factionrects[NUM_FACTIONS] = {
        {W, H * (1 + 6 * 0), W * 30 - L, H - L},
        {W, H * (1 + 6 * 1), W * 30 - L, H - L},
        {W, H * (1 + 6 * 2), W * 30 - L, H - L},
        {W, H * (1 + 6 * 3), W * 30 - L, H - L},
        {W, H * (1 + 6 * 4), W * 30 - L, H - L},
        {W, H * (1 + 6 * 5), W * 30 - L, H - L},
    };
    
    int holdfactionrecti = -1;
    Vector2 holdfactionrectrelpoint = { 0, 0 };
    int holdfactionrectoffset = 0;

    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "RoRRAY");

    SetTargetFPS(30);

    font = LoadFont("romulus.png");

    while (!WindowShouldClose())
    {

        lastGesture = currentGesture;
        currentGesture = GetGestureDetected();
        touchPosition = GetTouchPosition(0);

        if (holdfactionrecti == -1 && currentGesture != lastGesture && currentGesture == GESTURE_TAP)
        {
            for (int i = 0; i < NUM_FACTIONS; i++)
            {
                if (CheckCollisionPointRec(touchPosition, factionrects[i]))
                {
                    holdfactionrecti = i;
                    holdfactionrectrelpoint.x = touchPosition.x - factionrects[i].x;
                    holdfactionrectrelpoint.y = touchPosition.y - factionrects[i].y;
                    break;
                };
            };
        }
        else if (currentGesture != lastGesture)
        {
            switch (currentGesture)
            {
                case GESTURE_NONE:
                {
                    factionrects[holdfactionrecti].x = touchPosition.x - holdfactionrectrelpoint.x;
                    factionrects[holdfactionrecti].y = touchPosition.y - holdfactionrectrelpoint.y;
                    /* snap to grid */
                    holdfactionrectoffset = (int)factionrects[holdfactionrecti].x % W;
                    if ((W / 2) < holdfactionrectoffset)
                    {
                        factionrects[holdfactionrecti].x += W - holdfactionrectoffset;
                    }
                    else
                    {
                        factionrects[holdfactionrecti].x -= holdfactionrectoffset;
                    };
                    holdfactionrectoffset = (int)factionrects[holdfactionrecti].y % H;
                    if ((H / 2) < holdfactionrectoffset)
                    {
                        factionrects[holdfactionrecti].y += H - holdfactionrectoffset;
                    }
                    else
                    {
                        factionrects[holdfactionrecti].y -= holdfactionrectoffset;
                    };
                    holdfactionrecti = -1;
                } break;
            };
        }
        else
        {
            factionrects[holdfactionrecti].x = touchPosition.x - holdfactionrectrelpoint.x;
            factionrects[holdfactionrecti].y = touchPosition.y - holdfactionrectrelpoint.y;
        };

        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);

        DrawFPS(screenWidth-100, 10);        

        sprintf(str, "%X", header(rordata).grouptocaddr);
        DrawText(str, 400, 100, 20, BLUE);

        for (int i = 0; i < group(rordata, G_FACT).elems; i++)
        {
            v.x = factionrects[i].x + FONTSPACING;
            v.y = factionrects[i].y;           
            DrawRectangleRec(factionrects[i], DARKGRAY);
            DrawTextEx(font, TextToUpper((char*)(valabsaddr(rordata, G_FACT, A_FACT_NAME, i))), v, font.baseSize*1.0f, FONTSPACING, WHITE);
            for (int j = 0; j < group(rordata, G_SENA).elems; j++)
            {
                if (
                    ( *((A_SENA_CNGR_t*)(val0absaddr(rordata, G_SENA, A_SENA_CNGR))+j) == (A_SENA_CNGR_t)(G_FACT) ) // typecast is a must!
                    &&
                    ( *((A_SENA_CNNR_t*)(val0absaddr(rordata, G_SENA, A_SENA_CNNR))+j) == i )
                )
                {
                    v.y += H;
                    DrawTextEx(font, (char*)(valabsaddr(rordata, G_SENA, A_SENA_NAME, j)), v, font.baseSize*1.0f, FONTSPACING, WHITE);
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