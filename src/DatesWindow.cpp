/*
 
DatesWindow.cpp

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
#include "DatesWindow.h"
#include <stdio.h>
#include "TimeInfo.h"

datesWindow::datesWindow(BRect frame, BList *a_list, const char *title)
		: BWindow(frame, title, B_DOCUMENT_WINDOW, 0 ) {
		
	mainV=new BView(Bounds(), "mainV", B_FOLLOW_ALL, B_WILL_DRAW);
	mainV->SetViewColor(216,216,216);
	AddChild(mainV);
	
	appsTV=new BTextView(BRect(0,0,Bounds().Width()-B_V_SCROLL_BAR_WIDTH, Bounds().Height()-B_H_SCROLL_BAR_HEIGHT), "appsTV", BRect(2,2,10000,10000), B_FOLLOW_ALL, B_WILL_DRAW);
	appsTV->MakeEditable(false);
	appsTV->SetFontAndColor(be_fixed_font);
	appsSV=new BScrollView("appsSV", appsTV, B_FOLLOW_ALL, B_WILL_DRAW, true, true);
	mainV->AddChild(appsSV);
	list=a_list;
}

datesWindow::~datesWindow() {
}

int compfunc(const void *a,  const void *b) {

	float 	h1=(float)(*((Appointment **)(a)))->GetHour(),
		h2=(float)(*((Appointment **)(b)))->GetHour(),
		m1=(float)(*((Appointment **)(a)))->GetMinute(),
		m2=(float)(*((Appointment **)(b)))->GetMinute();

	float	doy1=(float)GetDOY(	(*((Appointment **)(a)))->GetDay(),
					(*((Appointment **)(a)))->GetMonth(),
					(*((Appointment **)(a)))->GetYear());
	float	doy2=(float)GetDOY(	(*((Appointment **)(b)))->GetDay(),
					(*((Appointment **)(b)))->GetMonth(),
					(*((Appointment **)(b)))->GetYear());

	doy1+=((h1+(m1/60.0))/24.0);
	doy2+=((h2+(m2/60.0))/24.0);
	if (doy1<doy2) return -1; else
	if (doy1>doy2) return 1;
	return 0;
}


void	datesWindow::Go() {
	BString		s="";
	int		day, month, year, hour, minute;
	int		last_month=0;
	list->SortItems(compfunc);
	
	for (int32 i=0; i<list->CountItems(); i++) {
		day= ((Appointment *)list->ItemAt(i))->GetDay();
		month= ((Appointment *)list->ItemAt(i))->GetMonth()+1;
		year= ((Appointment *)list->ItemAt(i))->GetYear();
		hour= ((Appointment *)list->ItemAt(i))->GetHour();
		minute= ((Appointment *)list->ItemAt(i))->GetMinute();
		if (last_month!=month) {
			s << ((s.Length()>0)?"\n":"") << monthname[month-1] << "\n";
			last_month=month;
		}
		s << ((day<10) ? "0": "") << day <<  "." << ((month<10) ? "0": "")<< month <<"." << year << " " << ((hour<10) ? "0":"") << hour << ":"<<((minute<10)?"0":"") << minute << "  "<< ((Appointment *)list->ItemAt(i))->GetNote() << "\n";
	}
	appsTV->SetText(s.String());
	this->Show();
}

void	datesWindow::MessageReceived(BMessage *msg) {
	switch(msg->what) {
		case B_QUIT_REQUESTED: {
			Quit();
			break;
		}
		
		default: {
			BWindow::MessageReceived(msg);
			break;
		}
	}
}




