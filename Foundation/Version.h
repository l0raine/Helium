#pragma once

#include "Platform/Assert.h"

#define HELIUM_VERSION_MAJOR 0
#define HELIUM_VERSION_FEATURE 0 
#define HELIUM_VERSION_PATCH 1

HELIUM_COMPILE_ASSERT( HELIUM_VERSION_FEATURE < 100 && HELIUM_VERSION_PATCH < 100 );

#define HELIUM_VERSION_STRINGIFY( major, feature, patch ) #major"."#feature"."#patch
#define HELIUM_VERSION_TOSTRING( major, feature, patch ) HELIUM_VERSION_STRINGIFY( major,feature,patch)
#define HELIUM_VERSION_STRING HELIUM_VERSION_TOSTRING( HELIUM_VERSION_MAJOR, HELIUM_VERSION_FEATURE, HELIUM_VERSION_PATCH )
#define HELIUM_VERSION_NUMBER ( ( HELIUM_VERSION_MAJOR * 10000 ) + ( HELIUM_VERSION_FEATURE * 100 ) + ( HELIUM_VERSION_PATCH ) )

