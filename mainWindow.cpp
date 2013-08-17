/*
 
mainWindow.cpp

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
#include "mainWindow.h"
#include "BookTab.h"
#include "TimeInfo.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "ErrorBox.h"
#include "Appointment.h"
#include "DatesWindow.h"
#include "PrefWin.h"
#include <support/Beep.h>
#include <Entry.h>
#include <Path.h>


mainWindow::mainWindow(BRect frame, const char *title, float width, float height)
		: BWindow(frame, title, B_TITLED_WINDOW, B_NOT_ZOOMABLE | B_NOT_RESIZABLE) {
		
//	app_info	ai;
//	if (be_app->GetAppInfo(&ai)==B_OK) {
//		BEntry	*entry=new BEntry(&ai.ref);
//		BPath	*path=new BPath();
//		BString	*str=new BString("");
//		
//		entry->GetPath(path);
//		str->SetTo(path->Path());
//		str->RemoveLast(path->Leaf());
//		str->Append("dates.abook");
		
		mw_datesfolder=strdup("/boot/home/config/etc/dates.abook");
//		printf("path=%s\n", mw_datesfolder);
//		delete path;
//		delete entry;
//	}


	// main view
	mainV=new MainView(this->Bounds());
	AddChild(mainV);
	
	// time line
	timeTL=new TimeLine(BRect(0, this->Bounds().bottom-16, this->Bounds().right, this->Bounds().bottom));
	AddChild(timeTL);
	
	// calendar view
	BRect	tabframe(1,20,Bounds().right-1, Bounds().bottom-20-B_H_SCROLL_BAR_HEIGHT);
	calendarTV=new BTabView(tabframe, "calendarTV", (width<80) ? B_WIDTH_FROM_LABEL : B_WIDTH_FROM_WIDEST, B_FOLLOW_NONE);
	calendarTV->SetFontSize(10);
	mainV->AddChild(calendarTV);
	
	tabframe=calendarTV->Bounds();
	//tabframe.top+=calendarTV->TabHeight()-10;
	tabframe.left=0;
	
	list=new BList();
	LoadAppointments();
	
	yearSB=new MyScrollBar(BRect(1,Bounds().bottom-timeTL->Bounds().Height()-B_H_SCROLL_BAR_HEIGHT-2, Bounds().right-1, Bounds().bottom-timeTL->Bounds().Height()-2), "yearSB", NULL, 2000.0, 2100.0, B_HORIZONTAL);
	yearSB->SetSteps(1.0, 1.0);
	yearSB->SetValue(GetYear());
	mainV->AddChild(yearSB);

	for (int i=1; i<13;i++)
		calendarTV->AddTab(new BookTab(tabframe,width, height, i,GetYear(), GetDOW(1,i,GetYear()), list));

	int i=-1;
	while (calendarTV->TabAt(++i)!=NULL)
		calendarTV->TabAt(i)->SetLabel((width<80) ? monthname_shortest[i] : monthname_short[i]);
	
	for (int i=1; i<13; i++)
	if (FindDay(1, i)) {
		FindDay(1, i)->DeselectNext();
		FindDay(1, i)->DeselectPrevious();
	}

	calendarTV->Select(GetMonth());
	MarkDay(GetDay(), GetMonth()+1);
	SelectDay(GetDay(), GetMonth()+1);
	
	
	
	// MENU BAR
	
	BMenuBar 	*mainMB=new BMenuBar(BRect(0,0,Bounds().right, 14), "mainMB");
	BMenu		*fileM=new BMenu("File");
	BMenuItem	*aboutMI=new BMenuItem("About...", new BMessage(B_ABOUT_REQUESTED), 'A');
	BMenuItem	*prefsMI=new BMenuItem("Preferences...", new BMessage(SHOW_PREFS_MSG), 'P');
	BMenuItem	*exitMI=new BMenuItem("Exit", new BMessage(B_QUIT_REQUESTED), 'Q');
	BMenu		*showM=new BMenu("Show");
	BMessage	*msg=new BMessage(SHOW_DATES_MSG);	
	msg->AddInt8("which",0);
	BMenuItem	*thisdayM=new BMenuItem("This Day's Dates", msg);
	msg=new BMessage(SHOW_DATES_MSG);
	msg->AddInt8("which",1);
	BMenuItem	*thismonthM=new BMenuItem("This Month's Dates", msg);
	msg=new BMessage(SHOW_DATES_MSG);
	msg->AddInt8("which",2);
	BMenuItem	*thisyearM=new BMenuItem("This Year's Dates", msg);
	msg=new BMessage(SHOW_DATES_MSG);
	msg->AddInt8("which",3);
	BMenuItem	*allM=new BMenuItem("All Dates", msg);

	
	mainMB->AddItem(fileM);
	fileM->AddItem(aboutMI);
	fileM->AddItem(prefsMI);
	fileM->AddSeparatorItem();
	fileM->AddItem(exitMI);
	
	mainMB->AddItem(showM);
	showM->AddItem(thisdayM);
	showM->AddItem(thismonthM);
	showM->AddItem(thisyearM);
	showM->AddItem(allM);
	
	mainV->AddChild(mainMB);
	mainMB->GetPreferredSize(&mw_menuwidth, &mw_menuheight);
	calendarTV->MoveTo(1,mw_menuheight+2);
	
	fileM->SetTargetForItems(be_app_messenger);
	
	SetPulseRate(250000);
	
	checkerT=-1;
}

mainWindow::~mainWindow() {
	delete list;
}

void	mainWindow::SetBeep(bool state) {
	beepAtMsg=state;
}

void	mainWindow::ResizeAllToFit() {
	
	
	
	calendarTV->ResizeTo(((BookTab *)calendarTV->TabAt(0)->View())->Bounds().Width(), ((BookTab *)calendarTV->TabAt(0)->View())->Bounds().Height()+calendarTV->TabHeight());
	mainV->ResizeTo(calendarTV->Bounds().Width()+2, calendarTV->Bounds().Height()+mw_menuheight+timeTL->Bounds().Height()+B_H_SCROLL_BAR_HEIGHT+6);
	ResizeTo(mainV->Bounds().Width(), mainV->Bounds().Height());
}

void mainWindow::MessageReceived(BMessage *msg) {
	switch(msg->what) {
		case SHOW_DATES_MSG: {
			BList	*a_list;
			int8	which;
			BString	s;
			msg->FindInt8("which", &which);
			switch(which) {
				case 0: {
					a_list=CheckForDateThisDay(GetDay(), GetMonth()+1, GetYear());
					s="This Day's Dates";
					break;
				}
				case 1: {
					a_list=CheckForDateThisMonth(GetMonth()+1, GetYear());
					s="This Month's Dates";
					break;
				}
				case 2: {
					a_list=CheckForDateThisYear(GetYear());
					s="This Year's Dates";
					break;
				}
				case 3: {
					a_list=new BList(*list);
					s="All Dates";
					break;
				}
				default: {
					break;
				}
			}
			datesWindow	*dw=new datesWindow(BRect(100,100,550,450), a_list, s.String());
			dw->Go();
			delete a_list;
			break;
		}
	
		case VALUECHANGED: {
			BTab	*tab;
			
			Lock();
			BeginViewTransaction();
			
			for (int i=0; (tab=calendarTV->TabAt(i))!=NULL; i++) {
				((BookTab *)tab->View())->SetYear(((int)yearSB->Value()));
			}
			
			if (((int)yearSB->Value())==GetYear()) MarkDay(GetDay(), GetMonth()+1);
			
			EndViewTransaction();
			Unlock();
			break;
		}
		default: {
			BWindow::MessageReceived(msg);
			break;
		}
	}
}

bool mainWindow::QuitRequested() {
	SaveAppointments();
	be_app->PostMessage(B_QUIT_REQUESTED);
	return false;
}

DayView 	*mainWindow::FindDay(int day, int month) {
	BookTab	*bt;
	for (int m=1; m<13; m++) {
		if ( (bt=((BookTab *)calendarTV->TabAt(m-1)->View()) )->GetMonth()==month) {
			return bt->FindDay(day);
		}
	}
	return NULL;
}

void	mainWindow::ShowTodaysDates() {

	if (FindDay(GetDay(), GetMonth()+1)) {
		FindDay(GetDay(), GetMonth()+1)->EditApps();
	}
//	return;

//	Appointment	**a=CheckForDateThisDay(GetDay(),GetMonth()+1,GetYear());
//	int32		c=0;
//	BString		s="";
//	
//	while (a[c]!=NULL) {
//		s << ((a[c]->GetHour()<10) ? ("0") : ("")) << a[c]->GetHour() << ":" << ((a[c]->GetMinute()<10) ? ("0") : ("")) << a[c]->GetMinute() << "  " << a[c]->GetNote() << "\n";
//		c++;
//	}
//	
//	ErrorBox	*eb=new ErrorBox(E_GREEN_COLOR, "Dates for Today", s.String(), "OK");
//	eb->Go();
//	
//	free(a);
}


void	mainWindow::SelectDay(int day, int month) {
	if (FindDay(day, month)) {
		FindDay(day, month)->DeselectNext();
		FindDay(day, month)->DeselectPrevious();
		FindDay(day, month)->Select(true);
	}
}

void	mainWindow::MarkDay(int day, int month) {
	if (FindDay(day, month)) {
		FindDay(day, month)->UnmarkNext();
		FindDay(day, month)->UnmarkPrevious();
		FindDay(day, month)->Mark(true);
	}
}


void	mainWindow::AddToMessage(Appointment *a, BMessage *msg) {
	msg->AddInt16("day", a->GetDay());
	msg->AddInt16("month", a->GetMonth());
	msg->AddInt16("year", a->GetYear());
	msg->AddInt16("hour", a->GetHour());
	msg->AddInt16("minute", a->GetMinute());
	msg->AddString("note", a->GetNote());
}

Appointment	*mainWindow::ReadFromMessage(BMessage *msg, int32 index) {
	int16	day, month, year, hour, minute;
	BString	s;
	
	msg->FindInt16("day", index, &day);
	msg->FindInt16("month", index, &month);
	msg->FindInt16("year", index, &year);
	msg->FindInt16("hour", index, &hour);
	msg->FindInt16("minute", index, &minute);
	msg->FindString("note", index, &s);
	
	return new Appointment(day, month, year, hour, minute, s.String());
}

void	mainWindow::SaveAppointments() {
	Appointment	*a;
	BMessage	*msg=new BMessage();
	BFile		*file=new BFile(mw_datesfolder, B_READ_WRITE | B_CREATE_FILE | B_ERASE_FILE);

	if (file->InitCheck()==B_OK) {
		for (int32 i=0; i<list->CountItems(); i++) {
			a=((Appointment *)list->ItemAt(i));
			AddToMessage(a, msg);
		}
		msg->Flatten(file);
	} else {
 		ErrorBox	*eb=new ErrorBox(E_RED_COLOR, "File Error","The file where your dates are to be saved could not be created. Be sure that your home directory and the file \"dates.abook\" are not write-protected.", "OK");
		eb->Go();
	}
	file->Unset();
}

void	mainWindow::LoadAppointments() {
	BMessage	*msg=new BMessage();
	BFile		*file=new BFile(mw_datesfolder, B_READ_ONLY);
	int16		in;
	
	if (file->InitCheck()==B_OK) {
		if (msg->Unflatten(file)==B_OK)
			for (int32 i=0; msg->FindInt16("day", i, &in)==B_OK; i++) {
				list->AddItem(ReadFromMessage(msg, i));
			}
	}
	file->Unset();
	delete msg;
	
}

void	mainWindow::StartCheckerThread() {
	if (checkerT<0) {
		threadmsgM=new BMessage();
		threadmsgM->AddPointer("mainWindow", this);
		threadmsgM->AddPointer("beep", &beepAtMsg);
		checkerT=spawn_thread(CheckThread, "Date Checker", 2, threadmsgM);
		resume_thread(checkerT);
	}
}

void	mainWindow::StopCheckerThread() {
	if (checkerT>0) {
		kill_thread(checkerT);
		delete threadmsgM;
		checkerT=-1;
	}
}

void	mainWindow::SetMinutesBefore(int32 minutes) {
	if (minutes<0) minutes=0;
	if (minutes>360) minutes=360;
	mw_minutesbefore=minutes;
}

int32	mainWindow::GetMinutesBefore() {
	return mw_minutesbefore;
}

int32	CheckThread(void *data) {
	
	int	last_min=GetMinute();
	int	min;
	BMessage	*msg=(BMessage *)data;
	BList 	*a;
	mainWindow	*mainW;
	bool	*beepB;
	int 	day, month, year, hour, minute;
	Appointment	*ap;
	
	msg->FindPointer("mainWindow",(void **)(&mainW));
	msg->FindPointer("beep", (void **)(&beepB));
	
	while (true) {
		if (last_min!=(min=GetMinute())) {
			last_min=min;
			mainW->Lock();
			
			day=GetDay();
			month=GetMonth()+1;
			year=GetYear();
			hour=GetHour();
			minute=min;
			
		if (mainW->GetMinutesBefore()>0)
				AddMinutesToDate(day, month, year, hour, minute, mainW->GetMinutesBefore());
			printf("Checking for date on %d.%d.%d at %d:%d\n", day, month, year, hour, minute);
			a=mainW->CheckForDateOn(day, month, year, hour, minute);
			mainW->Unlock();
			// there is a date now!
			if (a->CountItems()>0) {
				BString	s="", note="";
				
				
				for (int c=0; c < a->CountItems(); c++) {
					ap=(Appointment *)a->ItemAt(c);
					note=ap->GetNote();
					if (note.Length()>44) note.Remove(44,note.Length()-44).Append(B_UTF8_ELLIPSIS);
					s <<  ap->GetHour() << ":" << ap->GetMinute() << "  " << note.String() << "\n";
					
					// for every date show its own ErrorBox
					ErrorBox	*eb=new ErrorBox(E_YELLOW_COLOR, "You Have a Date!", s.String(), "OK");
					if (*beepB) {
						beep();
					}
					eb->Go();
				}
				
			}
			delete a;
		}
		snooze(1000000);
	}	
}




BList	*mainWindow::CheckForDateThisMinute() {
	Appointment	*a;
	BList		*a_list=new BList();
	BString s;
		
	for (int32 i=0; i<list->CountItems(); i++) {
		a=((Appointment *)list->ItemAt(i));
		if ( (a->GetDay()==GetDay()) && (a->GetMonth()==GetMonth()) && (a->GetYear()==GetYear()) && (a->GetMinute()==GetMinute()) && (a->GetHour()==GetHour()))
			a_list->AddItem(a);
	}
	return a_list;
}

BList	*mainWindow::CheckForDateOn(int day, int month, int year, int hour, int minute) {
	Appointment	*a;
	BList		*a_list=new BList();
	BString s;
		
	for (int32 i=0; i<list->CountItems(); i++) {
		a=((Appointment *)list->ItemAt(i));
		if ( (a->GetDay()==day) && (a->GetMonth()+1==month) && (a->GetYear()==year) && (a->GetMinute()==minute) && (a->GetHour()==hour))
			a_list->AddItem(a);
	}
	return a_list;
}


BList	*mainWindow::CheckForDateThisHour() {
	Appointment	*a;
	BList		*a_list=new BList();
	BString s;
		
	for (int32 i=0; i<list->CountItems(); i++) {
		a=((Appointment *)list->ItemAt(i));
		if ( (a->GetDay()==GetDay()) && (a->GetMonth()==GetMonth()) && (a->GetYear()==GetYear()) && (a->GetHour()==GetHour()))
			a_list->AddItem(a);
	}
	return a_list;
}

BList	*mainWindow::CheckForDateThisDay(int day, int month, int year) {
	Appointment	*a;
	BList		*a_list=new BList();
	BString s;
		
	for (int32 i=0; i<list->CountItems(); i++) {
		a=((Appointment *)list->ItemAt(i));
		if ( (a->GetDay()==day) && (a->GetMonth()==month-1) && (a->GetYear()==year))
			a_list->AddItem(a);
	}
	return a_list;
}


BList	*mainWindow::CheckForDateThisMonth(int month, int year) {
	Appointment	*a;
	BList		*a_list=new BList();
	BString s;
		
	for (int32 i=0; i<list->CountItems(); i++) {
		a=((Appointment *)list->ItemAt(i));
		if ((a->GetMonth()==month-1) && (a->GetYear()==year))
			a_list->AddItem(a);
	}
	return a_list;
}

BList	*mainWindow::CheckForDateThisYear(int year) {
	Appointment	*a;
	BList		*a_list=new BList();
	BString s;
		
	for (int32 i=0; i<list->CountItems(); i++) {
		if  ( (a=((Appointment *)list->ItemAt(i)))->GetYear()==year)
			a_list->AddItem(a);
	}
	return a_list;
}

void	mainWindow::PrintDatesOfDay() {
}

