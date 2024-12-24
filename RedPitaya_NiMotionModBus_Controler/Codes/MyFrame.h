#pragma once
#include <wx/wx.h>


class MyApp : public wxApp {
public:
	virtual bool OnInit();
};

class MyFrame : public wxFrame {
public:
	MyFrame(const wxString& title);

	//Menus
	void OnRefreshCurrentPage(wxCommandEvent& event);
	void OnRefresh(wxCommandEvent& event);
	void OnUserManual(wxCommandEvent& event);
	void OnExit(wxCommandEvent& event);

	//Buttons
	void OnConnectN(wxCommandEvent& event);
	void OnConnectR(wxCommandEvent& event);
	void OnDisConnectN(wxCommandEvent& event);
	void OnDisConnectR(wxCommandEvent& event);

	void OnNControlMove1(wxCommandEvent& event);
	void OnNControlMove2(wxCommandEvent& event);
	void OnNControlGetP1(wxCommandEvent& event);
	void OnNControlGetP2(wxCommandEvent& event);
	void OnNControlSet1(wxCommandEvent& event);
	void OnNControlSet2(wxCommandEvent& event);

	void OnRControlLS(wxCommandEvent& event);
	void OnRControlLS_L(wxCommandEvent& event);
	void OnRControlCD(wxCommandEvent& event);
	void OnRControlDFTH(wxCommandEvent& event);
	void OnRControlRun(wxCommandEvent& event);	
	void OnRControlRun2(wxCommandEvent& event);
	void OnRControlRun3(wxCommandEvent& event);

	void OnAcquisitionOne(wxCommandEvent& event);
	void OnAcquisitionTwo(wxCommandEvent& event);
	void OnTestMovement(wxCommandEvent& event);


	//Inputs
	void OnNControlD1(wxSpinEvent& event);
	void OnNControlD2(wxSpinEvent& event);

	void OnDecimation1(wxCommandEvent& event);
	void OnDecimation2(wxCommandEvent& event);

	void OnFileName1(wxCommandEvent& event);
	void OnFileName2(wxCommandEvent& event);
	void OnDirectory(wxCommandEvent& event);

	void OnCount1(wxSpinEvent& event);
	void OnCount2(wxSpinEvent& event);
	void OnDistance(wxSpinEvent& event);
	void OnTimesX(wxSpinEvent& event);
	void OnTimesY(wxSpinEvent& event);
	void OnSleepS(wxSpinEvent& event);
	void OnSleepR(wxSpinEvent& event);
	void OnSleep0(wxSpinEvent& event);

	void OnTextCtrl(wxCommandEvent& event);

private:
	DECLARE_EVENT_TABLE()
};

DECLARE_APP(MyApp)

enum IDs {
	//menus
	ID_REFRESHCURRENTPAGE,
	ID_REFRESH,
	ID_USERMANUAL,
	//panles
	ID_NOTEBOOK,
	ID_PAGE1,
	ID_PAGE2,
	ID_PAGE3,
	ID_PAGE4,
	ID_PAGE5,
	//Buttons
	ID_CONNECT_N,
	ID_CONNECT_R,
	ID_DISCONNECT_N,
	ID_DISCONNECT_R,

	ID_N_CONTROL_MOVE1,
	ID_N_CONTROL_MOVE2,
	ID_N_CONTROL_GETP1,
	ID_N_CONTROL_GETP2,
	ID_N_CONTROL_SET1,
	ID_N_CONTROL_SET2,

	ID_R_CONTROL_LS,
	ID_R_CONTROL_LS_L,
	ID_R_CONTROL_CD,
	ID_R_CONTROL_DF_TH,
	ID_R_CONTROL_RUN,
	ID_R_CONTROL_RUN2,
	ID_R_CONTROL_RUN3,

	ID_ACQUISITION_ONE,
	ID_ACQUISITION_TWO,
	ID_TESTMOVEMENT,
	//Inputs
	ID_TEXTCONTROL,

	ID_N_CONTROL_DISTANCE1,
	ID_N_CONTROL_DISTANCE2,

	ID_DECIMATION1,
	ID_DECIMATION2,
	ID_DIRECTORY,
	ID_FILENAME1,
	ID_FILENAME2,
	ID_COUNT1,
	ID_COUNT2,
	ID_DISTANCE,
	ID_TIMESX,
	ID_TIMESY,
	ID_SLEEPS,
	ID_SLEEPR,
	ID_SLEEP0,
	};