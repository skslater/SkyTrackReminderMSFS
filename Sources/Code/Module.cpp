#include <MSFS\MSFS.h>
#include <MSFS\MSFS_WindowsTypes.h>
#include <SimConnect.h>
#include "Module.h"
#include <stdio.h>
#include <string.h>


HANDLE g_hSimConnect;
BOOL g_bBatteryOn;
BOOL g_bPrevBatteryOn;
BOOL g_bAPUActive;
BOOL g_bPrevAPUActive;
BOOL g_bEnginesRunning;

void CALLBACK MessageDispatchProc(SIMCONNECT_RECV* pData, DWORD cbData, void* pContext);

extern "C" MSFS_CALLBACK void module_init(void)
{
	g_hSimConnect = 0;
	g_bBatteryOn = false;
	g_bPrevBatteryOn = false;

	HRESULT hr = SimConnect_Open(&g_hSimConnect, PLUGIN_NAME, 0, 0, 0, 0);
	hr = SimConnect_CallDispatch(g_hSimConnect, MessageDispatchProc, 0);
}

extern "C" MSFS_CALLBACK void module_deinit(void)
{

	if (!g_hSimConnect)
		return;
	SimConnect_Close(g_hSimConnect);
}

void CALLBACK MessageDispatchProc(SIMCONNECT_RECV* pData, DWORD cbData, void* pContext)
{
	switch (pData->dwID)
	{
		case SIMCONNECT_RECV_ID_OPEN:
		{
			SIMCONNECT_RECV_OPEN* opn = (SIMCONNECT_RECV_OPEN*)pData;
			HRESULT hr = SimConnect_AddToDataDefinition(g_hSimConnect, STRSIMDATA_DEFINITION_ID, "ELECTRICAL MASTER BATTERY", "Bool", SIMCONNECT_DATATYPE_INT32);
			hr = SimConnect_AddToDataDefinition(g_hSimConnect, STRSIMDATA_DEFINITION_ID, "APU GENERATOR ACTIVE:1", "Bool", SIMCONNECT_DATATYPE_INT32);
			hr = SimConnect_AddToDataDefinition(g_hSimConnect, STRSIMDATA_DEFINITION_ID, "APU GENERATOR ACTIVE:2", "Bool", SIMCONNECT_DATATYPE_INT32);
			hr = SimConnect_AddToDataDefinition(g_hSimConnect, STRSIMDATA_DEFINITION_ID, "ENG COMBUSTION:1", "Bool", SIMCONNECT_DATATYPE_INT32);
			hr = SimConnect_AddToDataDefinition(g_hSimConnect, STRSIMDATA_DEFINITION_ID, "ENG COMBUSTION:2", "Bool", SIMCONNECT_DATATYPE_INT32);
			hr = SimConnect_AddToDataDefinition(g_hSimConnect, STRSIMDATA_DEFINITION_ID, "ENG COMBUSTION:3", "Bool", SIMCONNECT_DATATYPE_INT32);
			hr = SimConnect_AddToDataDefinition(g_hSimConnect, STRSIMDATA_DEFINITION_ID, "ENG COMBUSTION:4", "Bool", SIMCONNECT_DATATYPE_INT32);
			hr = SimConnect_RequestDataOnSimObject(g_hSimConnect, STRSIMDATA_REQUEST_ID, STRSIMDATA_DEFINITION_ID, SIMCONNECT_OBJECT_ID_USER, SIMCONNECT_PERIOD_SECOND, SIMCONNECT_DATA_REQUEST_FLAG_CHANGED);
			break;
		}

		case SIMCONNECT_RECV_ID_SIMOBJECT_DATA:
		{
			static const char message[] = REMINDER_TEXT;

			SIMCONNECT_RECV_SIMOBJECT_DATA* dta = (SIMCONNECT_RECV_SIMOBJECT_DATA*)pData;
			if (dta->dwRequestID == STRSIMDATA_REQUEST_ID)
			{
				STRSIMDATA* simData = (STRSIMDATA*)&dta->dwData;
				g_bEnginesRunning = simData->engine1Running || simData->engine2Running || simData->engine3Running || simData->engine4Running;
				g_bPrevBatteryOn = g_bBatteryOn;
				g_bBatteryOn = simData->batteryState != 0;
				g_bPrevAPUActive = g_bAPUActive;
				g_bAPUActive = simData->apu1Active != 0 || simData->apu2Active != 0;
				if ((g_bBatteryOn && !g_bPrevBatteryOn) || (g_bAPUActive && !g_bPrevAPUActive))
				{
					if (!g_bEnginesRunning)
					{
						SimConnect_Text(g_hSimConnect, SIMCONNECT_TEXT_TYPE_PRINT_WHITE, 1.0f, EVENT_REMINDER, sizeof(message) / sizeof(*(message)), (void*)message);
					}
				}
			}
			break;
		}

		case SIMCONNECT_RECV_ID_EXCEPTION:
		{
			SIMCONNECT_RECV_EXCEPTION* exc = (SIMCONNECT_RECV_EXCEPTION*)pData;
			fprintf(stderr, "%s: Exception %ld, Index %ld", PLUGIN_NAME, exc->dwException, exc->dwIndex);
			break;
		}

		default:
		{
			break;
		}
	}
}
