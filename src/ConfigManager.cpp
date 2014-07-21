/*
 
ConfigManager.cpp

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
#include "ConfigManager.h"
#include <stdio.h>
#include <TypeConstants.h>
#include <Application.h>
#include <AppFileInfo.h>
#include <app/Roster.h>
#include <Bitmap.h>

ConfigManager::ConfigManager(const char *filename) {
	initcheck=B_OK;
	configMsg=new BMessage();
	configfile=new BFile(filename, B_READ_WRITE | B_CREATE_FILE);
	ReadConfiguration();
}

ConfigManager::~ConfigManager() {
	delete configMsg;
	delete configfile;
}

status_t ConfigManager::InitCheck() {
	return initcheck;
}

bool ConfigManager::HasData(const char *name) {
	return configMsg->GetInfo(name, &codeFound)==B_OK;
}

// BOOL
void ConfigManager::SetBool(const char *name, bool value) {
	configMsg->RemoveName(name);
	configMsg->AddBool(name, value);
}

bool ConfigManager::GetBool(const char *name) {
	bool value;
	if (configMsg->FindBool(name, &value)!=B_OK) value=false;
	return value;
}

// INT32
void ConfigManager::SetInt32(const char *name, int32 value) {
	configMsg->RemoveName(name);
	configMsg->AddInt32(name, value);
}

int32 ConfigManager::GetInt32(const char *name) {
	int32 value;
	configMsg->FindInt32(name, &value);
	return value;
}

// INT16
void ConfigManager::SetInt16(const char *name, int16 value) {
	configMsg->RemoveName(name);
	configMsg->AddInt16(name, value);
}

int16 ConfigManager::GetInt16(const char *name) {
	int16 value;
	configMsg->FindInt16(name, &value);
	return value;
}

// INT8
void ConfigManager::SetInt8(const char *name, int8 value) {
	configMsg->RemoveName(name);
	configMsg->AddInt8(name, value);
}

int8 ConfigManager::GetInt8(const char *name) {
	int8 value;
	configMsg->FindInt8(name, &value);
	return value;
}

// RECT
void ConfigManager::SetRect(const char *name, BRect value) {
	configMsg->RemoveName(name);
	configMsg->AddRect(name, value);
}

BRect *ConfigManager::GetRect(const char *name) {
	BRect *value=new BRect();
	configMsg->FindRect(name, value);
	return value;
}

// STRING
void ConfigManager::SetString(const char *name, BString *value) {
	configMsg->RemoveName(name);
	configMsg->AddString(name, *value);
}

void ConfigManager::SetString(const char *name, const char *value) {
	configMsg->RemoveName(name);
	configMsg->AddString(name, value);
}

BString *ConfigManager::GetString(const char *name) {
	BString *value=new BString("");
	configMsg->FindString(name, value);
	return value;
}

// STRING WITH INDICES
void ConfigManager::SetStringI(const char *name, BString *value, int32 index) {
	if (index+1>CountStrings(name))
		configMsg->AddString(name, *value);
	else
		configMsg->ReplaceString(name, index, *value);
	
}

void ConfigManager::SetStringI(const char *name, const char *value, int32 index) {
	if (index+1>CountStrings(name))
		configMsg->AddString(name, value);
	else
		configMsg->ReplaceString(name, index, value);
}

BString *ConfigManager::GetStringI(const char *name, int32 index) {
	BString *value=new BString("");
	configMsg->FindString(name, index, value);
	return value;
}

int32	ConfigManager::CountStrings(const char *name) {
	int32	count=0;
	type_code	code;
	configMsg->GetInfo(name, &code, &count);
	return count;
}

void	ConfigManager::RemoveString(const char *name, int32 index) {
	configMsg->RemoveData(name, index);
}

void ConfigManager::ReadConfiguration() {
	if ((initcheck=configfile->InitCheck())==B_OK) {
		off_t filesize=0;
		configfile->GetSize(&filesize);
		if (filesize>0) {
			configfile->Seek(0, SEEK_SET);
			if (configMsg->Unflatten(configfile)==B_BAD_VALUE) {
				configfile->SetSize(0);
			}
		} else {
			// first launch here
		}
	}
}

void ConfigManager::WriteConfiguration() {
	if ((initcheck=configfile->InitCheck())==B_OK) {
		configfile->Seek(0, SEEK_SET);
		configMsg->Flatten(configfile);
	} else {
	}
}