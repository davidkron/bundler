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


	Debug::PopupMessage( "OK" );
	
	
}