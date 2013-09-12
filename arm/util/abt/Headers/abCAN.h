#ifndef ABCAN_H_INCLUDED
#define ABCAN_H_INCLUDED

#include "baseTypes.h"

#define MAX_VEHICLEID_LENGTH 50

/// \enum canProtocol_t
/// Type of protocol
///
typedef enum {
    PR_NONE=0,           ///< No data collection will be performed...
    PR_FMS=1,
    PR_J1939=2
} canProtocol_t;

/// \enum direction_t
/// vehicle direction
///
typedef enum{
	C_CAN0=0,
	C_CAN1=1
} canDevice_t;

/// \enum direction_t
/// vehicle direction
///
typedef enum{
	D_FORWARD=0,                            ///< Forward
	D_BACKWARD=1                             ///< Backward
} direction_t;

/// \enum pedalState_t
/// State of vehicle pedal
///
typedef enum{
	P_RELEASED=0,                            ///< Pedal released
	P_PRESSED=1                               ///< Pedal pressed
} pedalState_t;

/// \enum driverState_t
/// State of driver
///
typedef enum{
	S_REST=0,                                  ///< Rest
	S_DRIVERAVAILABLE=1,                       ///< Driver available
	S_WORK=2,                                  ///< Working
	S_DRIVE=3,                                 ///< Driving
	S_ERROR=4,                                 ///< Error
	S_UNAVAILABLE=5                            ///< Not available
} driverState_t;

/// \struct canData_t
/// Can bus informations
///
typedef struct{
    struct{
        float fuelRate;		                        ///< Liters per hour
        float fuelEconomy;		                    ///< Km per liter
        float averageFuelEconomy;
        float fuelLevel;			                ///< Percent
        float totalFuelUsed;                        ///< Total fuel used by the vehicle
        float tripFuelUsed;
        float fuelDeliveryPressure;
    }fuel;
    struct{
        int engineSpeed;			                ///< RPM
        float engineThrottlePosition;
        float engineHours; 		                    ///< H
        int engineLoad;                             ///< Load at current speed 0-125 %
        int engineTorque;
        bool_t engineStarter;
        int intercoolerTemperature;
    }engine;
    struct{
        int hydraulicOilTemperature;
        bool_t winchOilOverMinimum;
        int hydraulicOilLevel;
        float engineFuelTemperature;
        float engineOilTemperature;
        int turboOilTemperature;
        float engineOilLevel;
        float engineOilPressure;
        int engineCoolantTemperature;
        float engineCoolantPressure;
        float engineCoolantLevel;
        float ptoOilTemperature;
        float washerLevel;
        float transmissionOilLevel;
        float transmissionOilPressure;
        float transmissionOilTemeperature;
    }fluids;
    struct{
        float pneumaticSupplyPressure;
        float parkingTrailerAirPressure;
        float auxiliaryEquipmentPressure;
        float airSuspensionPressure;
        bool_t airCompressorEnabled;
    }airSupply;
    struct{
        int axleLocation;			                ///< Axle location
        float axleWeight;			                ///< Weight (kg)
        int tireLocation;			                ///< Tire location
        float trailerWeight;
        float cargoWeight;
    }vehicleWeight;
    struct{
        pedalState_t clutchSwitch;                  ///< Cluth pedal
        pedalState_t brakeSwitch;                   ///< Brake pedal
        int acceleratorPosition;		            ///< Accelerator pressure (percent)
        int remoteAcceleratorPressure;              ///< Remote accelerator pressure (percent)
    }pedals;
    struct{
        bool_t workLight;
        bool_t mainLight;
        bool_t turnSignal;
        bool_t hazardLight;
        bool_t highBeamEnabled;
    }lights;
    struct{
        bool_t vehicleMoving;			            ///< Moving state
        direction_t direction;			            ///< vehicle direction
        int wheelBasedSpeed;			                    ///< Wheel speed (kmh)
        bool_t overspeed;			                ///< Overspeed
        int tachographicSpeed;                      ///< Tachographic speed
        bool_t cruiseControlActive;			        ///< Cruise control
        int cruiseControlSetSpeed;
        bool_t roadSpeedLimitStatus;
    }navigation;
    struct{
        int currentGear;                            ///< Current gear (0 neutral, 251 park)
        float gearRatio;
        bool_t reverseSwitch;
        bool_t neutralSwitch;
        bool_t forwardSwitch;
        struct{
            bool_t frontAxle1;
            bool_t frontAxle2;
            bool_t rearAxle1;
            bool_t rearAxle2;
            bool_t central;
            bool_t frontCentral;
            bool_t rearCentral;
        }differentialLock;
    }transmission;
    struct{
        bool_t ptoActive;			                ///< Power takeoff
        int ptoSpeed;
        long vehicleDistance;		                ///< Total distance (m)
        int serviceDistance;		                ///< Service distance (km)
        bool_t parkingBrakeSwitch;
        float cargoAmbientTemperature;
        float auxiliaryWaterPumpPressure;
    }vehicleState;
    struct{
        float particulateTrapInletPressure;
        float turbochargerBoostPressure;
        float engineAirInletPressure;
        float exhaustGasTemperature;
    }inletExhaust;
    struct{
        float batteryCurrent;
        float alternatorCurrent;
        float chargingSystemVoltage;
        float electricalVoltage;
        float batteryVoltage;
    }electricalPower;
    struct{
        float airTemperature;		                ///< Ambient air temperature (°C)
        float barometricPressure;
        float cabineTemperature;
        float inductedAirTemperature;
        float roadTemperature;
    }ambient;
    struct{
        bool_t driver1CardPresent;			        ///< Driver 1 card presence
        driverState_t driver1WorkingState;	        ///< Driver 1 working state
        bool_t driver2CardPresent;			        ///< Driver 2 card presence
        driverState_t driver2WorkingState;	        ///< Driver 2 working state
    }driver;
}canData_t;


/// \struct canSettings_t
/// CAN settings
///
typedef struct{
    struct{
        int baudrate;
        canProtocol_t protocol;
    }can0;
    struct{
        int baudrate;
        canProtocol_t protocol;
    }can1;
}canSettings_t;



/// \brief Power functions
/// These functions permits to power on / off the hardware
///
abError_t abCAN_PowerOn();
abError_t abCAN_PowerOff();

/// \brief Settings functions
/// These functions permits to configure the library
///
abError_t abCAN_Settings(canSettings_t settings);

/// \brief Data colelctor functions
/// These functions permits to control the data collection
///
abError_t abCAN_Start();
abError_t abCAN_Stop();
abError_t abCAN_Read(canData_t* can0Data, canData_t* can1Data);

#endif // ABCAN_H_INCLUDED
