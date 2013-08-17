/*
 
BookTab.h

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
#ifndef _BOOKTAB_H_
#define _BOOKTAB_H_

#include <InterfaceKit.h>
#include "DayView.h"
#include <List.h>

class BookTab : public BView {
	public:
		BookTab(BRect, float width, float height, int month, int year, int firstday, BList *);	// firstday: 0=sunday
		~BookTab();
	
		void	DeselectAll();
		void	DeselectThis();
		void	DeselectNext();
		void	DeselectPrevious();
		int	GetMonth();
		void	SetYear(int year);
		
		DayView *FindDay(int day);
	
	private:
		void	DetachDayViews();
		void	AttachDayViews(int firstday, int month, int year);
		int	BTm, BTy, BTfirstday;
		BList	*a_list;
		BStringView	*monthSV;
		float	bt_height, bt_width;
};

#endif
