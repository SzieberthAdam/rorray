K NUL ; Null / Nowhere / None / Empty
F 00000000000000
V 0 0 0x526F ; "Ro" File Signature

K GOD ; Game Itself / God (Game Master) Mode as Player Location
F 00000000000001
V 0 0 0x5220 ; "R " File Signature cont.

K VER ; File Version
F 00000000000010
V 0 0 1 ; File Version

K GAM ; Game Settings
F 000000 VVVVVVVV + 3

K SET ; Player Settings
F 000001 NNN VVVVV
N PLA

K RUL ; Era Specific Rules
F 000010 NNN VVVVV
N era ; era=0 : current era

K STA ; Rome / State / Game State
F 0000110000 VVVV

K DI6 ; Dice
F 0000110001 NN VV
N dice_nr
L 1 BRD
L 2 BRD
L 3 BRD
V 3 1 1 ; Colored

K PLA ; Player / Faction Hand
F 000011001 NNN VV
N player_nr
S 0 0 Game Master
S 1 0 Aquila
S 2 0 Luna Crescens
S 3 0 Parma
S 4 0 Corona
S 5 0 Manus
S 6 0 Porta
L 0 GOD
L 1 BRD
L 2 BRD
L 3 BRD
L 4 BRD
L 5 BRD
L 6 BRD

K DE1 ; Deck Pool #1
F 00001101 NNNN VV
N deck_id

K DE2 ; Deck Pool #2
F 00001110 NNNN VV
N deck2_id

K BRD ; Board
F 000011111 01 000

K DIS ; Discards
F 000011111 01 001

K DRA ; Draw Pile
F 000011111 01 010

K LE0 ; Force Pool - Legion
F 000011111 01 100

K FL0 ; Force Pool - Fleet
F 000011111 01 110

K FRM ; Forum
F 000011111 10 000

K TRE ; Treasury (Location)
F 000011111 10 001

K UNR ; Unrest (Location)
F 000011111 10 010

K LA1 ; Active Law Cards
F 000011111 10 011

K LE1 ; Active Forces - Legion
F 000011111 10 100

K VE1 ; Active Forces - Veteran
F 000011111 10 101

K FL1 ; Active Forces - Fleet
F 000011111 10 110

K LRU ; Enemy Leaders without Matching War
F 000011111 11 001

K REP ; Repopulating Rome
F 000011111 11 010

K DES ; Destroyed Concessions
F 000011111 11 011

K INA ; Inactive Wars
F 000011111 11 100

K IMM ; Imminent Wars / Time Delayed Wars
F 000011111 11 101

K UNP ; Unprosecuted Wars
F 000011111 11 110

K ACT ; Active Wars
F 000011111 11 111

K TAL ; Talents
F 0001 NNNNNNNN VV

K BIL ; Land Bills
F 001000000 NNN VV         

K MOR ; Mortality Chits
F 001001 NNNNNN VV

K LEG ; Legion
F 001010 NNNNN VVV

K FLE ; Fleet
F 001011 NNNNN VVV         

K EV1 ; Event (Gray/Pink)
F 00111 NNNNNN VVV

K PRV ; Province
F 0111 NNNNNN VVVV

K INT ; Intrigue
F 100 NNNNNNN VVVV

K CON ; Concession
F 101000 NNNNN VVV

K LA0 ; Law
F 101001 NNNNN VVV        

K LR2 ; Leader (Red Card)
F 101011 NNNN VVVV

K LR1 ; Leader (Black Card)
F 10110 NNNNN VVVV        

K EV2 ; Event (Black Card)
F 101111 NNNNN VVV

K WAR ; War
F 110 NNNNNN VVVVV

K SEN ; Family Card / Senator
F 1110 NNNNNN VVVV
S  1 0 Cornelius
S  2 0 Fabius
S  3 0 Valerius
S  4 0 Julius
S  5 0 Claudius
S  6 0 Manlius
S  7 0 Fulvius
S  8 0 Furius
S  9 0 Aurelius
S 10 0 Junius
S 11 0 Papirius
S 12 0 Acilius
S 13 0 Flaminius
S 14 0 Aelius
S 15 0 Sulpicius
S 16 0 Calpurnius
S 17 0 Plautius
S 18 0 Quinctius
S 19 0 Aemilius
S 20 0 Terentius
S 21 0 Servilius
S 22 0 Porcius
S 23 0 Popillius
S 24 0 Cassius
S 25 0 Sempronius
S 26 0 Octavius
S 27 0 Marius
S 28 0 Tullius
S 29 0 Licinius
S 30 0 Pompeius
S 31 0 Domitius
S 32 0 Antonius
S 33 0 Caecilius
S 34 0 Marcius
L  1 DE1 0
L  2 DE1 0
L  3 DE1 0
L  4 DE1 0
L  5 DE1 0
L  6 DE1 0
L  7 DE1 0
L  8 DE1 0
L  9 DE1 0
L 10 DE1 0
L 11 DE1 0
L 12 DE1 0
L 13 DE1 0
L 14 DE1 0
L 15 DE1 0
L 16 DE1 0
L 17 DE1 0
L 18 DE1 0
L 19 DE1 0
L 20 DE1 0

K MAN ; Statesman
F 1111 NNNNNN VVVV
S  0 0 Military Leader
S  1 0 Scipio
S  2 0 Scipio Ae.
S  3 0 Sulla
S  4 0 Cunctator
S  5 0 Caesar
S  6 0 Caecus
S  7 0 Flaccus
S  8 0 Camillus
S  9 0 Brutus
S 10 0 Regulus
S 11 0 Flaminius
S 12 0 Cincinnatus
S 13 0 Macedonicus
S 14 0 Lepidus
S 15 0 Glaucia
S 16 0 Cato C.
S 17 0 Cato U.
S 18 0 Laenas
S 19 0 Cassius
S 20 0 T. Gracchus
S 21 0 G. Gracchus
S 22 0 Thurinus
S 23 0 Marius
S 24 0 Cicero
S 25 0 Crassus
S 26 0 Lucullus
S 27 0 Pompeius
S 28 0 Antonius