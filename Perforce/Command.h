#pragma once

#ifndef P4CLIENTAPI_H
# define P4CLIENTAPI_H
# pragma warning (disable : 4267 4244)
# include "p4/clientapi.h"
# pragma warning (default : 4267 4244)
#endif

#include "Provider.h"
#include "Exceptions.h"
#include "Windows/Windows.h"
#include "Windows/Thread.h"
#include "Platform/Thread.h"
#include "Profile/Timer.h"
#include "RCS/Types.h"

#define PERFORCE_MAX_DICT_ENTRIES 64

namespace Perforce
{
  //
  // Command transaction thread object
  //

  namespace CommandPhases
  {
    enum CommandPhase
    {
      Unknown,
      Executing,
      Connecting,
      Complete,
    };
  };
  typedef CommandPhases::CommandPhase CommandPhase;

  class Command : public ClientUser, public KeepAlive, public WaitInterface
  {
  public:
    Command( Provider* provider, const char* command = "" )
      : m_Phase ( CommandPhases::Unknown )
      , m_Abort ( false )
      , m_Provider( provider )
      , m_Command( command )
      , m_ErrorCount( 0 )
    {

    }

    virtual void Run();

  private:
    bool Connect();
    void Execute();
    virtual int	IsAlive();
    virtual bool StopWaiting();

  public:
    void AddArg( const std::string& arg )
    {
      m_Arguments.push_back( arg.c_str() );
    }
    void AddArg( const char* arg )
    {
      m_Arguments.push_back( arg );
    }

    virtual void HandleError( Error* error );
    virtual void OutputStat( StrDict* dict );

    std::string AsString();

  private:
    Platform::Thread  m_Thread;
    Profile::Timer    m_ConnectTimer;
    CommandPhase      m_Phase;
    bool              m_Abort;

  protected:
    Provider*         m_Provider;
    const char*       m_Command;
    V_string          m_Arguments;
    int               m_ErrorCount;
    std::string       m_ErrorString;
  };


  //
  // Translate string to enum
  //
  inline RCS::Operation GetOperationEnum( const std::string &operation )
  {
    if( operation == "add" )
      return RCS::Operations::Add;
    else if( operation == "delete" )
      return RCS::Operations::Delete;
    else if( operation == "edit" )
      return RCS::Operations::Edit;
    else if( operation == "branch" )
      return RCS::Operations::Branch;
    else if( operation == "integrate" )
      return RCS::Operations::Integrate;
    else if( operation == "" )
      return RCS::Operations::None;

    return RCS::Operations::Unknown;
  }

  inline RCS::FileType GetFileType( const std::string& fileType )
  {
    if ( fileType.find( "binary" ) != std::string::npos )
    {
      return RCS::FileTypes::Binary;
    }
    else if ( fileType.find( "text" ) != std::string::npos )
    {
      return RCS::FileTypes::Text;
    }

    return RCS::FileTypes::Unknown;
  }

  template <class ResultType>
  inline RCS::Operation ResultAsOperation( ResultType& results, int i )
  {
    return GetOperationEnum( ResultAsString( results, i ) );
  }

  inline void SetFlags( const std::string& flags, RCS::File* info )
  {
    info->m_Flags = 0;
    if ( flags.find_first_of( "l" ) != flags.npos )
      info->m_Flags |= RCS::FileFlags::Locking;
    if ( flags.find_first_of( "S" ) != flags.npos )
      info->m_Flags |= RCS::FileFlags::HeadOnly;
  }
}