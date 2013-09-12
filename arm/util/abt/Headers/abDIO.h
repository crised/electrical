#ifndef ABDIO_H_INCLUDED
#define ABDIO_H_INCLUDED

#include "baseTypes.h"

/// \enum dioState_t
/// Type of digital I/O states.
///
typedef enum{
    DIOS_UNKNOWN=-1,
    DIOS_OFF=0,
    DIOS_ON=1,
    DIOS_FLOAT=2
}dioState_t;

/// \enum dioOutput_t
/// Type of digital I/O outputs.
///
typedef enum{
    DIOD_DOUT1=0,
    DIOD_DOUT2=1,
    DIOD_DOUT3=2,
    DIOD_DOUT4=3,
    DIOD_RELAYA=4,
    DIOD_RELAYB=5,
    DIOD_LEDRED=6,
    DIOD_LEDGREEN=7
}dioOutput_t;


/// \struct dioData_t
/// States of digital I/O's
///
typedef struct{
    struct{
        dioState_t din1;
        dioState_t din2;
        dioState_t din3;
        dioState_t din4;
        dioState_t din5;
        dioState_t din6;
        dioState_t din7;
        dioState_t din8;
    }inputs;
    struct{
        dioState_t dout1;
        dioState_t dout2;
        dioState_t dout3;
        dioState_t dout4;
        dioState_t relayA;
        dioState_t relayB;
        dioState_t ledRed;
        dioState_t ledGreen;
    }outputs;
}dioData_t;



/// \brief Init functions
/// These functions permits to initialize/uninitialize the library
///
abError_t abDIO_Initialize();
abError_t abDIO_Uninitialize();

/// \brief RW functions
/// These functions permits to read inputs state and set outputs
///
abError_t abDIO_Read(dioData_t* dioData);
abError_t abDIO_Write(dioOutput_t output, dioState_t value);


#endif // ABDIO_H_INCLUDED
