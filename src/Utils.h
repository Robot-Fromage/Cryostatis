#pragma once


#include <filesystem>
#include <string>


std::wstring            getAppNameWithExtension();
std::wstring            getAppNameWithoutExtension();
std::filesystem::path   getCurrentDir();
std::filesystem::path   getTmpDir();
void                    getFilenamesFromCurrentDirectory( std::vector< std::wstring >& iVec );
void                    extractBinaryResourceToFile( std::wstring iStrCustomResName, int iNResourceId, std::wstring iStrDstDir, std::wstring iStrOutputName );
void                    createProcessFromCommandAndWaitForTermination( std::wstring iCommand );
void                    createProcessFromCommandAndProceed( std::wstring iCommand );