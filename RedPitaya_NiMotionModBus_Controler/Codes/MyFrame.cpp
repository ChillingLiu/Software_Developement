#include "MyFrame.h"
#include "MyFunctions.h"

#include <wx/notebook.h>
#include <wx/artprov.h>
#include <wx/spinctrl.h>
#include <wx/textctrl.h>

//Write here to be the global variables.
//All the functions of NiMotion ModBus and RedPitaya and all important parameters.
MyFunctions* functions;

//All the constructions of the GUI.
wxNotebook* notebook;

//Process output to show the process of functions.
wxString outputstring;
wxString textctrl;

wxTextCtrl* rcontrol_controltext;

int statusN = 0;
int statusR = 0;
wxArrayString decimations;

wxPanel* CreatePage1(wxNotebook* parent);
wxPanel* CreatePage2(wxNotebook* parent);
wxPanel* CreatePage3(wxNotebook* parent);
wxPanel* CreatePage4(wxNotebook* parent);
wxPanel* CreatePage5(wxNotebook* parent);
void CreateInitalPages(wxNotebook* parent);



wxIMPLEMENT_APP(MyApp);



bool MyApp::OnInit() {
	MyFrame* frame = new MyFrame(wxT("Data Acquisition on Red Pitaya with NiMotion Stepping Machine"));

	frame->SetClientSize(900, 500);

	frame->Show(true);

	return true;
}

MyFrame::MyFrame(const wxString& title) : wxFrame(NULL, wxID_ANY, title) {
	wxMenu* editMenu = new wxMenu;
	editMenu->Append(ID_REFRESHCURRENTPAGE, wxT("&Refresh Current Page\tF1"), wxT("Recreate Current Page"));
	editMenu->Append(ID_REFRESH, wxT("&Refresh\tF2"), wxT("Recreate All Pages"));

	wxMenu* helpMenu = new wxMenu;
	helpMenu->Append(ID_USERMANUAL, wxT("&User Maunal\tAlt-Z"), wxT("Some information of this program"));
	helpMenu->Append(wxID_EXIT, wxT("&Exit\tAlt-X"), wxT("Quit this Program"));

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append(editMenu, wxT("&Edit"));
	menuBar->Append(helpMenu, wxT("&Help"));

	SetMenuBar(menuBar);

	CreateStatusBar();

	SetStatusText(wxT("Welcome to wxWidgets!"));

	//Initialize the two classes
	functions = new MyFunctions();
	notebook = new wxNotebook(this, ID_NOTEBOOK);

	decimations.Add("16");
	decimations.Add("32");
	decimations.Add("64");
	decimations.Add("128");
	decimations.Add("256");

	CreateInitalPages(notebook);
}



BEGIN_EVENT_TABLE(MyFrame, wxFrame)

EVT_MENU(ID_REFRESHCURRENTPAGE, MyFrame::OnRefreshCurrentPage)
EVT_MENU(ID_REFRESH, MyFrame::OnRefresh)
EVT_MENU(ID_USERMANUAL, MyFrame::OnUserManual)
EVT_MENU(wxID_EXIT, MyFrame::OnExit)

EVT_BUTTON(ID_CONNECT_N, MyFrame::OnConnectN)
EVT_BUTTON(ID_CONNECT_R, MyFrame::OnConnectR)
EVT_BUTTON(ID_DISCONNECT_N, MyFrame::OnDisConnectN)
EVT_BUTTON(ID_DISCONNECT_R, MyFrame::OnDisConnectR)

EVT_BUTTON(ID_N_CONTROL_MOVE1, MyFrame::OnNControlMove1)
EVT_BUTTON(ID_N_CONTROL_MOVE2, MyFrame::OnNControlMove2)
EVT_BUTTON(ID_N_CONTROL_GETP1, MyFrame::OnNControlGetP1)
EVT_BUTTON(ID_N_CONTROL_GETP2, MyFrame::OnNControlGetP2)
EVT_BUTTON(ID_N_CONTROL_SET1, MyFrame::OnNControlSet1)
EVT_BUTTON(ID_N_CONTROL_SET2, MyFrame::OnNControlSet2)

EVT_BUTTON(ID_R_CONTROL_LS, MyFrame::OnRControlLS)
EVT_BUTTON(ID_R_CONTROL_LS_L, MyFrame::OnRControlLS_L)
EVT_BUTTON(ID_R_CONTROL_CD, MyFrame::OnRControlCD)
EVT_BUTTON(ID_R_CONTROL_DF_TH, MyFrame::OnRControlDFTH)
EVT_BUTTON(ID_R_CONTROL_RUN, MyFrame::OnRControlRun)
EVT_BUTTON(ID_R_CONTROL_RUN2, MyFrame::OnRControlRun2)
EVT_BUTTON(ID_R_CONTROL_RUN3, MyFrame::OnRControlRun3)

EVT_BUTTON(ID_ACQUISITION_ONE, MyFrame::OnAcquisitionOne)
EVT_BUTTON(ID_ACQUISITION_TWO, MyFrame::OnAcquisitionTwo)
EVT_BUTTON(ID_TESTMOVEMENT, MyFrame::OnTestMovement)


EVT_CHOICE(ID_DECIMATION1, MyFrame::OnDecimation1)
EVT_CHOICE(ID_DECIMATION2, MyFrame::OnDecimation2)

EVT_SPINCTRL(ID_N_CONTROL_DISTANCE1, MyFrame::OnNControlD1)
EVT_SPINCTRL(ID_N_CONTROL_DISTANCE2, MyFrame::OnNControlD2)

EVT_SPINCTRL(ID_COUNT1, MyFrame::OnCount1)
EVT_SPINCTRL(ID_COUNT2, MyFrame::OnCount2)
EVT_SPINCTRL(ID_DISTANCE, MyFrame::OnDistance)
EVT_SPINCTRL(ID_TIMESX, MyFrame::OnTimesX)
EVT_SPINCTRL(ID_TIMESY, MyFrame::OnTimesY)
EVT_SPINCTRL(ID_SLEEPS, MyFrame::OnSleepS)
EVT_SPINCTRL(ID_SLEEPR, MyFrame::OnSleepR)
EVT_SPINCTRL(ID_SLEEP0, MyFrame::OnSleep0)

EVT_TEXT(ID_TEXTCONTROL, MyFrame::OnTextCtrl)
EVT_TEXT(ID_FILENAME1, MyFrame::OnFileName1)
EVT_TEXT(ID_FILENAME2, MyFrame::OnFileName2)
EVT_TEXT(ID_DIRECTORY, MyFrame::OnDirectory)

END_EVENT_TABLE()



/*---------------------------------------Menus---------------------------------------*/



void RefreshPage(wxNotebook* parent, int i) {
	wxPanel* panel;
	switch (i) {
	case 0:
		parent->DeletePage(0);
		panel = CreatePage1(notebook);
		parent->InsertPage(0, panel, wxT("Connection Status"), true);
		break;
	case 1:
		parent->DeletePage(1);
		panel = CreatePage2(notebook);
		parent->InsertPage(1, panel, wxT("NiMotion Modbus Controller"), true);
		break;
	case 2:
		parent->DeletePage(2);
		panel = CreatePage3(notebook);
		parent->InsertPage(2, panel, wxT("RedPitaya Controller"), true);
		break;
	case 3:
		parent->DeletePage(3);
		panel = CreatePage4(notebook);
		parent->InsertPage(3, panel, wxT("One Input Acquisition"), true);
		break;
	case 4:
		parent->DeletePage(4);
		panel = CreatePage5(notebook);
		parent->InsertPage(4, panel, wxT("Two Inputs Acquisition"), true);
	}
}

void MyFrame::OnRefreshCurrentPage(wxCommandEvent& event) {
	RefreshPage(notebook, notebook->GetSelection());
}

void MyFrame::OnRefresh(wxCommandEvent& event) {
	for (int i = 0; i < 5; i++)
		RefreshPage(notebook, i);
}

void MyFrame::OnUserManual(wxCommandEvent& event) {
	outputstring = "Author of this program: Xiaoqi LIU, Khadga's Group, GTIIT";
	outputstring << "\r\nFor more information, please see the UserManual.pdf in the directory.";
	outputstring << "\r\nFor any help, please contact Email: xiaoqi.liuc@gamil.com or WeChat: +86 15665285263.";
	wxMessageBox(outputstring);
}

void MyFrame::OnExit(wxCommandEvent& event) {
	Close();
}



/*---------------------------------------Buttons Page1---------------------------------------*/



void MyFrame::OnConnectN(wxCommandEvent& event) {
	if (functions->ConnectModbus(outputstring)) {
		statusN = 1;
		RefreshPage(notebook, 0);
		wxMessageBox(wxT("NiMotion Budbus Connection Succeeded!"));
	}
	else {
		statusN = 0;
		RefreshPage(notebook, 0);
		wxMessageBox(wxT("NiMotion Mudbus Connection Failed!!!"));
	}
}

void MyFrame::OnConnectR(wxCommandEvent& event) {
	if (functions->ConnectRedPitaya(functions->ssh, outputstring)) {
		statusR = 1;
		RefreshPage(notebook, 0);
		wxMessageBox(wxT("RedPitaya Connection Succeeded!"));
	}
	else {
		statusR = 0;
		RefreshPage(notebook, 0);
		wxMessageBox(wxT("RedPitaya Connection Failed!!!"));
	}
}

void MyFrame::OnDisConnectN(wxCommandEvent& event) {
	if (functions->DisConnectModbus(outputstring)) {
		statusN = 0;
		RefreshPage(notebook, 0);
		wxMessageBox(wxT("NiMotion Budbus DisConnection Succeeded!"));
	}
}

void MyFrame::OnDisConnectR(wxCommandEvent& event) {
	statusR = 0;
	RefreshPage(notebook, 0);
	wxMessageBox(wxT("RedPitaya DisConnection Succeeded!"));
}

//i = 0 -> check N, i = 1 -> check R, i = 2 -> check N and R
bool CheckStatus(int i) {
	if (i == 0 && statusN != 1) {
		wxMessageBox(wxT("You didn't connect NiMotion Modbus!!!"));
		return false;
	}

	if (i == 1 && statusR != 1) {
		wxMessageBox(wxT("You didn't connect RedPitaya!!!"));
		return false;
	}

	if (i == 2) {
		if (statusN != 1) {
			wxMessageBox(wxT("You didn't connect NiMotion Modbus!!!"));
			return false;
		}
		if (statusR != 1) {
			wxMessageBox(wxT("You didn't connect RedPitaya!!!"));
			return false;
		}
	}

	return true;
}



/*---------------------------------------Buttons Page2---------------------------------------*/



void MyFrame::OnNControlMove1(wxCommandEvent& event) {
	if (CheckStatus(0)) {
		functions->MoveModbus1(functions->move1);
		wxLogStatus(wxT("Wait until Movement finish - 0.8 second."));
		Sleep(800);
		OnNControlGetP1(event);
		wxLogStatus(wxT("Movement finished"));
	}
}

void MyFrame::OnNControlMove2(wxCommandEvent& event) {
	if (CheckStatus(0)) {
		functions->MoveModbus2(functions->move2);
		wxLogStatus(wxT("Wait until Movement finish - 0.8 second."));
		Sleep(800);
		OnNControlGetP2(event);
		wxLogStatus(wxT("Movement finished"));
	}
}

void MyFrame::OnNControlGetP1(wxCommandEvent& event) {
	if (CheckStatus(0)) {
		functions->currentP1 = functions->GetCurrentPosition1(outputstring);
		RefreshPage(notebook, 1);
	}
}

void MyFrame::OnNControlGetP2(wxCommandEvent& event) {
	if (CheckStatus(0)) {
		functions->currentP2 = functions->GetCurrentPosition2(outputstring);
		RefreshPage(notebook, 1);
	}
}

void MyFrame::OnNControlSet1(wxCommandEvent& event) {
	if (CheckStatus(0)) {
		outputstring = functions->Set_Zero_Origin1(outputstring);
		OnNControlGetP1(event);
		wxMessageBox(outputstring);
	}
}

void MyFrame::OnNControlSet2(wxCommandEvent& event) {
	if (CheckStatus(0)) {
		outputstring = functions->Set_Zero_Origin2(outputstring);
		OnNControlGetP2(event);
		wxMessageBox(outputstring);
	}
}



/*---------------------------------------Buttons Page3---------------------------------------*/



void MyFrame::OnRControlLS(wxCommandEvent& event) {
	if (CheckStatus(1)) {
		outputstring = functions->RedPitayaLS(functions->ssh, outputstring);
		wxMessageBox(outputstring);
	}
}

void MyFrame::OnRControlLS_L(wxCommandEvent& event) {
	if (CheckStatus(1)) {
		outputstring = functions->RedPitayaLS_L(functions->ssh, outputstring);
		wxMessageBox(outputstring);
	}
}

void MyFrame::OnRControlCD(wxCommandEvent& event) {
	if (CheckStatus(1)) {
		outputstring = functions->RedPitayaCD(functions->ssh, outputstring);
		wxMessageBox(outputstring);
	}
}

void MyFrame::OnRControlDFTH(wxCommandEvent& event) {
	if (CheckStatus(1)) {
		outputstring = functions->RedPitayaDFTH(functions->ssh, outputstring);
		wxMessageBox(outputstring);
	}
}

void MyFrame::OnRControlRun(wxCommandEvent& event) {
	if (CheckStatus(1)) {
		outputstring = functions->RedPitayaRun(functions->ssh, textctrl, outputstring);
		wxMessageBox(outputstring);
	}
}

void MyFrame::OnRControlRun2(wxCommandEvent& event) {
	if (CheckStatus(1)) {
		outputstring = functions->RedPitayaRun2(functions->ssh, functions->decimation1, functions->filename1, functions->count1, functions->directory, outputstring);
		string d = "ls " + functions->directory;
		outputstring << functions->RedPitayaRun(functions->ssh, d, outputstring);
		wxMessageBox(outputstring);
	}
}

void MyFrame::OnRControlRun3(wxCommandEvent& event) {
	if (CheckStatus(1)) {
		outputstring = functions->RedPitayaRun3(functions->ssh, functions->decimation2, functions->filename1, functions->count1, functions->filename2, functions->count2, functions->directory, outputstring);
		string d = "ls " + functions->directory;
		outputstring << functions->RedPitayaRun(functions->ssh, d, outputstring);
		wxMessageBox(outputstring);
	}
}



/*---------------------------------------Buttons Page4-5---------------------------------------*/



void MyFrame::OnAcquisitionOne(wxCommandEvent& event) {
	if (CheckStatus(2)) {
		clock_t start = clock();

		int max = functions->timesx * functions->timesy;
		wxProgressDialog dialog(wxT("Progress dialog example"), wxT("An informative message"), max, notebook, wxPD_CAN_ABORT | wxPD_APP_MODAL | wxPD_ELAPSED_TIME | wxPD_ESTIMATED_TIME | wxPD_REMAINING_TIME);
		outputstring = functions->AcquireDataInputOne(functions->ssh, functions->decimation1, functions->count1, functions->distance, functions->timesy, functions->timesx, functions->sleeps, functions->sleepr, functions->sleep0, max, &dialog, outputstring);

		clock_t end = clock();
		double duration = ((double)(end - start)) / CLOCKS_PER_SEC;
		outputstring << "Total time =";
		outputstring.append(to_string(duration)).append("s");
		wxMessageBox(outputstring);
	}
}

void MyFrame::OnAcquisitionTwo(wxCommandEvent& event) {
	if (CheckStatus(2)) {
		clock_t start = clock();

		int max = functions->timesx * functions->timesy;
		wxProgressDialog dialog(wxT("Progress dialog example"), wxT("An informative message"), max, notebook, wxPD_CAN_ABORT | wxPD_APP_MODAL | wxPD_ELAPSED_TIME | wxPD_ESTIMATED_TIME | wxPD_REMAINING_TIME);
		outputstring = functions->AcquireDataInputsTwo(functions->ssh, functions->decimation2, functions->count1, functions->count2, functions->distance, functions->timesy, functions->timesx, functions->sleeps, functions->sleepr, functions->sleep0, max, &dialog, outputstring);

		clock_t end = clock();
		double duration = ((double)(end - start)) / CLOCKS_PER_SEC;
		outputstring << "Total time =";
		outputstring.append(to_string(duration)).append("s");
		wxMessageBox(outputstring);
	}
}

void MyFrame::OnTestMovement(wxCommandEvent& event) {
	if (CheckStatus(2)) {
		clock_t start = clock();

		int max = functions->timesx * functions->timesy;
		wxProgressDialog dialog(wxT("Progress dialog example"), wxT("An informative message"), max, notebook, wxPD_CAN_ABORT | wxPD_APP_MODAL | wxPD_ELAPSED_TIME | wxPD_ESTIMATED_TIME | wxPD_REMAINING_TIME);
		outputstring = functions->TestMovement(functions->distance, functions->timesy, functions->timesx, functions->sleeps, functions->sleepr, max, &dialog, outputstring);

		clock_t end = clock();
		double duration = ((double)(end - start)) / CLOCKS_PER_SEC;
		outputstring << "Total time =";
		outputstring.append(to_string(duration)).append("s");
		wxMessageBox(outputstring);
	}
}


/*---------------------------------------Inputs Page1-3---------------------------------------*/



void MyFrame::OnNControlD1(wxSpinEvent& event) {
	functions->move1 = event.GetValue();
	wxLogStatus("Distance to move for ModBus 1 is %d, um", functions->move1);
}

void MyFrame::OnNControlD2(wxSpinEvent& event) {
	functions->move2 = event.GetValue();
	wxLogStatus("Distance to move for ModBus 2 is %d, um", functions->move2);
}

void MyFrame::OnTextCtrl(wxCommandEvent& event) {
	textctrl = event.GetString();
	wxLogStatus("Command want to run is %s", textctrl);
}

void MyFrame::OnFileName1(wxCommandEvent& event) {
	functions->filename1 = event.GetString();
	wxLogStatus("The name of the file 1 to store the data is %s", functions->filename1);
}

void MyFrame::OnFileName2(wxCommandEvent& event) {
	functions->filename2 = event.GetString();
	wxLogStatus("The name of the file 2 to store the data is %s", functions->filename2);
}

void MyFrame::OnDirectory(wxCommandEvent& event) {
	functions->directory = event.GetString();
	wxLogStatus("The directory to store the files is %s", functions->directory);
}



/*---------------------------------------InPuts Page4-5---------------------------------------*/



void MyFrame::OnDecimation1(wxCommandEvent& event) {
	functions->decimation1 = wxAtoi(event.GetString());
	wxLogStatus("Decimation = %d, recommanded stable value is 32", functions->decimation1);
}

void MyFrame::OnDecimation2(wxCommandEvent& event) {
	functions->decimation2 = wxAtoi(event.GetString());
	wxLogStatus("Decimation = %d, recommanded stable value is 128", functions->decimation2);
}

void MyFrame::OnCount1(wxSpinEvent& event) {
	functions->count1 = event.GetValue();
	wxLogStatus("Count of points for channel IN1 = %d, 1 count = 8192 points", functions->count1);
}

void MyFrame::OnCount2(wxSpinEvent& event) {
	functions->count2 = event.GetValue();
	wxLogStatus("Count of points for channel IN1 = %d, 1 count = 8192 points", functions->count2);
}

void MyFrame::OnDistance(wxSpinEvent& event) {
	functions->distance = event.GetValue();
	wxLogStatus("Distance of movement per step = %d, um", functions->distance);
}

void MyFrame::OnTimesX(wxSpinEvent& event) {
	functions->timesx = event.GetValue();
	wxLogStatus("Times of movement on X-axis = %d", functions->timesx);
}

void MyFrame::OnTimesY(wxSpinEvent& event) {
	functions->timesy = event.GetValue();
	wxLogStatus("Times of movement on Y-axis = %d", functions->timesy);
}

void MyFrame::OnSleepS(wxSpinEvent& event) {
	functions->sleeps = event.GetValue();
	wxLogStatus("Sleep time for per movement = %d, us", functions->sleeps);
}

void MyFrame::OnSleepR(wxSpinEvent& event) {
	functions->sleepr = event.GetValue();
	wxLogStatus("Sleep time for per acquisition = %d", functions->sleepr);
}

void MyFrame::OnSleep0(wxSpinEvent& event) {
	functions->sleep0 = event.GetValue();
	wxLogStatus("Sleep time for modbus to return to origin = %d", functions->sleep0);
}



/*---------------------------------------Generate Pages---------------------------------------*/



void CreateInitalPages(wxNotebook* parent) {
	wxPanel* panel = CreatePage1(parent);
	parent->AddPage(panel, wxT("Connection Status"), true);

	panel = CreatePage2(parent);
	parent->AddPage(panel, wxT("NiMotion Modbus Controller"), false);

	panel = CreatePage3(parent);
	parent->AddPage(panel, wxT("RedPitaya Controller"), false);

	panel = CreatePage4(parent);
	parent->AddPage(panel, wxT("One Input Acquisition"), false);

	panel = CreatePage5(parent);
	parent->AddPage(panel, wxT("Two Inputs Acquisition"), false);
}

wxPanel* CreatePage1(wxNotebook* parent) {
	wxPanel* panel = new wxPanel(parent, ID_PAGE1);

	wxButton* connectN = new wxButton(panel, ID_CONNECT_N, wxT("Connect NiMotion Modbus"), wxPoint(25, 25), wxSize(200, 35), wxALIGN_LEFT);
	wxButton* connectR = new wxButton(panel, ID_CONNECT_R, wxT("Connect RedPitaya"), wxPoint(25, 75), wxSize(150, 35), wxALIGN_LEFT);

	wxStaticText* status1 = new wxStaticText(panel, wxID_ANY, wxT("Status:"), wxPoint(245, 33), wxSize(50, -1), wxALIGN_LEFT);
	wxStaticText* status1_imagesquare = new wxStaticText(panel, wxID_ANY, wxT(""), wxPoint(300, 33), wxSize(20, 20), wxALIGN_LEFT);
	if(statusN == 0)
		status1_imagesquare->SetBackgroundColour(*wxRED);
	else
		status1_imagesquare->SetBackgroundColour(*wxGREEN);

	wxStaticText* status2 = new wxStaticText(panel, wxID_ANY, wxT("Status:"), wxPoint(245, 83), wxSize(50, -1), wxALIGN_LEFT);
	wxStaticText* status2_imagesquare = new wxStaticText(panel, wxID_ANY, wxT(""), wxPoint(300, 83), wxSize(20, 20), wxALIGN_LEFT);
	if (statusR == 0)
		status2_imagesquare->SetBackgroundColour(*wxRED);
	else
		status2_imagesquare->SetBackgroundColour(*wxGREEN);

	wxButton* disconnectN = new wxButton(panel, ID_DISCONNECT_N, wxT("DisConnect NiMotion Modbus"), wxPoint(345, 25), wxSize(220, 35), wxALIGN_LEFT);
	wxButton* disconnectR = new wxButton(panel, ID_DISCONNECT_R, wxT("DisConnect RedPitaya"), wxPoint(345, 75), wxSize(168, 35), wxALIGN_LEFT);

	return panel;
}

wxPanel* CreatePage2(wxNotebook* parent) {
	wxPanel* panel = new wxPanel(parent, ID_PAGE3);

	wxStaticText* ncontrol1 = new wxStaticText(panel, wxID_ANY, wxT("Distance to move for ModBus 1: "), wxPoint(25, 25), wxDefaultSize, wxALIGN_LEFT);
	wxSpinCtrl* ncontrolD1 = new wxSpinCtrl(panel, ID_N_CONTROL_DISTANCE1, "", wxPoint(255, 22), wxSize(80, -1), 0, -5000, 5000, functions->move1);
	wxButton* ncontrol_move1 = new wxButton(panel, ID_N_CONTROL_MOVE1, "Run", wxPoint(355, 22), wxDefaultSize, wxALIGN_CENTER_VERTICAL);

	wxStaticText* ncontrolP1 = new wxStaticText(panel, wxID_ANY, wxT("Current position of ModBus 1: "), wxPoint(25, 65), wxDefaultSize, wxALIGN_LEFT);
	wxStaticText* ncontrolP1_ = new wxStaticText(panel, wxID_ANY, to_string(functions->currentP1), wxPoint(255, 65), wxDefaultSize, wxALIGN_LEFT);
	wxButton* ncontrol_getP1 = new wxButton(panel, ID_N_CONTROL_GETP1, wxT("Get"), wxPoint(355, 62), wxDefaultSize, wxALIGN_CENTER_VERTICAL);
	wxButton* ncontrol_set1 = new wxButton(panel, ID_N_CONTROL_SET1, wxT("Set here to zero and origin"), wxPoint(455, 62), wxDefaultSize, wxALIGN_CENTER_VERTICAL);

	wxStaticText* ncontrol2 = new wxStaticText(panel, wxID_ANY, wxT("Distance to move for ModBus 2: "), wxPoint(25, 145), wxDefaultSize, wxALIGN_LEFT);
	wxSpinCtrl* ncontrolD2 = new wxSpinCtrl(panel, ID_N_CONTROL_DISTANCE2, "", wxPoint(255, 142), wxSize(80, -1), 0, -5000, 5000, functions->move2);
	wxButton* ncontrol_move2 = new wxButton(panel, ID_N_CONTROL_MOVE2, "Run", wxPoint(355, 142), wxDefaultSize, wxALIGN_CENTER_VERTICAL);

	wxStaticText* ncontrolP2 = new wxStaticText(panel, wxID_ANY, wxT("Current position of ModBus 2: "), wxPoint(25, 185), wxDefaultSize, wxALIGN_LEFT);
	wxStaticText* ncontrolP2_ = new wxStaticText(panel, wxID_ANY, to_string(functions->currentP2), wxPoint(255, 185), wxDefaultSize, wxALIGN_LEFT);
	wxButton* ncontrol_getP2 = new wxButton(panel, ID_N_CONTROL_GETP2, wxT("Get"), wxPoint(355, 182), wxDefaultSize, wxALIGN_CENTER_VERTICAL);
	wxButton* ncontrol_set2 = new wxButton(panel, ID_N_CONTROL_SET2, wxT("Set here to zero and origin"), wxPoint(455, 182), wxDefaultSize, wxALIGN_CENTER_VERTICAL);

	return panel;
}

wxPanel* CreatePage3(wxNotebook* parent) {
	wxPanel* panel = new wxPanel(parent, ID_PAGE2);

	wxStaticText* rcontrol1 = new wxStaticText(panel, wxID_ANY, wxT("Some Useful Simple Command:"), wxPoint(25, 25), wxSize(250, -1), wxALIGN_LEFT);
	wxButton* rcontrol_ls = new wxButton(panel, ID_R_CONTROL_LS, wxT("ls"), wxPoint(25, 50), wxSize(50, 30), wxALIGN_CENTER_VERTICAL);
	wxButton* rcontrol_ls_l = new wxButton(panel, ID_R_CONTROL_LS_L, wxT("ls -l"), wxPoint(80, 50), wxSize(50, 30), wxALIGN_CENTER_VERTICAL);
	wxButton* rcontrol_cd = new wxButton(panel, ID_R_CONTROL_CD, wxT("cd ~"), wxPoint(135, 50), wxSize(50, 30), wxALIGN_CENTER_VERTICAL);
	wxButton* rcontrol_df_th = new wxButton(panel, ID_R_CONTROL_DF_TH, wxT("df -Th"), wxPoint(190, 50), wxSize(50, 30), wxALIGN_CENTER_VERTICAL);

	wxStaticText* rcontrol2 = new wxStaticText(panel, wxID_ANY, wxT("Enter your command here to control the RedPitaya directly:"), wxPoint(25, 90), wxSize(500, -1), wxALIGN_LEFT);
	rcontrol_controltext = new wxTextCtrl(panel, ID_TEXTCONTROL, wxT(""), wxPoint(25, 115), wxSize(400, -1), wxALIGN_LEFT);
	wxButton* rcontrol_run = new wxButton(panel, ID_R_CONTROL_RUN, wxT("Run"), wxPoint(430, 115), wxSize(50, -1), wxALIGN_CENTER_VERTICAL);

	wxStaticText* rcontrl3 = new wxStaticText(panel, wxID_ANY, wxT("One Input Acqusistion without NiMotionModBus:"), wxPoint(25, 175), wxDefaultSize, wxALIGN_LEFT);
	wxButton* rcontrl3_run = new wxButton(panel, ID_R_CONTROL_RUN2, wxT("Run"), wxPoint(370, 172), wxSize(50, -1), wxALIGN_CENTER_VERTICAL);
	wxStaticText* rcontrl3_decimation = new wxStaticText(panel, wxID_ANY, wxT("Decimation:"), wxPoint(25, 205), wxDefaultSize, wxALIGN_LEFT);
	wxChoice* rcontrl3_decimation_input = new wxChoice(panel, ID_DECIMATION1, wxPoint(115, 202), wxSize(65, -1), decimations);
	rcontrl3_decimation_input->Select(1);
	wxStaticText* rcontrl3_filename = new wxStaticText(panel, wxID_ANY, wxT("FileName:"), wxPoint(200, 205), wxDefaultSize, wxALIGN_LEFT);
	wxTextCtrl* rcontrl3_filename_input = new wxTextCtrl(panel, ID_FILENAME1, functions->filename1, wxPoint(280, 202), wxSize(65, -1), wxALIGN_LEFT);
	wxStaticText* rcontrl3_count = new wxStaticText(panel, wxID_ANY, wxT("Count:"), wxPoint(360, 205), wxDefaultSize, wxALIGN_LEFT);
	wxSpinCtrl* rcontrl3_count_input = new wxSpinCtrl(panel, ID_COUNT1, "", wxPoint(440, 202), wxSize(65, -1), 0, 2, 1000, 100);	
	wxStaticText* rcontrl3_directory = new wxStaticText(panel, wxID_ANY, wxT("Directory to store the file:"), wxPoint(520, 205), wxDefaultSize, wxALIGN_LEFT);
	wxTextCtrl* rcontrol3_directory_input = new wxTextCtrl(panel, ID_DIRECTORY, functions->directory, wxPoint(700, 202), wxSize(65, -1), wxALIGN_LEFT);

	wxStaticText* rcontrl4 = new wxStaticText(panel, wxID_ANY, wxT("Two Inputs Acqusistion without NiMotionModBus:"), wxPoint(25, 265), wxDefaultSize, wxALIGN_LEFT);
	wxButton* rcontrl4_run = new wxButton(panel, ID_R_CONTROL_RUN3, wxT("Run"), wxPoint(370, 262), wxSize(50, -1), wxALIGN_CENTER_VERTICAL);
	wxStaticText* rcontrl4_decimation = new wxStaticText(panel, wxID_ANY, wxT("Decimation:"), wxPoint(25, 295), wxDefaultSize, wxALIGN_LEFT);
	wxChoice* rcontrl4_decimation2_input = new wxChoice(panel, ID_DECIMATION2, wxPoint(115, 292), wxSize(65, -1), decimations);
	rcontrl4_decimation2_input->Select(3);
	wxStaticText* rcontrl4_filename1 = new wxStaticText(panel, wxID_ANY, wxT("FileName1:"), wxPoint(200, 295), wxDefaultSize, wxALIGN_LEFT);
	wxTextCtrl* rcontrl4_filename1_input = new wxTextCtrl(panel, ID_FILENAME1, functions->filename1, wxPoint(280, 292), wxSize(65, -1), wxALIGN_LEFT);
	wxStaticText* rcontrl4_count1 = new wxStaticText(panel, wxID_ANY, wxT("Count1:"), wxPoint(360, 295), wxDefaultSize, wxALIGN_LEFT);
	wxSpinCtrl* rcontrl4_count1_input = new wxSpinCtrl(panel, ID_COUNT1, "", wxPoint(440, 292), wxSize(65, -1), 0, 2, 1000, 100);
	wxStaticText* rcontrl4_directory = new wxStaticText(panel, wxID_ANY, wxT("Directory to store the file:"), wxPoint(520, 295), wxDefaultSize, wxALIGN_LEFT);
	wxTextCtrl* rcontrol4_directory_input = new wxTextCtrl(panel, ID_DIRECTORY, functions->directory, wxPoint(700, 292), wxSize(65, -1), wxALIGN_LEFT);
	wxStaticText* rcontrl4_filename2 = new wxStaticText(panel, wxID_ANY, wxT("FileName2:"), wxPoint(200, 325), wxDefaultSize, wxALIGN_LEFT);
	wxTextCtrl* rcontrl4_filename2_input = new wxTextCtrl(panel, ID_FILENAME2, functions->filename1, wxPoint(280, 322), wxSize(65, -1), wxALIGN_LEFT);
	wxStaticText* rcontrl4_count2 = new wxStaticText(panel, wxID_ANY, wxT("Count2:"), wxPoint(360, 325), wxDefaultSize, wxALIGN_LEFT);
	wxSpinCtrl* rcontrl4_count2_input = new wxSpinCtrl(panel, ID_COUNT2, "", wxPoint(440, 322), wxSize(65, -1), 0, 2, 1000, 100);

	return panel;
}

wxPanel* CreatePage4(wxNotebook* parent) {
	wxPanel* panel = new wxPanel(parent, ID_PAGE3);

	wxStaticText* acqone1 = new wxStaticText(panel, wxID_ANY, wxT("Decimation: "), wxPoint(25, 25), wxDefaultSize, wxALIGN_LEFT);
	wxChoice* inputD = new wxChoice(panel, ID_DECIMATION1, wxPoint(115, 22), wxSize(65, -1), decimations);
	inputD->Select(1);
	wxStaticText* acqone2 = new wxStaticText(panel, wxID_ANY, wxT("Count: "), wxPoint(205, 25), wxDefaultSize, wxALIGN_LEFT);
	wxSpinCtrl* inputC = new wxSpinCtrl(panel, ID_COUNT1, "", wxPoint(295, 22), wxSize(65, -1), 0, 0, 2000, functions->count1);

	wxStaticText* acqone3 = new wxStaticText(panel, wxID_ANY, wxT("Distance: "), wxPoint(25, 65), wxDefaultSize, wxALIGN_LEFT);
	wxSpinCtrl* inputDI = new wxSpinCtrl(panel, ID_DISTANCE, "", wxPoint(115, 62), wxSize(65, -1), 0, 0, 2000, functions->distance);
	wxStaticText* acqone4 = new wxStaticText(panel, wxID_ANY, wxT("TimesX: "), wxPoint(205, 65), wxDefaultSize, wxALIGN_LEFT);
	wxSpinCtrl* inputTX = new wxSpinCtrl(panel, ID_TIMESX, "", wxPoint(295, 62), wxSize(65, -1), 0, 0, 200, functions->timesx);
	wxStaticText* acqone5 = new wxStaticText(panel, wxID_ANY, wxT("TimesY: "), wxPoint(385, 65), wxDefaultSize, wxALIGN_LEFT);
	wxSpinCtrl* inputTY = new wxSpinCtrl(panel, ID_TIMESY, "", wxPoint(465, 62), wxSize(65, -1), 0, 0, 200, functions->timesy);

	wxStaticText* acqone6 = new wxStaticText(panel, wxID_ANY, wxT("SleepTimeS: "), wxPoint(25, 105), wxDefaultSize, wxALIGN_LEFT);
	wxSpinCtrl* inputSS = new wxSpinCtrl(panel, ID_SLEEPS, "", wxPoint(115, 102), wxSize(65, -1), 0, 0, 2000, functions->sleeps);
	wxStaticText* acqone7 = new wxStaticText(panel, wxID_ANY, wxT("SleepTimeR: "), wxPoint(205, 105), wxDefaultSize, wxALIGN_LEFT);
	wxSpinCtrl* inputSR = new wxSpinCtrl(panel, ID_SLEEPR, "", wxPoint(295, 102), wxSize(65, -1), 0, 0, 2000, functions->sleepr);
	wxStaticText* acqone8 = new wxStaticText(panel, wxID_ANY, wxT("SleepTime0: "), wxPoint(385, 105), wxDefaultSize, wxALIGN_LEFT);
	wxSpinCtrl* inputS0 = new wxSpinCtrl(panel, ID_SLEEP0, "", wxPoint(465, 102), wxSize(65, -1), 0, 0, 100000, functions->sleep0);

	wxButton* acqone = new wxButton(panel, ID_ACQUISITION_ONE, "Start One Input Acquisition!", wxPoint(25, 145), wxDefaultSize, wxALIGN_LEFT);
	wxButton* testm = new wxButton(panel, ID_TESTMOVEMENT, "Test Movement of the Stages (Better to check before Acquisition)", wxPoint(250, 145), wxDefaultSize, wxALIGN_LEFT);

	return panel;
}

wxPanel* CreatePage5(wxNotebook* parent) {
	wxPanel* panel = new wxPanel(parent, ID_PAGE4);

	wxStaticText* acqtwo1 = new wxStaticText(panel, wxID_ANY, wxT("Decimation: "), wxPoint(25, 25), wxDefaultSize, wxALIGN_LEFT);
	wxChoice* inputD = new wxChoice(panel, ID_DECIMATION2, wxPoint(115, 22), wxSize(65, -1), decimations);
	inputD->Select(3);
	wxStaticText* acqtwo2 = new wxStaticText(panel, wxID_ANY, wxT("Count1: "), wxPoint(205, 25), wxDefaultSize, wxALIGN_LEFT);
	wxSpinCtrl* inputC1 = new wxSpinCtrl(panel, ID_COUNT1, "", wxPoint(295, 22), wxSize(65, -1), 0, 0, 2000, functions->count1);
	wxStaticText* acqtwo3 = new wxStaticText(panel, wxID_ANY, wxT("Count2: "), wxPoint(385, 25), wxDefaultSize, wxALIGN_LEFT);
	wxSpinCtrl* inputC2 = new wxSpinCtrl(panel, ID_COUNT2, "", wxPoint(465, 22), wxSize(65, -1), 0, 0, 2000, functions->count2);

	wxStaticText* acqtwo4 = new wxStaticText(panel, wxID_ANY, wxT("Distance: "), wxPoint(25, 65), wxDefaultSize, wxALIGN_LEFT);
	wxSpinCtrl* inputDI = new wxSpinCtrl(panel, ID_DISTANCE, "", wxPoint(115, 62), wxSize(65, -1), 0, 0, 2000, functions->distance);
	wxStaticText* acqtwo5 = new wxStaticText(panel, wxID_ANY, wxT("TimesX: "), wxPoint(205, 65), wxDefaultSize, wxALIGN_LEFT);
	wxSpinCtrl* inputTX = new wxSpinCtrl(panel, ID_TIMESX, "", wxPoint(295, 62), wxSize(65, -1), 0, 0, 200, functions->timesx);
	wxStaticText* acqtwo6 = new wxStaticText(panel, wxID_ANY, wxT("TimesY: "), wxPoint(385, 65), wxDefaultSize, wxALIGN_LEFT);
	wxSpinCtrl* inputTY = new wxSpinCtrl(panel, ID_TIMESY, "", wxPoint(465, 62), wxSize(65, -1), 0, 0, 200, functions->timesy);

	wxStaticText* acqtwo7 = new wxStaticText(panel, wxID_ANY, wxT("SleepTimeS: "), wxPoint(25, 105), wxDefaultSize, wxALIGN_LEFT);
	wxSpinCtrl* inputSS = new wxSpinCtrl(panel, ID_SLEEPS, "", wxPoint(115, 102), wxSize(65, -1), 0, 0, 2000, functions->sleeps);
	wxStaticText* acqtwo8 = new wxStaticText(panel, wxID_ANY, wxT("SleepTimeR: "), wxPoint(205, 105), wxDefaultSize, wxALIGN_LEFT);
	wxSpinCtrl* inputSR = new wxSpinCtrl(panel, ID_SLEEPR, "", wxPoint(295, 102), wxSize(65, -1), 0, 0, 2000, functions->sleepr);
	wxStaticText* acqtwo9 = new wxStaticText(panel, wxID_ANY, wxT("SleepTime0: "), wxPoint(385, 105), wxDefaultSize, wxALIGN_LEFT);
	wxSpinCtrl* inputS0 = new wxSpinCtrl(panel, ID_SLEEP0, "", wxPoint(465, 102), wxSize(65, -1), 0, 0, 100000, functions->sleep0);

	wxButton* acqone = new wxButton(panel, ID_ACQUISITION_TWO, "Start Two Inputs Acquisition!", wxPoint(25, 145), wxDefaultSize, wxALIGN_LEFT);
	wxButton* testm = new wxButton(panel, ID_TESTMOVEMENT, "Test Movement of the Stages (Better to check before Acquisition)", wxPoint(250, 145), wxDefaultSize, wxALIGN_LEFT);

	return panel;
}