#pragma once

//#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "../NIMSTMMODBUS/NiMotionMotorSDK.lib")
#pragma comment(lib, "../Chilkat/libs/ChilkatDbg.lib")
#pragma comment(lib, "../Chilkat/libs/ChilkatDbgDll.lib")
#pragma comment(lib, "../Chilkat/libs/ChilkatRel.lib")
#pragma comment(lib, "../Chilkat/libs/ChilkatRelDll.lib")
#include <iostream>
#include "../NIMSTMMODBUS/NiMotionMotorSDK.h"
#include "../Chilkat/include/CkSsh.h"
#include "../Chilkat/include/CkGlobal.h" 
#include <Windows.h>							//<windows.h> must be behind the "NiMotionMotorSDK.h" (There is a "winsock2.h" inside)
#include <string>

#include <wx/wx.h>
#include <wx/progdlg.h>

using namespace std;

class MyFunctions {
public:
	MyFunctions();

	bool ConnectModbus(wxString outputstring);
	bool DisConnectModbus(wxString outputstring);
	void ChilkatUnlock(wxString outputstring);
	bool ConnectRedPitaya(CkSsh* ssh, wxString outputstring);

	void MoveModbus1(int d);
	void MoveModbus2(int d);
	int GetCurrentPosition1(wxString outputstring);
	int GetCurrentPosition2(wxString outputstring);
	wxString Set_Zero_Origin1(wxString outputstring);
	wxString Set_Zero_Origin2(wxString outputstring);

	wxString RedPitayaLS(CkSsh* ssh, wxString outputstring);
	wxString RedPitayaLS_L(CkSsh* ssh, wxString outputstring);
	wxString RedPitayaCD(CkSsh* ssh, wxString outputstring);
	wxString RedPitayaDFTH(CkSsh* ssh, wxString outputstring);
	wxString RedPitayaRun(CkSsh* ssh, wxString textctrl, wxString outputstring);
	wxString RedPitayaRun2(CkSsh* ssh, int Decimation, string Filename, int Count, string Directory, wxString outputstring);
	wxString RedPitayaRun3(CkSsh* ssh, int Decimation, string Filename1, int Count1, string filename2, int Count2, string Directory, wxString outputstring);

	wxString AcquireDataInputOne(CkSsh* ssh, int Decimation, int Count1, int distance, int timesY, int timesX, int SleepTimeS, int SleepTimeR, int SleepTime0, int max, wxProgressDialog* dialog, wxString outputstring);
	wxString AcquireDataInputsTwo(CkSsh* ssh, int Decimation, int Count1, int Count2, int distance, int timesY, int timesX, int SleepTimeS, int SleepTimeR, int SleepTime0, int max, wxProgressDialog* dialog, wxString outputstring);
	wxString TestMovement(int distance, int timesY, int timesX, int SleepTimeS, int SleepTime0, int max, wxProgressDialog* dialog, wxString outputstring);

	CkSsh* ssh;

	int move1;
	int move2;
	int currentP1;
	int currentP2;

	int decimation1;
	int decimation2;
	string directory;
	string filename1;
	string filename2;
	int count1;
	int count2;
	int distance;
	int timesx;
	int timesy;
	int sleeps;
	int sleepr;
	int sleep0;
private:
};