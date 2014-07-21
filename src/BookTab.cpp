/*
 
BookTab.cpp

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
#include "BookTab.h"
#include "DayView.h"
#include "TimeInfo.h"
#include <support/ClassInfo.h>
#include <Window.h>
#include <String.h>
#include <Font.h>

BookTab::BookTab(BRect frame, float width, float height, int month, int year, int firstday, BList *list)
		: BView(frame, "month", B_FOLLOW_NONE, B_NAVIGABLE) {
		
	BTm=month;
	BTy=year;
	BTfirstday=firstday;
	a_list=list;
	bt_height=height;
	bt_width=width;
	SetViewColor(216,216,216);
	
	BString s=monthname[month-1];
	
	s << " " <<BTy;
	
	monthSV=new BStringView(BRect(0,0,frame.Width(), 27), "monthSV", s.String(), B_FOLLOW_TOP | B_FOLLOW_LEFT_RIGHT);
	monthSV->SetAlignment(B_ALIGN_CENTER);
	monthSV->SetFont(be_plain_font);
	monthSV->SetFontSize(24);
	AddChild(monthSV);
	
	AttachDayViews(firstday, month, year);
}

BookTab::~BookTab() {
}

void	BookTab::DetachDayViews() {
	
	DayView	*dv;
	
	while (CountChildren()>1) {
		(dv=((DayView *)ChildAt(1)))->RemoveSelf();
		delete dv;
	}
}

void	BookTab::AttachDayViews(int firstday, int month, int year) {
	int emptydays=firstday-1;
	int day=2-firstday;
	
	if (emptydays<0) day-=7;
	
	for (int y=0; y < 6; y++) {
		for (int x=0; x < 7; x++) {
			if (day>GetDaysInMonth(month, year))
				AddChild(new DayView(BRect( (bt_width*x)+4, (bt_height*y)+bt_height/2, ((bt_width*x)+(bt_width-2))+4, ((bt_height*y)+(bt_height-2)+bt_height/2) ), -1, x+1, month, year, a_list));
			else
				AddChild(new DayView(BRect( (bt_width*x)+4, (bt_height*y)+bt_height/2, ((bt_width*x)+(bt_width-2))+4, ((bt_height*y)+(bt_height-2)+bt_height/2) ), day, x+1, month, year, a_list));
			day++;
		}
	}
	ResizeTo(bt_width*7+8, bt_height*6+bt_height/2+6);
	monthSV->ResizeTo(bt_width*7+2,bt_height/2-3);
	monthSV->SetFontSize(bt_height/3);
}

void BookTab::DeselectNext() {
	if (NextSibling())
		if (is_instance_of(NextSibling(), BookTab)) {
			((BookTab *)NextSibling())->DeselectThis();
			((BookTab *)NextSibling())->DeselectNext();
		}
}

void BookTab::DeselectPrevious() {
	if (PreviousSibling())
		if (is_instance_of(PreviousSibling(), BookTab)) {
			((BookTab *)PreviousSibling())->DeselectThis();
			((BookTab *)PreviousSibling())->DeselectPrevious();
		}
}


void BookTab::DeselectAll() {
	DeselectNext();
	DeselectPrevious();
	DeselectThis();
}

void BookTab::DeselectThis() {
	for (int i=1; i<CountChildren(); i++) {
		((DayView *)ChildAt(i))->Select(false);
	}
}

int BookTab::GetMonth() {
	return BTm;
}

void	BookTab::SetYear(int year) {

	if (year!=BTy) {
		BTy=year;
		
		DetachDayViews();
		AttachDayViews(GetDOW(1,BTm,BTy), BTm, BTy);	

		BString s=monthname[BTm-1];
		s << " " <<BTy;
		monthSV->SetText(s.String());
	}
}

DayView *BookTab::FindDay(int day) {
	for (int i=0; i<CountChildren(); i++) {
		if (is_instance_of(ChildAt(i), DayView)) {
			if (((DayView *)ChildAt(i))->GetDay()==day)
				return (DayView *)ChildAt(i);
		}
	}
	return NULL;
}

