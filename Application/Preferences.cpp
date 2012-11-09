#include "ApplicationPch.h"
#include "Preferences.h"

#include "Platform/Process.h"

bool Helium::GetPreferencesDirectory( Helium::Path& preferencesDirectory )
{
    tstring prefDirectory = Helium::GetPreferencesDirectory();
	if ( !prefDirectory.empty() )
    {
		prefDirectory += TXT( "/.Helium/" );
		preferencesDirectory.Set( prefDirectory );
    }

    return false;
}