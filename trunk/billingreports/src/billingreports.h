
#ifndef BILLINGREPORTS_H
#define BILLINGREPORTS_H

#include <qmainwindow.h>

class QTextEdit;

class billingreports: public QMainWindow
{
    Q_OBJECT

public:
    billingreports();
    ~billingreports();

protected:
    void closeEvent( QCloseEvent* );

private slots:
    void newDoc();
    void choose();
    void load( const QString &fileName );
    void save();
    void saveAs();
    void print();

    void about();
    void aboutQt();

private:
    QPrinter *printer;
    QTextEdit *e;
    QString filename;
};


#endif
