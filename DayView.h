/*
 
DayView.h

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
#ifndef _DAYVIEW_H_
#define _DAYVIEW_H_

#include <InterfaceKit.h>
#include <List.h>
#include "Appointment.h"

class DayView : public BView {
	public:
		DayView(BRect frame, int day, int dow, int month, int year, BList *);
		~DayView();

		void	Select(bool);
		void	DeselectPrevious();
		void	DeselectNext();
		int	GetDay();
		void	Mark(bool);
		void	UnmarkPrevious();
		void	UnmarkNext();
		int32	CountDates(bool draw=true);
		void	EditApps();	
		
	private:
		void	Draw(BRect updateRect);
		void	AttachedToWindow();
		void	MouseDown(BPoint);
		void	DrawMark(bool);
		void	DrawEntries(int32 count);
		void	DrawDate();
		void	DrawDayMark(bool enabled);

		void	MessageReceived(BMessage *);


		int		DVday,
				DVdow,
				DVmonth,
				DVyear;
		bool		selected,
				marked;
		rgb_color	saturday, sunday, selected_rgb;
		
		BList	*a_list;
		
		BPopUpMenu	*cmenu;
		
		int32		dates;
};

#endif
