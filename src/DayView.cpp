/*
 
DayView.cpp

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
#include "DayView.h"
#include <support/ClassInfo.h>
#include <String.h>
#include "BookTab.h"
#include "MainWindow.h"
#include "TimeInfo.h"
#include "AppEditWindow.h"

#define EDIT_APPS_MSG	'DV00'



DayView::DayView(BRect frame, int day, int dow, int month, int year, BList *list)
		: BView(frame, "dayview", B_FOLLOW_NONE, B_WILL_DRAW) {
	
	switch(dow) {
		case 6: SetViewColor(255,195,195);break;
		case 7: SetViewColor(255,145,145);break;
		default: SetViewColor(255,255,255);break;
	}
	
	a_list=list;
	
	DVday=day;
	DVdow=dow;
	DVmonth=month-1;
	DVyear=year;
//	if (day>0) {
//		BString		str="";
//		str << day;
//		daySV=new BStringView(BRect(5,5,Bounds().right-5, Bounds().bottom-25), "daySV", str.String(), B_FOLLOW_LEFT_RIGHT | B_FOLLOW_TOP);
//		daySV->SetFont(be_plain_font);
//		daySV->SetFontSize(38);
//		daySV->SetAlignment(B_ALIGN_CENTER);
//		if (dow==7) daySV->SetHighColor(245, 20,20);
//		if (dow==6) daySV->SetHighColor(170, 30, 30);
//		daySV->SetViewColor(255,255,255);
//		daySV->SetLowColor(255,255,255);
//		AddChild(daySV);
//	}
	selected=false;
	marked=false;
	saturday=(rgb_color){255,200,200};
	sunday=(rgb_color){255,150,150};
	selected_rgb=(rgb_color){ViewColor().red,ViewColor().green-52,ViewColor().blue-52};
	// create list of appointments
	dates=CountDates(false);
//	cmenu=new BPopUpMenu("cmenu", false, false);
//	cmenu->AddItem(new BMenuItem("Edit Appointments", new BMessage(EDIT_APPS_MSG)));
	
}

DayView::~DayView() {
}

void DayView::AttachedToWindow() {
	//Draw(Bounds());
	BView::AttachedToWindow();
}

int	DayView::GetDay() {
	return DVday;
}

void DayView::Draw(BRect updateRect) {

	DrawMark(selected);
	DrawDayMark(marked);
	DrawDate();
	DrawEntries(dates);
}

void DayView::DrawDate() {
	if (DVday>0) {
		BString	d="";
		d<<DVday;
		if (selected) SetLowColor(selected_rgb); else
				SetLowColor(ViewColor());
		switch(DVdow) {
//			case 6: SetHighColor(180,20,20);break;
//			case 7: SetHighColor(250,20,20);break;
			default: SetHighColor(0,0,0);break;
		}
		SetFontSize(Bounds().Width()/2-2);
		DrawString(d.String(), BPoint((Bounds().Width()-StringWidth(d.String()))/2, Bounds().Height()-(Bounds().Width()/2-2)/2));
		d=dayofweek_short[DVdow];
		SetFontSize(((float)Bounds().Width()/6)-1);
		DrawString(d.String(), BPoint((Bounds().Width()-StringWidth(d.String()))/2, Bounds().top+(2*((float)Bounds().Width()/6))));
	}
}

void DayView::DrawMark(bool enabled) {
	BRect frame=Bounds();

	frame.InsetBy(2,2);
	
	if (enabled) {
		SetHighColor(selected_rgb);
		FillRect(frame);
	} else {
		SetHighColor(ViewColor());
		FillRect(frame);
	}
}

void DayView::DrawEntries(int32 count) {
	if (count>0) {
		BString	d="";
		SetFontSize(Bounds().Width()/10);
		if (selected) SetLowColor(selected_rgb); else
				SetLowColor(ViewColor());

		d << count << " DATE" << ((count!=1) ? ("S") : (""));
		SetHighColor(10,10,235);
		DrawString(d.String(), BPoint((Bounds().Width()-StringWidth(d.String()))/2, Bounds().bottom-4));
	}
}

void DayView::DrawDayMark(bool enabled) {
	BRect frame=Bounds();

	if (enabled) {
		SetHighColor(255,5,5);
		StrokeRect(frame);
		frame.InsetBy(1,1);
		StrokeRect(frame);
	} else {
		SetHighColor(0,0,0);
		StrokeRect(frame);
		SetHighColor(255,255,255);
		frame.InsetBy(1,1);
		StrokeRect(frame);
	}
}

void DayView::DeselectNext() {
	if (selected)
		Select(false);
	if (NextSibling())
		if (is_instance_of(NextSibling(), DayView))
			((DayView *)NextSibling())->DeselectNext();
}

void DayView::DeselectPrevious() {
	if (selected)
		Select(false);
	if (PreviousSibling())
		if (is_instance_of(PreviousSibling(), DayView))
			((DayView *)PreviousSibling())->DeselectPrevious();
}

void DayView::Select(bool markit) {
	if (markit!=selected) {
		selected=markit;
		DrawMark(markit);
		DrawDate();
		DrawEntries(dates);
	}
}

void DayView::UnmarkNext() {
	if (marked)
		Mark(false);
	if (NextSibling())
		if (is_instance_of(NextSibling(), DayView))
			((DayView *)NextSibling())->UnmarkNext();
}

void DayView::UnmarkPrevious() {
	if (marked)
		Mark(false);
	if (PreviousSibling())
		if (is_instance_of(PreviousSibling(), DayView))
			((DayView *)PreviousSibling())->UnmarkPrevious();
}

void DayView::Mark(bool markit) {
	if (markit!=marked) {
		marked=markit;
		DrawDayMark(markit);
	}
}


void DayView::MouseDown(BPoint point) {
	if (DVday>0) {
		if (!selected) {
			BTab	*tab;
			for (int32 i=0; (tab=((mainWindow *)Window())->calendarTV->TabAt(i))!=NULL; i++) {
				((BookTab *)tab->View())->DeselectThis();
//				((DayView *)(((BookTab *)tab->View())->ChildAt(5)))->DeselectPrevious();
			}
			this->Select(true);
		}
		
		
		BMessage *currentMsg = Window()->CurrentMessage();
		if (currentMsg->what == B_MOUSE_DOWN) {
			uint32 buttons = 0;
			currentMsg->FindInt32("buttons", (int32 *)&buttons);		

//			uint32 modifiers = 0;
//			currentMsg->FindInt32("modifiers", (int32 *)&modifiers);
			if (buttons & B_SECONDARY_MOUSE_BUTTON) {
				EditApps();
			}
		}
	}
}


void	DayView::MessageReceived(BMessage *msg) {
	switch(msg->what) {
		case EDIT_APPS_MSG: {
			EditApps();
			break;
		}
		
		default: {
			BView::MessageReceived(msg);
			break;
		}
	}
}

void	DayView::EditApps() {
	// 0,0,420,300
	AppEditWindow *aew=new AppEditWindow(BRect(100,100,520,400), DVdow, DVday, DVmonth, DVyear, "", a_list, this);
		BRect win=Window()->Frame();
		aew->MoveTo(win.left+(win.Width()-aew->Bounds().Width())/2, win.top+(win.Height()-aew->Bounds().Height())/2);
	aew->Show();
}


int32	DayView::CountDates(bool draw) {
	Appointment	*a;
	BString s;
	int32	counter=0;
		
	for (int32 i=0; i<a_list->CountItems(); i++) {
		a=(Appointment *)a_list->ItemAt(i);
		if ( (a->GetDay()==DVday) && (a->GetMonth()==DVmonth) && (a->GetYear()==DVyear)) {
			counter++;
		}
	}
	dates=counter;
	if (draw)
		Draw(Bounds());
	return counter;
}

