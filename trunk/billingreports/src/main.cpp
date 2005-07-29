#include <qapplication.h>
#include "billing.h"
#include "connect.h"

#include <qinputdialog.h>
#include "dream/dconnection.h"
#include "dream/ddataset.h"
#include "dream2/dutils.h"

int main( int argc, char ** argv ) 
{
        QApplication a( argc, argv );

	connection = new DConnection( "localhost" , "freebilling" , "freebilling" , "" );
	connection->open();
	
	DDataSet* dataset = new DDataSet( connection );
	dataset->setSQL( "SELECT * FROM tbl_admin WHERE username = 'admin'" );
	dataset->open();
	
	if ( md5Str( QInputDialog::getText( "Admin's Password" , "Password" , QLineEdit::Password ) ) == dataset->getFieldByName( "password" ) )
	{
		FormBilling * mw = new FormBilling();
		mw->setCaption( "Billing Reports" );
		mw->show();
		a.connect( &a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()) );
		return a.exec();
	}
}
