#include <stdint.h>

#include "raylib.h"

#include "rorfile.h"

#define NUM_FACTIONS    6
#define RIGHT  11
#define DOWN   13
#define PADDING   1

#define FONTSPACING     2
#define FONTSIZE        font.baseSize*1.0f

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif

__INITRORAPI__ // initializes the API structs


#define SENATORNAMEWIDTH  8
#define MOUSEHOVERCOLOR  DARKBLUE


void decrease(unsigned char *rordata, uint16_t groupidx, uint16_t attridx, uint16_t elemidx)
{
    Attr attr = attr(rordata, groupidx, attridx);
    switch (attr.type)
    {
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
    TextRight = 2
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
        } break;
        case TextRight:
        {
            position.x += (int)(box.width - textsize.x - FONTSPACING);
        } break;
        case TextLeft:
        default:
            position.x += FONTSPACING;
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

    //Rectangle* rects = (Rectangle *)MemAlloc(header(rordata).elems*sizeof(Rectangle));

    SetConfigFlags(FLAG_VSYNC_HINT);
    InitWindow(screenWidth, screenHeight, "RoRRAY");

    SetTargetFPS(30);

    font = LoadFont("romulus.png");

    while (!WindowShouldClose())
    {
        mouse = GetTouchPosition(0);

        BeginDrawing();

        ClearBackground(RAYWHITE);
        DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);

        DrawFPS(screenWidth-100, 10);        

        cursor.x = RIGHT;
        cursor.y = DOWN;

        for (int i = 0; i < group(rordata, G_FACT).elems; i++)
        {
            cursor.x = RIGHT;

            r = rect(cursor, 30, 2);
            DrawRectangleRec(r, DARKGRAY);
            DrawText2(font, TextToUpper((char*)(valabsaddr(rordata, G_FACT, A_FACT_NAME, i))), r, FONTSIZE, FONTSPACING, WHITE, TextLeft);

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
                        DrawRectangleRec(r, MOUSEHOVERCOLOR);
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
                        DrawRectangleRec(r, MOUSEHOVERCOLOR);
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
                        DrawRectangleRec(r, MOUSEHOVERCOLOR);
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
                        DrawRectangleRec(r, MOUSEHOVERCOLOR);
                        if (IsKeyPressed(KEY_KP_ADD)) increase(rordata, G_SENA, A_SENA_INF1, j);
                        if (IsKeyPressed(KEY_KP_SUBTRACT)) decrease(rordata, G_SENA, A_SENA_INF1, j);
                        if (IsKeyPressed(KEY_BACKSPACE)) *valabsaddr(rordata, G_SENA, A_SENA_INF1, j) = *valabsaddr(rordata, G_SENA, A_SENA_INF0, j);
                    }
                    DrawText2(font, str, r, FONTSIZE, FONTSPACING, WHITE, TextCenter);
                    cursor.x += 2 * RIGHT;
                    sprintf(str, "%d", *valabsaddr(rordata, G_SENA, A_SENA_POP1, j));
                    r = rect(cursor, 2, 1);
                    if (CheckCollisionPointRec(mouse, r))
                    {
                        DrawRectangleRec(r, MOUSEHOVERCOLOR);
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