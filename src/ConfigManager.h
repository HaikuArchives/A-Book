/*
 
ConfigManager.h

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
#ifndef _CONFIGMANAGER_H_
#define _CONFIGMANAGER_H_

#include <Message.h>
#include <File.h>
#include <String.h>

class ConfigManager {
public:
	ConfigManager(const char *filename);
	~ConfigManager();
	
	status_t InitCheck();
	
	bool	HasData(const char *name);

	void	SetBool(const char *name, bool value);
	bool	GetBool(const char *name);

	void	SetInt32(const char *name, int32 value);
	int32	GetInt32(const char *name);

	void	SetInt16(const char *name, int16 value);
	int16	GetInt16(const char *name);

	void	SetInt8(const char *name, int8 value);
	int8	GetInt8(const char *name);

	void	SetRect(const char *name, BRect value);
	BRect	*GetRect(const char *name);

	void	SetString(const char *name, BString *value);
	void	SetString(const char *name, const char *value);
	BString	*GetString(const char *name);
	
	void	SetStringI(const char *name, BString *value, int32 index);
	void	SetStringI(const char *name, const char *value, int32 index);
	BString	*GetStringI(const char *name, int32 index);
	int32	CountStrings(const char *name);
	void	RemoveString(const char *name, int32 index);

	void	ReadConfiguration();
	void	WriteConfiguration();
	
private:
	BFile		*configfile;
	BMessage	*configMsg;
	status_t	initcheck;
	type_code	codeFound;
};


#endif
