#pragma once

#define PLUGIN_NAME					"SkyTrack Reminder"
#define REMINDER_TEXT				"Don't forget to start SkyTrack!"

#define STRSIMDATA_REQUEST_ID		100
#define STRSIMDATA_DEFINITION_ID	200

#define DISPLAY_INTERVAL			300

typedef struct _STRSIMDATA
{
	INT32 batteryState;
	INT32 apu1Active;
	INT32 apu2Active;
	INT32 engine1Running;
	INT32 engine2Running;
	INT32 engine3Running;
	INT32 engine4Running;
} STRSIMDATA;

enum
{
	EVENT_REMINDER
};