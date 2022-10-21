#ifndef __DEFINE_RORHFILE__
#define __DEFINE_RORHFILE__

#include <stdbool.h>
#include <stdint.h>


#define ITEMTYPECOUNT 8
#define TEMPSIZE 12

// uint32_t range : 0 to 4.294.967.295

typedef struct __attribute__((__packed__, __scalar_storage_order__("big-endian"))) {
    uint8_t     yea;        //  the number of years since 1990 (AH RoR release).
    uint8_t     mon;
    uint8_t     day;
    uint8_t     hou;
    uint8_t     min;
    uint8_t     sec;
} RoR_Time_t;               //  1990-00-00 00:00:00 is a nullish value


enum GamePhase {
    SetupPhase = 0,
    MortalityPhase = 1,
    RevenuePhase = 2,
    ForumPhase = 3,
    PopulationPhase = 4,
    SenatePhase = 5,
    CombatPhase = 6,
    RevolutionPhase = 7
};


enum Phase {
    PhSetGamename           = (SetupPhase << 28)        +      1,
    PhSetupRules            = (SetupPhase << 28)        +      2,
    PhPickScenario          = (SetupPhase << 28)        + 300000,
    PhTakeFactions          = (SetupPhase << 28)        + 301200,
    PhDealSenators          = (SetupPhase << 28)        + 301420,
    PhTemporaryRomeConsul   = (SetupPhase << 28)        + 301600,
    PhSelectFactionLeaders  = (SetupPhase << 28)        + 301700,
    PhInitialFactionPhase   = (SetupPhase << 28)        + 301900
};


enum Language {
    English = 0
};


typedef struct __attribute__((__packed__, __scalar_storage_order__("big-endian"))) {
    char        sign[3];
    uint8_t     vers;
    uint8_t     lang;
    char        name[15];
    char        desc[76];
    RoR_Time_t  strt;       //  start
    RoR_Time_t  end;
    uint8_t     nfac;       //  number of factions
    uint8_t     peri;       //  period number
    int16_t     turn;
    uint32_t    phse;       //  phase ((rulebook phase << 28) + subphase)
                            //  this value is the main logic control
} RoR_Header_t;             //  size: 116 bytes


typedef struct __attribute__((__packed__, __scalar_storage_order__("big-endian"))) {
    uint16_t    cnt;
    uint32_t    adr;        //  address of the first elem
} RoR_ItemTypeInfo_t;       //  size: 6 bytes each


enum ItemType {
    EraItem = 1,
    FactionItem,
    MainLocationItem,
    DeckItem,
    OfficeItem,
    MagistrateItem,
    SenatorItem,
    StatesmanItem
};


typedef struct __attribute__((__packed__, __scalar_storage_order__("big-endian"))) {
    char        name[16];
    uint8_t     nsen;       //  number of senators per faction to deal at period start
    uint8_t     terc;       //  temporary rome consul protocol at period start
                            //      bit0: no TERC (0) / TERC (1)
                            //      bit1: random draw (0) / lowest senator ID (1)
                            //      bit2: before(0) / after(1) faction leaders
                            //      bit3: repeat if dies in first mortality phase (1)
                            //      bit7: not resolved (0) / resolved (1)
} RoR_EraItem_t;            //  size: 18 bytes each


enum FactionType {
    FactionUnused = 0,
    FactionUsed = 1,
    //FactionHuman,
    //FactionPopulists,
    //FactionConservatives,
    //FactionPlutocrats,
    //FactionImperials,
    //FactionAI,
    FactionUnset = 255
};


typedef struct __attribute__((__packed__, __scalar_storage_order__("big-endian"))) {
    char        name[16];
    uint8_t     type;
    uint16_t    leit;       //  Faction Leader ItemType (0=Null / Senator=Family Card / Statesman)
    uint16_t    lenr;       //  Faction Leader ElemNr
    char        pad;
} RoR_FactionItem_t;        //  size: 20 bytes each


enum MainLocationElemNr {
    Discards = 1,
    Forum = 2,
    EnemyLeaders = 3,
    RepopulatingRome = 4,
    DestroyedConcessions = 5,
    InactiveWars = 6,
    UnprosecutedWars = 7,
    ActiveWars = 8,
    ActiveForces = 9,
    Laws = 10,
    ImminentWars = 11
};


typedef struct __attribute__((__packed__, __scalar_storage_order__("big-endian"))) {
    char        name[32];
    uint16_t    seri;       // serial
} RoR_MainLocationItem_t;   //  size: 34 bytes each


enum DeckType {
    Deck = 0,
    EraTransDeck = 1,
    FinalDeck = 2,
    EraStartSenatorPool = 3,
    EraStartFactionCardPool = 4
};


typedef struct __attribute__((__packed__, __scalar_storage_order__("big-endian"))) {
    uint8_t     peri;       //  Period number
    uint8_t     type;       //  Deck type
    int16_t     size;
    uint8_t     prev;
    uint8_t     next;
    uint16_t    topr;       //  First N cards goes to previous deck
    uint16_t    tonx;       //  Last N cards goes to next deck
} RoR_DeckItem_t;           //  size: 10 bytes each


enum OfficeType {
    ConsulForLife = 1,
    Dictator = 2,
    RomeConsul = 3,
    FieldConsul = 4,
    Censor = 5,
    MasterOfHorse = 6,
    DominantPlayer = 7,
    PontifexMaximus = 8,
    Proconsul = 9
};


typedef struct __attribute__((__packed__, __scalar_storage_order__("big-endian"))) {
    char        snam[4];    //  Short name eg. "RC" stands for Rome Consul
    char        name[24];   //  Name
    char        ordr[2];    //  Order of precedence mark
    uint8_t     type;
    bool        life;       //  For life
    int8_t      infl;       //  Influence gain for taking
    char        pad;
} RoR_OfficeItem_t;         //  size: 34 bytes each


enum MagistrateType {
    HRAO = 1,
    PresidingMagistrate = 2,
};


typedef struct __attribute__((__packed__, __scalar_storage_order__("big-endian"))) {
    char        snam[2];    //  "H" / "M"
    char        name[6];    //  "HRAO", "PRMA"
    char        lame[36];   //  "Highest Ranking Available Officer", "Presiding Magistrate"
    uint8_t     type;
    uint16_t    owit;       //  Owner's ItemType (Senator / Statesman)
    uint16_t    ownr;       //  Owner's ElemNr
    char        pad;
} RoR_MagistrateItem_t;     //  size: 50 bytes each


typedef struct __attribute__((__packed__, __scalar_storage_order__("big-endian"))) {
    char        name[16];
    uint8_t     peri;       //  Period number
    uint8_t     idnr;       //  Family ID number (#x)
    uint8_t     mil0;       //  initial Military
    uint8_t     ora0;       //  initial Oratory
    uint8_t     loy0;       //  initial Loyalty
    uint8_t     inf0;       //  initial Influence
    int8_t      pop0;       //  initial Popularity
    uint8_t     mil1;       //  achieved Military
    uint8_t     ora1;       //  achieved Oratory
    uint8_t     loy1;       //  achieved Loyalty
    uint8_t     inf1;       //  achieved Influence
    int8_t      pop1;       //  achieved Popularity
    uint8_t     mil2;       //  current Military
    uint8_t     ora2;       //  current Oratory
    uint8_t     loy2;       //  current Loyalty
    uint8_t     inf2;       //  current Influence
    int8_t      pop2;       //  current Popularity
    uint16_t    loit;       //  Location ItemType (0: Null / Deck / Discard / Forum (In Rome) / Repopulation Rome / Province / War)
    uint16_t    lonr;       //  Location ElemNr
    uint16_t    losr;       //  Location SerialNr (Mainly for Repopulating Rome)
    bool        proc;       //  Proconsul
    uint8_t     pric;       //  Prior Consul counter
    char        pad;
} RoR_SenatorItem_t;        //  size: 42 bytes each


typedef struct __attribute__((__packed__, __scalar_storage_order__("big-endian"))) {
    char        snam[16];   //  Short name
    uint8_t     peri;       //  Period number
    uint8_t     idnr;       //  Family ID number (#x)
    char        idch[2];    //  ID character (#xA/#xB/..)
    char        name[64];   //  Name
    char        time[32];   //  Time
    char        spec[128];  //  Specialty
    uint8_t     fact;       //  Aligned to Faction#
    uint8_t     mil0;       //  initial Military
    uint8_t     ora0;       //  initial Oratory
    uint8_t     loy0;       //  initial Loyalty
    uint8_t     inf0;       //  initial Influence
    int8_t      pop0;       //  initial Popularity
    uint8_t     mil1;       //  achieved Military
    uint8_t     ora1;       //  achieved Oratory
    uint8_t     loy1;       //  achieved Loyalty
    uint8_t     inf1;       //  achieved Influence
    int8_t      pop1;       //  achieved Popularity
    uint8_t     mil2;       //  current Military
    uint8_t     ora2;       //  current Oratory
    uint8_t     loy2;       //  current Loyalty
    uint8_t     inf2;       //  current Influence
    int8_t      pop2;       //  current Popularity
    uint16_t    loit;       //  Location ItemType (0: Null / Deck / Discard / Forum (In Rome) / Repopulation Rome / Province / War)
    uint16_t    lonr;       //  Location ElemNr
    bool        proc;       //  Proconsul
    uint8_t     pric;       //  Prior Consul counter
    uint8_t     lo0w[4];    //  Loyalty=0 if in same Faction with StatesmanNr (StatesmanId+1); 0: None
    uint8_t     lo0n[2];    //  Loyalty=0 if NOT in same Faction with StatesmanNr (StatesmanId+1); 0: None
} RoR_StatesmanItem_t;      //  size: 272 bytes each

#define p_HEADER(rordata)  ((RoR_Header_t*)(rordata))
#define p_TEMP(rordata)  ((void*)(rordata+sizeof(RoR_Header_t)))
#define p_ITEMTYPEINFO(rordata)  ((RoR_ItemTypeInfo_t*)(rordata+sizeof(RoR_Header_t)+TEMPSIZE))
#define p_FACTIONITEM(rordata)  ((RoR_FactionItem_t*)(rordata+p_ITEMTYPEINFO(rordata)[FactionItem-1].adr))
#define group(rordata, G)  (*((Group*)(rordata+header(rordata).grouptocaddr) + G))
#define attr(rordata, G, A)  (*((Attr*)(rordata+header(rordata).attrtocaddr) + (group(rordata, G).attr0idx) + A))
#define val0reladdr(rordata, G, A)  (header(rordata).attrvalsaddr + attr(rordata, G, A).addr)
#define val0absaddr(rordata, G, A)  (rordata+val0reladdr(rordata, G, A))
#define valsize(rordata, G, A)  (attr(rordata, G, A).type & 0x00FF)
#define valreladdr(rordata, G, A, i)  (header(rordata).attrvalsaddr + attr(rordata, G, A).addr) + (i*valsize(rordata, G, A))
#define valabsaddr(rordata, G, A, i)  (rordata+valreladdr(rordata, G, A, i))

#endif  /* #ifndef __DEFINE_RORHFILE__ */
;