/*
 *  mac_datadir.c
 *  OpenMortal
 *
 *  Created by Sebestyén Gábor on Sat Apr 03 2004.
 *  Copyright (c) 2004 __MyCompanyName__. All rights reserved.
 *
 */

//#include "mac_datadir.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

#include <CoreFoundation/CoreFoundation.h>

const char *data_path_sys = "/Library/Application Support/OpenMortal/data";

char mac_datadir[255];

void getResourcePath() {
	CFURLRef	resUrl, dataUrl;
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	
	// get resources path
	resUrl = CFBundleCopyResourcesDirectoryURL (mainBundle);
	dataUrl = CFURLCreateCopyAppendingPathComponent (
													 kCFAllocatorDefault,
													 resUrl,
													 CFSTR("data"),
													 true
													 );
	CFURLGetFileSystemRepresentation (
									  dataUrl,
									  true,
									  mac_datadir,
									  255
									  );
}

void initMacDataDir()
{
	struct stat sb;

	// First, check local path
	char *homedir = getenv("HOME");
	if (homedir) {
		strcpy(mac_datadir, homedir);
		strcat(mac_datadir, data_path_sys);

		if (!stat(mac_datadir, &sb))
			return;
	}
	
	// Second, check the sys path
	strcpy(mac_datadir, data_path_sys);	
	if (!stat(mac_datadir, &sb))
		return;
	
	// get the path inside from the .app folder
	getResourcePath();
}

