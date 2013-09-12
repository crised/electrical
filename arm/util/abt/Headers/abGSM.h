#ifndef ABGSM_H_INCLUDED
#define ABGSM_H_INCLUDED

#include "baseTypes.h"

#define MAX_GSM_NUMBER_SIZE 20
#define MAX_APN_SETTINGS_SIZE 50
#define MAX_SMS_SIZE 200

/// \enum gsmDevice_t
/// Gsm Devices
///
typedef enum{
	G_GSM1=1,
	G_GSM2=2
} gsmDevice_t;



/// \enum registrationStates_t
/// Registration status
///
typedef enum{
    REG_NOTREGISTERED=0,
	REG_REGISTERED=1,
	REG_SEARCHING=2,
	REG_REGISTEREDROAMING=3
} registrationStates_t;


/// \enum callType_t
/// Call type
///
typedef enum{
	CALL_VOICE=0,
	CALL_DATA=1,
} callType_t;

/// \enum callDirection_t
/// Call direction
///
typedef enum{
	CALL_INCOMING=0,
	CALL_OUTGOING=1,
} callDirection_t;

/// \enum callStatus_t
/// Call status
///
typedef enum{
	CALL_NOCALL=0,
	CALL_RINGING=1,
	CALL_ACTIVE=2,
	CALL_STANDBY=3
} callStatus_t;


/// \enum gsmNetworkSelection_t
/// Network selection
///
typedef enum{
	NET_AUTOMATIC=0,
	NET_MANUAL=1,
} gsmNetworkSelection_t;

/// \enum gsmAudioChannel_t
/// GSM audio channels
///
typedef enum{
	AUDIO_NOTAMPLIFIED=0,
	AUDIO_HANDSFREE=1,
} gsmAudioChannel_t;


/// \struct gsmStatus_t
/// GSM state informations
///
typedef struct{
    registrationStates_t registrationState;
    char currentOperator[50];
    bool_t pppUp;
    int signalQuality;
    bool_t newSms;
    struct{
        callType_t callType;
        callStatus_t callStatus;
        callDirection_t callDirection;
        char number[MAX_GSM_NUMBER_SIZE];
    }call;
}gsmStatus_t;


/// \struct gsmSettings_t
/// GSM settings
///
typedef struct{
    gsmNetworkSelection_t networkSelection;
    char networkOperatorsCode[10];
    char smsServiceCenter[MAX_GSM_NUMBER_SIZE];
    struct{
        gsmAudioChannel_t channel;
        int speakerVolume;
        int microphoneVolume;
    }audio;
    struct{
        char apn[MAX_APN_SETTINGS_SIZE];
        char apnUsername[MAX_APN_SETTINGS_SIZE];
        char apnPassword[MAX_APN_SETTINGS_SIZE];
    }ppp;
}gsmSettings_t;

/// \struct gsmSms_t
/// GSM SMS
///
typedef struct{
    char number[MAX_GSM_NUMBER_SIZE];
    char payload[MAX_SMS_SIZE];
    timeDate_t timeDate;
}gsmSms_t;



/// \brief Power functions
/// These functions permits to power on/off the gsm module
/// and initialize/uninitialize the library
///
abError_t abGSM_PowerOn(gsmDevice_t device, char* pinCode);
abError_t abGSM_PowerOff(gsmDevice_t device);

abError_t abGSM_ReadStatus(gsmDevice_t device, gsmStatus_t* status);

abError_t abGSM_Settings(gsmDevice_t device, gsmSettings_t settings);

abError_t abGSM_MakeCall(gsmDevice_t device, callType_t callType, char number[MAX_GSM_NUMBER_SIZE]);
abError_t abGSM_CloseCall(gsmDevice_t device);
abError_t abGSM_AnswerCall(gsmDevice_t device, int numRings);

abError_t abGSM_DataCallSend(gsmDevice_t device, char* payload, int plLength);
abError_t abGSM_DataCallReceive(gsmDevice_t device, char* payload, int* plLength);

abError_t abGSM_SendSMS(gsmDevice_t device, gsmSms_t sms);
abError_t abGSM_ReadSMS(gsmDevice_t device, gsmSms_t* sms);

abError_t abGSM_ConnectPPP(gsmDevice_t device);
abError_t abGSM_DisconnectPPP(gsmDevice_t device);

abError_t abGSM_Standby();
abError_t abGSM_Restore();

#endif // ABGSM_H_INCLUDED
