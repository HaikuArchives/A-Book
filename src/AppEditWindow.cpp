/*
 
AppEditWindow.cpp

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
#include "AppEditWindow.h"
#include "TimeInfo.h"
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "ErrorBox.h"
#include <BeBuild.h>

#define ADD_MSG		'AW00'
#define MODIFY_MSG	'AW01'
#define DELETE_MSG	'AW02'
#define SELECTION_MSG	'AW03'

AppEditWindow::AppEditWindow(BRect frame, int dow, int day, int month, int year, const char *title, BList *applist, DayView *parent) : BWindow(frame, title, B_FLOATING_WINDOW_LOOK, B_NORMAL_WINDOW_FEEL, B_NOT_ZOOMABLE | B_NOT_RESIZABLE) {
	
	a_list=applist;
	d=day;
	m=month;
	y=year;
	parentDV=parent;
	
	mainV=new BView(Bounds(), "mainV", B_FOLLOW_ALL, B_WILL_DRAW);
	mainV->SetViewColor(216, 216, 216);
	AddChild(mainV);
	
	BString s="";
	s << dayofweek[dow] << ", " << monthname[month] << " " << day << ", "<<year;
	SetTitle(s.String());
	
	// hour field
	hourTC=new BTextControl(BRect(5,15,55,35), "hourTC", "Time:", "", NULL);
	hourTC->SetDivider(hourTC->StringWidth("Time:")+2);
	hourTC->SetAlignment(B_ALIGN_LEFT, B_ALIGN_RIGHT);
	// minute field
	minuteTC=new BTextControl(BRect(56,15, 85, 35), "minuteTC", ":", "", NULL);
	minuteTC->SetDivider(minuteTC->StringWidth(":")+2);
	
	// text field
	textTC=new BTextControl(BRect(86, 15,Bounds().right-5,35), "textTC", "Note:", "", NULL);
	textTC->SetDivider(textTC->StringWidth("Text:")+2);
	
	mainV->AddChild(hourTC);
	mainV->AddChild(minuteTC);
	mainV->AddChild(textTC);
	
	float	width, height;
	
	// add button
	addB=new BButton(BRect(0,0,1,1), "addB", "Add", new BMessage(ADD_MSG), B_FOLLOW_RIGHT | B_FOLLOW_TOP);
	addB->MakeDefault(true);
	addB->GetPreferredSize(&width, &height);
	addB->ResizeTo(width, height);
	addB->MoveTo(Bounds().Width()-5-width, 38);
	mainV->AddChild(addB);

	modifyB=new BButton(BRect(0,0,1,1), "modifyB", "Modify", new BMessage(MODIFY_MSG), B_FOLLOW_TOP | B_FOLLOW_RIGHT);
	modifyB->GetPreferredSize(&width, &height);
	modifyB->ResizeTo(width, height);
	modifyB->MoveTo((addB->Frame().left-7)-width, 38+(addB->Bounds().Height()-height)/2 );
	modifyB->SetEnabled(false);
	mainV->AddChild(modifyB);
	
	deleteB=new BButton(BRect(0,0,1,1), "deleteB", "Remove", new BMessage(DELETE_MSG), B_FOLLOW_RIGHT | B_FOLLOW_TOP);
	deleteB->GetPreferredSize(&width, &height);
	deleteB->ResizeTo(width, height);
	deleteB->MoveTo(Bounds().Width()-5-width, (Bounds().bottom-40)+(40-height)/2);
	deleteB->SetEnabled(false);
	mainV->AddChild(deleteB);
	
	
	applistLV=new BListView(BRect(5, addB->Frame().bottom+7, Bounds().right-5-B_V_SCROLL_BAR_WIDTH, Bounds().bottom-40-B_H_SCROLL_BAR_HEIGHT), "applistLV", B_SINGLE_SELECTION_LIST, B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP_BOTTOM);
	applistLV->SetSelectionMessage(new BMessage(SELECTION_MSG));
	applistLV->SetTarget(this);
	applistSV=new BScrollView("applistSV", applistLV, B_FOLLOW_ALL, 0, true, true);
	mainV->AddChild(applistSV);
	
	AddAppsOfTheDay();
	applistLV->SortItems((int (*)(const void * , const void *))compfunc);
}





AppEditWindow::~AppEditWindow() {
}


bool 	AppEditWindow::QuitRequested() {
	Quit();
	return false;
}


int	GetHourOfItem(BStringItem *si) {
	BString s=si->Text();
	s.Remove(s.FindFirst(":"), s.Length()-s.FindFirst(":"));
	return atoi(s.String());
}

int	GetMinuteOfItem(BStringItem *si) {
	BString s=si->Text();
	
	s.Remove(0, s.FindFirst(":")+1);
	s.Remove(s.FindFirst(" "), s.Length()-s.FindFirst(" "));
	return atoi(s.String());
}

const char *AppEditWindow::GetNoteOfItem(BStringItem *si) {
	BString s=si->Text();
	
	s.Remove(0, s.FindFirst("  ")+2);
	return strdup(s.String());	
}


void	AppEditWindow::AddAppsOfTheDay() {
	Appointment	*a;
	BStringItem	*si;
	BString s;
	float	time;
		
	for (int32 i=0; i<a_list->CountItems(); i++) {
		a=(Appointment *)a_list->ItemAt(i);
		if ( (a->GetDay()==d) && (a->GetMonth()==m) && (a->GetYear()==y)) {
			s="";
			s << ((a->GetHour()<10) ? ("0") : ("")) << a->GetHour() << ":" << ((a->GetMinute()<10) ? ("0") : ("")) << a->GetMinute() << "  " << a->GetNote();
			time=(float)a->GetHour()+((float)a->GetMinute()/60.0);
			si=new BStringItem(s.String());
			applistLV->AddItem(si);
		}
	}
}

int32	AppEditWindow::GetIndexOf(int day, int month, int year, int hour, int minute, const char *text) {
	Appointment	*a;
	
	for (int32 i=0; i<a_list->CountItems(); i++) {
		a=(Appointment *)a_list->ItemAt(i);
		if ( (a->GetDay()==day) && (a->GetMonth()==month) && (a->GetYear()==year) && (a->GetHour()==hour) && (a->GetMinute()==minute) && (strcmp(a->GetNote(), text)==0)  ) {
			return i;
		}
	}
	
	return -1;
}

int AppEditWindow::compfunc(const void *a,  const void *b) {

	float 	h1=(float)GetHourOfItem(*((BStringItem **)(a))),
		h2=(float)GetHourOfItem(*((BStringItem **)(b))),
		m1=(float)GetMinuteOfItem(*((BStringItem **)(a))),
		m2=(float)GetMinuteOfItem(*((BStringItem **)(b)));


	if ((h1+(m1/60.0))<(h2+(m2/60.0))) return -1; else
	if ((h1+(m1/60.0))>(h2+(m2/60.0))) return 1;
	return 0;
}

void	AppEditWindow::MessageReceived(BMessage *msg) {
	switch(msg->what) {	
		case ADD_MSG: {
			if ((atoi(hourTC->Text())<0) || (atoi(hourTC->Text())>23) || (atoi(minuteTC->Text())<0) || (atoi(minuteTC->Text())>59) || (strlen(textTC->Text())==0)) {
				// Please enter a valid value for minute and hour!
				ErrorBox	*eb=new ErrorBox(E_RED_COLOR, "Wrong Format","Either you entered no time value or no number at all or you left empty the \"Note\" field.", "OK");
				eb->Go();
				break;
			}
			// ERRORWINDOW!!!
			
			BString s="";
			a_list->AddItem(new Appointment(d, m, y, atoi(hourTC->Text()), atoi(minuteTC->Text()), textTC->Text()));
			s << ((atoi(hourTC->Text())<10) ? ("0") : ("")) << atoi(hourTC->Text()) << ":" << ((atoi(minuteTC->Text())<10) ? ("0") : ("")) << atoi(minuteTC->Text()) << "  " << textTC->Text();
			applistLV->AddItem(new BStringItem(s.String()));
			hourTC->SetText("");
			minuteTC->SetText("");
			textTC->SetText("");
			
			applistLV->SortItems((int (*)(const void * , const void *))compfunc);
//			applistLV->Invalidate();
			parentDV->Window()->Lock();
			parentDV->CountDates();
			parentDV->Window()->Unlock();
			
			break;
		}
		
		case MODIFY_MSG: {
			if ((atoi(hourTC->Text())<0) || (atoi(hourTC->Text())>23) || (atoi(minuteTC->Text())<0) || (atoi(minuteTC->Text())>59) || (strlen(textTC->Text())==0)) {
				ErrorBox	*eb=new ErrorBox(E_RED_COLOR, "Wrong Format","Either you entered no time value or no number at all or you left empty the \"Note\" field.", "OK");
				eb->Go();
				break;
			}
			// ERRORWINDOW!!!
			if (applistLV->CurrentSelection()<0) {
				ErrorBox	*eb=new ErrorBox(E_RED_COLOR, "No Date Selected","First, you should select the date which you want to change.", "OK");
				eb->Go();
				break;
			}
			// ERRORWINDOW!!!
			
			BString s="";
			int32 index=GetIndexOf(d,m,y,GetHourOfItem(((BStringItem *)applistLV->ItemAt(applistLV->CurrentSelection()))), GetMinuteOfItem(((BStringItem *)applistLV->ItemAt(applistLV->CurrentSelection()))), GetNoteOfItem(((BStringItem *)applistLV->ItemAt(applistLV->CurrentSelection()))));
			((Appointment *)a_list->ItemAt(index))->SetHour(atoi(hourTC->Text()));
			((Appointment *)a_list->ItemAt(index))->SetMinute(atoi(minuteTC->Text()));
			((Appointment *)a_list->ItemAt(index))->SetNote(textTC->Text());

			s << ((atoi(hourTC->Text())<10) ? ("0") : ("")) << atoi(hourTC->Text()) << ":" << ((atoi(minuteTC->Text())<10) ? ("0") : ("")) << atoi(minuteTC->Text()) << "  " << textTC->Text();
			
			((BStringItem *)applistLV->ItemAt(applistLV->CurrentSelection()))->SetText(s.String());
			applistLV->InvalidateItem(applistLV->CurrentSelection());
			applistLV->SortItems((int (*)(const void * , const void *))compfunc);
			hourTC->SetText("");
			minuteTC->SetText("");
			textTC->SetText("");
			break;
		}
		
		case DELETE_MSG: {
			if (applistLV->CurrentSelection()<0) {
				ErrorBox	*eb=new ErrorBox(E_RED_COLOR, "No Date Selected","You should select the date which you want to remove.", "OK");
				eb->Go();
				break;
			}
			// ERRORWINDOW!!!
			
			a_list->RemoveItem(GetIndexOf(d,m,y,GetHourOfItem(((BStringItem *)applistLV->ItemAt(applistLV->CurrentSelection()))), GetMinuteOfItem(((BStringItem *)applistLV->ItemAt(applistLV->CurrentSelection()))), GetNoteOfItem(((BStringItem *)applistLV->ItemAt(applistLV->CurrentSelection())))));
			applistLV->RemoveItem(applistLV->CurrentSelection());
		
			hourTC->SetText("");
			minuteTC->SetText("");
			textTC->SetText("");
			parentDV->Window()->Lock();
			parentDV->CountDates();
			parentDV->Window()->Unlock();
			break;
		}
		
		case SELECTION_MSG: {
			if (applistLV->CurrentSelection()<0) {
				hourTC->SetText("");
				minuteTC->SetText("");
				textTC->SetText("");
				modifyB->SetEnabled(false);
				deleteB->SetEnabled(false);
				break;
			} else {
				BString	s="";
				s << GetHourOfItem(((BStringItem *)applistLV->ItemAt(applistLV->CurrentSelection())));
				if (s.Length()<2) s.Prepend("0");
				hourTC->SetText(s.String());
				s="";
				s << GetMinuteOfItem(((BStringItem *)applistLV->ItemAt(applistLV->CurrentSelection())));
				if (s.Length()<2) s.Prepend("0");
				minuteTC->SetText(s.String());
				textTC->SetText(GetNoteOfItem(((BStringItem *)applistLV->ItemAt(applistLV->CurrentSelection()))));
				modifyB->SetEnabled(true);
				deleteB->SetEnabled(true);
			}
			break;
		}
		
		default: {
			BWindow::MessageReceived(msg);
			break;
		}
	}
}

