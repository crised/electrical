#ifndef BASETYPES_H_INCLUDED
#define BASETYPES_H_INCLUDED

// Signals for sleep preparation
#define JUSTFORSLEEP 11

typedef unsigned char byte_t;

/// \enum bool_t
/// Type of boolean states.
///
typedef enum {
	B_FALSE=0,
	B_TRUE=1
} bool_t;

/// \enum standbyType_t
/// Type of standby for gps and gsm
///
typedef enum {
	S_FULLSTANDBY=0,
	S_SOFTSTANDBY=1
} standbyType_t;

/// \enum abError_t
/// Type of error states.
///
typedef enum{
    ABERR_OK=0,
    ABERR_IOERROR=-1,
    ABERR_OPERROR=-2,
    ABERR_VALERROR=-3,
    ABERR_SEMERROR=-4,
    ABERR_PRODCHKERROR=-5,
    ABERR_NOHARDWARE=-6,
    ABERR_INITERROR=-7,
    ABERR_ATERROR=-8
}abError_t;


/// \struct timeDate_t
/// Time and date
///
typedef struct{
    int day;
    int month;
    int year;
    int hour;
    int minutes;
    int seconds;
}timeDate_t;

#endif // BASETYPES_H_INCLUDED

