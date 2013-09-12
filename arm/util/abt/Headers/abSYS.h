#ifndef ABSYS_H_INCLUDED
#define ABSYS_H_INCLUDED

#include "baseTypes.h"

#define MAX_SYSTEM_UPDATES 10



/// \enum wakeupDevices_t
/// States for sleep with digital inputs
///
typedef enum UNSIGNED_ENUM{
    WKD_UNKNOWN        =0,             // must be enabled trough rtc alarm
    WKD_DIN1HI         =1,
    WKD_DIN1LOW        =2,
    WKD_DIN2HI         =3,
    WKD_DIN2LOW        =4,
    WKD_DIN3HI         =5,
    WKD_DIN3LOW        =6,
    WKD_DIN4HI         =7,
    WKD_DIN4LOW        =8,
    WKD_DIN5HI         =9,
    WKD_DIN5LOW        =10,
    WKD_DIN6HI         =11,
    WKD_DIN6LOW        =12,
    WKD_POWERFAULT     =13,
    WKD_BATTFAULT      =14,
    WKD_BATTCHARGE     =15,
    WKD_MOVEMENT       =16,
    WKD_RTC            =17,
    WKD_COM2DATA       =18,
    WKD_COM3DATA       =19,
    WKD_COM4DATA       =20,
    WKD_GSM1RING       =21,
    WKD_GSM2RING       =22,
    WKD_GPSANTENNA     =23
} wakeupDevices_t;

/// \enum sleepType_t
/// Type of sleep
///
typedef enum {
    ST_NORMAL=0,             // must be enabled trough rtc alarm
    ST_DEEP=1
} sleepType_t;


/// \struct sysData_t
/// System state informations
///
typedef struct{
    timeDate_t timeDate;
    int temperature;
}sysData_t;


/// \brief Init functions
/// These functions permits to initialize/uninitialize the library
///
abError_t abSYS_Initialize();
abError_t abSYS_Uninitialize();

/// \brief Sleep functions
/// These functions permits to control sleep and wakeup
///
abError_t abSYS_Sleep(wakeupDevices_t* deviceList, int numDevices, sleepType_t sleepType, wakeupDevices_t* wakeupReason);

/// \brief Read functions
/// These functions retrieves the system state
///
abError_t abSYS_Read(sysData_t* sysData);

/// \brief RTC Functions
/// These functions permits set clock and eventually alarm
///
abError_t abSYS_SetTimeDate(timeDate_t timeDate);
abError_t abSYS_SetAlarm(timeDate_t alarm);
abError_t abSYS_DisableAlarm();

/// \brief Watchdog functions
/// These functions permits control the watchdog
///
abError_t abSYS_EnableWatchdog();
abError_t abSYS_PulseWatchdog();


/// \brief Utilities
/// Some utilities
///
char* abSYS_Wakeup2String(wakeupDevices_t device);

#endif // ABSYS_H_INCLUDED
