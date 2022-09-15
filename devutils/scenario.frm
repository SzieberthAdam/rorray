|SIG "RoR"
|VER 1


:G NULL ;


:G GAME ; "Game"
:A NAME string(16) "Ludus" ; name
:A TURN int16 0 ; turn
:ANOPAD
:A PHSE int16 0 ; phase
:ANOPAD
:A SPHS uint32 0 ; subphase (technical)
:ANOPAD
:A STYR uint8 0 ; start year (0: 1990, 1: 1991, ...)
:ANOPAD
:A STMO uint8 0 ; start month
:ANOPAD
:A STDY uint8 0 ; start day
:ANOPAD
:A STHO uint8 0 ; start hour
:ANOPAD
:A STMI uint8 0 ; start minute
:ANOPAD
:A STSE uint8 0 ; start seconds
:ANOPAD
:A STMS uint16 0 ; start milliseconds
:ANOPAD
:A ENYR uint8 0 ; end year (0: 1990, 1: 1991, ...)
:ANOPAD
:A ENMO uint8 0 ; end month
:ANOPAD
:A ENDY uint8 0 ; end day
:ANOPAD
:A ENHO uint8 0 ; end hour
:ANOPAD
:A ENMI uint8 0 ; end minute
:ANOPAD
:A ENSE uint8 0 ; end seconds
:ANOPAD
:A ENMS uint16 0 ; end milliseconds
:ANOPAD


:G PLYR ; "Player"
:GELEMS 7
:A GAME group GAME, * ; belongs to GAME (in play) or NULL (not in play)
:A NAME string(16) "Jupiter", "Player 1", "Player 2", "Player 3", "Player 4", "Player 5", "Player 6" ; player name, Jupiter (id=0) does the automated stuff
:A PERM uint8 0xFF, 0x03, * ; permissions bit1: can see public stuff, bit2: can see/have own stuff (off for possible spectators); bit3: can see other player's stuff; bit4: can see decks


:G FACT ; "Faction"
:A CNGR group NULL, * ; connection, PLYR (in play) or NULL (not in play); note that factions represent clockwise order
:A CNNR elem 0, * ; connection (player) number X if taken by player X, otherwise 0
:A NAME string(16) ; faction name
        "unaligned", "Aquila", "Luna Crescens", "Parma", "Corona", "Manus", "Porta"


:G SENA ; "Family Card / Senator"
:A IDNR uint8 ; family ID number (#X)
         1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 
	11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 
	21, 22, 23, 24, 25, 26, 27, 28, 29, 30
:A NAME string(16) ; family name
        "Cornelius" , "Fabius"    , "Valerius"  , "Julius"    , "Claudius"  , 
	"Manlius"   , "Fulvius"   , "Furius"    , "Aurelius"  , "Junius"    ,
	"Papirius"  , "Acilius"   , "Flaminius" , "Aelius"    , "Sulpicius" ,
	"Calpurnius", "Plautius"  , "Quinctius" , "Aemilius"  , "Terentius" ,
	"Servilius" , "Porcius"   , "Popillius" , "Cassius"   , "Sempronius",
	"Octavius"  , "Marius"    , "Tullius"   , "Licinius"  , "Pompeius"
:A MIL0 uint8 ; initial Military
	4,  4,  1,  4,  2,  3,  2,  3,  2,  1,
	1,  2,  4,  3,  3,  1,  2,  3,  4,  2,
	3,  2,  1,  3,  1,  2,  4,  2,  3,  2
:A ORA0 uint8 ; initial Oratory
	3,  2,  2,  3,  3,  2,  2,  3,  3,  2,
	2,  2,  2,  4,  2,  2,  1,  2,  2,  1,
	4,  4,  3,  3,  3,  3,  2,  3,  2,  2
:A LOY0 uint8 ; initial Loyalty
	9,  9, 10,  9,  7,  7,  8,  8,  7,  8,
	6,  7,  6,  7,  8,  9,  6,  6,  8,  6,
	9, 10,  7,  9,  6,  9,  9,  7,  9,  7
:A INF0 uint8 ; initial Influence
	5,  5,  5,  4,  4,  4,  4,  3,  3,  3,
	3,  3,  3,  2,  2,  2,  2,  1,  1,  1,
	4,  1,  3,  3,  3,  4,  4,  3,  3,  2
:A POP0 int8 ; initial Popularity
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0
:A MIL1 uint8 ; current Military
	4,  4,  1,  4,  2,  3,  2,  3,  2,  1,
	1,  2,  4,  3,  3,  1,  2,  3,  4,  2,
	3,  2,  1,  3,  1,  2,  4,  2,  3,  2
:A ORA1 uint8 ; current Oratory
	3,  2,  2,  3,  3,  2,  2,  3,  3,  2,
	2,  2,  2,  4,  2,  2,  1,  2,  2,  1,
	4,  4,  3,  3,  3,  3,  2,  3,  2,  2
:A LOY1 uint8 ; current Loyalty
	9,  9, 10,  9,  7,  7,  8,  8,  7,  8,
	6,  7,  6,  7,  8,  9,  6,  6,  8,  6,
	9, 10,  7,  9,  6,  9,  9,  7,  9,  7
:A INF1 uint8 ; current Influence
	5,  5,  5,  4,  4,  4,  4,  3,  3,  3,
	3,  3,  3,  2,  2,  2,  2,  1,  1,  1,
	4,  1,  3,  3,  3,  4,  4,  3,  3,  2
:A POP1 int8 ; current Popularity
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0
:A ALIG int16 ; aligned to faction  ; mimics http://www.amarriner.com/ror/auctoritas/turn1-10.htm
         1,  2,  6,  3,  1,  2, -1,  2,  5,  3,
	 1,  5,  4,  4,  3,  6, -1,  5,  4,  6,  
	-2, *
