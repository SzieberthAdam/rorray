FILESIGNATURE "RoR"
FILEVERSION uint8_t 1
ADDRTYPE uint32_t
ALIGN 4

ANCHOR NULL
ITEM 1
ADDR NULL

ANCHOR FACT ; "Faction"
ITEM 6
VAR NAM string(20) 
"Aquila", "Luna Crescens", "Parma", "Corona", "Manus", "Porta"
ADDR NULL *

ANCHOR SENA ; "Family Card / Senator"
ITEM 34
VAR NUM uint8_t 
 1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 
11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 
31, 32, 33, 34
VAR NAM string
"Cornelius" , "Fabius"    , "Valerius"  , "Julius"    , "Claudius"  ,
"Manlius"   , "Fulvius"   , "Furius"    , "Aurelius"  , "Junius"    ,
"Papirius"  , "Acilius"   , "Flaminius" , "Aelius"    , "Sulpicius" ,
"Calpurnius", "Plautius"  , "Quinctius" , "Aemilius"  , "Terentius" ,
"Servilius" , "Porcius"   , "Popillius" , "Cassius"   , "Sempronius",
"Octavius"  , "Marius"    , "Tullius"   , "Licinius"  , "Pompeius"  ,
"Domitius"  , "Antonius"  , "Caecilius" , "Marcius"
ADDR 
FACT.1, FACT.2, FACT.6, FACT.3, FACT.1,
FACT.2, NULL  , FACT.2, FACT.5, FACT.3,
FACT.1, FACT.5, FACT.4, FACT.4, FACT.3,
FACT.6, NULL  , FACT.5, FACT.4, FACT.6,
NULL, *
