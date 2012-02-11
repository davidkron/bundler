#include "Bootil/Bootil.h"

using namespace Bootil;

bool BuildFromFolder( const BString& strRootFolder, Compression::Zip::File& zip )
{
	String::List files;
	File::GetFilesInFolder( strRootFolder, files, true );

	BOOTIL_FOREACH( f, files, String::List )
	{
		if ( !zip.AddFile( *f, strRootFolder + "/" + *f ) )
		{
			Output::Msg( "Couldn't add file %s\n", (strRootFolder + "/" + *f).c_str() );
		}
	}

	return true;
}

int main( int argc, char *argv[] )
{
	CommandLine::Set( argc, argv );

	if ( CommandLine::GetArgCount() == 0 )
	{
		Output::Msg( "No arguments\n" );
		return 0;
	}

	Compression::Zip::File	ZipFile;
	BString strFolderOrZip = CommandLine::GetArg( 0, "" );

	//
	// Create a zip from the folder
	//
	if ( File::IsFolder( strFolderOrZip ) )
	{
		if ( !BuildFromFolder( strFolderOrZip, ZipFile ) )
		{
			Output::Msg( "Error with folder\n" );
			return 0;
		}
	}

	ZipFile.Write( strFolderOrZip + ".zip" );

	Debug::PopupMessage( "HUUUH: %s", strFolderOrZip.c_str() );
}