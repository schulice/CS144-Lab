#include "address.hh"
#include "socket.hh"

#include <cstdlib>
#include <format>
#include <iostream>
#include <span>
#include <string>
#include <string_view>
#include <sys/socket.h>

using namespace std;

void get_URL( const string& host, const string& path )
{
  TCPSocket soc;
  soc.connect( Address( host, "http" ) );
  soc.write( std::format( "GET {} HTTP/1.1\r\n", path ) );
  soc.write( std::format( "Host: {}\r\n", host ) );
  soc.write( "Connection: close\r\n" );
  soc.write( "\r\n" ); // more line to end read
  while ( !soc.eof() ) {
    string buffer;
    soc.read( buffer );
    cout << buffer;
  }
  soc.close();
  soc.shutdown( SHUT_RDWR );
}

int main( int argc, char* argv[] )
{
  try {
    if ( argc <= 0 ) {
      abort(); // For sticklers: don't try to access argv[0] if argc <= 0.
    }

    auto args = span( argv, argc );

    // The program takes two command-line arguments: the hostname and "path" part of the URL.
    // Print the usage message unless there are these two arguments (plus the program name
    // itself, so arg count = 3 in total).
    if ( argc != 3 ) {
      cerr << "Usage: " << args.front() << " HOST PATH\n";
      cerr << "\tExample: " << args.front() << " stanford.edu /class/cs144\n";
      return EXIT_FAILURE;
    }

    // Get the command-line arguments.
    const string host { args[1] };
    const string path { args[2] };

    // Call the student-written function.
    get_URL( host, path );
  } catch ( const exception& e ) {
    cerr << e.what() << "\n";
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}