/*
 
PrefWin.h

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
#ifndef _PREFWIN_H_
#define _PREFWIN_H_

#include <InterfaceKit.h>

#define SETTODESKBAR		'PW00'
#define WATCHORNOT		'PW01'
#define BEEPATDATE_MSG	'PW02'
#define SLIDER_MSG		'PW03'

class PrefWin : public BWindow {
	public:
		PrefWin();
		~PrefWin();
	
		bool	ShowsDates();
		void	SetShowDates(bool);	
		bool	IsWatching();
		void	SetWatching(bool);
		bool	DoesBeepAtDate();
		void	SetBeepAtDate(bool);
		int	GetEarlyRemind();
		void	SetEarlyRemind(int);
		bool	IsSmallSize();
		void	SetSmallSize(bool);
	
	private:
		void	MessageReceived(BMessage *msg);
		bool	QuitRequested();
		
		BView	*mainV;
		
		BCheckBox	*showCB,
				*deskbarCB,
				*watchCB,
				*beepatdateCB,
				*smallCB;
		BSlider		*alarmS;
		BStringView	*alarmSV;
};

#endif

