K NIL
F 00000000000000
S 0 0 Nil
V 0 0 0x526F ; "Ro" File Signature

K JUP ; Jupiter / System / Game
F 00000000000001
S 0 0 Jupiter
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
L 0 BRD
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

K OFL ; Offline
F 000011111 00 110
S 0 0 Offline

K ONL ; Online
F 000011111 00 111
S 0 0 Online

K BRD ; Board
F 000011111 01 000
S 0 0 Board

K DIS ; Discards
F 000011111 01 001
S 0 0 Discards

K DRA ; Draw Pile
F 000011111 01 010
S 0 0 Draw Pile

K LE0 ; Force Pool - Legion
F 000011111 01 100
S 0 0 Legion Pool

K FL0 ; Force Pool - Fleet
F 000011111 01 110
S 0 0 Fleet Pool

K FRM ; Forum
F 000011111 10 000
S 0 0 Forum

K TRE ; Treasury (Location)
F 000011111 10 001
S 0 0 Treasury

K UNR ; Unrest (Location)
F 000011111 10 010
S 0 0 Unrest

K LA1 ; Active Law Cards
F 000011111 10 011
S 0 0 Active Law Cards

K LE1 ; Active Forces - Legion
F 000011111 10 100
S 0 0 Available Legions

K VE1 ; Active Forces - Veteran
F 000011111 10 101
S 0 0 Available Veteran Legions

K FL1 ; Active Forces - Fleet
F 000011111 10 110
S 0 0 Available Fleets

K LRU ; Enemy Leaders without Matching War
F 000011111 11 001
S 0 0 Enemy Leaders without Matching War

K REP ; Repopulating Rome
F 000011111 11 010
S 0 0 Repopulating Rome

K DES ; Destroyed Concessions
F 000011111 11 011
S 0 0 Destroyed Concessions

K INA ; Inactive Wars
F 000011111 11 100
S 0 0 Inactive Wars

K IMM ; Imminent Wars / Time Delayed Wars
F 000011111 11 101
S 0 0 Imminent Wars

K UNP ; Unprosecuted Wars
F 000011111 11 110
S 0 0 Unprosecuted Wars

K ACT ; Active Wars
F 000011111 11 111
S 0 0 Active Wars

K TAL ; Talents
F 0001 NNNNNNNN VV
S 0 0 Bank

K BIL ; Land Bills
F 001000000 NNN VV         
S 0 0 Land Bills

K MOR ; Mortality Chits
F 001001 NNNNNN VV
S 0 0 Mortality Chit Bag

K LEG ; Legion
F 001010 NNNNN VVV
S  1 0 I
S  2 0 II
S  3 0 III
S  4 0 IV
S  5 0 V
S  6 0 VI
S  7 0 VII
S  8 0 VIII
S  9 0 IX
S 10 0 X
S 11 0 XI
S 12 0 XII
S 13 0 XIII
S 14 0 XIV
S 15 0 XV
S 16 0 XVI
S 17 0 XVII
S 18 0 XVIII
S 19 0 XIX
S 20 0 XX
S 21 0 XXI
S 22 0 XXII
S 23 0 XXIII
S 24 0 XXIV
S 25 0 XXV


K FLE ; Fleet
F 001011 NNNNN VVV         
S  1 0 I
S  2 0 II
S  3 0 III
S  4 0 IV
S  5 0 V
S  6 0 VI
S  7 0 VII
S  8 0 VIII
S  9 0 IX
S 10 0 X
S 11 0 XI
S 12 0 XII
S 13 0 XIII
S 14 0 XIV
S 15 0 XV
S 16 0 XVI
S 17 0 XVII
S 18 0 XVIII
S 19 0 XIX
S 20 0 XX
S 21 0 XXI
S 22 0 XXII
S 23 0 XXIII
S 24 0 XXIV
S 25 0 XXV

K EV1 ; Event (Gray/Pink)
F 00111 NNNNNN VVV
S  1 0 Ally Deserts
S  2 0 Roman Auxiliaries Desert
S  3 0 Allied Enthusiasm
S  4 0 Extreme Allied Enthusiasm
S  5 0 Barbarian Raids
S  6 0 Barbarian Raids Increase
S  7 0 Drought
S  8 0 Severe Drought
S  9 0 Enemy Leader Dies
S 10 0 Enemy Sues for Peace
S 11 0 Enemy's Ally Deserts
S 12 0 Enemy Mercenaries Desert
S 13 0 Epidemic
S 14 0 Foreign Epidemic
S 15 0 Evil Omens
S 16 0 More Evil Omens
S 17 0 Internal Disorder
S 18 0 Increased Internal Disorder
S 19 0 Manpower Shortage
S 20 0 Extreme Manpower Shortage
S 21 0 No Recruitment
S 22 0 Mob Violence
S 23 0 More Mob Violence
S 24 0 Senate Attacked
S 25 0 People Revolt
S 26 0 Natural Disaster
S 27 0 Widespread Natural Disaster
S 28 0 New Alliance
S 29 0 Another New Alliance
S 30 0 Pretender Emerges
S 31 0 Pretender Victorious
S 32 0 Refuge
S 33 0 Rise from Refuge
S 34 0 Rhodian Maritime Alliance
S 35 0 Increased Rhodian Involvemnent
S 36 0 Storm at Sea
S 37 0 Another Storm at Sea
S 38 0 Trial of Verres
S 39 0 Another Corruption Trial

K PRV ; Province
F 0111 NNNNNN VVVV

K INT ; Intrigue
F 100 NNNNNNN VVVV
S 0x01 0 Tribune 1
S 0x02 0 Tribune 2
S 0x03 0 Tribune 3
S 0x04 0 Tribune 4
S 0x05 0 Tribune 5
S 0x06 0 Tribune 6
S 0x07 0 Tribune 7
S 0x08 0 Tribune 8
S 0x09 0 Tribune 9
S 0x0A 0 Tribune 10
S 0x0B 0 Tribune 11
S 0x0C 0 Tribune 12
S 0x0D 0 Tribune 13
S 0x0E 0 Tribune 14
S 0x0F 0 Tribune 15
S 0x10 0 Tribune 16
S 0x11 0 Tribune 17
S 0x12 0 Tribune 18
S 0x13 0 Tribune 19
S 0x14 0 Tribune 20
S 0x15 0 Tribune 21
S 0x16 0 Tribune 22
S 0x17 0 Tribune 23
S 0x21 0 Assassin 1
S 0x22 0 Assassin 2
S 0x23 0 Assassin 3
S 0x24 0 Assassin 4
S 0x25 0 Assassin 5
S 0x26 0 Assassin 6
S 0x27 0 Assassin 7
S 0x31 0 Influence Peddling 1
S 0x32 0 Influence Peddling 2
S 0x33 0 Influence Peddling 3
S 0x34 0 Influence Peddling 4
S 0x41 0 Murder of a Tribune 1
S 0x42 0 Murder of a Tribune 2
S 0x43 0 Murder of a Tribune 3
S 0x44 0 Murder of a Tribune 4
S 0x51 0 Secret Bodyguard 1
S 0x52 0 Secret Bodyguard 2
S 0x53 0 Secret Bodyguard 3
S 0x54 0 Secret Bodyguard 4
S 0x55 0 Secret Bodyguard 5
S 0x60 0 Blackmail
S 0x69 0 Graft 1
S 0x6A 0 Graft 2
S 0x78 0 Mob Incited to Violence
S 0x80 0 Open Bodyguard
S 0x88 0 Proscription
S 0x90 0 Seduction
S 0x98 0 Circus Maximus
S 0xA0 0 Marriage
S 0xA9 0 Scandal 1
S 0xAA 0 Scandal 2
S 0xB0 0 Spolia Opima
S 0xF0 0 Appian Way
S 0xF1 0 Juba I
S 0xF2 0 The Ultimate Decree
S 0xF3 0 Ariobarzanes III
S 0xF4 0 Assassination Conspiracy
S 0xF5 0 Annexation of Egypt
S 0xF0 1 312 BC - 308 BC
S 0xF1 1 60 BC - 46 BC
S 0xF2 1 52 BC
S 0xF3 1 51 BC - 42 BC
S 0xF4 1 44 BC
S 0xF5 1 30 BC

K CON ; Concession
F 101000 NNNNN VVV
S  1 0 Tax Farmer 1
S  2 0 Tax Farmer 2
S  3 0 Tax Farmer 3
S  4 0 Tax Farmer 4
S  5 0 Tax Farmer 5
S  6 0 Tax Farmer 6
S  7 0 Armaments
S  8 0 Egyptian Grain
S  9 0 Harbor Fees
S 10 0 Land Comissioner
S 11 0 Mining
S 12 0 Ship Building
S 13 0 Sicilian Grain

K LA0 ; Law
F 101001 NNNNN VVV        
S  1 0 Acilian Law
S  2 0 Calpurnian Law
S  3 0 Servilian Law
S  4 0 Military Reforms
S  5 0 Tradition Erodes
S  6 0 Julian Law
S  7 0 Plautian-Papirian Law
S  8 0 Gabinian Law
S  9 0 Manilian Law
S 10 0 Vatinian Law
S 11 0 Military Command

K LR2 ; Leader (Red Card)
F 101011 NNNN VVVV
S  1 0 Cleopatra VII
S  1 1 69 BC - 30 BC

K LR1 ; Leader (Black Card)
F 10110 NNNNN VVVV        
S  1 0 Lucius Tarquinius Superbus
S  2 0 Gaius Marcius Coriolanus
S  3 0 Pyrrhus
S  4 0 Hamilcar
S  5 0 Hannibal
S  6 0 Antiochus III
S  7 0 Philip V
S  8 0 Viriathus
S  9 0 Mithridates VI
S 10 0 Spartacus
S 11 0 Vercingetorix
S  1 1 ? - 495 BC
S  2 1 ? - 487 BC
S  3 1 c. 319 BC - 272 BC
S  4 1 c. 275 BC - 229 BC
S  5 1 247 BC - 183 BC
S  6 1 c. 242 BC - 187 BC
S  7 1 238 BC - 179 BC
S  8 1 c. 180 BC - 139 BC
S  9 1 120 BC - 63 BC
S 10 1 c. 111 BC - 71 BC
S 11 1 82 BC - 46 BC

K EV2 ; Event (Black Card)
F 101111 NNNNN VVV
S  1 0 1st Plebian Secession
S  2 0 Agrarian Unrest
S  3 0 The Twelve Tables
S  4 0 2nd Plebian Secession
S  5 0 Spurius Maelius
S  6 0 Sack of Rome
S  7 0 Plebian Revolt
S  8 0 Gallic Raids
S  9 0 Pergamene Bequest
S 10 0 Ptolemy Apion Bequest
S 11 0 Bithynian Bequest
S 12 0 2nd Catiline Conspiracy
S 13 0 Mob Riots
S 14 0 Annexation of Egypt
S 15 0 Civil Unrest
S 16 0 Republic Restored
S  1 1 494 BC
S  2 1 485 BC
S  3 1 450 BC
S  4 1 449 BC
S  5 1 439 BC
S  6 1 390 BC
S  7 1 287 BC
S  8 1 3rd century BC
S  9 1 133 BC
S 10 1 96 BC
S 11 1 75 BC
S 12 1 63 BC
S 13 1 53 BC
S 14 1 30 BC

K WAR ; War
F 110 NNNNNN VVVVV
S  1 0 1st Latin War
S  2 0 Volscian War
S  3 0 Aequian War
S  4 0 Veientine War
S  5 0 Siege of Veii
S  6 0 Tarquinian War
S  7 0 1st Samnite War
S  8 0 2nd Latin War
S  9 0 2nd Samnite War
S 10 0 3rd Samnite War
S 11 0 Pyrrhic War
S 12 0 1st Punic War
S 13 0 1st Illyrian War
S 14 0 1st Gallic War
S 15 0 2nd Illyrian War
S 16 0 2nd Punic War
S 17 0 1st Macedonian War
S 18 0 2nd Macedonian War
S 19 0 Syrian War
S 20 0 3rd Macedonian War
S 21 0 Spanish Revolts (Celtiberia / Lusitania)
S 22 0 3rd Punic War
S 23 0 4th Macedonian War
S 24 0 Numantine War
S 25 0 1st Sicilian Slave Revolt
S 26 0 Germanic Migrations (Cimbri / Teutons)
S 27 0 Jugurthine War
S 28 0 1st Cilician Pirates
S 29 0 2nd Sicilian Slave Revolt
S 30 0 Social War
S 31 0 1st Mithridatic War
S 32 0 2nd Mithridatic War
S 33 0 Sertorian Revolt
S 34 0 3rd Mithridatic War
S 35 0 Gladiator Slave Revolt
S 36 0 2nd Cilician Pirates
S 37 0 2nd Gallic War
S 38 0 Invasion of Britain
S 39 0 Invasion of Germany
S 40 0 Parthian War
S 41 0 3rd Gallic War
S 42 0 Alexandrine War
S 43 0 4th Mithridatic War (Pontic War)
S 44 0 Moesian War
S 45 0 Invasion of Partha
S 46 0 Dalmatian War
S 47 0 Cantabrian War
S 48 0 Invasion of Arabia
S  1 1 499 BC - 493 BC
S  2 1 c. 494 BC - 455 BC
S  3 1 c. 494 BC - 455 BC
S  4 1 483 BC - 435 BC
S  5 1 406 BC - 396 BC
S  6 1 358 BC - 351 BC
S  7 1 343 BC - 341 BC
S  8 1 340 BC - 338 BC
S  9 1 327 BC - 304 BC
S 10 1 298 BC - 290 BC
S 11 1 280 BC - 272 BC
S 12 1 264 BC - 241 BC
S 13 1 229 BC - 228 BC
S 14 1 225 BC - 222 BC
S 15 1 220 BC - 219 BC
S 16 1 218 BC - 201 BC
S 17 1 215 BC - 205 BC
S 18 1 200 BC - 196 BC
S 19 1 192 BC - 189 BC
S 20 1 171 BC - 167 BC
S 21 1 154 BC - 139 BC
S 22 1 149 BC - 146 BC
S 23 1 149 BC - 148 BC
S 24 1 143 BC - 133 BC
S 25 1 135 BC - 132 BC
S 26 1 111 BC - 102 BC
S 27 1 111 BC - 105 BC
S 28 1 104 BC - 101 BC
S 29 1 104 BC - 101 BC
S 30 1 90 BC - 88 BC
S 31 1 89 BC - 85 BC
S 32 1 83 BC - 82 BC
S 33 1 77 BC - 71 BC
S 34 1 74 BC - 63 BC
S 35 1 73 BC - 71 BC
S 36 1 67 BC - 61 BC
S 37 1 58 BC - 56 BC
S 38 1 55 BC - 54 BC
S 39 1 55 BC
S 40 1 53 BC
S 41 1 52 BC - 51 BC
S 42 1 48 BC - 47 BC
S 43 1 47 BC
S 44 1 29 BC
S 45 1 36 BC
S 46 1 35 BC - 34 BC
S 47 1 28 BC - 19 BC
S 48 1 24 BC

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
L  1 PLA 1
L  2 PLA 2
L  3 PLA 6
L  4 PLA 3
L  5 PLA 1
L  6 PLA 2
L  7 DE1 0
L  8 PLA 2
L  9 PLA 5
L 10 PLA 3
L 11 PLA 1
L 12 PLA 5
L 13 PLA 4
L 14 PLA 4
L 15 PLA 3
L 16 PLA 6
L 17 DE1 0
L 18 PLA 5
L 19 PLA 4
L 20 PLA 6

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
S  0 1 xA 
S  1 1 1A 
S  2 1 1B 
S  3 1 1C 
S  4 1 2A 
S  5 1 4A 
S  6 1 5A 
S  7 1 7A 
S  8 1 8A 
S  9 1 10A
S 10 1 12A
S 11 1 18A
S 12 1 18B
S 13 1 19A
S 14 1 19B
S 15 1 21A
S 16 1 22A
S 17 1 22B
S 18 1 23A
S 19 1 24A
S 20 1 25A
S 21 1 25B
S 22 1 26A
S 23 1 27A
S 24 1 28A
S 25 1 29A
S 26 1 29B
S 27 1 30A
S 28 1 32A
