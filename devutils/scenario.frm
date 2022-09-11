|SIG "RoR"
|VER 1


:G NULL ;


:G GAME ; "Game"
:A NAME string(16) "" ; name
:A SEQU raw(8) \x00\x00\x00\x00\x00\x00\x00\x00 ; game sequence struct; turn(uint16)-phase(uint8)-subphase(uint24, for technical business)
:A DAST raw(8) \x00\x00\x00\x00\x00\x00\x00\x00 ; date of start struct; year(uint8: 0=1990)-month-day-hour-minute-seconds-milliseconds(uint16)
:A DAEN raw(8) \x00\x00\x00\x00\x00\x00\x00\x00 ; date of end


:G PLYR ; "Player"
:GELEMS 7
:A GAME group GAME, * ; belongs to GAME (in play) or NULL (not in play)
:A NAME string(16) "Jupiter", "", * ; player name, Jupiter (id=0) does the automated stuff
:A PERM uint8 0xFF, 0x03, * ; permissions bit1: can see public stuff, bit2: can see/have own stuff (off for possible spectators); bit3: can see other player's stuff; bit4: can see decks


:G FACT ; "Faction"
:A CNGR group NULL, * ; connection, PLYR (in play) or NULL (not in play); note that factions represent clockwise order
:A CNNR elem 0, * ; connection (player) number X if taken by player X, otherwise 0
:A NAME string(16) ; faction name
        "Aquila", "Luna Crescens", "Parma", "Corona", "Manus", "Porta"


:G SENA ; "Family Card / Senator"
:A CNGR group ; group connection: faction (aligned), forum (unaligned), deck, null (not in play)
        NULL, FACT, FACT, FACT, FACT, FACT, FACT, NULL, FACT, FACT, 
        FACT, FACT, FACT, FACT, FACT, FACT, FACT, NULL, FACT, FACT, 
        FACT, NULL, *
:A CNNR elem ; connection elem (usually Xth faction)
        0, 1, 2, 6, 3, 1, 2, 0, 2, 5, 
        3, 1, 5, 4, 4, 3, 6, 0, 5, 4,
        6, 0, *
:A IDNR uint8 ; family ID number (#X)
         0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  
        10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 
        20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 
        30, 31, 32, 33, 34
:A NAME string(16) ; family name
        "none"      , "Cornelius" , "Fabius"    , "Valerius"  , "Julius"    , 
        "Claudius"  , "Manlius"   , "Fulvius"   , "Furius"    , "Aurelius"  ,
        "Junius"    , "Papirius"  , "Acilius"   , "Flaminius" , "Aelius"    , 
        "Sulpicius" , "Calpurnius", "Plautius"  , "Quinctius" , "Aemilius"  , 
        "Terentius" , "Servilius" , "Porcius"   , "Popillius" , "Cassius"   , 
        "Sempronius", "Octavius"  , "Marius"    , "Tullius"   , "Licinius"  , 
        "Pompeius"  , "Domitius"  , "Antonius"  , "Caecilius" , "Marcius"
