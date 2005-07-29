/****************************************************************************
** Form interface generated from reading ui file '_reports.ui'
**
** Created: Mon Jul 25 10:07:04 2005
**      by: The User Interface Compiler ($Id: qt/main.cpp   3.3.4   edited Nov 24 2003 $)
**
** WARNING! All changes made in this file will be lost!
****************************************************************************/

#ifndef _FORMBILLING_H
#define _FORMBILLING_H

#include <qvariant.h>
#include <qpixmap.h>
#include <qwidget.h>

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QSpacerItem;
class QFrame;
class QPushButton;
class QLabel;
class QComboBox;
class DQdbListView;
class DQdbLabel;
class QDateEdit;

class _FormBilling : public QWidget
{
    Q_OBJECT

public:
    _FormBilling( QWidget* parent = 0, const char* name = 0, WFlags fl = 0 );
    ~_FormBilling();

    QFrame* frame3;
    QPushButton* btn_1;
    QPushButton* btn_2;
    QLabel* textLabel1;
    QPushButton* btnExit;
    QPushButton* btnPrintView;
    QFrame* line1;
    QComboBox* comboOperator;
    QComboBox* comboIPAddress;
    QComboBox* comboType;
    DQdbListView* dbGrid;
    DQdbLabel* dbLabelTotal;
    QLabel* textLabel1_2;
    QLabel* textLabel3;
    QLabel* textLabel1_4;
    QLabel* textLabel2;
    QLabel* textLabel1_3;
    QLabel* textLabel2_2;
    QPushButton* btnProcessFilter;
    QPushButton* btnBackup;
    QPushButton* btnRestore;
    QDateEdit* dateAwal;
    QDateEdit* dateAkhir;

protected:

protected slots:
    virtual void languageChange();

private:
    QPixmap image0;
    QPixmap image1;
    QPixmap image2;

};

#endif // _FORMBILLING_H
