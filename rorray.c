#include <stdint.h>

#include "raylib.h"

#include "rorfile.h"

#define NUM_FACTIONS    6
#define W   13
#define H   13
#define L   1

#define FONTSPACING     2


struct _Group {
   uint16_t items;
   uint16_t attrs;
   uint32_t firstattridx;
};


struct _Attr {
   uint16_t groupidx;
   uint16_t type;
   uint32_t addr;
};


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

    unsigned int rordataLength = SIZE;
    unsigned int addr = 0;
    unsigned char *rordata = LoadFileData("scenario.ror", &rordataLength);
 
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

    Attr a;
    Group g;


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
        //TextFormat(str, "V = %X", SEN_REF_ADDR);
        // sprintf(str, "%X", rordata[SEN_REF_ADDR + (j << SEN_VAL_BITS) * VAL_SIZE ]);
        // DrawText(str, 400, 400, 20, BLUE);
        // sprintf(str, "%X", (((PLA_REF_ADDR + (i << PLA_VAL_BITS) * VAL_SIZE)) >> 8));
        // DrawText(str, 400, 500, 20, BLUE);
        // sprintf(str, "%X", (unsigned char)(((PLA_REF_ADDR + (i << PLA_VAL_BITS) * VAL_SIZE)) >> 8));
        // DrawText(str, 500, 500, 20, BLUE);
        // sprintf(str, "%X", rordata[SEN_REF_ADDR + (j << SEN_VAL_BITS) * VAL_SIZE + 1 ]);
        // DrawText(str, 400, 600, 20, BLUE);
        // sprintf(str, "%X", (unsigned char)(((PLA_REF_ADDR + (i << PLA_VAL_BITS) * VAL_SIZE )) & 0xFF));
        // DrawText(str, 400, 700, 20, BLUE);
        // sprintf(str, "%X", (((unsigned short *)rordata)[SEN_REF_ADDR / 2 + (j << SEN_VAL_BITS) ]));
        // DrawText(str, 400, 800, 20, BLUE);
        // sprintf(str, "%X", SEN_REF_ADDR / 2 + (j << SEN_VAL_BITS));
        // DrawText(str, 600, 800, 20, BLUE);
        // sprintf(str, "%X", PLA_REF_ADDR / 2 + (i << PLA_VAL_BITS));
        // DrawText(str, 400, 900, 20, BLUE);
        // sprintf(str, "%X", MAINLENGTH+scenarioStrPos[SEN_STR_IDX+j]);
        // DrawText(str, 400, 1000, 20, BLUE);        
        // sprintf(str, "%X", (rordata[SEN_REF_ADDR + (j << SEN_VAL_BITS) * VAL_SIZE ] == (unsigned char)(((PLA_REF_ADDR + (i << PLA_VAL_BITS) * VAL_SIZE)) >> 8)));
        // DrawText(str, 600, 1000, 20, BLUE);      

        // sprintf(str, "%X", GROUPTOC + GROUPTOC_ITEMSIZE * G_SENA);  // 0x38: SENA GROUPTOCITEM address
        //sprintf(str, "%X", *(unsigned short *)(&rordata[GROUPTOC + GROUPTOC_ITEMSIZE * G_SENA]));  //0x23: SENA elemcount
        //sprintf(str, "%d", *(unsigned int *)(&rordata[GROUPTOC + GROUPTOC_ITEMSIZE * G_SENA + GROUPTOC_FIRSTATTR]));  //24: SENA first attribute
        //sprintf(str, "%X", ATTRTOC + ATTRTOC_ITEMSIZE * A_SENA_NAME + ATTRTOC_ADDR);  // 0x011C: SENA NAME values address
        //sprintf(str, "%X", *(unsigned int *)(&rordata[ATTRTOC + ATTRTOC_ITEMSIZE * A_SENA_NAME + ATTRTOC_ADDR])); //01E8 = 488: 
        //sprintf(str, "%X", ATTRVALS + 0x1E8);  // 0x0308: first senator name address

        // these are identical
        //sprintf(str, "%X", *(Attr*)(&rordata[ATTRTOC]));
        //DrawText(str, 400, 300, 20, BLUE);
        //sprintf(str, "%X", *(Attr*)(rordata+ATTRTOC)); 
        //DrawText(str, 400, 400, 20, BLUE);

        g = group(rordata, G_FACT);
        a = attr(rordata, G_SENA, A_SENA_NAME);

        sprintf(str, "%X", valreladdr(rordata, G_SENA, A_SENA_CNGR, 1));
        DrawText(str, 400, 100, 20, BLUE);

        //sprintf(str, "%X", (uint16_t*)(&rordata+val0addr(rordata, G_SENA, A_SENA_CNGR)) );
        //sprintf(str, "%X", (*( (uint16_t*)(&rordata+val0addr(rordata, G_SENA, A_SENA_CNGR) ) ) )+1             );
        //sprintf(str, "%X", (*((uint16_t*)(rordata+val0addr(rordata, G_SENA, A_SENA_CNGR))+8)) );   // worked
        sprintf(str, "%X", (*((uint16_t*)(valabsaddr(rordata, G_SENA, A_SENA_CNGR, 0))+8)) );
        //sprintf(str, "%X", (*((A_SENA_CNGR*)(valabsaddr(rordata, G_SENA, A_SENA_CNGR, 8)))) );
        DrawText(str, 400, 200, 20, BLUE);

        sprintf(str, "%d", (  *( (Group*)(&rordata[GROUPTOC])+G_SENA ) ).firstattridx  );  //24: SENA first attribute
        DrawText(str, 400, 300, 20, BLUE);

        sprintf(str, "%X", ATTRVALS + (*((Attr*)(&rordata[ATTRTOC])+((*((Group*)(&rordata[GROUPTOC])+G_SENA)).firstattridx)+A_SENA_NAME)).addr);  // 0x0308: address of the 0th Family Card's name
        DrawText(str, 400, 400, 20, BLUE);

        sprintf(str, "%X", val0reladdr(rordata, G_SENA, A_SENA_NAME));  // 0x0308: address of the 0th Family Card's name
        DrawText(str, 400, 500, 20, BLUE);

        DrawText((char*)(rordata+0x0308), 400, 600, 20, BLUE);

        

        for (int i = 0; i < group(rordata, G_FACT).items; i++)
        {
            v.x = factionrects[i].x + FONTSPACING;
            v.y = factionrects[i].y;           
            DrawRectangleRec(factionrects[i], DARKGRAY);
            DrawTextEx(font, TextToUpper((char*)(valabsaddr(rordata, G_FACT, A_FACT_NAME, i))), v, font.baseSize*1.0f, FONTSPACING, WHITE);
            for (int j = 0; j < group(rordata, G_SENA).items; j++)
            {
                //if (rordata[SEN_REF_ADDR + (j << SEN_VAL_BITS) * VAL_SIZE ] == PLA_REF_ADDR + (i << PLA_VAL_BITS) * VAL_SIZE)
                //if (unsigned short *)(rordata[SEN_REF_ADDR + (j << SEN_VAL_BITS) * VAL_SIZE ]) == PLA_REF_ADDR + (i << PLA_VAL_BITS) * VAL_SIZE )
                //if (((unsigned short *)rordata[SEN_REF_ADDR / 2 + (j << SEN_VAL_BITS) ]) == PLA_REF_ADDR / 2 + (i << PLA_VAL_BITS) )
                if (
                    ( *((A_SENA_CNGR_t*)(val0absaddr(rordata, G_SENA, A_SENA_CNGR))+j) == (A_SENA_CNGR_t)(G_FACT) ) // typecast is a must!
                    &&
                    ( *((A_SENA_CNNR_t*)(val0absaddr(rordata, G_SENA, A_SENA_CNNR))+j) == i )
                    //(rordata[SEN_REF_ADDR + (j << SEN_VAL_BITS) * VAL_SIZE + 1 ] == PLA_REF_ADDR & 0xFF)
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