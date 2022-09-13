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
    char str[99];  // TODO: for debugging

    const int screenWidth = 800;
    const int screenHeight = 600;

    Font font;

    unsigned int rordataLength = SIZE;
    unsigned char *rordata = LoadFileData("scenario.ror", &rordataLength);
 
    Rectangle* rects = (Rectangle *)MemAlloc(header(rordata).elems*sizeof(Rectangle));

    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "RoRRAY");

    SetTargetFPS(30);

    font = LoadFont("romulus.png");

    while (!WindowShouldClose())
    {

        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);

        DrawFPS(screenWidth-100, 10);        

        for (int i = 0; i < group(rordata, G_FACT).elems; i++)
        {
            rects[group(rordata, G_FACT).elem0idx].x = W;
            rects[group(rordata, G_FACT).elem0idx].y = H * (1 + 6 * i);
            rects[group(rordata, G_FACT).elem0idx].width = W * 30 - L;
            rects[group(rordata, G_FACT).elem0idx].height = H - L;

            v.x = rects[group(rordata, G_FACT).elem0idx].x + FONTSPACING;
            v.y = rects[group(rordata, G_FACT).elem0idx].y;           
            DrawRectangleRec(rects[group(rordata, G_FACT).elem0idx], DARKGRAY);
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