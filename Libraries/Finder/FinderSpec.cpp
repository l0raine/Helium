#include "FinderSpec.h"
#include "Finder.h"

#include "AppUtils/Preferences.h"
#include "Common/Assert.h"
#include "Common/File/Path.h"
#include "FileSystem/FileSystem.h"

namespace Finder
{
  FileSpec* FileSpec::s_LinkedListHead = NULL;
  ModifierSpec* ModifierSpec::s_LinkedListHead = NULL;
  FolderSpec* FolderSpec::s_LinkedListHead = NULL;
  FilterSpec* FilterSpec::s_LinkedListHead = NULL;

  /////////////////////////////////////////////////////////////////////////////
  std::string FinderSpec::GetFilter() const
  {
    throw Exception ( "This FinderSpec cannot be filtered by" );
  }

  /////////////////////////////////////////////////////////////////////////////
  std::string FinderSpec::GetDialogFilter() const
  {
    FINDER_SCOPE_TIMER((""));

    std::string filter = GetFilter();

    return m_UIName + " (" + filter + ")|" + filter;
  }

  /////////////////////////////////////////////////////////////////////////////
  std::string SuffixSpec::GetSuffix() const
  {
    FINDER_SCOPE_TIMER((""));

    return m_Value;
  }

  /////////////////////////////////////////////////////////////////////////////
  void SuffixSpec::Modify( std::string& path ) const
  {
    FINDER_SCOPE_TIMER((""));

    std::string extension;
    FileSystem::GetExtension( path, extension );
    FileSystem::StripExtension( path );
    path += m_Value + extension;
  }

  /////////////////////////////////////////////////////////////////////////////
  std::string FamilySpec::GetFamily() const
  {
    FINDER_SCOPE_TIMER((""));

    std::string result = m_Value;
    FileSystem::GuaranteeDot( result );

    return result;
  }

  /////////////////////////////////////////////////////////////////////////////
  void FamilySpec::Modify( std::string& path ) const
  {
    FINDER_SCOPE_TIMER((""));

    std::string extension;
    FileSystem::GetExtension(path, extension);
    FileSystem::StripExtension(path, 2);
    path += GetFamily() + extension;
  }

  /////////////////////////////////////////////////////////////////////////////
  std::string ExtensionSpec::GetFilter() const
  {
    FINDER_SCOPE_TIMER((""));

    std::string result ("*.");

    return result + m_Value;
  }

  /////////////////////////////////////////////////////////////////////////////
  std::string ExtensionSpec::GetExtension() const
  {
    FINDER_SCOPE_TIMER((""));

    std::string result = m_Value;
    FileSystem::GuaranteeDot( result );

    return result;
  }

  /////////////////////////////////////////////////////////////////////////////
  void ExtensionSpec::Modify( std::string& path ) const
  {
    FINDER_SCOPE_TIMER((""));

    FileSystem::StripExtension( path, 1 );
    path += GetExtension();
  }

  /////////////////////////////////////////////////////////////////////////////
  std::string DecorationSpec::GetDecoration() const
  {
    std::string result;

    if (m_Suffix)
    {
      result += m_Suffix->GetSuffix();
    }

    if (m_Family)
    {
      result += m_Family->GetFamily();
    }

    if (m_Extension)
    {
      result += m_Extension->GetExtension();
    }

    return result;
  }

  /////////////////////////////////////////////////////////////////////////////
  std::string DecorationSpec::GetFilter() const
  {
    FINDER_SCOPE_TIMER((""));

    std::string result ("*");

    return result + GetDecoration();
  }

  /////////////////////////////////////////////////////////////////////////////
  void DecorationSpec::Modify(std::string& path) const
  {
    FINDER_SCOPE_TIMER((""));

    FileSystem::StripExtension( path );

    path += GetDecoration();
  }

  /////////////////////////////////////////////////////////////////////////////
  std::string FileSpec::GetFile() const
  {
    FINDER_SCOPE_TIMER((""));

    std::string result = m_Value;

    if (m_Modifier)
    {
      m_Modifier->Modify(result);
    }

    return result;
  }

  /////////////////////////////////////////////////////////////////////////////
  std::string FileSpec::GetFile( const std::string& folder ) const
  {
    FINDER_SCOPE_TIMER((""));

    return ( folder + GetFile() );
  }

  /////////////////////////////////////////////////////////////////////////////
  std::string FileSpec::GetFile( const FolderSpec& folderSpec ) const
  {
    FINDER_SCOPE_TIMER((""));

    return ( folderSpec.GetFolder() + GetFile() );
  }

  /////////////////////////////////////////////////////////////////////////////
  std::string FolderSpec::GetFolder() const
  {
    FINDER_SCOPE_TIMER((""));

    Nocturnal::Path dir;

    if(m_ParentFolder)
    {
      dir.Set( m_ParentFolder->GetFolder() ); 
    }
    else
    {
      switch ( m_Root )
      {
        case FolderRoots::None:
          break;

        case FolderRoots::UserPrefs:
            if ( !AppUtils::GetPreferencesDirectory( dir ) )
            {
                throw Nocturnal::Exception( "Could not determine preferences directory.  Is the APPDATA environment variable set?" );
            }
            break;

        default:
          NOC_BREAK();
      }

    }
    
    dir.Set( dir.Get() + '/' + m_Value + '/');
    return dir.Get();
  }

  
  /////////////////////////////////////////////////////////////////////////////
  std::string FolderSpec::GetRelativeFolder() const 
  {
    std::string result; 

    if(m_ParentFolder)
    {
      result = m_ParentFolder->GetRelativeFolder(); 
      result += m_Value; 
    }
    else
    {
      result = m_Value; 
    }

    FileSystem::GuaranteeSlash(result); 
    return result; 
  }

  /////////////////////////////////////////////////////////////////////////////
  // FilterSpec
  /////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////////////////
  // Adds an ExtensionSpec to this filter.
  // 
  void FilterSpec::AddSpec( const ExtensionSpec& spec )
  {
    DoAddSpec( spec );
  }

  /////////////////////////////////////////////////////////////////////////////
  // Adds a DecorationSpec to this filter.
  // 
  void FilterSpec::AddSpec( const DecorationSpec& spec )
  {
    DoAddSpec( spec );
  }

  /////////////////////////////////////////////////////////////////////////////
  // Returns a filter made up of all the included specs.
  // 
  std::string FilterSpec::GetDialogFilter() const
  {
    FINDER_SCOPE_TIMER((""));

    std::string result = __super::GetDialogFilter();

    OS_ModifierSpecConstDumbPtr::Iterator itr = m_Specs.Begin();
    OS_ModifierSpecConstDumbPtr::Iterator end = m_Specs.End();
    for ( ; itr != end; ++itr )
    {
      const ModifierSpec* spec = *itr;
      
      if ( !result.empty() )
      {
        result += "|";
      }

      result += spec->GetDialogFilter();
    }

    return result;
  }

  /////////////////////////////////////////////////////////////////////////////
  // The filter is maintained as a semicolon delimited list of extensions.
  // 
  std::string FilterSpec::GetFilter() const
  {
    return m_Value;
  }

  /////////////////////////////////////////////////////////////////////////////
  // Returns true if the extension matches one of the existing specs.
  // 
  bool FilterSpec::IsExtensionValid( std::string ext ) const
  {
    FileSystem::GuaranteeDot( ext );

    OS_ModifierSpecConstDumbPtr::Iterator itr = m_Specs.Begin();
    OS_ModifierSpecConstDumbPtr::Iterator end = m_Specs.End();
    for ( ; itr != end; ++itr )
    {
      const ModifierSpec* spec = *itr;
      if ( spec->GetModifier() == ext )
      {
        return true;
      }
    }

    return false;
  }

  /////////////////////////////////////////////////////////////////////////////
  // Helper function to add a modifier spec to the list.  This function is not
  // public because we want to limit what kinds of specs can be added to this
  // one.
  // 
  void FilterSpec::DoAddSpec( const ModifierSpec& spec )
  {
    FINDER_SCOPE_TIMER((""));

    if ( m_Specs.Append( &spec ) )
    {
      std::string filter = spec.GetFilter();

      // Keep value up to date
      if ( !m_Value.empty() && !filter.empty() )
      {
        m_Value += ";";
      }
      m_Value += filter;
    }
  }

}