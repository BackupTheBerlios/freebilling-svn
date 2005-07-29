#include "dwidget/dqdbgrid.h"
#include "dwidget/dqdblabel.h"
#include <qdatetimeedit.h>
#include <qdatetime.h>
#include <qmessagebox.h> 
#include <qfiledialog.h> 
#include <qinputdialog.h>

#include "billing.h"

FormBilling::FormBilling(QWidget* parent, const char* name, WFlags fl)
: _FormBilling(parent,name,fl)
{
//MAIN DATASET
	dataset = new DDataSet( connection );
	dataset->setSQL( "SELECT oid, * FROM tbl_history_template ORDER BY tanggal" );
	dataset->open();
	
	dataset->getFieldDefs()->getFieldDef( "tanggal" )->setDisplayedFieldName( "Tanggal" );
	dataset->getFieldDefs()->getFieldDef( "logintime" )->setDisplayedFieldName( "Login" );
	dataset->getFieldDefs()->getFieldDef( "logouttime" )->setDisplayedFieldName( "Logout" );
	dataset->getFieldDefs()->getFieldDef( "lama" )->setDisplayedFieldName( "Waktu Pemakaian" );
	dataset->getFieldDefs()->getFieldDef( "jumlahpembayaran" )->setDisplayedFieldName( "Jumlah Pembayaran" );
	dataset->getFieldDefs()->getFieldDef( "servicetype" )->setDisplayedFieldName( "Jenis Pemakaian" );
	dataset->getFieldDefs()->getFieldDef( "status" )->setDisplayedFieldName( "Status" );
	dataset->getFieldDefs()->getFieldDef( "ipaddress" )->setDisplayedFieldName( "IP Address" );
	dataset->getFieldDefs()->getFieldDef( "operatorusername" )->setDisplayedFieldName( "Operator Username" );
	dataset->getFieldDefs()->getFieldDef( "jumlahpembayaran" )->setMoneyView( TRUE );
	
	dataset->setReadOnly( TRUE );
	
//FILTER INIT
//OPERATOR
	dateAwal->setDate( QDate::currentDate().addMonths( -1 ) );
	dateAkhir->setDate( QDate::currentDate() );
	
	tmpdataset = new DDataSet( connection );
//USER	
	tmpdataset->setTableName( "tbl_admin" );
	tmpdataset->open();
	tmpdataset->first();
	comboOperator->insertItem( "--ALL--" );
	
	while ( !tmpdataset->eof() )
	{
		comboOperator->insertItem( tmpdataset->getFieldByName( "username" ) );
		tmpdataset->next();
	}
	tmpdataset->close();
//IP	
	tmpdataset->setSQL( "SELECT ipaddress FROM tbl_ipaddress_alias GROUP BY ipaddress" );
	tmpdataset->open();
	tmpdataset->first();
	comboIPAddress->insertItem( "--ALL--" );
	
	while ( !tmpdataset->eof() )
	{
		comboIPAddress->insertItem( tmpdataset->getFieldByName( "ipaddress" ) );
		tmpdataset->next();
	}
	tmpdataset->close();
//TYPE	
	tmpdataset->setSQL( "SELECT servicetype FROM tbl_history_template GROUP BY servicetype" );
	tmpdataset->open();
	tmpdataset->first();
	comboType->insertItem( "--ALL--" );
	
	while ( !tmpdataset->eof() )
	{
		comboType->insertItem( tmpdataset->getFieldByName( "servicetype" ) );
		tmpdataset->next();
	}
	tmpdataset->close();
	
//DATEAWAL	
	tmpdataset->setSQL( "SELECT MIN(tanggal) AS tanggals FROM tbl_history_template" );
	tmpdataset->open();
	tmpdataset->first();
	
	dateAwal->setDate( QDate::fromString( tmpdataset->getFieldByName( "tanggals" ) , Qt::ISODate ).addDays( -1 ) );
	dateAwal->setOrder( QDateEdit::DMY );
	dateAwal->setAutoAdvance( TRUE );
	
	tmpdataset->close();
//TIMEAWAL
// 	timeAwal->setTime( QTime::fromString( "06:00:00" , Qt::ISODate ) );
	
//DATEAKHIR	
	tmpdataset->setSQL( "SELECT MAX(tanggal) AS tanggals FROM tbl_history_template" );
	tmpdataset->open();
	tmpdataset->first();
	
	dateAkhir->setDate( QDate::fromString( tmpdataset->getFieldByName( "tanggals" ) , Qt::ISODate ).addDays( 1 ) );
	dateAkhir->setOrder( QDateEdit::DMY );
	dateAkhir->setAutoAdvance( TRUE );
	
	tmpdataset->close();
//TIMEAKHIR
// 	timeAkhir->setTime( QTime::fromString( "06:00:00" , Qt::ISODate ) );

//SET DATE RANGE
	dateAwal->setRange( dateAwal->date().addDays( -2 ) , dateAkhir->date().addDays( 2 ) );
	dateAkhir->setRange( dateAwal->date().addDays( -2 ) , dateAkhir->date().addDays( 2 ) );
	
//TOTAL
	tmpdataset->setSQL( "SELECT sum(jumlahpembayaran) AS jumlah FROM tbl_history_template" );
	tmpdataset->open();
	tmpdataset->first();
	tmpdataset->getFieldDefs()->getFieldDef( "jumlah" )->setMoneyView( TRUE );
	
	dbLabelTotal->setDataSet( tmpdataset );
	dbLabelTotal->setFieldName( "jumlah" );
	
//EXECUTE FILTER
	filterChanged();
	
//MAIN GRID
	dbGrid->setDataSet( dataset );
	dbGrid->hideColumn( dataset->getFieldIndex( "oid" ) );
	
//REPORTS
	REPORTLIST_INIT(reportlist,freebilling,/opt/dreamware/reports)
	
	connect( btnProcessFilter , SIGNAL( clicked() ) , this , SLOT( filterChanged() ) ); 
	connect( comboOperator , SIGNAL( activated( int ) ) , this , SLOT( filterChanged( int ) ) ); 
	connect( comboIPAddress , SIGNAL( activated( int ) ) , this , SLOT( filterChanged( int ) ) ); 
	connect( comboType , SIGNAL( activated( int ) ) , this , SLOT( filterChanged( int ) ) ); 
	connect( dateAwal , SIGNAL( valueChanged( QDate ) ) , this , SLOT( filterChanged( QDate ) ) ); 
	connect( dateAkhir , SIGNAL( valueChanged( QDate ) ) , this , SLOT( filterChanged( QDate ) ) ); 
	
	connect( btnPrintView , SIGNAL( clicked() ) , this , SLOT( btnPrintViewClicked() ) );
	connect( btnExit, SIGNAL( clicked() ) , this , SLOT( close() ) );
	
	connect( btnBackup , SIGNAL( clicked() ) , this , SLOT( backupAllPressed() ) );
	connect( btnRestore , SIGNAL( clicked() ) , this , SLOT( backupRestoreAllPressed() ) );
}

void FormBilling::filterChanged()
{
	QString sql;

//MAIN DATASET QUERY
	sql = "SELECT oid, * FROM tbl_history_template WHERE ";
	
	sql += " tanggal BETWEEN '"+dateAwal->date().toString( Qt::ISODate )+"' AND '"+dateAkhir->date().toString( Qt::ISODate )+"' ";
	
	if ( !(comboOperator->currentText() == "--ALL--") ) sql += "AND operatorusername = '"+comboOperator->currentText()+"' ";
	if ( !(comboIPAddress->currentText() == "--ALL--") ) sql += "AND ipaddress = '"+comboIPAddress->currentText()+"' ";
	if ( !(comboType->currentText() == "--ALL--") ) sql += "AND servicetype = '"+comboType->currentText()+"' ";
	
	sql += "ORDER BY tanggal";
	
	dataset->setSQL( sql );
	dataset->refresh();
	
//TOTAL DATASET QUERY
	sql = "SELECT sum(jumlahpembayaran) AS jumlah FROM tbl_history_template WHERE ";
	
	sql += " tanggal BETWEEN '"+dateAwal->date().toString( Qt::ISODate )+"' AND '"+dateAkhir->date().toString( Qt::ISODate )+"' ";
	
	if ( !(comboOperator->currentText() == "--ALL--") ) sql += "AND operatorusername = '"+comboOperator->currentText()+"' ";
	if ( !(comboIPAddress->currentText() == "--ALL--") ) sql += "AND ipaddress = '"+comboIPAddress->currentText()+"' ";
	if ( !(comboType->currentText() == "--ALL--") ) sql += "AND servicetype = '"+comboType->currentText()+"' ";
	
	tmpdataset->setSQL( sql );
	tmpdataset->refresh();
}

void FormBilling::filterChanged( int index )
{
	filterChanged();
}

void FormBilling::filterChanged( const QDate & date )
{
	filterChanged();
}

void FormBilling::btnPrintViewClicked()
{
	reportlist->show();
}

void FormBilling::backupAllPressed() 
{ 
	bool ok; 
	QString server = QInputDialog::getText( "Backup Sistem dari Server" , "Masukkan Nama Server atau nomor IP :", QLineEdit::Normal, "localhost", &ok, this ); 

	if ( ok && !server.isEmpty() ) { 
		QString filename = QFileDialog::getSaveFileName( 
			QDir::homeDirPath(), 
			"SQL (*.sql)", 
			this, 
			"Save file dialog", 
			"Insert Backup File Name" ); 

		if (filename != "" )  
		{ 

			if ( filename.find( ".sql" , 0 , FALSE ) >= -1 ) filename.remove( ".sql" ); 

			filename += ".sql"; 

			system( "pg_dump -U dreamware -h "+server+" freebilling -c > "+filename ); 

			QMessageBox::information( this , "Backup" , "Semua Data Sistem FreeBilling di Server "+server+" telah di simpan di "+filename , 1 ); 
		} 
	} else { 
		QMessageBox::information( this , "Backup" , "Backup Di batalkan" , 1 ); 
	} 
} 
 
void FormBilling::backupRestoreAllPressed() 
{ 
	bool ok; 
	QString server = QInputDialog::getText( "Mengembalikan Data ke Server" , "Masukkan Nama Server atau nomor IP :", QLineEdit::Normal, "localhost", &ok, this ); 

	if ( ok && !server.isEmpty() ) { 
	QString filename = QFileDialog::getOpenFileName( 
			QDir::homeDirPath(), 
			"SQL (*.sql)", 
			this, 
			"Open file dialog", 
			"Insert Backup File Name" ); 

		if (filename != "" )  
		{ 
			system( "psql -U dreamware -h "+server+" freebilling < "+filename ); 
			QMessageBox::information( this , "Backup" , "Data FreeBilling di Server "+server+" telah dikembalikan dari file "+filename , 1 ); 
		} 
	} else { 
		QMessageBox::information( this , "Backup" , "Pengembalian Data Dibatalkan" , 1 ); 
	} 
} 
 
FormBilling::~FormBilling()
{
}

/*$SPECIALIZATION$*/


