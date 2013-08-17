/*
 
AppEditWindow.h

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
#ifndef _APPEDITWINDOW_H_
#define _APPEDITWINDOW_H_

#include <InterfaceKit.h>
#include "Appointment.h"
#include <List.h>
#include "DayView.h"

class AppEditWindow : public BWindow {
	public:
		AppEditWindow(BRect frame, int dow, int day, int month, int year, const char *title, BList *applist, DayView *parent);
		~AppEditWindow();
	
	private:
	
		void	AddAppsOfTheDay();
		const char *GetNoteOfItem(BStringItem *si);
		int32	GetIndexOf(int day, int month, int year, int hour, int minute, const char *text);
		static int	compfunc( const void *a, const void *b);
	
		bool	QuitRequested();
		void	MessageReceived(BMessage *msg);
		
		
		BList	*a_list;
		
		BStringView	*toplineSV;
		BView		*mainV;
		
		BTextControl	*hourTC,
				*minuteTC,
				*textTC;
				
		BScrollView	*applistSV;
		BListView	*applistLV;
		
		BButton	*addB,
				*modifyB,
				*deleteB;
				
		int		d, m, y;
		DayView	*parentDV;
};

#endif
