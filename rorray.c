#include <stdint.h>

#include "raylib.h"

#include "rorfile.h"

#define NUM_FACTIONS    6
#define RIGHT   13
#define DOWN   13
#define PADDING   1

#define FONTSPACING     2

__INITRORAPI__ // initializes the API structs



Vector2 textvector(Vector2 v)
{
    Vector2 v2 = {v.x + FONTSPACING, v.y};
    return v2;
}



int main(void)
{
    Vector2 v1;
    Vector2 v2;
    Rectangle r1;
    char str[99];  // TODO: for debugging

    const int screenWidth = 800;
    const int screenHeight = 600;

    Font font;

    unsigned int rordataLength = SIZE;
    unsigned char *rordata = LoadFileData("scenario.ror", &rordataLength);
 
    //Rectangle* rects = (Rectangle *)MemAlloc(header(rordata).elems*sizeof(Rectangle));

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

        v1.x = RIGHT;
        v1.y = DOWN;

        r1.width = 30 * RIGHT - PADDING;
        r1.height = 2 * DOWN - PADDING;   
        // note: if I ever would create a vector from a rectangle.
        // v1 = *(Vector2 *)(&r); 

        for (int i = 0; i < group(rordata, G_FACT).elems; i++)
        {
            r1.x = v1.x;
            r1.y = v1.y;

            DrawRectangleRec(r1, DARKGRAY);

            DrawTextEx(font, TextToUpper((char*)(valabsaddr(rordata, G_FACT, A_FACT_NAME, i))), textvector(v1), font.baseSize*1.0f, FONTSPACING, WHITE);

            v1.y += 1 * DOWN;

            v2.y = v1.y;
            v2.x = v1.x + (2+2+8) * RIGHT;
            DrawTextEx(font, "M", textvector(v2), font.baseSize*1.0f, FONTSPACING, WHITE);
            v2.x += 2 * RIGHT;
            DrawTextEx(font, "O", textvector(v2), font.baseSize*1.0f, FONTSPACING, WHITE);
            v2.x += 2 * RIGHT;
            DrawTextEx(font, "L", textvector(v2), font.baseSize*1.0f, FONTSPACING, WHITE);
            v2.x += 2 * RIGHT;
            DrawTextEx(font, "I", textvector(v2), font.baseSize*1.0f, FONTSPACING, WHITE);
            v2.x += 2 * RIGHT;
            DrawTextEx(font, "P", textvector(v2), font.baseSize*1.0f, FONTSPACING, WHITE);
            v1.y += 1 * DOWN;

            for (int j = 0; j < group(rordata, G_SENA).elems; j++)
            {
                if ( *((A_SENA_ALIG_t*)(val0absaddr(rordata, G_SENA, A_SENA_ALIG))+j) == (A_SENA_ALIG_t)(i) )  // typecast is a must!
                {
                    v2.y = v1.y;
                    v2.x = v1.x + 2 * RIGHT;
                    sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_IDNR, j));
                    DrawTextEx(font, str, textvector(v2), font.baseSize*1.0f, FONTSPACING, WHITE);
                    v2.x += 2 * RIGHT;
                    DrawTextEx(font, (char*)(valabsaddr(rordata, G_SENA, A_SENA_NAME, j)), textvector(v2), font.baseSize*1.0f, FONTSPACING, WHITE);
                    v2.x += 8 * RIGHT;
                    sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_MIL1, j));
                    DrawTextEx(font, str, textvector(v2), font.baseSize*1.0f, FONTSPACING, WHITE);
                    v2.x += 2 * RIGHT;
                    sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_ORA1, j));
                    DrawTextEx(font, str, textvector(v2), font.baseSize*1.0f, FONTSPACING, WHITE);
                    v2.x += 2 * RIGHT;
                    sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_LOY1, j));
                    DrawTextEx(font, str, textvector(v2), font.baseSize*1.0f, FONTSPACING, WHITE);
                    v2.x += 2 * RIGHT;
                    sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_INF1, j));
                    DrawTextEx(font, str, textvector(v2), font.baseSize*1.0f, FONTSPACING, WHITE);
                    v2.x += 2 * RIGHT;
                    sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_POP1, j));
                    DrawTextEx(font, str, textvector(v2), font.baseSize*1.0f, FONTSPACING, WHITE);
                    
                    v1.y += DOWN;
                }
            }
            v1.y += DOWN;
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