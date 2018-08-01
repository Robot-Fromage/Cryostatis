#include "Utils.h"

#include <assert.h>
#include <fstream>
//#include <iostream>
//#include <sstream>
#include <windows.h>


using namespace std;
namespace fs = filesystem;

wstring
getAppNameWithExtension()
{
    wstring  wstr;
    wchar_t  tmp[MAX_PATH];
    if( GetModuleFileName( NULL, tmp, MAX_PATH ) )
    {
        wstr = tmp;
        wstring  app_name_with_extension = fs::path( wstr ).filename().wstring();
        return  app_name_with_extension;
    }
    else
    {
        return  wstring( L"" );
    }
}

wstring
getAppNameWithoutExtension()
{
    wstring     wstr        = getAppNameWithExtension();
    if( wstr == wstring( L"" ) )
    {
        return  wstring( L"" );
    }
    else
    {
        size_t      lastindex   = wstr.find_last_of( L"." );
        wstring     rawname     = wstr.substr( 0, lastindex );
        return      rawname;
    }

}

fs::path
getCurrentDir()
{
    wstring wstr;
    wchar_t tmp[MAX_PATH];
    if (GetModuleFileName(NULL, tmp, MAX_PATH))
        wstr = tmp;

    return  fs::path(wstr).parent_path();
}

fs::path
getTmpDir()
{
    wstring wstr;
    wchar_t tmp[MAX_PATH];
    if (GetTempPathW(MAX_PATH, tmp))
        wstr = tmp;

    return  fs::path(wstr);
}


void
getFilenamesFromCurrentDirectory(vector< wstring >& iVec)
{
    iVec.clear();
    for( auto & p : fs::directory_iterator( getCurrentDir() ) )
        iVec.push_back( p.path().filename().wstring() );
}


void
extractBinaryResourceToFile( wstring iStrCustomResName, int iNResourceId, wstring iStrDstDir, wstring iStrOutputName )
{
    HGLOBAL hResourceLoaded;        // handle to loaded resource 
    HRSRC hRes;                     // handle/ptr. to res. info. 
    char *lpResLock;                // pointer to resource data 
    DWORD dwSizeRes;
    wstring strOutputLocation;
    wstring strOutDir;

    // lets get the location
    if( iStrDstDir == L"" )
    {
        strOutDir = getCurrentDir();
    }
    else
    {
        if( !fs::exists( fs::path( iStrDstDir ) ) )
            strOutDir = getCurrentDir();
        else
            strOutDir = iStrDstDir;
    }

    strOutputLocation = strOutDir += L"\\";
    strOutputLocation += iStrOutputName;

    // find location of the resource and get handle to it
    hRes = FindResource( NULL, MAKEINTRESOURCE( iNResourceId ), iStrCustomResName.c_str() );

    // loads the specified resource into global memory. 
    hResourceLoaded = LoadResource( NULL, hRes );

    // get a pointer to the loaded resource!
    lpResLock = (char*)LockResource( hResourceLoaded );

    // determine the size of the resource, so we know how much to write out to file!  
    dwSizeRes = SizeofResource( NULL, hRes );

    ofstream outputFile( strOutputLocation.c_str(), ios::binary );

    outputFile.write( (const char*)lpResLock, dwSizeRes );
    outputFile.close();
}


void
createProcessFromCommandAndWaitForTermination( wstring iCommand )
{
    // Process Data
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    // Init Structures
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Start Program from CMD
    BOOL result = CreateProcess( NULL,   // the path
                                (wchar_t*)iCommand.c_str(), // Command line
                                 NULL,   // Process handle not inheritable
                                 NULL,   // Thread handle not inheritable
                                 FALSE,  // Set handle inheritance to FALSE
                                 0,      // No creation flags
                                 NULL,   // Use parent's environment block
                                 NULL,   // Use parent's starting directory 
                                 &si,    // Pointer to STARTUPINFO structure
                                 &pi     // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
    );

    // Successfully created the process.  Wait for it to finish.
    WaitForSingleObject(pi.hProcess, INFINITE);

    // Get the exit code.
    DWORD exitCode;
    result = GetExitCodeProcess(pi.hProcess, &exitCode);

    // Close process and thread handles.
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}


void
createProcessFromCommandAndProceed( std::wstring iCommand )
{
    // Process Data
    STARTUPINFO si;
    PROCESS_INFORMATION pi;

    // Init Structures
    ZeroMemory(&si, sizeof(si));
    si.cb = sizeof(si);
    ZeroMemory(&pi, sizeof(pi));

    // Start Program from CMD
    BOOL result = CreateProcess( NULL,   // the path
                                (wchar_t*)iCommand.c_str(), // Command line
                                 NULL,   // Process handle not inheritable
                                 NULL,   // Thread handle not inheritable
                                 FALSE,  // Set handle inheritance to FALSE
                                 0,      // No creation flags
                                 NULL,   // Use parent's environment block
                                 NULL,   // Use parent's starting directory 
                                 &si,    // Pointer to STARTUPINFO structure
                                 &pi     // Pointer to PROCESS_INFORMATION structure (removed extra parentheses)
    );

    // Close process and thread handles.
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
}

