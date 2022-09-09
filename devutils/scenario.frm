SIG "RoR"
VER 1


G NULL 1 0


G GAME 1 8 ; "Game"
A NAME string(16) "" ; name
A STYR uint16_t 0 ; start year
A STMO uint8_t 0 ; start month
A STDY uint8_t 0 ; start day
A STHO uint8_t 0 ; start hour
A STMI uint8_t 0 ; start minute
A STSE uint8_t 0 ; start seconds
A STMS uint16_t 0 ; start milliseconds
A TURN uint16_t 0 ; turn
A PHASE uint_t 0 ; phase
A SUBPHASE uint32_t ; subphase (technical)


G PLYR 7 3 ; "Player"
A GAME group GAME, * ; belongs to GAME (in play) or NULL (not in play)
A NAME string(16) "Jupiter", "", * ; player name, Jupiter (id=0) does the automated stuff
A PERM uint8_t 0xFF, 0x03, * ; permissions bit1: can see public stuff, bit2: can see/have own stuff (off for possible spectators); bit3: can see other player's stuff; bit4: can see decks


G FACT 6 1 ; "Faction"
A CNGR group NULL, * ; connection, PLYR (in play) or NULL (not in play); note that factions represent clockwise order
A CNNR item 0, * ; connection (player) number X if taken by player X, otherwise 0
A NAME string(16) ; faction name
        "Aquila", "Luna Crescens", "Parma", "Corona", "Manus", "Porta"


G SENA 35 1 ; "Family Card / Senator"
A CNGR group ; group connection: faction (aligned), forum (unaligned), deck, null (not in play)
        NULL, FACT, FACT, FACT, FACT, FACT, FACT, NULL, FACT, FACT, 
        FACT, FACT, FACT, FACT, FACT, FACT, FACT, NULL, FACT, FACT, 
        FACT, NULL, *
A CNNR item
        0, 1, 2, 6, 3, 1, 2, 0, 2, 5, 
        3, 1, 5, 4, 4, 3, 6, 0, 5, 4,
        6, 0, *
A IDNR uint8_t 
         0,  1,  2,  3,  4,  5,  6,  7,  8,  9,  
        10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 
        20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 
        30, 31, 32, 33, 34
A NAME string(16)
        "none",       "Cornelius" , "Fabius"    , "Valerius"  , "Julius"    , 
        "Claudius"  , "Manlius"   , "Fulvius"   , "Furius"    , "Aurelius"  ,
        "Junius"    , "Papirius"  , "Acilius"   , "Flaminius" , "Aelius"    , 
        "Sulpicius" , "Calpurnius", "Plautius"  , "Quinctius" , "Aemilius"  , 
        "Terentius" , "Servilius" , "Porcius"   , "Popillius" , "Cassius"   , 
        "Sempronius", "Octavius"  , "Marius"    , "Tullius"   , "Licinius"  , 
        "Pompeius"  , "Domitius"  , "Antonius"  , "Caecilius" , "Marcius"
