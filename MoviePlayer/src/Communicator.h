#ifndef __COMMUNICATOR__
#define __COMMUNICATOR__

#include <iostream>
#include <string>

#ifndef _WINDOWS
#include <unistd.h>
#endif

#include <boost/array.hpp>
#include <boost/asio.hpp>

class Communicator
{
public:

   Communicator( std::string host = "localhost", std::string port = "22222", int retries = 2 ): success( false ), reply_length( 0 ) {}
   virtual ~Communicator() {}

   virtual void reset( void ) = 0;
   virtual void read( void ) = 0;
   virtual void close( void ) = 0;

   char* data( void ) { return buf.data(); }
   size_t message_length( void ) { return reply_length; }
   
   bool success;
   boost::asio::io_service io_service;
   
protected:
   size_t reply_length;
   boost::array<char, 128> buf;
   boost::system::error_code error;
};


class VoidCommunicator: public Communicator
{
public:

   VoidCommunicator( std::string host = "localhost", std::string port = "22222", int retries = 2 ): 
      Communicator( host, port, retries ) {}
   virtual ~VoidCommunicator() {}

   void reset( void ) {}
   void read( void ) {}
   void close( void ) {}

};


class TCPCommunicator: public Communicator
{
public:

   TCPCommunicator( std::string host = "localhost", std::string port = "22222", int retries = 2 );

   virtual ~TCPCommunicator() { socket.close(); }

   void reset( void ) { socket.connect( *endpoint_iterator ); }
   void read( void );
   
   void close( void ) { socket.close(); }

private:
   boost::asio::ip::tcp::socket socket;
   boost::asio::ip::tcp::resolver::iterator endpoint_iterator;

};


class UDPCommunicator: public Communicator
{
public:

   UDPCommunicator( std::string host = "localhost", std::string port = "22222", int retries = 2 );   
   virtual ~UDPCommunicator() {}

   void reset( void ) {}

   void read( void );

   void close( void ) {}

private:
   boost::asio::ip::udp::socket socket;
   boost::asio::ip::udp::resolver::iterator endpoint_iterator;
   
};


#endif
