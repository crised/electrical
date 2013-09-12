#ifndef ABPWR_H_INCLUDED
#define ABPWR_H_INCLUDED

#include "baseTypes.h"

/// \enum battState_t
/// States of internal battery
///
typedef enum{
    BATT_FAULT=-1,
    BATT_NOCHARGE=0,
    BATT_CHARGE=1
}battState_t;

/// \struct pwrData_t
/// Data retrieved from power sources
///
typedef struct{
    float mainPowerLevel;
    float batteryLevel;
    battState_t batteryState;
}pwrData_t;



/// \brief Init functions
/// These functions permits to initialize/uninitialize the library
///
abError_t abPWR_Initialize();
abError_t abPWR_Uninitialize();

/// \brief Battery control and read functions
/// These functions permits to read power state and manage the battery charge
///
abError_t abPWR_Read(pwrData_t* pwrData);
abError_t abPWR_StartBatteryCharge();
abError_t abPWR_StopBatteryCharge();


#endif // ABPWR_H_INCLUDED
