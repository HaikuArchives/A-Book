/*
 
PrefWin.cpp

a-book -- a small calendar application with reminder

Copyright (C) 2002 Maurice Michalski, http://fetal.de, http://maurice-michalski.de

Permission is hereby granted, free of charge, to any person obtaining a copy of this
software and associated documentation files (the "Software"), to deal in the Software
without restriction, including without limitation the rights to use, copy, modify,
merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be included in all copies
or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE
OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 
*/
#include "PrefWin.h"
#include <Application.h>
#include <String.h>
#include "app.h"

PrefWin::PrefWin() 
		: BWindow(BRect(200,200,500,337), "Preferences", B_TITLED_WINDOW, B_NOT_ZOOMABLE | B_NOT_RESIZABLE | B_ASYNCHRONOUS_CONTROLS){
		
	mainV=new BView(Bounds(), "mainV", B_FOLLOW_ALL, B_WILL_DRAW);
	mainV->SetViewColor(216,216,216);
	AddChild(mainV);
	
	showCB=new BCheckBox(BRect(10,10,Bounds().Width()-10, 30), "showCB", "Show Dates at Program Startup", NULL, B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP);
	mainV->AddChild(showCB);

	watchCB=new BCheckBox(BRect(10,30,Bounds().Width()-10, 50), "watchCB", "Remind Me When I Have a Date", new BMessage(WATCHORNOT), B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP);
	watchCB->SetTarget(be_app_messenger);
	mainV->AddChild(watchCB);
	
	alarmSV=new BStringView(BRect(28,51,Bounds().Width()-28, 66), "alarmSV", "", B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP);
	mainV->AddChild(alarmSV);
	
	alarmS=new BSlider(BRect(30,67,Bounds().Width()-30, 88), "alarmS", NULL, new BMessage(SLIDER_MSG), 0, 72, B_BLOCK_THUMB, B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP);
	alarmS->SetModificationMessage(new BMessage(SLIDER_MSG));
	mainV->AddChild(alarmS);
	PostMessage(SLIDER_MSG);

	beepatdateCB=new BCheckBox(BRect(29,89,Bounds().Width()-29, 109), "beepatdateCB", "Beep", new BMessage(BEEPATDATE_MSG), B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP);
	beepatdateCB->SetTarget(be_app_messenger);
	mainV->AddChild(beepatdateCB);

	smallCB=new BCheckBox(BRect(10,109,Bounds().Width()-10, 129), "smallCB", "Small Window (Needs Restart)", NULL, B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP);
	mainV->AddChild(smallCB);
	
}


PrefWin::~PrefWin() {
}

bool	PrefWin::ShowsDates() {
	return showCB->Value()==B_CONTROL_ON;
}

void	PrefWin::SetShowDates(bool state) {
	showCB->SetValue(state ? B_CONTROL_ON : B_CONTROL_OFF);
}

bool	PrefWin::IsWatching() {
	return watchCB->Value()==B_CONTROL_ON;
}

void	PrefWin::SetWatching(bool state) {
	watchCB->SetValue(state ? B_CONTROL_ON : B_CONTROL_OFF);
	if (state) {
		beepatdateCB->SetEnabled(true);
		alarmS->SetEnabled(true);
	} else {
		beepatdateCB->SetEnabled(false);
		alarmS->SetEnabled(false);
	}
}

bool	PrefWin::DoesBeepAtDate() {
	return beepatdateCB->Value()==B_CONTROL_ON;
}

void	PrefWin::SetBeepAtDate(bool state) {
	beepatdateCB->SetValue(state ? B_CONTROL_ON : B_CONTROL_OFF);
}

int	PrefWin::GetEarlyRemind() {
	return alarmS->Value()*5;
}

void	PrefWin::SetEarlyRemind(int minutes) {
	alarmS->SetValue(minutes/5);
	((app *)be_app)->MainWindow()->SetMinutesBefore(minutes);
}

bool	PrefWin::IsSmallSize() {
	return smallCB->Value()==B_CONTROL_ON;
}

void	PrefWin::SetSmallSize(bool state) {
	smallCB->SetValue(state ? B_CONTROL_ON : B_CONTROL_OFF);
}

bool	PrefWin::QuitRequested() {
	Hide();
	((app *)be_app)->SaveSettings();
	return false;
}

void	PrefWin::MessageReceived(BMessage *msg) {
	switch(msg->what) {
		case SLIDER_MSG: {
			BString	s="";
			s << alarmS->Value()*5 << " Minutes Before";
			SetEarlyRemind(alarmS->Value()*5);
			alarmSV->SetText(s.String());
			break;
		}
		
		default: {
			BWindow::MessageReceived(msg);
			break;
		}
	}
}


