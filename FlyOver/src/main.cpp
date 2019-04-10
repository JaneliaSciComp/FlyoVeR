#include <QApplication>
#include <fstream>
using std::ifstream;
#include <iostream>
#include <Config_Memento.h>
#include <Console.h>
#include <sstream>

int main( int argc, char** argv )
{
   QApplication app(argc, argv);
   Q_INIT_RESOURCE( jovian );
   
   osg::ArgumentParser arguments( &argc, argv );

   Console* console = new Console( arguments );
   console->setGeometry( 100, 100, 800, 600 );

#ifdef FLYOVER_VERSION
   std::ostringstream versionString;
   versionString << FLYOVER_VERSION;
   std::string vstring(versionString.str());
   console->setVersionString(vstring);
#endif

   console->show();
   
   app.setQuitOnLastWindowClosed( true );

	char* fileName = getenv( "JOVIAN_CONFIG_FILE" );
	if ( fileName != NULL )
	{
		ifstream config( fileName );
		Config_Memento* cm = console->model()->current_configuration();
		cm->initialize( console );
		cm->load( config );

		console->load_config( cm );
		config.close();
	}

   return app.exec();
}
