#include "MyFunctions.h"

MyFunctions::MyFunctions() {
	ssh = new CkSsh();

	move1 = 2000;
	move2 = 2000;
	currentP1 = 0;
	currentP2 = 0;

	decimation1 = 32;
	decimation2 = 128;
	directory = "/Data/";
	filename1 = "1.bin";
	filename2 = "2.bin";
	count1 = 100;
	count2 = 100;
	distance = 50;
	timesx = 30;
	timesy = 30;
	sleeps = 500;
	sleepr = 150;
	sleep0 = 3000;
}



/*---------------------------------------Buttons Page1---------------------------------------*/



bool MyFunctions::ConnectModbus(wxString outputstring) {
	outputstring = "----------Connecting NiMotion Mudbus:----------";
	
	int nAddr = 1;
	int addrs[20];
	int nCount = 0;
	int rc = 0;
	int PositionX, PositionY;

	wxLogStatus(wxT("Wait until connection finish - 2 seconds"));

	rc = NiM_openDevice(0, "{\"DeviceName\": \"COM3\", \"Baudrate\" : 115200, \"Parity\" : \"None\", \"DataBits\" : 8, \"StopBits\" : 1}");
	if (rc != 0) {
		outputstring << "Open device faild" << "\r\n";
		return false;
	}

	rc = NiM_scanMotors(1, 10);
	rc = NiM_getOnlineMotors(addrs, &nCount);
	for (int i = 0; i < nCount; i++)
		outputstring << "\r\nThe Motor " << addrs[i] << " is online";

	rc = NiM_getCurrentPosition(1, &PositionY);
	rc = NiM_getCurrentPosition(2, &PositionX);

	outputstring << "\r\nCurrent position of Y " << PositionY;
	outputstring << "\r\nCurrent position of X " << PositionX;

	if (abs(PositionY) > 10 || abs(PositionX) > 10) {
		rc = NiM_powerOn(1);
		rc = NiM_powerOn(2);
		rc = NiM_moveAbsolute(1, 0);
		rc = NiM_moveAbsolute(2, 0);
		outputstring << "************************************************" << "\r\n";
		outputstring << "Error! The program quited abnormally last time!" << "\r\n";
		outputstring << "We already helped you to move the Modbuses to absolute 0 last time you saved." << "\r\n";
		outputstring << "You can choose to continue working using the position now or adjust your Modbuses manually" << "\r\n";
	}

	for (nAddr = 1; nAddr < 3; nAddr++) {
		rc = NiM_changeWorkMode(nAddr, POSITION_MODE);
		rc = NiM_powerOff(nAddr); //The set of 'home' and 'zero' must under the poweroff situation.
		rc = NiM_saveAsHome(nAddr);
		rc = NiM_saveAsZero(nAddr);
		rc = NiM_powerOn(nAddr);
	}

	wxLogStatus(wxT("Connection finished"));
	wxMessageBox(outputstring);
	return true;
}

bool MyFunctions::DisConnectModbus(wxString outputstring) {
	outputstring = "";
	int PositionY, PositionX;

	NiM_getCurrentPosition(1, &PositionY);
	NiM_getCurrentPosition(2, &PositionX);
	if (abs(PositionY) > 10 || abs(PositionX) > 10) {
		NiM_powerOn(1);
		NiM_powerOn(2);
		NiM_moveAbsolute(1, 0);
		wxLogStatus(wxT("Wait until Movement to absolute zero finish - 4 seconds."));
		Sleep(2000);
		NiM_moveAbsolute(2, 0);
		Sleep(2000);
		wxLogStatus(wxT("Movement finished"));
	}

	NiM_powerOff(1);
	NiM_saveAsHome(1);
	NiM_saveAsZero(1);
	NiM_powerOff(2);
	NiM_saveAsHome(2);
	NiM_saveAsZero(2);

	NiM_closeDevice();
	return true;
}

void MyFunctions::ChilkatUnlock(wxString outputstring) {
	CkGlobal glob;
	bool success = glob.UnlockBundle("DUveVK.CB10723_dYu1eQdq50kP");
	if (success != true) {
		outputstring << glob.lastErrorText() << "\r\n";
		return;
	}

	int status = glob.get_UnlockStatus();
	if (status == 2)
		outputstring << "Unlocked using purchased unlock code." << "\r\n";
	else
		outputstring << "Unlocked in trial mode." << "\r\n";

	outputstring << glob.lastErrorText() << "\r\n";
}

bool MyFunctions::ConnectRedPitaya(CkSsh* ssh, wxString outputstring) {
	outputstring = "-----Connecting RedPitaya:-----";
	ChilkatUnlock(outputstring);

	int port = 22;
	bool success = ssh->Connect("169.254.154.226", port);
	if (success != true) {
		outputstring << ssh->lastErrorText() << "\r\n";
		return false;
	}

	ssh->put_ConnectTimeoutMs(5000);
	ssh->put_IdleTimeoutMs(5000);  //Wait a max of 5 seconds when reading responses.

	success = ssh->AuthenticatePw("root", "root");
	if (success != true) {
		outputstring << ssh->lastErrorText() << "\r\n";
		return false;
	}
	outputstring << "\r\nConnection finished!" << "\r\n";

	const char* strOutput = ssh->quickCommand("source ~/.bashrc", "ansi");  //Add additional library path
	if (ssh->get_LastMethodSuccess() != true) {
		outputstring << ssh->lastErrorText() << "\r\n";
		return false;
	}
	
	/*
	strOutput = ssh.quickCommand("mount /dev/mmcblk0p4 /Data", "ansi");  //Mount memories.
	if (ssh.get_LastMethodSuccess() != true) {
		std::cout << ssh.lastErrorText() << "\r\n";
		return;
	}
	std::cout << strOutput << "\r\n";
	std::cout << "---- mount /dev/mmcblk0p4 /Data ----" << "\r\n";
	*/

	outputstring << "Preparation finish!" << "\r\n\n";

	outputstring << "---- ls ----" << "\r\n";
	strOutput = ssh->quickCommand("ls", "ansi");
	if (ssh->get_LastMethodSuccess() != true) {
		outputstring << ssh->lastErrorText() << "\r\n";
		return false;
	}
	outputstring << strOutput << "\r\n";

	wxMessageBox(outputstring);
	return true;
}



/*---------------------------------------Buttons Page2---------------------------------------*/



void MyFunctions::MoveModbus1(int d) {
	NiM_changeWorkMode(1, POSITION_MODE);
	NiM_moveRelative(1, d);
}

void MyFunctions::MoveModbus2(int d) {
	NiM_changeWorkMode(2, POSITION_MODE);
	NiM_moveRelative(2, d);
}

int MyFunctions::GetCurrentPosition1(wxString outputstring) {
	int p;
	NiM_getCurrentPosition(1, &p);
	return p;
}

int MyFunctions::GetCurrentPosition2(wxString outputstring) {
	int p;
	NiM_getCurrentPosition(2, &p);
	return p;
}

wxString MyFunctions::Set_Zero_Origin1(wxString outputstring) {
	NiM_powerOff(1);
	NiM_saveAsHome(1);
	NiM_saveAsZero(1);
	NiM_powerOn(1);
	outputstring = "Set current position of Modbus 1 to origin succeed!";
	return outputstring;
}

wxString MyFunctions::Set_Zero_Origin2(wxString outputstring) {
	NiM_powerOff(2);
	NiM_saveAsHome(2);
	NiM_saveAsZero(2);
	NiM_powerOn(2);	
	outputstring = "Set current position of Modbus 2 to origin succeed!";
	return outputstring;
}



/*---------------------------------------Buttons Page3---------------------------------------*/



wxString MyFunctions::RedPitayaLS(CkSsh* ssh, wxString outputstring) {
	outputstring = ssh->quickCommand("ls", "ansi");
	if (ssh->get_LastMethodSuccess() != true) {
		outputstring = ssh->lastErrorText();
	}
	return outputstring;
}

wxString MyFunctions::RedPitayaLS_L(CkSsh* ssh, wxString outputstring) {
	outputstring = ssh->quickCommand("ls -l", "ansi");
	if (ssh->get_LastMethodSuccess() != true) {
		outputstring = ssh->lastErrorText();
	}
	return outputstring;
}

wxString MyFunctions::RedPitayaCD(CkSsh* ssh, wxString outputstring) {
	outputstring = ssh->quickCommand("cd ~", "ansi");
	if (ssh->get_LastMethodSuccess() != true) {
		outputstring = ssh->lastErrorText();
	}
	return outputstring;
}

wxString MyFunctions::RedPitayaDFTH(CkSsh* ssh, wxString outputstring) {
	outputstring = ssh->quickCommand("df -Th", "ansi");
	if (ssh->get_LastMethodSuccess() != true) {
		outputstring = ssh->lastErrorText();
	}
	return outputstring;
}

wxString MyFunctions::RedPitayaRun(CkSsh* ssh, wxString textctrl, wxString outputstring) {
	outputstring = ssh->quickCommand(textctrl, "ansi");
	if (ssh->get_LastMethodSuccess() != true) {
		outputstring = ssh->lastErrorText();
	}
	return outputstring;
}

wxString MyFunctions::RedPitayaRun2(CkSsh* ssh, int Decimation, string Filename, int Count, string Directory, wxString outputstring) {
	string run = "~/AcquireContinuouslyOne ";
	run.append(to_string(Decimation));
	run.append(" ").append(Directory).append(Filename).append(" ").append(to_string(Count));
	const char* Command = run.c_str();

	outputstring = ssh->quickCommand(Command, "ansi");
	if (ssh->get_LastMethodSuccess() != true) {
		outputstring = ssh->lastErrorText();
	}
	return outputstring;
}

wxString MyFunctions::RedPitayaRun3(CkSsh* ssh, int Decimation, string filename1, int Count1, string filename2, int Count2, string Directory, wxString outputstring) {
	string run = "~/AcquireContinuouslyTwo ";
	run.append(to_string(Decimation));
	run.append(" ").append(Directory).append(filename1).append(" ").append(to_string(Count1));
	run.append(" ").append(Directory).append(filename2).append(" ").append(to_string(Count2));
	const char* Command = run.c_str();

	outputstring = ssh->quickCommand(Command, "ansi");	
	if (ssh->get_LastMethodSuccess() != true) {
		outputstring = ssh->lastErrorText();
	}
	return outputstring;
}



/*---------------------------------------Buttons Page4-5---------------------------------------*/



bool CheckProgress(int max, wxProgressDialog* dialog, int count, bool cont) {
	if (count > max) {
		dialog->Close();
		return false;
	}
	if(count == max)
		cont = dialog->Update(count, wxT("All processes run successfully!"));
	else if (count == max / 2)
		cont = dialog->Update(count, wxT("Only a half left!"));
	else
		cont = dialog->Update(count);

	if (!cont)
	{
		if (wxMessageBox(wxT("Do you really want to cancel?"), wxT("Progress dialog question"), wxYES_NO | wxICON_QUESTION) == wxYES) {
			dialog->Close();
			wxMessageBox("Progress canceled!");
			return false;
		}
	}
	return true;
}

wxString MyFunctions::AcquireDataInputOne(CkSsh* ssh, int Decimation, int Count1, int distance, int timesY, int timesX, int SleepTimeS, int SleepTimeR, int SleepTime0, int max, wxProgressDialog* dialog, wxString outputstring) {
	int nAddr = 1;
	int rc = 0;
	int PositionY = 0;
	int PositionX = 0;
	const char* strOutput;

	int count = 0;
	bool cont = true;

	outputstring = "";

	string run = "~/AcquireContinuouslyOne ";
	run.append(to_string(Decimation));

	string FileName = run; //FileName here is the string of command for RedPitaya.
	FileName.append(" /Data/").append(to_string(PositionY)).append("-").append(to_string(PositionX)).append(".bin ").append(to_string(Count1));
	const char* Command = FileName.c_str();

	for (int j = 0; j < timesY; j++) {		//J represents Y-axis. Ex: 00, 01, 02, 03, 04, 14, 13, 12, 11, 10...
		nAddr = 1;		//Move X, Bus 1 is X-axis, Bus 2 is Y-axis
		if (j % 2 == 0)
			for (int i = 0; i < timesX - 1; i++) {
				if (CheckProgress(max, dialog, count++, cont)) {
					strOutput = ssh->quickCommand(Command, "ansi");  //RedPitaya to store the data at this time.
					if (ssh->get_LastMethodSuccess() != true) {
						outputstring << ssh->lastErrorText() << "\r\n";
						return outputstring;
					}
					outputstring << strOutput;
					Sleep(SleepTimeR);

					rc = NiM_moveRelative(nAddr, distance);
					Sleep(SleepTimeS);
					rc = NiM_getCurrentPosition(nAddr, &PositionX);  //Get the latest position.

					FileName = run;
					FileName.append(" /Data/").append(to_string(PositionY)).append("-").append(to_string(PositionX)).append(".bin ").append(to_string(Count1));
					Command = FileName.c_str();
				}
				else
					break;
			}
		else
			for (int i = timesX - 1; i > 0; i--) {
				if (CheckProgress(max, dialog, count++, cont)) {
					strOutput = ssh->quickCommand(Command, "ansi");  //RedPitaya to store the data at this time.
					if (ssh->get_LastMethodSuccess() != true) {
						outputstring << ssh->lastErrorText() << "\r\n";
						return outputstring;
					}
					outputstring << strOutput;
					Sleep(SleepTimeR);

					rc = NiM_moveRelative(nAddr, -distance);
					Sleep(SleepTimeS);
					rc = NiM_getCurrentPosition(nAddr, &PositionX);  //Get the latest position.

					FileName = run;
					FileName.append(" /Data/").append(to_string(PositionY)).append("-").append(to_string(PositionX)).append(".bin ").append(to_string(Count1));
					Command = FileName.c_str();
				}
				else
					break;
			}

		if (CheckProgress(max, dialog, count++, cont)) {
			strOutput = ssh->quickCommand(Command, "ansi");  //RedPitaya to store the data at this time.
			if (ssh->get_LastMethodSuccess() != true) {
				outputstring << ssh->lastErrorText() << "\r\n";
				return outputstring;
			}
			outputstring << strOutput;
			Sleep(SleepTimeR);

			nAddr = 2;  //Move Y
			rc = NiM_moveRelative(nAddr, distance);
			Sleep(SleepTimeS);
			rc = NiM_getCurrentPosition(nAddr, &PositionY);

			FileName = run;
			FileName.append(" /Data/").append(to_string(PositionY)).append("-").append(to_string(PositionX)).append(".bin ").append(to_string(Count1));
			Command = FileName.c_str();
		}
		else
			break;
	}

	Command = NULL;
	for (nAddr = 1; nAddr < 3; nAddr++) {
		rc = NiM_moveAbsolute(nAddr, 0);
		Sleep(SleepTime0);
		rc = NiM_powerOff(nAddr); //The set of 'home' and 'zero' must under the poweroff situation.
		rc = NiM_saveAsHome(nAddr);
		rc = NiM_saveAsZero(nAddr);
		rc = NiM_powerOn(nAddr);
	}

	//NiM_closeDevice();
	outputstring << "Program succeeded!";
	return outputstring;
}

wxString MyFunctions::AcquireDataInputsTwo(CkSsh* ssh, int Decimation, int Count1, int Count2, int distance, int timesY, int timesX, int SleepTimeS, int SleepTimeR, int SleepTime0, int max, wxProgressDialog* dialog, wxString outputstring) {
	int nAddr = 1;
	int rc = 0;
	int PositionY = 0;
	int PositionX = 0;
	const char* strOutput;

	int count = 0;
	bool cont = true;
	outputstring = "";

	string run = "~/AcquireContinuouslyTwo ";
	run.append(to_string(Decimation));

	string FileName = run; //FileName here is the string of command for RedPitaya.
	FileName.append(" /Data/").append("1-").append(to_string(PositionY)).append("-").append(to_string(PositionX)).append(".bin ").append(to_string(Count1));
	FileName.append(" /Data/").append("2-").append(to_string(PositionY)).append("-").append(to_string(PositionX)).append(".bin ").append(to_string(Count2));
	const char* Command = FileName.c_str();

	for (int j = 0; j < timesY; j++) {		//J represents Y-axis. Ex: 00, 01, 02, 03, 04, 14, 13, 12, 11, 10...
		nAddr = 1;		//Move X, Bus 1 is X-axis, Bus 2 is Y-axis
		if (j % 2 == 0)
			for (int i = 0; i < timesX - 1; i++) {
				if (CheckProgress(max, dialog, count++, cont)) {
					strOutput = ssh->quickCommand(Command, "ansi");  //RedPitaya to store the data at this time.
					if (ssh->get_LastMethodSuccess() != true) {
						outputstring << ssh->lastErrorText() << "\r\n";
						return outputstring;
					}
					outputstring << strOutput;
					Sleep(SleepTimeR);

					rc = NiM_moveRelative(nAddr, distance);
					Sleep(SleepTimeS);
					rc = NiM_getCurrentPosition(nAddr, &PositionX);  //Get the latest position.

					FileName = run;
					FileName.append(" /Data/").append("1-").append(to_string(PositionY)).append("-").append(to_string(PositionX)).append(".bin ").append(to_string(Count1));
					FileName.append(" /Data/").append("2-").append(to_string(PositionY)).append("-").append(to_string(PositionX)).append(".bin ").append(to_string(Count2));
					Command = FileName.c_str();
				}
				else
					break;
			}
		else
			for (int i = timesX - 1; i > 0; i--) {
				if (CheckProgress(max, dialog, count++, cont)) {
					strOutput = ssh->quickCommand(Command, "ansi");  //RedPitaya to store the data at this time.
					if (ssh->get_LastMethodSuccess() != true) {
						outputstring << ssh->lastErrorText() << "\r\n";
						return outputstring;
					}
					outputstring << strOutput;
					Sleep(SleepTimeR);

					rc = NiM_moveRelative(nAddr, -distance);
					Sleep(SleepTimeS);
					rc = NiM_getCurrentPosition(nAddr, &PositionX);  //Get the latest position.

					FileName = run;
					FileName.append(" /Data/").append("1-").append(to_string(PositionY)).append("-").append(to_string(PositionX)).append(".bin ").append(to_string(Count1));
					FileName.append(" /Data/").append("2-").append(to_string(PositionY)).append("-").append(to_string(PositionX)).append(".bin ").append(to_string(Count2));
					Command = FileName.c_str();
				}
				else
					break;
			}

		if (CheckProgress(max, dialog, count++, cont)) {
			strOutput = ssh->quickCommand(Command, "ansi");  //RedPitaya to store the data at this time.
			if (ssh->get_LastMethodSuccess() != true) {
				outputstring << ssh->lastErrorText() << "\r\n";
				return outputstring;
			}
			outputstring << strOutput;
			Sleep(SleepTimeR);

			nAddr = 2;  //Move Y
			rc = NiM_moveRelative(nAddr, distance);
			Sleep(SleepTimeS);
			rc = NiM_getCurrentPosition(nAddr, &PositionY);

			FileName = run;
			FileName.append(" /Data/").append("1-").append(to_string(PositionY)).append("-").append(to_string(PositionX)).append(".bin ").append(to_string(Count1));
			FileName.append(" /Data/").append("2-").append(to_string(PositionY)).append("-").append(to_string(PositionX)).append(".bin ").append(to_string(Count2));
			Command = FileName.c_str();
		}
		else
			break;
	}

	Command = NULL;
	for (nAddr = 1; nAddr < 3; nAddr++) {
		outputstring << "Returning to 0" << "\r\n";
		rc = NiM_moveAbsolute(nAddr, 0);
		Sleep(SleepTime0);
		rc = NiM_powerOff(nAddr); //The set of 'home' and 'zero' must under the poweroff situation.
		rc = NiM_saveAsHome(nAddr);
		rc = NiM_saveAsZero(nAddr);
		rc = NiM_powerOn(nAddr);
	}

	//NiM_closeDevice();
	outputstring << "Program Succeeded!" << "\r\n";
	return outputstring;
}


wxString MyFunctions::TestMovement(int distance, int timesY, int timesX, int SleepTimeS, int SleepTime0, int max, wxProgressDialog* dialog, wxString outputstring) {
	int nAddr = 1;
	int rc = 0;
	int PositionY = 0;
	int PositionX = 0;

	int count = 0;
	bool cont = true;

	outputstring = "";

	for (int j = 0; j < timesY; j++) {		//J represents Y-axis. Ex: 00, 01, 02, 03, 04, 14, 13, 12, 11, 10...
		nAddr = 1;		//Move X, Bus 1 is X-axis, Bus 2 is Y-axis
		if (j % 2 == 0)
			for (int i = 0; i < timesX - 1; i++) {
				if (CheckProgress(max, dialog, count++, cont)) {
					rc = NiM_moveRelative(nAddr, distance);
					Sleep(SleepTimeS);
					rc = NiM_getCurrentPosition(nAddr, &PositionX);  //Get the latest position.
				}
				else
					break;
			}
		else
			for (int i = timesX - 1; i > 0; i--) {
				if (CheckProgress(max, dialog, count++, cont)) {
					rc = NiM_moveRelative(nAddr, -distance);
					Sleep(SleepTimeS);
					rc = NiM_getCurrentPosition(nAddr, &PositionX);  //Get the latest position.
				}
				else
					break;
			}

		if (CheckProgress(max, dialog, count++, cont)) {
			nAddr = 2;  //Move Y
			rc = NiM_moveRelative(nAddr, distance);
			Sleep(SleepTimeS);
			rc = NiM_getCurrentPosition(nAddr, &PositionY);
		}
		else
			break;
	}

	for (nAddr = 1; nAddr < 3; nAddr++) {
		rc = NiM_moveAbsolute(nAddr, 0);
		Sleep(SleepTime0);
		rc = NiM_powerOff(nAddr); //The set of 'home' and 'zero' must under the poweroff situation.
		rc = NiM_saveAsHome(nAddr);
		rc = NiM_saveAsZero(nAddr);
		rc = NiM_powerOn(nAddr);
	}

	//NiM_closeDevice();
	outputstring << "Program succeeded!";
	return outputstring;
}