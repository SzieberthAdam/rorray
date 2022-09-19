|SIG "RoR"
|VER 1


:G NULL ;

:G RULE ; "Rules"
:GELEMS 1
:A TERC uint8 0 ; Temporary Rome Consul (0xFF: no Temporary Rome Consul; bit0: resolve before(0)/after(1) faction leaders; bit1: lowest ID family card(hard)-AH(1)/random draw-VG(0); bit7: random draw has been done)
	; uncovered AHLRB: If, during the first mortality phase, the number is drawn of the Temporary Rome Consul and the senator is killed (faction leader or not) this process is immediately repeated.
:A NSEN uint8 3 ; initial number of senators per faction (4.4)
:ANOPAD

:G GAME ; "Game"
:A NFAC int8 -1 ; number of factions + 1 (faction#0 is unaligned) (value -1: unset)
:A TURN int16 0 ; turn
:ANOPAD
:A PHSE int16 0 ; phase
:ANOPAD
:A SPHS uint32 0 ; subphase (technical)
:ANOPAD

:G FORU ; "Forum"


:G REPR ; "Curia / Repopulating Rome"


:G PLYR ; "Player"
:GELEMS 7
:A GAME group GAME, * ; belongs to GAME (in play) or NULL (not in play)
:A NAME string(16) "Jupiter", "Player 1", "Player 2", "Player 3", "Player 4", "Player 5", "Player 6" ; player name, Jupiter (id=0) does the automated stuff
:A PERM uint8 0xFF, 0x03, * ; permissions bit1: can see public stuff, bit2: can see/have own stuff (off for possible spectators); bit3: can see other player's stuff; bit4: can see decks


:G FACT ; "Faction"
:A CNGR group GAME, PLYR, * ; connection, PLYR (in play) or NULL (not in play); note that factions represent clockwise order
:A CNNR elem 0, * ; connection (player) number X if taken by player X, otherwise 0
:A NAME string(16) ; faction name
        "unaligned", "Aquila", "Luna Crescens", "Parma", "Corona", "Manus", "Porta"


:G DECK ; "Deck"
:GELEMS 1

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
:A ALIG int16 ; aligned to faction
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-2, *
:A CNGR group ; connection group (NULL, DECK, FORU, REPR, PROV, WARE)
	DECK, DECK, DECK, DECK, DECK, DECK, DECK, DECK, DECK, DECK,
	DECK, DECK, DECK, DECK, DECK, DECK, DECK, DECK, DECK, DECK,
	NULL, *
:A CNNR elem 0, * ; connection group elem index
:A OFFI uint8 ; office
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0
:A PRCO uint8 ; Prior Consul
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0


:G OFFI; "Office"
:A NAME string(24) 
	"none", "Consul for Life", "Dictator", "Rome Consul", "Field Consul", 
	"Censor", "Master of Horse", "Dominant Player", "Pontifex Maximus",
	"Proconsul"
:A SNAM string(4) ; short name
	"", "CfL", "DI", "RC", "FC", "CE", "MoH", "DP", "PM", "PC"
:A ORDR string(1) ; order of precedence mark
	"", "*", "1", "2", "3", "4", "5", "5", "6", "X"
:A FORL uint8 ; applies for life
	0, 1, 0, 0, 0, 0, 0, 0, 1, 0
:A INFL int8 ; influence gain for taking
	0, 0, 7, 5, 5, 5, 3, 3, 5, 0