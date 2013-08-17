/*
 
MainView.cpp

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
#include "MainView.h"
#include "TimeInfo.h"
#include "mainWindow.h"
#include "BookTab.h"
#include "MyScrollBar.h"

MainView::MainView(BRect frame) : BView(frame, "mainV", B_FOLLOW_NONE, B_WILL_DRAW | B_PULSE_NEEDED) {
	SetViewColor(216, 216, 216);
	last_day=last_month=last_year-1;
}

MainView::~MainView() {
}

void	MainView::Pulse() {
	if (last_day!=GetDay()) {
		((mainWindow *)Window())->MarkDay(GetDay(), GetMonth()+1);
		last_day=GetDay();
	}
	if (last_month!=GetMonth()) {
		((mainWindow *)Window())->calendarTV->Select(GetMonth());
		last_month=GetMonth();
		((mainWindow *)Window())->MarkDay(GetDay(), GetMonth()+1);
		last_day=GetDay();
	}
	if (last_year!=GetYear()) {
		((mainWindow *)Window())->yearSB->SetValue(GetYear());
		((mainWindow *)Window())->PostMessage(VALUECHANGED);
		last_year=GetYear();
	}
}

void	MainView::MessageReceived(BMessage *msg) {
	switch(msg->what) {
		default: {
			BView::MessageReceived(msg);
			break;
		}
	}
}