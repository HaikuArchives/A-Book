/*
 
TimeLine.cpp

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
#include "TimeLine.h"
#include <String.h>
#include "TimeInfo.h"

TimeLine::TimeLine(BRect frame)
		: BView(frame, "timeline", B_FOLLOW_BOTTOM | B_FOLLOW_LEFT_RIGHT, B_WILL_DRAW | B_PULSE_NEEDED){
		
	SetViewColor(176, 176, 176);
	
	timeSV=new BStringView(BRect(Bounds().Width()/2,0,Bounds().right-1,Bounds().bottom),"timeSV", "00:00.00", B_FOLLOW_RIGHT | B_FOLLOW_BOTTOM);
	timeSV->SetAlignment(B_ALIGN_RIGHT);
	timeSV->SetFont(be_fixed_font);
	timeSV->SetFontSize(11);
	AddChild(timeSV);

	dateSV=new BStringView(BRect(1, 0, Bounds().Width()/2, Bounds().bottom),"dateSV", "", B_FOLLOW_LEFT | B_FOLLOW_BOTTOM);
	dateSV->SetAlignment(B_ALIGN_LEFT);
	dateSV->SetFont(be_fixed_font);
	timeSV->SetFontSize(11);
	AddChild(dateSV);
	last_min=last_day=-1;
	Pulse();
}

TimeLine::~TimeLine() {
}

void TimeLine::Pulse() {
	BString	time="";
	int	hour=GetHour(),
		min=GetMinute(),
		dow=GetDayOfWeek(),
		day=GetDay(),
		month=GetMonth(),
		year=GetYear();
	
	if ((last_min!=min) || (last_hour!=hour)) {
		time << hour << ":" << ((min<10) ? ("0") : ("")) << min;
		timeSV->SetText(time.String());
		last_min=min;
		last_hour=hour;
	}
	
	if ((last_day!=day) || (last_month!=month) || (last_year!=year)) {
		time="";
		time << dayofweek[dow] << ", " << monthname[month] << " " << day << ", " << year;
		dateSV->SetText(time.String());
		last_day=day;
		last_month=month;
		last_year=year;
	}
}


