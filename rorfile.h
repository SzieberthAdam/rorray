#ifndef __DEFINE_RORHFILE__
#define __DEFINE_RORHFILE__


typedef struct _Header Header;
typedef struct _Group Group;
typedef struct _Attr Attr;

#define __INITRORAPI__    \
                          \
struct _Header {          \
   char     sig[3];       \
   uint8_t  ver;          \
   uint32_t elems;        \
   uint32_t attrs;        \
   uint32_t grouptocaddr; \
   uint32_t attrtocaddr;  \
   uint32_t attrvalsaddr; \
};                        \
                          \
                          \
struct _Group {           \
   char     group[4];     \
   uint16_t elems;        \
   uint16_t attrs;        \
   uint32_t elem0idx;     \
   uint32_t attr0idx;     \
};                        \
                          \
                          \
struct _Attr {            \
   char     group[4];     \
   char     attr[4];      \
   uint16_t groupidx;     \
   uint16_t type;         \
   uint32_t addr;         \
};


#define header(rordata)  (*(Header*)(rordata))
#define group(rordata, G)  (*((Group*)(rordata+header(rordata).grouptocaddr) + G))
#define attr(rordata, G, A)  (*((Attr*)(rordata+header(rordata).attrtocaddr) + (group(rordata, G).attr0idx) + A))
#define val0reladdr(rordata, G, A)  (header(rordata).attrvalsaddr + attr(rordata, G, A).addr)
#define val0absaddr(rordata, G, A)  (rordata+val0reladdr(rordata, G, A))
#define valsize(rordata, G, A)  (attr(rordata, G, A).type & 0x00FF)
#define valreladdr(rordata, G, A, i)  (header(rordata).attrvalsaddr + attr(rordata, G, A).addr) + (i*valsize(rordata, G, A))
#define valabsaddr(rordata, G, A, i)  (rordata+valreladdr(rordata, G, A, i))

#define SIZE 2545

#define G_NULL 0
#define G_RULE 1
#define A_RULE_TERC 0
#define A_RULE_TERC_t uint8_t
#define A_RULE_NSEN 1
#define A_RULE_NSEN_t uint8_t
#define G_GAME 2
#define A_GAME_NFAC 0
#define A_GAME_NFAC_t int8_t
#define A_GAME_TURN 1
#define A_GAME_TURN_t int16_t
#define A_GAME_PHSE 2
#define A_GAME_PHSE_t int16_t
#define A_GAME_SPHS 3
#define A_GAME_SPHS_t uint32_t
#define G_FORU 3
#define G_REPR 4
#define G_PLYR 5
#define A_PLYR_GAME 0
#define A_PLYR_GAME_t uint16_t
#define A_PLYR_NAME 1
#define A_PLYR_NAME_t char[16]
#define A_PLYR_PERM 2
#define A_PLYR_PERM_t uint8_t
#define G_FACT 6
#define A_FACT_CNGR 0
#define A_FACT_CNGR_t uint16_t
#define A_FACT_CNNR 1
#define A_FACT_CNNR_t uint16_t
#define A_FACT_NAME 2
#define A_FACT_NAME_t char[16]
#define G_DECK 7
#define G_SENA 8
#define A_SENA_IDNR 0
#define A_SENA_IDNR_t uint8_t
#define A_SENA_NAME 1
#define A_SENA_NAME_t char[16]
#define A_SENA_MIL0 2
#define A_SENA_MIL0_t uint8_t
#define A_SENA_ORA0 3
#define A_SENA_ORA0_t uint8_t
#define A_SENA_LOY0 4
#define A_SENA_LOY0_t uint8_t
#define A_SENA_INF0 5
#define A_SENA_INF0_t uint8_t
#define A_SENA_POP0 6
#define A_SENA_POP0_t int8_t
#define A_SENA_MIL1 7
#define A_SENA_MIL1_t uint8_t
#define A_SENA_ORA1 8
#define A_SENA_ORA1_t uint8_t
#define A_SENA_LOY1 9
#define A_SENA_LOY1_t uint8_t
#define A_SENA_INF1 10
#define A_SENA_INF1_t uint8_t
#define A_SENA_POP1 11
#define A_SENA_POP1_t int8_t
#define A_SENA_ALIG 12
#define A_SENA_ALIG_t int16_t
#define A_SENA_CNGR 13
#define A_SENA_CNGR_t uint16_t
#define A_SENA_CNNR 14
#define A_SENA_CNNR_t uint16_t
#define A_SENA_OFFI 15
#define A_SENA_OFFI_t uint8_t
#define A_SENA_CEXP 16
#define A_SENA_CEXP_t uint8_t
#define A_SENA_HRAO 17
#define A_SENA_HRAO_t uint8_t
#define A_SENA_PRMA 18
#define A_SENA_PRMA_t uint8_t
#define G_OFFI 9
#define A_OFFI_NAME 0
#define A_OFFI_NAME_t char[24]
#define A_OFFI_SNAM 1
#define A_OFFI_SNAM_t char[4]
#define A_OFFI_ORDR 2
#define A_OFFI_ORDR_t char[1]
#define A_OFFI_FORL 3
#define A_OFFI_FORL_t uint8_t
#define A_OFFI_INFL 4
#define A_OFFI_INFL_t int8_t

#define T_ATTR 0x0A02
#define T_BOOL 0x0F11
#define T_GROUP 0x0602
#define T_INT16 0x0102
#define T_INT32 0x0104
#define T_INT8 0x0101
#define T_ELEM 0x0E02
#define T_UINT16 0x0002
#define T_UINT32 0x0004
#define T_UINT8 0x0001

#define FACT_ELEMCOUNT 7
#define OFFICE_ROME_CONSUL 3

#define PHSE_PREP 0

#define SPHS_PREP_TAKEFACTIONS 201200
#define SPHS_PREP_DEALSENATORS 301420
#define SPHS_PREP_DEALSENATORS_RANDOM_ENTROPYREQ 301421
#define SPHS_PREP_TEMPORARYROMECONSUL 301600
#define SPHS_PREP_TEMPORARYROMECONSUL_RANDOM_ENTROPYREQ 301601
#define SPHS_PREP_SELECTFACTIONLEADERS 301700

#endif  /* #ifndef __DEFINE_RORHFILE__ */
;