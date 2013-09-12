#ifndef ABMSR_H_INCLUDED
#define ABMSR_H_INCLUDED

#include "baseTypes.h"


/// \enum accScale_t
/// Type of sleep
///
typedef enum {
    ACC_LOW=0,
    ACC_MEDIUM=1,
    ACC_HIGH=2,
    ACC_VERYHIGH=3
} accScale_t;


/// \struct accData_t
/// Accelerometer value
///
typedef struct{
    float x;
    float y;
    float z;
}accData_t;

/// \struct gyrData_t
/// Gyroscope value
///
typedef struct{
    float x;
    float z;
}gyrData_t;

/// \struct analogData_t
/// Analog inputs
///
typedef struct{
    float analogInput1;
    float analogInput2;
    float analogInput3;
    float analogInput4;
}analogData_t;

/// \struct odoData_t
/// Odometer data
///
typedef struct{
    float speed;
    float distance;  // meters per pulse
}odoData_t;

/// \struct msrSettings_t
/// Settings for MSR devices
///
typedef struct{
    accScale_t acceleratorScale;
    float odometerCalibration;  // meters per pulse
    int movementTreshold;
}msrSettings_t;




/// \brief Init functions
/// These functions permits to initialize/uninitialize the library
///
abError_t abMSR_Initialize();
abError_t abMSR_Uninitialize();

/// \brief Settings functions
/// These functions permits to set the library parameters
///
abError_t abMSR_Settings(msrSettings_t settings);

/// \brief Read functions
/// These functions permits to read sensors values
///
abError_t abMSR_ReadAnalogInputs(analogData_t* data);
abError_t abMSR_ReadAccelerometer(accData_t* data);
abError_t abMSR_ReadGyroscope(gyrData_t* data);

/// \brief Odometer functions
/// These functions permits to manage the odometer
///
abError_t abMSR_StartOdometer();
abError_t abMSR_StopOdometer();
abError_t abMSR_ReadOdometer(odoData_t* odoData);

#endif // ABMSR_H_INCLUDED
