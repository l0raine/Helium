#include "Precompile.h"
#include "DetailsFrame.h"

#include "Pipeline/Asset/AssetFile.h"
#include "Application/UI/ArtProvider.h"
#include "Application/RCS/RCS.h"

using namespace Luna;

static const i32 s_DisplayTimeSize = 32;

tstring TimeAsString( u64 time )
{
  tstring result;

  // try to get a printer friendly version of the datetime
  __time64_t timeT  = ( __time64_t ) ( time /*/ 1000*/ );

  tchar timePrint[s_DisplayTimeSize];
  if ( _tctime64_s( timePrint, s_DisplayTimeSize, &timeT ) == 0 )
  {
    // timeT
    result = timePrint;
    result = result.substr( 0, result.length() - 1 );
  }
  else
  {
    _stprintf( timePrint, TXT( "%ld" ), time );
    result = timePrint;
  }    
  return result;
}

tstring GetOperationString( RCS::Operation operation )
{
  switch ( operation )
  {
  case RCS::Operations::None:
    return TXT( "None" );

  case RCS::Operations::Add:
    return TXT( "Add" );

  case RCS::Operations::Edit:
    return TXT( "Edit" );

  case RCS::Operations::Delete:
    return TXT( "Delete" );

  case RCS::Operations::Branch:
    return TXT( "Branch" );

  case RCS::Operations::Integrate:
    return TXT( "Integrate" );

  case RCS::Operations::Unknown:
  default:
    return TXT( "Unknown" );
  }
}

tstring GetRevisionString( const RCS::Revision* revision )
{
  tstring str;
  str += revision->m_Username + TXT( "@" ) + revision->m_Client + TXT( " (" ) + GetOperationString( revision->m_Operation ) + TXT( ")\n" );
  str += TimeAsString( revision->m_Time ) + TXT( "\n" );
  str += revision->m_Description;
  return str;
}

DetailsFrame::DetailsFrame( wxWindow* parent )
: DetailsFrameGenerated( parent )
{
}

void DetailsFrame::Populate( Asset::AssetFile* file )
{
  tstring name = file->GetShortName() + file->GetExtension();
  SetTitle( name + TXT( " - " ) + GetTitle() );

  m_Name->SetValue( name );

  tstring fileType( Asset::AssetClass::GetAssetTypeName( file->GetAssetType() ) );
  if ( fileType == TXT( "Unknown" ) || fileType == TXT( "Null" ) )
  {
    fileType = file->GetFileType();
  }
  m_FileType->SetValue( fileType );

  m_FileID->SetValue( file->GetFilePath() );

  Nocturnal::Path folder( file->GetFilePath() );
  m_Folder->SetValue( folder.Directory() );

  bool gotRevisionInfo = false;
  RCS::File rcsFile( file->GetFilePath() );
  try
  {
    rcsFile.GetInfo( (RCS::GetInfoFlag)( RCS::GetInfoFlags::GetHistory | RCS::GetInfoFlags::GetIntegrationHistory ) );
    gotRevisionInfo = true;
  }
  catch ( const Nocturnal::Exception& e )
  {
    Log::Error( TXT( "%s\n" ), e.What() );
  }

  if ( gotRevisionInfo && rcsFile.ExistsInDepot() )
  {
    if ( rcsFile.IsCheckedOutByMe() )
    {
      m_RevisionStatusIcon->SetBitmap( wxArtProvider::GetBitmap( NOCTURNAL_UNKNOWN_ART_ID ) );
      m_RevisionStatus->SetLabel( TXT( "Checked out to you" ) );
    }
    else if ( rcsFile.IsCheckedOutBySomeoneElse() )
    {
      m_RevisionStatusIcon->SetBitmap( wxArtProvider::GetBitmap( NOCTURNAL_UNKNOWN_ART_ID ) );
      
      tstring usernames;
      rcsFile.GetOpenedByUsers( usernames );
      
      tstring message( TXT( "Checked out to: " ) );
      message += usernames;
      
      m_RevisionStatus->SetLabel( message );
    }
    else if ( !rcsFile.IsUpToDate() )
    {
      m_RevisionStatusIcon->SetBitmap( wxArtProvider::GetBitmap( NOCTURNAL_UNKNOWN_ART_ID ) );
      m_RevisionStatus->SetLabel( TXT( "Out of date" ) );
    }
    else
    {
      m_RevisionStatusIcon->SetBitmap( wxArtProvider::GetBitmap( NOCTURNAL_UNKNOWN_ART_ID ) );
      m_RevisionStatus->SetLabel( TXT( "Available for check out" ) );
    }

    if ( !rcsFile.m_Revisions.empty() )
    {
      const RCS::Revision* firstRevision = *rcsFile.m_Revisions.rbegin();
      const RCS::Revision* lastRevision = *rcsFile.m_Revisions.begin();

      if ( firstRevision == lastRevision )
      {
        m_LastCheckInPanel->Hide();
      }
      else
      {
        tstring lastCheckIn = GetRevisionString( lastRevision );
        m_LastCheckIn->SetValue( lastCheckIn );
        m_LastCheckInPanel->Show();
      }

      tstring firstCheckIn = GetRevisionString( firstRevision );
      m_FirstCheckIn->SetValue( firstCheckIn );
    }
  }
  else
  {
    m_RevisionStatus->SetLabel( TXT( "No Perforce information available" ) );
    m_RevisionStatusIcon->SetBitmap( wxArtProvider::GetBitmap( NOCTURNAL_UNKNOWN_ART_ID ) );
    m_LastCheckInPanel->Hide();
    m_FirstCheckInPanel->Hide();
  }

  Layout();
  GetSizer()->Layout();
}
