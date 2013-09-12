#ifndef ABGPS_H_INCLUDED
#define ABGPS_H_INCLUDED

#include "baseTypes.h"

#define _GPS_LEAVEPOWERED


/// \enum fixState_t
/// Type of fix states.
///
typedef enum{
    GPSFIX_NOFIX=0,
    GPSFIX_2D=1,
    GPSFIX_3D=2,
    GPSFIX_MODULEERROR=3
}fixState_t;


/// \struct gpsData_t
/// Data retrieved from GPS module
///
typedef struct{
    struct{
        float latitude;
        float longitude;
        float altitude;
    }position;
    timeDate_t timeDate;
    struct{
        float speed;
        float direction;
    }navigation;
    struct{
        fixState_t fixState;
        int numSatellites;
        float dop;
        bool_t antennaAttached;
    }dataQuality;
}gpsData_t;




/// \brief Power functions
/// These functions permits to power on/off the gps module
/// and initialize/uninitialize the library
///
abError_t abGPS_PowerOn();
abError_t abGPS_PowerOff();

/// \brief Standby functions
/// These functions permits to suspend/resume the gps module
///
abError_t abGPS_Standby(standbyType_t standby);
abError_t abGPS_Restore(standbyType_t standby);

/// \brief Read functions
/// These functions permits to control the data collector
/// and get all GPS information
///
abError_t abGPS_Read(gpsData_t* gpsData);

abError_t abGPS_WritePOFF();

#endif // ABGPS_H_INCLUDED
