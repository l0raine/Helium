#include "Precompile.h"
#include "Tracker.h"

#include "Pipeline/Asset/AssetClass.h"
#include "Foundation/File/Path.h"
#include "Foundation/Component/SearchableProperties.h"
#include "Foundation/String/Wildcard.h"
#include "Foundation/Version.h"

using namespace Helium;
using namespace Helium::Editor;

///////////////////////////////////////////////////////////////////////////////
// Sleep between runs and yield to other threads
// The complex loop is to prevent Editor from hanging on exit (max hang will be "increments" seconds)
inline void SleepBetweenTracking( bool* cancel = NULL, const uint32_t minutes = 1 )
{
    const uint32_t increments = 5;
    uint32_t totalSeconds = 60 * minutes;
    for ( uint32_t seconds = 0; seconds < totalSeconds; seconds += increments )
    {
        Sleep( increments * 1000 );

        if ( ( cancel != NULL )
            && *cancel )
        {
            break;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
int Tracker::s_InitCount = 0;
Helium::InitializerStack Tracker::s_InitializerStack;

Tracker::Tracker()
: m_TrackerDB( NULL )
, m_StopTracking( false )
, m_InitialIndexingCompleted( false )
, m_IndexingFailed( false )
, m_Total( 0 )
, m_CurrentProgress( 0 )
{
    if ( ++s_InitCount == 1 )
    {
        //s_InitializerStack.Push( Reflect::RegisterClassType< AssetIndexData >( TXT( "Editor::AssetIndexData" ) ) );
    }
}

Tracker::~Tracker()
{
    if ( IsThreadRunning() )
    {
        StopThread();
    }

    delete m_TrackerDB;

    if ( --s_InitCount == 0 )
    {
        //s_InitializerStack.Cleanup();
    }
    HELIUM_ASSERT( s_InitCount >= 0 );
}

void Tracker::SetProject( Project* project )
{
    bool restartThread = false;
    if ( IsThreadRunning() )
    {
        restartThread = true;
        StopThread();
    }

    if ( m_TrackerDB )
    {
        delete m_TrackerDB;
        m_TrackerDB = NULL;
    }

    m_Project = project;

    if ( m_Project )
    {
        Path dbPath = m_Project->GetTrackerDB();

        if ( !dbPath.MakePath() )
        {
            throw Helium::Exception( TXT( "Could not create database directory: %s" ), dbPath.Directory().c_str() );
        }

        tstring dbSpec = tstring( TXT( "database=" ) ) + dbPath.Get();
        m_TrackerDB = new TrackerDBGenerated( TXT("sqlite3"), dbSpec.c_str() );

        if ( restartThread )
        {
            StartThread();
        }
    }
}

const Project* Tracker::GetProject() const
{
    return m_Project;
}

void Tracker::StartThread()
{
    HELIUM_ASSERT( !IsThreadRunning() );
    HELIUM_ASSERT( m_Project );

    m_StopTracking = false;

    Helium::CallbackThread::Entry entry = &Helium::CallbackThread::EntryHelper<Tracker, &Tracker::TrackEverything>;
    if ( !m_Thread.Create( entry, this, TXT( "Tracker Thread" ), Thread::PRIORITY_LOW ) )
    {
        throw Exception( TXT( "Unable to create thread for asset tracking." ) );
    }
}

void Tracker::StopThread()
{
    HELIUM_ASSERT( IsThreadRunning() );

    m_StopTracking = true;

    m_Thread.Join();
}

bool Tracker::IsThreadRunning()
{
    return ( m_Thread.IsRunning() );
}

void Tracker::TrackEverything()
{
    HELIUM_ASSERT( m_TrackerDB );
    HELIUM_ASSERT( m_Project );

    m_StopTracking = false;
    m_InitialIndexingCompleted = false;
    m_IndexingFailed = false;

    // create tables, sequences and indexes
    m_TrackerDB->verbose = true;

    try
    {
        m_TrackerDB->create();
    }
    catch( const litesql::SQLError& )
    {
        m_TrackerDB->upgrade();
    }

    std::set< Helium::Path > assetFiles;
    while ( !m_StopTracking )
    {
        Log::Print( m_InitialIndexingCompleted ? Log::Levels::Verbose : Log::Levels::Default,
            m_InitialIndexingCompleted ? TXT("Tracker: Looking for new or updated files...\n") : TXT("Tracker: Finding asset files...\n" ));

        // find all the files in the project
        {
            SimpleTimer timer;
            Helium::Directory directory( m_Project->a_Path.Get().Directory() );
            directory.GetFiles( assetFiles, true );
            Log::Print( m_InitialIndexingCompleted ? Log::Levels::Verbose : Log::Levels::Default, TXT("Tracker: File reslover database lookup took %.2fms\n"), timer.Elapsed() );
        }

        // for each file
        m_CurrentProgress = 0;
        m_Total = (uint32_t)assetFiles.size();

        SimpleTimer timer;
        Log::Print( m_InitialIndexingCompleted ? Log::Levels::Verbose : Log::Levels::Default, TXT("Tracker: Scanning %d asset file(s) for changes...\n"), (uint32_t)assetFiles.size() );

        for( std::set< Helium::Path >::const_iterator assetFileItr = assetFiles.begin(), assetFileItrEnd = assetFiles.end();
            !m_StopTracking && assetFileItr != assetFileItrEnd; ++assetFileItr )
        {
            Log::Listener listener ( ~Log::Streams::Error );
            ++m_CurrentProgress;

            // see if the file has changed
            // insert/update the file: path, timestamp, etc...
            const Helium::Path& assetFilePath = (*assetFileItr);

#pragma TODO( "Make a configurable list of places to ignore" )
            // skip files in the meta directory
            if ( assetFilePath.IsUnder( m_Project->a_Path.Get().Directory() + TXT( ".Helium/" ) ) )
            {
                continue;
            }

            TrackedFile assetTrackedFile( *m_TrackerDB );

            // start transaction
            m_TrackerDB->begin();
            try
            {
                try
                {
                    assetTrackedFile = litesql::select<TrackedFile>( *m_TrackerDB, TrackedFile::MPath == assetFilePath.Get() ).one();
                    if ( assetFilePath.ChangedSince( assetTrackedFile.mLastModified.value().timeStamp() ) || assetTrackedFile.mToolsVersion != HELIUM_VERSION_NUMBER )
                    {
                        assetTrackedFile.properties().del();
                        assetTrackedFile.fileReferences().del();
                    }
                    else
                    {
                        continue;
                    }
                }
                catch( const litesql::NotFound& )
                {
                    Log::Debug( TXT("Caught litesql::NotFound excption when selecting file from DB" ));
                }

                if ( WildcardMatch( TXT( "Helium*" ), assetFilePath.Extension().c_str() ) )
                {
                    const Asset::AssetClassPtr assetClass = Asset::AssetClass::LoadAssetClass( assetFilePath );
                    if ( assetClass.ReferencesObject() )
                    {
                        // get file's properties
                        Helium::SearchableProperties fileProperties;
                        assetClass->GatherSearchableProperties( &fileProperties );
                        for( std::multimap< tstring, tstring >::const_iterator filePropertiesItr = fileProperties.GetStringProperties().begin(),
                            filePropertiesItrEnd = fileProperties.GetStringProperties().end();
                            filePropertiesItr != filePropertiesItrEnd; ++filePropertiesItr )
                        {
                            //TrackedProperty
                            TrackedProperty prop( *m_TrackerDB );
                            prop.mName = filePropertiesItr->first;
                            prop.update();

                            assetTrackedFile.properties().link( prop, filePropertiesItr->second );
                        }

                        // get file's dependencies
                        std::set< Helium::Path > fileReferences;
                        assetClass->GetFileReferences( fileReferences );
                        for( std::set< Helium::Path >::const_iterator fileRefsItr = fileReferences.begin(),
                            fileRefsItrEnd = fileReferences.end();
                            fileRefsItr != fileRefsItrEnd; ++fileRefsItr )
                        {
                            //   see if the file has changed
                            const Helium::Path& fileRefPath = (*fileRefsItr);

                            TrackedFile fileRefTrackedFile( *m_TrackerDB );
                            fileRefTrackedFile.mPath = fileRefPath.Get();
                            fileRefTrackedFile.update();

                            assetTrackedFile.fileReferences().link( fileRefTrackedFile );
                        }
                    }
                }

                assetTrackedFile.mBroken = 0;
            }
            catch ( const Helium::Exception& e )
            {
                Log::Error( TXT( "Exception in Tracker thread: %s" ), e.What() );
                assetTrackedFile.mBroken = 1;
            }
            catch ( ... )
            {
                Log::Error( TXT( "Unknown exception in Tracker thread." ) );

                m_TrackerDB->rollback();

                // the consequences could never be the same here, rethrow
                throw;
            }

            // update LastModified
            assetTrackedFile.mPath = assetFilePath.Get();
            assetTrackedFile.mSize = (int32_t) assetFilePath.Size();
            assetTrackedFile.mLastModified = litesql::DateTime( assetFilePath.ModifiedTime() );
            assetTrackedFile.mToolsVersion = HELIUM_VERSION_NUMBER;
            assetTrackedFile.update();

            // commit transaction
            m_TrackerDB->commit();
        }

        if ( m_StopTracking )
        {
            uint32_t percentComplete = (uint32_t)(((float32_t)m_CurrentProgress/(float32_t)m_Total) * 100);
            Log::Print( m_InitialIndexingCompleted ? Log::Levels::Verbose : Log::Levels::Default, TXT("Tracker: Indexing (%d%% complete) pre-empted after %.2fm\n"), percentComplete, timer.Elapsed() / 1000.f / 60.f );
        }
        else if ( !m_InitialIndexingCompleted )
        {
            m_InitialIndexingCompleted = true;
            Log::Print( TXT("Tracker: Initial indexing completed in %.2fm\n"), timer.Elapsed() / 1000.f / 60.f );
        }
        else 
        {
            Log::Print( Log::Levels::Verbose, TXT("Tracker: Indexing updated in %.2fm\n") , timer.Elapsed() / 1000.f / 60.f );
        }

        m_Total = 0;
        m_CurrentProgress = 0;

        ////////////////////////////////
        // Recurse
        if ( !m_StopTracking )
        {
            // Sleep between runs and yield to other threads
            // The complex loop is to prevent Editor from hanging on exit (max hang will be "increments" seconds)
            SleepBetweenTracking( &m_StopTracking );
        }
    }
}

bool Tracker::InitialIndexingCompleted() const
{
    return m_InitialIndexingCompleted;
}

bool Tracker::DidIndexingFail() const
{
    return m_IndexingFailed;
}

uint32_t Tracker::GetCurrentProgress() const
{
    return m_CurrentProgress;
}

uint32_t Tracker::GetTrackingTotal() const 
{
    return m_Total;
}
