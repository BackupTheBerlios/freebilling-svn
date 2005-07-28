/***************************************************************************
 *   Copyright (C) 2005 by Dreamware					   *
 *   support@dreamware.web.id						   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef FORMLOGIN_H
#define FORMLOGIN_H

#include <qthread.h>
#include <qsocket.h>

#include "_login.h"
#include "formclient.h"

class MyThread : public QThread
{
public :
	MyThread();
	~MyThread();
	void setDialog(QDialog *dialog);
protected :
	QDialog *dialog;
	virtual void run();
	
};


class FormLogin: public _FormLogin {
Q_OBJECT
public:
    FormLogin(QWidget *parent = 0, const char *name = 0);
    QDateTime *getStartDateTime();
public slots:
    virtual void btnStart_clicked();
    void writeLineToSocket(QString line);
    
    
private:
    MyThread session;
    QSocket socket;
    QString host;
    FormClient *fclient;
    QString user,ipaddress, serverport;;
    QString clienttype;
    QTimer pingTimer;
    QTimer waitTimer;
    
protected slots:
    void socketReadyRead();
    void socketConnected();
    void socketError(int);
    void connectionClosed();
    void onPingTimer();
    void onWaitTimer();
    




};

#endif
