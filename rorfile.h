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

#define SIZE 8185

#define G_NULL 0
#define G_GAME 1
#define A_GAME_DESC 0
#define A_GAME_DESC_t char[32]
#define A_GAME_NAME 1
#define A_GAME_NAME_t char[16]
#define A_GAME_NFAC 2
#define A_GAME_NFAC_t int8_t
#define A_GAME_TURN 3
#define A_GAME_TURN_t int16_t
#define A_GAME_PHSE 4
#define A_GAME_PHSE_t int16_t
#define A_GAME_SPHS 5
#define A_GAME_SPHS_t uint32_t
#define G_RSET 2
#define A_RSET_TERC 0
#define A_RSET_TERC_t uint8_t
#define A_RSET_NSEN 1
#define A_RSET_NSEN_t uint8_t
#define G_RERA 3
#define A_RERA_NAME 0
#define A_RERA_NAME_t char[16]
#define G_FACT 4
#define A_FACT_CNGR 0
#define A_FACT_CNGR_t uint16_t
#define A_FACT_CNNR 1
#define A_FACT_CNNR_t uint16_t
#define A_FACT_NAME 2
#define A_FACT_NAME_t char[16]
#define A_FACT_LEGR 3
#define A_FACT_LEGR_t uint16_t
#define A_FACT_LENR 4
#define A_FACT_LENR_t uint16_t
#define G_DECK 5
#define G_OFFI 6
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
#define G_HRAO 7
#define A_HRAO_CNGR 0
#define A_HRAO_CNGR_t uint16_t
#define A_HRAO_CNNR 1
#define A_HRAO_CNNR_t uint16_t
#define G_PRMA 8
#define A_PRMA_CNGR 0
#define A_PRMA_CNGR_t uint16_t
#define A_PRMA_CNNR 1
#define A_PRMA_CNNR_t uint16_t
#define G_SENA 9
#define A_SENA_RERA 0
#define A_SENA_RERA_t uint8_t
#define A_SENA_IDNR 1
#define A_SENA_IDNR_t uint8_t
#define A_SENA_NAME 2
#define A_SENA_NAME_t char[16]
#define A_SENA_MIL0 3
#define A_SENA_MIL0_t uint8_t
#define A_SENA_ORA0 4
#define A_SENA_ORA0_t uint8_t
#define A_SENA_LOY0 5
#define A_SENA_LOY0_t uint8_t
#define A_SENA_INF0 6
#define A_SENA_INF0_t uint8_t
#define A_SENA_POP0 7
#define A_SENA_POP0_t int8_t
#define A_SENA_MIL1 8
#define A_SENA_MIL1_t uint8_t
#define A_SENA_ORA1 9
#define A_SENA_ORA1_t uint8_t
#define A_SENA_LOY1 10
#define A_SENA_LOY1_t uint8_t
#define A_SENA_INF1 11
#define A_SENA_INF1_t uint8_t
#define A_SENA_POP1 12
#define A_SENA_POP1_t int8_t
#define A_SENA_ALIG 13
#define A_SENA_ALIG_t int8_t
#define A_SENA_CNGR 14
#define A_SENA_CNGR_t uint16_t
#define A_SENA_CNNR 15
#define A_SENA_CNNR_t uint16_t
#define A_SENA_OFFI 16
#define A_SENA_OFFI_t uint8_t
#define A_SENA_PRIC 17
#define A_SENA_PRIC_t uint8_t
#define G_SMAN 10
#define A_SMAN_RERA 0
#define A_SMAN_RERA_t uint8_t
#define A_SMAN_IDNR 1
#define A_SMAN_IDNR_t uint8_t
#define A_SMAN_IDCH 2
#define A_SMAN_IDCH_t char[2]
#define A_SMAN_SNAM 3
#define A_SMAN_SNAM_t char[16]
#define A_SMAN_NAME 4
#define A_SMAN_NAME_t char[64]
#define A_SMAN_TIME 5
#define A_SMAN_TIME_t char[32]
#define A_SMAN_MIL0 6
#define A_SMAN_MIL0_t uint8_t
#define A_SMAN_ORA0 7
#define A_SMAN_ORA0_t uint8_t
#define A_SMAN_LOY0 8
#define A_SMAN_LOY0_t uint8_t
#define A_SMAN_INF0 9
#define A_SMAN_INF0_t uint8_t
#define A_SMAN_POP0 10
#define A_SMAN_POP0_t int8_t
#define A_SMAN_MIL1 11
#define A_SMAN_MIL1_t uint8_t
#define A_SMAN_ORA1 12
#define A_SMAN_ORA1_t uint8_t
#define A_SMAN_LOY1 13
#define A_SMAN_LOY1_t uint8_t
#define A_SMAN_INF1 14
#define A_SMAN_INF1_t uint8_t
#define A_SMAN_POP1 15
#define A_SMAN_POP1_t int8_t
#define A_SMAN_SPEC 16
#define A_SMAN_SPEC_t char[128]
#define A_SMAN_ALIG 17
#define A_SMAN_ALIG_t int8_t
#define A_SMAN_CNGR 18
#define A_SMAN_CNGR_t uint16_t
#define A_SMAN_CNNR 19
#define A_SMAN_CNNR_t uint16_t
#define A_SMAN_OFFI 20
#define A_SMAN_OFFI_t uint8_t
#define A_SMAN_PRIC 21
#define A_SMAN_PRIC_t uint8_t
#define A_SMAN_L0W1 22
#define A_SMAN_L0W1_t int8_t
#define A_SMAN_L0W2 23
#define A_SMAN_L0W2_t int8_t
#define A_SMAN_L0W3 24
#define A_SMAN_L0W3_t int8_t
#define A_SMAN_L0N1 25
#define A_SMAN_L0N1_t int8_t
#define G_FORU 11
#define G_REPR 12

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

#define SPHS_PREP_TAKEFACTIONS 301200
#define SPHS_PREP_DEALSENATORS 301420
#define SPHS_PREP_TEMPORARYROMECONSUL 301600
#define SPHS_PREP_SELECTFACTIONLEADERS 301700
#define SPHS_PREP_INITIALFACTIONPHASE 301900

#endif  /* #ifndef __DEFINE_RORHFILE__ */
;