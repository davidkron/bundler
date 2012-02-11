#include "Bootil/Bootil.h"
#include "bundler_header.h"

using namespace Bootil;

bool BuildFromFolder( const BString& strRootFolder, Compression::Zip::File& zip, BString& strLauncher )
{
	String::List files;
	File::GetFilesInFolder( strRootFolder, files, true );

	BOOTIL_FOREACH( f, files, String::List )
	{
		if ( strLauncher.empty() && String::EndsWith( *f, ".exe" ) )
			strLauncher = *f;

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
	BString					EmbeddedExe;
	BString					OutputFilename;

	BString strFolderOrZip = CommandLine::GetArg( 0, "" );

	//
	// Create a zip from the folder
	//
	if ( File::IsFolder( strFolderOrZip ) )
	{
		if ( !BuildFromFolder( strFolderOrZip, ZipFile, EmbeddedExe ) )
		{
			Output::Msg( "Error with folder\n" );
			return 0;
		}

		OutputFilename = strFolderOrZip + ".exe";
	}
	else
	{
		// TODO: Handle zip file.
		return 0;
	}

	//
	// Did we find an executable file in the folder?
	//
	if ( EmbeddedExe.empty() )
	{
		Output::Msg( "No executable file found!\n" );
		return 0;
	}

	//
	// Write the zip file to a buffer
	//
	AutoBuffer ZipBuffer;
	ZipFile.WriteAndClose( ZipBuffer );

	//
	// Load launcher.exe into a buffer
	//
	AutoBuffer Launcher;
	if ( !File::Read( Platform::ProgramFolder() + "/launcher.exe", Launcher ) )
	{
		Output::Msg( "Couldn't find launcher.exe\n" );
		return 0;
	}

	//
	// Create the BundleHeader
	//
	BundlerHeader header;
		memset( &header, 0, sizeof(header) );
		memcpy( header.ident, BundlerIdent, sizeof(BundlerIdent) );
		header.launchersize = Launcher.GetWritten();
		header.zipsize = ZipBuffer.GetWritten();
		memcpy( header.command, EmbeddedExe.c_str(), EmbeddedExe.length() );

	//
	// Append the zip file and the head to the end of the launcher
	//
	Launcher.WriteBuffer( ZipBuffer );
	Launcher.Write( &header, sizeof(header) );	

	//
	// Save the launcher as OutputFilename
	//
	File::Write( OutputFilename, Launcher );

	//
	// Report success
	//
	Output::Msg( "Outputted: %s (%s)\n", OutputFilename.c_str(), String::Format::Memory( Launcher.GetWritten() ).c_str() );

	return 0;
}