#include "raylib.h"

#define NUM_FACTIONS    6
#define W   13
#define H   13
#define L   1

#define FONTSPACING     2


int main(void)
{
    Vector2 v;
    Vector2 touchPosition = { 0, 0 };
    Rectangle r;
    char str[9999];

    const int screenWidth = 1920;
    const int screenHeight = 1080;

    Font font;

    int currentGesture = GESTURE_NONE;
    int lastGesture = GESTURE_NONE;

    unsigned char *scenarioDat = LoadFileData("scenario.dat", 65536);
    int scenarioStrLength = GetFileLength("scenario.str");
    unsigned char *scenarioStr = LoadFileData("scenario.str", &scenarioStrLength);

    int j = 1;
    for (int i=0 ; i < scenarioStrLength; i++)
    {
        if (scenarioStr[i] == 0)
        {
            scenarioDat[j] = (unsigned char)i+1;
            j += 1;
        }
    }

    SaveFileData("scenario.datdbg", scenarioDat, 65536);
   
    Rectangle factionrects[NUM_FACTIONS] = {
        {W, H * (5 + 6 * 0), W * 30 - L, H - L},
        {W, H * (5 + 6 * 1), W * 30 - L, H - L},
        {W, H * (5 + 6 * 2), W * 30 - L, H - L},
        {W, H * (5 + 6 * 3), W * 30 - L, H - L},
        {W, H * (5 + 6 * 4), W * 30 - L, H - L},
        {W, H * (5 + 6 * 5), W * 30 - L, H - L},
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
   
        DrawText("State of the Republic", W, H, 20, RED);        
               
        for (int i = 0; i < NUM_FACTIONS; i++)
        {
            v.x = factionrects[i].x + FONTSPACING;
            v.y = factionrects[i].y;           
            DrawRectangleRec(factionrects[i], DARKGRAY);
            DrawText("State of the Republic", W, H, 20, RED);
            sprintf(str, "%d", (int)scenarioDat[i]);
            DrawText(str, 10 * W, 5 * i * H, 20, BLUE);
            DrawTextEx(font, TextToUpper(scenarioStr+scenarioDat[i]), v, font.baseSize*1.0f, FONTSPACING, WHITE);
        }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadFileData(scenarioDat);      // Free memory from loaded file
    UnloadFileData(scenarioStr);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}