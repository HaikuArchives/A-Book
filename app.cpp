/*
 
app.cpp

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
#include "app.h"
#include <Alert.h>
#include <Entry.h>
#include <InterfaceKit.h>
#include <AppKit.h>
#include "AboutWindow.h"
#include <StorageKit.h>
#include "ConfigFields.h"
#include <Deskbar.h>
#include "DeskbarView.h"

app::app():BApplication("application/x-a-book-application") {
	CM=new ConfigManager("/boot/home/config/settings/a-book.settings");
}

app::~app() {
	delete CM;
}

void app::ReadyToRun() {
	ReadSettings();
	mainW=new mainWindow(BRect(100,100,700,694), "a-book", CM->GetInt16(CALENDARSIZE), CM->GetInt16(CALENDARSIZE));
	prefsW=new PrefWin();
	ApplySettings();
	if (mainW->Lock()) {
		mainW->ResizeAllToFit();
		mainW->Unlock();
	}
	mainW->Show();
	
	if (prefsW->ShowsDates()) {
		mainW->Lock();
		mainW->ShowTodaysDates();
		mainW->Unlock();
	}
	be_app->PostMessage(WATCHORNOT);
}


void app::ReadSettings() {
	
	CM->ReadConfiguration();
	// CHECK FOR DATA
	
	if (!CM->HasData(SHOWSDATES))		CM->SetBool(SHOWSDATES, false);
	if (!CM->HasData(WATCHDATES))	CM->SetBool(WATCHDATES, true);
	if (!CM->HasData(BEEPATDATE))		CM->SetBool(BEEPATDATE, true);
	if (!CM->HasData(MINUTESBEFORE))	CM->SetInt32(MINUTESBEFORE, 60);
	if (!CM->HasData(CALENDARSIZE))	CM->SetInt16(CALENDARSIZE, AB_BIG_WINDOW_SIZE);
	
	
}

void app::ApplySettings() {
	prefsW->SetShowDates(CM->GetBool(SHOWSDATES));
	prefsW->SetWatching(CM->GetBool(WATCHDATES));
	prefsW->SetBeepAtDate(CM->GetBool(BEEPATDATE));
	prefsW->SetEarlyRemind(CM->GetInt32(MINUTESBEFORE));
	prefsW->SetSmallSize(CM->GetInt16(CALENDARSIZE)<AB_BIG_WINDOW_SIZE);
	
	if (CM->HasData(MAINWINDOW_RECT)) {
		mainW->ResizeTo(CM->GetRect(MAINWINDOW_RECT)->Width(), CM->GetRect(MAINWINDOW_RECT)->Height());
		mainW->MoveTo(CM->GetRect(MAINWINDOW_RECT)->left, CM->GetRect(MAINWINDOW_RECT)->top);
	}
	if (CM->HasData(PREFSWINDOW_RECT)) {
		//prefsW->ResizeTo(CM->GetRect(PREFSWINDOW_RECT)->Width(), CM->GetRect(PREFSWINDOW_RECT)->Height());
		prefsW->MoveTo(CM->GetRect(PREFSWINDOW_RECT)->left, CM->GetRect(PREFSWINDOW_RECT)->top);
	}
}

void app::SaveSettings() {
	
	CM->SetRect(MAINWINDOW_RECT, mainW->Frame());
	CM->SetRect(PREFSWINDOW_RECT, prefsW->Frame());
	
	CM->SetBool(SHOWSDATES, prefsW->ShowsDates());
	CM->SetBool(WATCHDATES, prefsW->IsWatching());
	CM->SetBool(BEEPATDATE, prefsW->DoesBeepAtDate());
	CM->SetInt32(MINUTESBEFORE, prefsW->GetEarlyRemind());
	CM->SetInt16(CALENDARSIZE, prefsW->IsSmallSize() ? AB_SMALL_WINDOW_SIZE : AB_BIG_WINDOW_SIZE);
	
	CM->WriteConfiguration();
}


bool app::QuitRequested() {
	SaveSettings();
	
	if (mainW->Lock()) {
		mainW->Quit();
	}
	return true;
}

void SetAppToDeskBar(bool state) {
	BDeskbar	*db=new BDeskbar();
	if (state) {
			BRoster	roster;
			entry_ref	ref;

			roster.FindApp("application/x-a-book-application", &ref);
			if (db->AddItem(&ref, &(((app *)be_app)->db_id))==B_OK)
				be_app->PostMessage(B_QUIT_REQUESTED);

	} else {
			db->RemoveItem(((app *)be_app)->db_id);
	}
	
	delete db;
}

void app::MessageReceived(BMessage *msg) {
	switch(msg->what) {
		case BEEPATDATE_MSG: {
			if (mainW->Lock()) {
				mainW->SetBeep(prefsW->DoesBeepAtDate());
				mainW->Unlock();
			}
			break;
		}
		
		case WATCHORNOT: {
			mainW->Lock();
			if (prefsW->IsWatching())
				mainW->StartCheckerThread();
			else
				mainW->StopCheckerThread();
				
			// disable BeepCB control if watching is disabled.
			if (prefsW->Lock()) {
				prefsW->SetWatching(prefsW->IsWatching());
				prefsW->Unlock();
			}
			mainW->Unlock();
			break;
		}
	
		case SETTODESKBAR: {
			break;
		}

		case SHOW_PREFS_MSG: {
			prefsW->Lock();
			if (prefsW->IsHidden())
				prefsW->Show();
			prefsW->Unlock();
			break;
		}
		
		default: {
			BApplication::MessageReceived(msg);
			break;
		}
	}
}

void app::RefsReceived(BMessage *msg) {
	uint32 		type;
	int32 		count;
	BEntry		*entry=new BEntry();
	entry_ref	ref;

	msg->GetInfo("refs", &type, &count);
	
	// not a entry_ref?
	if (type != B_REF_TYPE) {
		delete entry;
		return;
	}
	
	if (msg->FindRef("refs", 0, &ref) == B_OK)
		if (entry->SetTo(&ref,true)==B_OK) {
			// entry is ok. use it here.
		}
	delete entry;	
}

void app::AboutRequested() {
	app_info	info;
	AboutWindow	*aboutW=new AboutWindow(BRect(100,100,400,280), "About");
	BScreen	*screen=new BScreen();

	aboutW->MoveTo((int)((screen->Frame().Width()-aboutW->Bounds().Width())/2), (int)((screen->Frame().Height()-aboutW->Bounds().Height())/2));
	delete screen;
	if (GetAppInfo(&info)==B_OK) {
		BBitmap bmp(BRect(0,0,31,31), B_CMAP8);
		if (BNodeInfo::GetTrackerIcon(&info.ref, &bmp, B_LARGE_ICON)==B_OK)
			aboutW->SetIcon(&bmp);
	}
	aboutW->SetApplicationName("a-book");
	aboutW->SetVersionNumber(AB_VERSION_NUMBER);
	aboutW->SetCopyrightString(B_UTF8_COPYRIGHT" by Maurice Michalski");
	aboutW->SetText(AB_ABOUT_TEXT);
	aboutW->Show();
}


mainWindow	*app::MainWindow() {
	return mainW;
}


int main() {
	new app();
	be_app->Run();
	delete be_app;
	return 0;
}
