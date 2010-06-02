#include "Precompile.h"
#include "BuildInfo.h"

#include "Common/Boost/Regex.h"
#include "FileSystem/FileSystem.h"

using namespace Luna;

///////////////////////////////////////////////////////////////////////////////
// Gets the root path to the level, and replaces all non-characters with 
// underscores.
// 
std::string LevelInfo::GetStrippedName() const
{
  const boost::regex s_NonWords( "(\\W)" );
  const boost::regex s_TrailingSlashes( "(/+$)" );
  std::string name( m_Name );
  FileSystem::StripLeaf( name );
  name = boost::regex_replace( name, s_TrailingSlashes, "" );
  name = boost::regex_replace( name, s_NonWords, "_" );
  return name;
}


///////////////////////////////////////////////////////////////////////////////
// Constructor
// 
BuildInfo::BuildInfo( const std::string& name )
: m_Name( name )
, m_Timestamp( 0 )
, m_CodeSucceeded( false )
, m_SymbolsSucceeded( false )
{
}

///////////////////////////////////////////////////////////////////////////////
// Destructor
// 
BuildInfo::~BuildInfo()
{
}

///////////////////////////////////////////////////////////////////////////////
// Returns true if the build is finished and the code and symbols succeeded, 
// and if the build contains at least one level with valid server data.
// 
bool BuildInfo::Succeeded() const
{
  bool succeeded = false;
  if ( m_CodeSucceeded && m_SymbolsSucceeded )
  {
    V_LevelInfo::const_iterator levelItr = m_Levels.begin();
    V_LevelInfo::const_iterator levelEnd = m_Levels.end();
    for ( ; levelItr != levelEnd; ++levelItr )
    {
      if ( ( *levelItr )->m_HasServerData )
      {
        succeeded = true;
        break;
      }
    }
  }

  return succeeded;
}