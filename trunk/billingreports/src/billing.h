
#ifndef BILLING_H
#define BILLING_H

#include "_reports.h"
#include "dream/dconnection.h"
#include "dream/ddataset.h"
#include <dreport/reportlist.h>
#include "connect.h"

class FormBilling : public _FormBilling
{
  Q_OBJECT

public:
  FormBilling(QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
  ~FormBilling();
  
	DDataSet* dataset, *tmpdataset;
	ReportList *reportlist;  

public slots:
	void btnPrintViewClicked();
	void filterChanged();
	void filterChanged( int index );
	void filterChanged( const QDate & date );
	void backupAllPressed();
	void backupRestoreAllPressed();

};

#endif

