#include <stdint.h>

#include "raylib.h"

#include "rorfile.h"

#define NUM_FACTIONS    6
#define W   13
#define H   13
#define L   1

#define FONTSPACING     2


int main(void)
{
    Vector2 v;
    Vector2 touchPosition = { 0, 0 };
    //Rectangle r;
    char str[9999];

    const int screenWidth = 1920;
    const int screenHeight = 1080;

    Font font;

    int currentGesture = GESTURE_NONE;
    int lastGesture = GESTURE_NONE;

    unsigned int scenarioRoRLength = RORH_SIZE;
    unsigned int addr = 0;
    unsigned char *scenarioRoR = LoadFileData("scenario.ror", &scenarioRoRLength);
 
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

        int i = 1;
        int j = 1;
        //TextFormat(str, "V = %X", RORH_SEN_REF_ADDR);
        // sprintf(str, "%X", scenarioRoR[RORH_SEN_REF_ADDR + (j << RORH_SEN_VAL_BITS) * RORH_VAL_SIZE ]);
        // DrawText(str, 400, 400, 20, BLUE);
        // sprintf(str, "%X", (((RORH_PLA_REF_ADDR + (i << RORH_PLA_VAL_BITS) * RORH_VAL_SIZE)) >> 8));
        // DrawText(str, 400, 500, 20, BLUE);
        // sprintf(str, "%X", (unsigned char)(((RORH_PLA_REF_ADDR + (i << RORH_PLA_VAL_BITS) * RORH_VAL_SIZE)) >> 8));
        // DrawText(str, 500, 500, 20, BLUE);
        // sprintf(str, "%X", scenarioRoR[RORH_SEN_REF_ADDR + (j << RORH_SEN_VAL_BITS) * RORH_VAL_SIZE + 1 ]);
        // DrawText(str, 400, 600, 20, BLUE);
        // sprintf(str, "%X", (unsigned char)(((RORH_PLA_REF_ADDR + (i << RORH_PLA_VAL_BITS) * RORH_VAL_SIZE )) & 0xFF));
        // DrawText(str, 400, 700, 20, BLUE);
        // sprintf(str, "%X", (((unsigned short *)scenarioRoR)[RORH_SEN_REF_ADDR / 2 + (j << RORH_SEN_VAL_BITS) ]));
        // DrawText(str, 400, 800, 20, BLUE);
        // sprintf(str, "%X", RORH_SEN_REF_ADDR / 2 + (j << RORH_SEN_VAL_BITS));
        // DrawText(str, 600, 800, 20, BLUE);
        // sprintf(str, "%X", RORH_PLA_REF_ADDR / 2 + (i << RORH_PLA_VAL_BITS));
        // DrawText(str, 400, 900, 20, BLUE);
        // sprintf(str, "%X", RORH_MAINLENGTH+scenarioStrPos[RORH_SEN_STR_IDX+j]);
        // DrawText(str, 400, 1000, 20, BLUE);        
        // sprintf(str, "%X", (scenarioRoR[RORH_SEN_REF_ADDR + (j << RORH_SEN_VAL_BITS) * RORH_VAL_SIZE ] == (unsigned char)(((RORH_PLA_REF_ADDR + (i << RORH_PLA_VAL_BITS) * RORH_VAL_SIZE)) >> 8)));
        // DrawText(str, 600, 1000, 20, BLUE);      

        // sprintf(str, "%X", RORH_GROUPTOC + RORH_GROUPTOC_ITEMSIZE * RORH_G_SENA);  // 0x38: SENA GROUPTOCITEM address
        //sprintf(str, "%X", *(unsigned short *)(&scenarioRoR[RORH_GROUPTOC + RORH_GROUPTOC_ITEMSIZE * RORH_G_SENA]));  //0x23: SENA elemcount
        //sprintf(str, "%d", *(unsigned int *)(&scenarioRoR[RORH_GROUPTOC + RORH_GROUPTOC_ITEMSIZE * RORH_G_SENA + RORH_GROUPTOC_FIRSTATTR]));  //24: SENA first attribute
        //sprintf(str, "%X", RORH_ATTRTOC + RORH_ATTRTOC_ITEMSIZE * RORH_A_SENA_NAME + RORH_ATTRTOC_ADDR);  // 0x011C: SENA NAME values address
        //sprintf(str, "%X", *(unsigned int *)(&scenarioRoR[RORH_ATTRTOC + RORH_ATTRTOC_ITEMSIZE * RORH_A_SENA_NAME + RORH_ATTRTOC_ADDR])); //01E8 = 488: 
        sprintf(str, "%X", RORH_ATTRVALS + 0x1E8);  // 0x0308: first senator name address
        DrawText(str, 400, 400, 20, BLUE);
        DrawText((char*)(scenarioRoR+0x0308), 400, 500, 20, BLUE);

        // for (int i = 0; i < NUM_FACTIONS; i++)
        // {
        //     v.x = factionrects[i].x + FONTSPACING;
        //     v.y = factionrects[i].y;           
        //     DrawRectangleRec(factionrects[i], DARKGRAY);
        //     int a = RORH_MAINLENGTH + scenarioStrPos[RORH_PLA_STR_IDX + i + 1];
        //     DrawTextEx(font, TextToUpper((char*)(scenarioRoR+a)), v, font.baseSize*1.0f, FONTSPACING, WHITE);
// 
        //     for (int j = 0; j < RORH_SEN_MAX_N + 1; j++)
        //     {
        //         //if (scenarioRoR[RORH_SEN_REF_ADDR + (j << RORH_SEN_VAL_BITS) * RORH_VAL_SIZE ] == RORH_PLA_REF_ADDR + (i << RORH_PLA_VAL_BITS) * RORH_VAL_SIZE)
        //         //if (unsigned short *)(scenarioRoR[RORH_SEN_REF_ADDR + (j << RORH_SEN_VAL_BITS) * RORH_VAL_SIZE ]) == RORH_PLA_REF_ADDR + (i << RORH_PLA_VAL_BITS) * RORH_VAL_SIZE )
        //         //if (((unsigned short *)scenarioRoR[RORH_SEN_REF_ADDR / 2 + (j << RORH_SEN_VAL_BITS) ]) == RORH_PLA_REF_ADDR / 2 + (i << RORH_PLA_VAL_BITS) )
        //         if (
        //             ( scenarioRoR[RORH_SEN_REF_ADDR + (j << RORH_SEN_VAL_BITS) * RORH_VAL_SIZE ] == (unsigned char)(((RORH_PLA_REF_ADDR + ((i+1) << RORH_PLA_VAL_BITS) * RORH_VAL_SIZE)) >> 8) ) // typecast is a must!
        //             &&
        //             ( scenarioRoR[RORH_SEN_REF_ADDR + (j << RORH_SEN_VAL_BITS) * RORH_VAL_SIZE + 1 ] == (unsigned char)(((RORH_PLA_REF_ADDR + ((i+1) << RORH_PLA_VAL_BITS) * RORH_VAL_SIZE )) & 0xFF) )
        //             //(scenarioRoR[RORH_SEN_REF_ADDR + (j << RORH_SEN_VAL_BITS) * RORH_VAL_SIZE + 1 ] == RORH_PLA_REF_ADDR & 0xFF)
        //         )
        //         {
        //             v.y += H;
        //             DrawTextEx(font, (char*)(scenarioRoR+(RORH_MAINLENGTH+scenarioStrPos[RORH_SEN_STR_IDX+j])), v, font.baseSize*1.0f, FONTSPACING, WHITE);
        //         }
        //     }
        // }
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadFileData(scenarioRoR);      // Free memory from loaded file
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}