|SIG string(3) "RoR"
|VER uint8 1
|DSC string(76) "VG, Advanced Game, Complete Extended Scenario"
|NAM string(15) ""
|LNG uint8 0 ### language (0: english)

:G NULL ###


:G GAME ### Game
:A NFAC int8 -1 ### number of factions + 1 (faction#0 is unaligned) (value -1: unset)
:ANOPAD
:A ERAI uint8 0 ### Era index (Era: historical period, see below)
:ANOPAD
:A TURN int16 0 ### turn
:ANOPAD
:A PHSE int16 -1 ### phase
:ANOPAD
:A SPHS uint32 0 ### subphase (technical)
:ANOPAD
:A TEMP raw(6) \x00\x00\x00\x00\x00\x00 ### memory
:ANOPAD


:G ERAR ### Rules of the historical periods (Era Rules)
:A NAME string(16) "Early Republic", "Middle Republic", "Late Republic"
:A NSEN uint8 3, 0, * ### number of senators per faction (4.4)
:A TERC uint8 1, 0, * ### Temporary Rome Consul (bit0: no TERC (0) / TERC (1); bit1: random draw (0) / lowest senator ID (1); bit2: before(0) / after(1) faction leaders; bit3: repeat if dies in first mortality phase (1); bit7: not resolved (0) / resolved (1))


:G FACT ### Faction (in clockwise seat order)
:A CNGR group GAME, * ### connection, GAME (in play) or NULL (not in play)
:A CNNR elem 0, * ### connection (player) number X if taken by player X, otherwise 0
:A NAME string(16) ### faction name
	"unaligned", "Manus", "Aquila", "Luna Crescens", "Corona", "Porta", "Parma"
:A LEGR group NULL, * ### Faction Leader group (SENA /SMAN)
:A LENR elem 0, * ### Faction Leader elem


:G DECK ### Deck
:GELEMS 1


:G OFFI ### Office
:A NAME string(24)
	"none", "Consul for Life", "Dictator", "Rome Consul", "Field Consul",
	"Censor", "Master of Horse", "Dominant Player", "Pontifex Maximus",
	"Proconsul"
:A SNAM string(4) ### short name
	"", "CfL", "DI", "RC", "FC", "CE", "MoH", "DP", "PM", "PC"
:A ORDR string(1) ### order of precedence mark
	"", "*", "1", "2", "3", "4", "5", "5", "6", "X"
:A FORL uint8 ### applies for life
	0, 1, 0, 0, 0, 0, 0, 0, 1, 0
:A INFL int8 ### influence gain for taking
	0, 0, 7, 5, 5, 5, 3, 3, 5, 0


:G HRAO ### HRAO
:GELEMS 1
:A CNGR group NULL
:A CNNR elem 0


:G PRMA ### Presiding Magistrate
:GELEMS 1
:A CNGR group NULL
:A CNNR elem 0


:G SENA ### Family Card / Senator
:A ERAI uint8 ### Era
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	1,  1,  1,  1,  1,  2,  2,  2,  2,  2
:A IDNR uint8 ### family ID number (#x)
         1,  2,  3,  4,  5,  6,  7,  8,  9, 10,
	11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
	21, 22, 23, 24, 25, 26, 27, 28, 29, 30
:A NAME string(16) ### family name
        "Cornelius" , "Fabius"    , "Valerius"  , "Julius"    , "Claudius"  ,
	"Manlius"   , "Fulvius"   , "Furius"    , "Aurelius"  , "Junius"    ,
	"Papirius"  , "Acilius"   , "Flaminius" , "Aelius"    , "Sulpicius" ,
	"Calpurnius", "Plautius"  , "Quinctius" , "Aemilius"  , "Terentius" ,
	"Servilius" , "Porcius"   , "Popillius" , "Cassius"   , "Sempronius",
	"Octavius"  , "Marius"    , "Tullius"   , "Licinius"  , "Pompeius"
:A MIL0 uint8 ### initial Military
	4,  4,  1,  4,  2,  3,  2,  3,  2,  1,
	1,  2,  4,  3,  3,  1,  2,  3,  4,  2,
	3,  2,  1,  3,  1,  2,  4,  2,  3,  2
:A ORA0 uint8 ### initial Oratory
	3,  2,  2,  3,  3,  2,  2,  3,  3,  2,
	2,  2,  2,  4,  2,  2,  1,  2,  2,  1,
	4,  4,  3,  3,  3,  3,  2,  3,  2,  2
:A LOY0 uint8 ### initial Loyalty
	9,  9, 10,  9,  7,  7,  8,  8,  7,  8,
	6,  7,  6,  7,  8,  9,  6,  6,  8,  6,
	9, 10,  7,  9,  6,  9,  9,  7,  9,  7
:A INF0 uint8 ### initial Influence
	5,  5,  5,  4,  4,  4,  4,  3,  3,  3,
	3,  3,  3,  2,  2,  2,  2,  1,  1,  1,
	4,  1,  3,  3,  3,  4,  4,  3,  3,  2
:A POP0 int8 ### initial Popularity
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0
:A MIL1 uint8 ### current Military
	4,  4,  1,  4,  2,  3,  2,  3,  2,  1,
	1,  2,  4,  3,  3,  1,  2,  3,  4,  2,
	3,  2,  1,  3,  1,  2,  4,  2,  3,  2
:A ORA1 uint8 ### current Oratory
	3,  2,  2,  3,  3,  2,  2,  3,  3,  2,
	2,  2,  2,  4,  2,  2,  1,  2,  2,  1,
	4,  4,  3,  3,  3,  3,  2,  3,  2,  2
:A LOY1 uint8 ### current Loyalty
	9,  9, 10,  9,  7,  7,  8,  8,  7,  8,
	6,  7,  6,  7,  8,  9,  6,  6,  8,  6,
	9, 10,  7,  9,  6,  9,  9,  7,  9,  7
:A INF1 uint8 ### current Influence
	5,  5,  5,  4,  4,  4,  4,  3,  3,  3,
	3,  3,  3,  2,  2,  2,  2,  1,  1,  1,
	4,  1,  3,  3,  3,  4,  4,  3,  3,  2
:A POP1 int8 ### current Popularity
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0
:A ALIG int8 ### aligned to faction
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
	-2, *
:A CNGR group ### connection group (NULL, DECK, DISC, FORU, REPR, PROV, WARR)
	DECK, DECK, DECK, DECK, DECK, DECK, DECK, DECK, DECK, DECK,
	DECK, DECK, DECK, DECK, DECK, DECK, DECK, DECK, DECK, DECK,
	NULL, *
:A CNNR elem 0, * ### connection group elem index
:A OFFI uint8 ### office
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0
:A PRIC uint8 ### Prior Consul
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,  0,  0

:G SMAN ### Statesman
:A ERAI uint8 ### Era
	0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1,
	2, 2, 2, 2, 2, 2
:A IDNR uint8 ### ID number (#x_)
	 1,  2, 18, 19, 22,
	 1,  1,  7, 21, 23, 25, 25, 27,
	 4, 22, 28, 29, 29, 30
:A IDCH string(2) ### ID character (#xA/#xB/..)
	"A", "A", "A", "A", "A",
	"B", "C", "A", "A", "A", "A", "B", "A",
	"A", "B", "A", "A", "B", "A"
:A SNAM string(16) ### short name
	### Early Republic
	"Scipio",
	"Cunctator",
	"Flamininus",
	"Paullus",
	"Cato",
	### Middle Republic
	"Scipio Ae.",
	"Sulla",
	"Flaccus",
	"Glaucia",
	"Laenas",
	"T. Gracchus",
	"G. Gracchus",
	"Marius",
	### Late Republic
	"Caesar",
	"Cato U.",
	"Cicero",
	"Crassus",
	"Lucullus",
	"Pompeius"
:A NAME string(64) ### name
	### Early Republic
	"Publius Cornelius Scipio Africanus",
	"Quintus Fabius Maximus Verrucosus Cunctator",
	"Titus Quinctius Flamininus",
	"Lucius Aemilius Paullus Macedonicus",
	"Marcus Porcius Cato Censorius",
	### Middle Republic
	"Publius Cornelius Scipio Aemilianus Africanus Numantinus",
	"Lucius Cornelius Sulla Felix",
	"Marcus Fulvius Flaccus",
	"Gaius Servilius Glaucia",
	"Publius Popillius Laenas",
	"Tiberius Sempronius Gracchus",
	"Gaius Sempronius Gracchus",
	"Gaius Marius",
	### Late Republic
	"Gaius Julius Caesar",
	"Marcus Porcius Cato Uticensis",
	"Marcus Tullius Cicero",
	"Marcus Licinius Crassus",
	"Lucius Licinius Lucullus",
	"Gnaeus Pompeius Magnus"
:A TIME string(32) ### time of life
	### Early Republic
	"235 BC - 183 BC",
	"c. 280 BC - 203 BC",
	"c. 229 BC - 174 BC",
	"c. 229 BC - 166 BC",
	"234 BC - 149 BC",
	### Middle Republic
	"185 BC - 129 BC",
	"c. 138 BC - 78 BC",
	"? - 121 BC",
	"c. 142 BC - 99 BC",
	"2nd century BC",
	"c. 166 BC - 133 BC",
	"c. 154 BC - 121 BC",
	"157 BC - 86 BC",
	### Late Republic
	"100 BC - 44 BC",
	"95 BC - 46 BC",
	"106 BC - 43 BC",
	"c. 115 BC - 53 BC",
	"c. 117 BC - 57/56 BC",
	"c. 106 BC - 48 BC"
:A MIL0 uint8 ### initial Military
	5,  5,  5,  5,  1,
	4,  4,  2,  1,  2,  1,  1,  5,
	6,  1,  1,  2,  5,  0x11 ### Pompeius bit4=1 tags dr+1
:A ORA0 uint8 ### initial Oratory
	5,  2,  4,  4,  6,
	3,  4,  5,  3,  5,  4,  5,  3,
	5,  6,  6,  2,  3,  3
:A LOY0 uint8 ### initial Loyalty
	7,  7,  7,  8, 10,
	7,  5,  6,  3,  6,  6,  6,  6,
	9, 11, 10,  4, 10,  9
:A INF0 uint8 ### initial Influence
	6,  3,  4,  4,  1,
	5,  5,  5,  3,  4,  3,  4,  5,
	5,  4,  3,  5,  3,  5
:A POP0 int8 ### initial Popularity
	0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  2,  3,  0,
	2,  0,  1,  0,  0,  5
:A MIL1 uint8 ### current Military
	5,  5,  5,  5,  1,
	4,  4,  2,  1,  2,  1,  1,  5,
	6,  1,  1,  2,  5,  0x11 ### Pompeius bit4=1 tags dr+1
:A ORA1 uint8 ### current Oratory
	5,  2,  4,  4,  6,
	3,  4,  5,  3,  5,  4,  5,  3,
	5,  6,  6,  2,  3,  3
:A LOY1 uint8 ### current Loyalty
	7,  7,  7,  8, 10,
	7,  5,  6,  3,  6,  6,  6,  6,
	9, 11, 10,  4, 10,  9
:A INF1 uint8 ### current Influence
	6,  3,  4,  4,  1,
	5,  5,  5,  3,  4,  3,  4,  5,
	5,  4,  3,  5,  3,  5
:A POP1 int8 ### current Popularity
	0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  2,  3,  0,
	2,  0,  1,  0,  0,  5
:A SPEC string(128)
	### Early Republic
	"Nullifies Punic War Disaster/Standoff. Cato Faction Loyalty: 0",
	"Halve all losses in Combat unless Master of Horse (Fractions rounded up).",
	"Nullifies any Macedonian War Disaster/Standoff.",
	"Nullifies Macedonian War Disaster/Standoff.",
	"1 Free Tribune Per Year. Scipios/Flamininus Faction Loyalty: 0",
	### Middle Republic
	"Nullifies 3rd Punic or Spanish War Disaster/Standoff.",
	"Nullifies Social/Mithridatic War Disaster/Standoff. Marius Faction Loyalty: 0",
	"May claim unheld Land Commissioner Office once without vote. Laenas Faction Loyalty: 0",
	"Mob Orator; may roll three dice if making popular Appeal. Sulla Faction Loyalty: 0",
	"May veto one Land Bill per Turn without using a Tribune card. Gracchi/Flaccus Faction Loyalty: 0",
	"May Propose 1 Land Bill/Year. Opposing G. Gracchus Faction Loyalty: 0",
	"May Propose 1 Land Bill/Year. Opposing T. Gracchus Faction Loyalty: 0",
	"+1 drm to Knight dr. Nullifies Social/Jugurthine War Disaster/Standoff. Sulla Faction Loyalty: 0",
	### Late Republic
	"Makes 2 Veteran Legions for each Victory. Cato Faction Loyalty: 0",
	"One free Tribune play/turn. Caesar Faction Loyalty: 0",
	"1 FREE Tribune play per Turn.",
	"Doubles all his income. May enter play even while Lucullus (29B) lives. Pompey Faction Loyalty: 0",
	"if Disaster/Standoff: losses are voided, but War is not Prosecuted & Lucullus must be recalled. Pompey Faction Loyalty: 0",
	"Popularity gains/losses are doubled. Crassus Faction Loyalty: 0"
:A ALIG int8 ### aligned to faction
	-1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, -1
:A CNGR group ### connection group (NULL, DECK, DISC, PROV, WARR)
	DECK, DECK, DECK, DECK, DECK,
	DECK, DECK, DECK, DECK, DECK, DECK, DECK, DECK,
	DECK, DECK, DECK, DECK, DECK, DECK
:A CNNR elem ### connection group elem index
	1, 1, 1, 1, 1,
	3, 3, 3, 3, 3, 3, 3, 3,
	4, 4, 4, 4, 4, 4
:A OFFI uint8 ### office
	0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0
:A PRIC uint8 ### Prior Consul
	0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0,  0,  0,
	0,  0,  0,  0,  0,  0
:A L0W1 int8 ### Loyalty = 0 if in same Faction with Statesman #x; negative (-1) value turns this off
	 4, -1, -1, -1,  0,
	-1, 12,  9,  6, 10, -1, -1,  6,
	14, 13, -1, 18, 18, 16
:A L0W2 int8 ### Loyalty = 0 if in same Faction with Statesman #x; negative (-1) value turns this off
	-1, -1, -1, -1,  2,
	-1, -1, -1, -1, 11, -1, -1, -1,
	-1, -1, -1, -1, -1, -1
:A L0W3 int8 ### Loyalty = 0 if in same Faction with Statesman #x; negative (-1) value turns this off
	-1, -1, -1, -1,  5,
	-1, -1, -1, -1,  7, -1, -1, -1,
	-1, -1, -1, -1, -1, -1
:A L0N1 int8 ### Loyalty = 0 if NOT in same Faction with Statesman #x; negative (-1) value turns this off
	-1, -1, -1, -1, -1,
	-1, -1, -1, -1, -1, 11, 10, -1,
	-1, -1, -1, -1, -1, -1


:G FORU ### Forum

:G REPR ### Curia / Repopulating Rome
