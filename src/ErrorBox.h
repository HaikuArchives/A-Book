/*
 
ErrorBox.h

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
#ifndef _ERRORBOX_H_
#define _ERRORBOX_H_

#include <Window.h>
#include <View.h>
#include <Bitmap.h>
#include <Rect.h>
#include <StringView.h>
#include <TextView.h>
#include <Button.h>

typedef enum {
	E_RED_COLOR=1,
	E_ORANGE_COLOR=2,
	E_YELLOW_COLOR=3,
	E_NORMAL_COLOR=4,
	E_BLUE_COLOR=5,
	E_GREEN_COLOR=6
} signal_color;


class ErrorView : public BView {
public:
	ErrorView(BRect frame, const char *name, signal_color color);
	~ErrorView();
	
	virtual void 	Draw(BRect updateRect);
private:
	rgb_color	viewColor;
};

class ErrorBox : public BWindow {
public:
	ErrorBox(signal_color color, const char *title, const char *errortext, const char *rightbutton, const char *middlebutton="", const char *leftbutton="");
	~ErrorBox();
	
	int 	Go();
	void 	SetText(const char *);
	void	SetShortcut(int32 index, uint32 shortcut);
	
	
private:

	virtual bool 	QuitRequested();
	virtual void	MessageReceived(BMessage *msg);

	ErrorView	*vError;
	BStringView	*errorSV,
			*versionNumberSV,
			*copyrightStringSV;
	BTextView	*textTV;
	BButton	*leftB,
			*middleB,
			*rightB;
	sem_id		sem;
	BRect		frame;
	int		retval;
	int8		lb, mb, rb;
};


#endif
