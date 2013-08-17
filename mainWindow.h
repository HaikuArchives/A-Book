/*
 
mainWindow.h

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
#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <InterfaceKit.h>
#include <AppKit.h>
#include "TimeLine.h"
#include "DayView.h"
#include <List.h>
#include "MainView.h"
#include "MyScrollBar.h"
#include <KernelKit.h>

#define SHOW_PREFS_MSG	'MW00'
#define SHOW_DATES_MSG	'MW01'

class mainWindow : public BWindow {
	public:
		mainWindow(BRect frame, const char *title,float width, float height);
		~mainWindow();
		
		void	SaveAppointments();
		void	LoadAppointments();
		void	SelectDay(int day, int month);
		void	MarkDay(int day, int month);
		void	ShowTodaysDates();
		void	StartCheckerThread();
		void	StopCheckerThread();
		void	SetBeep(bool);
		BList	*CheckForDateThisMinute();
		BList	*CheckForDateOn(int day, int month, int year, int hour, int minute);		void	SetMinutesBefore(int32);
		int32	GetMinutesBefore();
		void	ResizeAllToFit();
	
		MyScrollBar	*yearSB;
		BTabView	*calendarTV;
	private:
		Appointment	*ReadFromMessage(BMessage *msg, int32 index);
		void	AddToMessage(Appointment *a, BMessage *msg);
		void	MessageReceived(BMessage *);
		bool	QuitRequested();
		DayView 	*FindDay(int day, int month);	
		

		BList		*CheckForDateThisHour();
		BList		*CheckForDateThisDay(int day, int month, int year);
		BList		*CheckForDateThisMonth(int month, int year);
		BList		*CheckForDateThisYear(int year);

		void	PrintDatesOfDay();
		
		MainView		*mainV;
		TimeLine	*timeTL;
		BList		*list;
		thread_id	checkerT;
		BMessage	*threadmsgM;
		bool		beepAtMsg;
		
		int32		mw_minutesbefore;
		float		mw_menuheight, mw_menuwidth;
		const char	*mw_datesfolder;
		
};

int32	CheckThread(void *data);

#endif
