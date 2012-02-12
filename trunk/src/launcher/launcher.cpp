#include "Bootil/Bootil.h"
#include "bundler_header.h"

using namespace Bootil;

int main( int argc, char *argv[] )
{
	BString strCurrentDir		= Platform::CurrentDir();
	BString strCurrentProgram	= Platform::FullProgramName();

	//
	// Read the exe to a buffer
	//
	AutoBuffer buffer;
	if ( !File::Read( strCurrentProgram, buffer ) )
	{
		Output::Error( "Couldn't launch, sorry!" );
	}

	//
	// Read the header
	//
	BundlerHeader header;
	buffer.SetPos( buffer.GetWritten() - sizeof( BundlerHeader ) );
	buffer.Read( &header, sizeof( BundlerHeader ) );

	//
	// Compare the ident
	//
	if ( memcmp( header.ident, &BundlerIdent, sizeof( BundlerIdent ) ) != 0 )
	{
		Output::Error( "Couldn't launch - invalid ident [%i%i%i%i%i%i], sorry!", header.ident[0], header.ident[1], header.ident[2], header.ident[3], header.ident[4], header.ident[5] );
	}

	//
	// Find the zip file in the current program and open it from memory
	//
	buffer.SetPos( header.launchersize );
	Compression::Zip::File ZipFile( buffer.GetCurrent(), header.zipsize );
	if ( !ZipFile.IsOpen() )
	{
		Output::Error( "Couldn't launch - embedded zip file is invalid");
	}

	//
	// We're done with the buffer, free it
	//
	buffer.Clear();

	//
	// Create a temporary folder to extract the zip to
	//
	BString TempDir = Platform::TemporaryDir() + "/" + String::Random( 16, true, true, true ) ;
	File::CreateFolder( TempDir );

	//
	// Extract and close the zip (free memory)
	//
	ZipFile.ExtractToFolder( TempDir );
	ZipFile.Close();

	//
	// Run the process (launcher waits until the program is finished)
	//
	{
		Platform::ChangeDir( String::FileUtil::GetStripFilename( TempDir + "/"  + header.command) );
		Platform::StartProcess( (TempDir + "/"  + header.command).c_str(), true );
	}

	//
	// Clean up after ourselves
	//
	File::RemoveFolder( TempDir, true );	
	
}