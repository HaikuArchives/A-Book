/*
 
Appointment.h

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
#ifndef _APPOINTMENT_H_
#define _APPOINTMENT_H_

#include <String.h>

class Appointment {
	public:
		Appointment(int day, int month, int year, int hour, int minute, const char *note);
		~Appointment();
		
		int	GetDay();
		int	GetMonth();
		int	GetYear();
		int	GetHour();
		int 	GetMinute();
		
		void	SetDay(int day);
		void	SetMonth(int month);
		void	SetYear(int year);
		void	SetHour(int hour);
		void	SetMinute(int minute);
		
		void		SetNote(const char *note);
		const char	*GetNote();
		
	private:
		BString		ANote;
		int		Aday, Amonth, Ayear, Ahour, Aminute;		
};


#endif

