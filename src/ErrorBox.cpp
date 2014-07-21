/*
 
ErrorBox.cpp

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
#include "ErrorBox.h"
#include <GraphicsDefs.h>
#include <string.h>
#include <Screen.h>
#include <stdio.h>

#define RIGHT_BUTTON_MSG	'EB00'
#define MIDDLE_BUTTON_MSG	'EB01'
#define LEFT_BUTTON_MSG	'EB02'

ErrorView::ErrorView(BRect frame, const char *name, signal_color color)
		: BView(frame, name, B_FOLLOW_ALL, B_WILL_DRAW) {	
	SetViewColor(216,216,216);

	switch(color) {
		case E_RED_COLOR:
			viewColor=(rgb_color){184, 94, 94, 255};
			break;
		case E_ORANGE_COLOR:
			viewColor=(rgb_color){184, 94, 24, 255};
			break;
		case E_YELLOW_COLOR:
			viewColor=(rgb_color){232, 232, 0, 255};
			break;
		case E_NORMAL_COLOR:
			viewColor=(rgb_color){184, 184, 184, 255};
			break;
		case E_BLUE_COLOR:
			viewColor=(rgb_color){94, 94, 184, 255};
			break;
		case E_GREEN_COLOR:
			viewColor=(rgb_color){94, 184, 94, 255};
			break;
		default:
			viewColor=(rgb_color){184, 184, 184, 255};
			break;
	}
}

ErrorView::~ErrorView() {
}

void ErrorView::Draw(BRect updateRect) {
	if (BRect(0,0,10+23,Bounds().Height()).Intersects(updateRect)) {
		SetDrawingMode(B_OP_OVER);
		SetHighColor(viewColor);
		FillRect(BRect(0,0,10+23,Bounds().Height()));
	}
}




ErrorBox::ErrorBox(signal_color color, const char *title, const char *errortext, const char *rightbutton, const char *middlebutton, const char *leftbutton)
		: BWindow((BRect(0,0,310,170)), title, B_MODAL_WINDOW_LOOK, B_MODAL_APP_WINDOW_FEEL, B_NOT_ZOOMABLE | B_NOT_RESIZABLE) {

	float	width, height, left=310;
	const int textbottom=120;

	sem=create_sem(0,"ErrorBox");

	vError=new ErrorView(Bounds(), "ErrorView", color);	
	AddChild(vError);
	
	errorSV=new BStringView(BRect(45,1, 310, 28), "errorSV", title);
	errorSV->SetFont(be_bold_font);
	vError->AddChild(errorSV);

	textTV=new BTextView(BRect(45,38, 308, textbottom), "textTV", BRect(1,1,308-45,textbottom-38), B_FOLLOW_LEFT | B_FOLLOW_TOP, B_WILL_DRAW);
	textTV->SetViewColor(216,216,216,255);
	textTV->MakeEditable(false);
	textTV->MakeSelectable(false);
	textTV->SetWordWrap(true);
	BFont	*font=new BFont(be_plain_font);
	font->SetSize(10);
	textTV->SetFontAndColor(font);
	vError->AddChild(textTV);
	delete font;
	textTV->SetText(errortext);

	rightB=new BButton(BRect(0,0,1,1), "rightB", rightbutton, new BMessage(RIGHT_BUTTON_MSG));
	rightB->MakeDefault(true);
	vError->AddChild(rightB);
	rightB->GetPreferredSize(&width, &height);
	rightB->ResizeTo(width, height);
	rightB->MoveTo((left=(left-width-5)),textbottom+(180-textbottom-height-2)/2);
	
	if (strlen(middlebutton)>0) {
		middleB=new BButton(BRect(0,0,1,1), "middleB", middlebutton, new BMessage(MIDDLE_BUTTON_MSG));
		vError->AddChild(middleB);
		middleB->GetPreferredSize(&width, &height);
		middleB->ResizeTo(width, height);
		middleB->MoveTo((left=(left-width-5)),textbottom+(180-textbottom-height-2)/2);			

		if (strlen(leftbutton)>0) {
			leftB=new BButton(BRect(0,0,1,1), "leftB", leftbutton, new BMessage(LEFT_BUTTON_MSG));
			vError->AddChild(leftB);
			leftB->GetPreferredSize(&width, &height);
			leftB->ResizeTo(width, height);
			leftB->MoveTo((left=(left-width-5)),textbottom+(180-textbottom-height-2)/2);
		}
	}

	BScreen	*screen=new BScreen();
	MoveTo((int)((screen->Frame().Width()-Bounds().Width())/2), (int)((screen->Frame().Height()-Bounds().Height())/2));
	delete screen;
}


ErrorBox::~ErrorBox() {
	delete_sem(sem);
}

void ErrorBox::SetShortcut(int32 index, uint32 shortcut) {
	switch(index) {
		case 0: 
			rb=(int8)shortcut;
			break;
		case 1:
			mb=(int8)shortcut;
			break;
		case 2:
			lb=(int8)shortcut;
			break;
		default:
			break;
			
	}
} 


int ErrorBox::Go() {
	this->Show();
	acquire_sem(sem);	//wait
	return retval;
}

void ErrorBox::SetText(const char *text) {
	textTV->SetText(text);
}

bool ErrorBox::QuitRequested() {
	Quit();
	return false;
}

void ErrorBox::MessageReceived(BMessage *msg) {
	switch(msg->what) {
		case RIGHT_BUTTON_MSG: {
			retval=0;
			release_sem(sem);
			Quit();
			break;
		}
		
		case MIDDLE_BUTTON_MSG: {
			retval=1;
			release_sem(sem);
			Quit();
			break;
		}

		case LEFT_BUTTON_MSG: {
			retval=2;
			release_sem(sem);
			Quit();
			break;
		}

		case B_KEY_DOWN: {
			int8 byte;
			if (msg->FindInt8("byte", &byte)==B_OK) {
				if (byte==lb) PostMessage(LEFT_BUTTON_MSG); else
				if (byte==mb) PostMessage(MIDDLE_BUTTON_MSG); else
				if (byte==rb) PostMessage(RIGHT_BUTTON_MSG);
			}
		}

		default: {
			BWindow::MessageReceived(msg);
			break;
		}
	}
}

