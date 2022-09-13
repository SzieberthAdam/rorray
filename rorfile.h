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

#define SIZE 1584

#define G_NULL 0
#define G_GAME 1
#define A_GAME_NAME 0
#define A_GAME_NAME_t char[16]
#define A_GAME_TURN 1
#define A_GAME_TURN_t int16_t
#define A_GAME_PHSE 2
#define A_GAME_PHSE_t int16_t
#define A_GAME_SPHS 3
#define A_GAME_SPHS_t uint32_t
#define A_GAME_STYR 4
#define A_GAME_STYR_t uint8_t
#define A_GAME_STMO 5
#define A_GAME_STMO_t uint8_t
#define A_GAME_STDY 6
#define A_GAME_STDY_t uint8_t
#define A_GAME_STHO 7
#define A_GAME_STHO_t uint8_t
#define A_GAME_STMI 8
#define A_GAME_STMI_t uint8_t
#define A_GAME_STSE 9
#define A_GAME_STSE_t uint8_t
#define A_GAME_STMS 10
#define A_GAME_STMS_t uint16_t
#define A_GAME_ENYR 11
#define A_GAME_ENYR_t uint8_t
#define A_GAME_ENMO 12
#define A_GAME_ENMO_t uint8_t
#define A_GAME_ENDY 13
#define A_GAME_ENDY_t uint8_t
#define A_GAME_ENHO 14
#define A_GAME_ENHO_t uint8_t
#define A_GAME_ENMI 15
#define A_GAME_ENMI_t uint8_t
#define A_GAME_ENSE 16
#define A_GAME_ENSE_t uint8_t
#define A_GAME_ENMS 17
#define A_GAME_ENMS_t uint16_t
#define G_PLYR 2
#define A_PLYR_GAME 0
#define A_PLYR_GAME_t uint16_t
#define A_PLYR_NAME 1
#define A_PLYR_NAME_t char[16]
#define A_PLYR_PERM 2
#define A_PLYR_PERM_t uint8_t
#define G_FACT 3
#define A_FACT_CNGR 0
#define A_FACT_CNGR_t uint16_t
#define A_FACT_CNNR 1
#define A_FACT_CNNR_t uint16_t
#define A_FACT_NAME 2
#define A_FACT_NAME_t char[16]
#define G_SENA 4
#define A_SENA_CNGR 0
#define A_SENA_CNGR_t uint16_t
#define A_SENA_CNNR 1
#define A_SENA_CNNR_t uint16_t
#define A_SENA_IDNR 2
#define A_SENA_IDNR_t uint8_t
#define A_SENA_NAME 3
#define A_SENA_NAME_t char[16]

#endif  /* #ifndef __DEFINE_RORHFILE__ */
;