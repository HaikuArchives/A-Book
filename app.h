/*
 
app.h

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
#ifndef _APP_H_
#define _APP_H_

#include <Application.h>
#include "mainWindow.h"
#include "ConfigManager.h"
#include "PrefWin.h"

#define AB_VERSION_NUMBER	"1.1"
#define AB_ABOUT_TEXT		"A small calendar for the lovely BeOS. Do you like it? Why not send me a postcard? You can find my current address on my web site www.fetal.de.\n\nBug reports etc. to <moscht@gmx.de>."
#define AB_BIG_WINDOW_SIZE	82
#define AB_SMALL_WINDOW_SIZE	50

void	SetAppToDeskBar(bool state);


class app : public BApplication {
public:
	app();
	~app();

	virtual void ReadyToRun(void);
	virtual bool QuitRequested(void);
	virtual void MessageReceived(BMessage *message);
	virtual void RefsReceived(BMessage *message);
	virtual void AboutRequested(void);
	void		SaveSettings();
	int32			db_id;
	
	mainWindow	*MainWindow();

private:
	void	ReadSettings();
	void 	ApplySettings();
	
	mainWindow		*mainW;
	PrefWin		*prefsW;
	ConfigManager	*CM;

};

#endif
