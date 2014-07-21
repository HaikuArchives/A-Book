/*
 
DeskbarView.cpp

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
#include "DeskbarView.h"
#include <AppKit.h>
#include <NodeInfo.h>

DeskbarView::DeskbarView() : BView(BRect(0,0,15,15), "a-book DB", B_FOLLOW_NONE, B_WILL_DRAW){
	app_info	info;

	BRoster roster;
	entry_ref ref;
	roster.FindApp("application/x-a-book-application", &ref);

	bmp=new BBitmap(BRect(0,0,15,15), B_CMAP8);
	BNodeInfo::GetTrackerIcon(&ref, bmp, B_MINI_ICON);
}

DeskbarView::~DeskbarView() {
}


void DeskbarView::Draw(BRect updateRect) {
	DrawBitmap(bmp, BPoint(0,0));
	BView::Draw(updateRect);
}






