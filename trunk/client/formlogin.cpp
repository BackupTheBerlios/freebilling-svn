/***************************************************************************
 *   Copyright (C) 2005 by Dreamware  				      	   * 
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
#include "formlogin.h"
#include "formclient.h"
#include "_restriction.h"
#include "qapplication.h"
#include "formclient.h"
#include <qmessagebox.h>
#include <qhostaddress.h>
#include <qfile.h>
#include <qtextstream.h>
#include <sys/types.h>
#include <signal.h>
#include <qcombobox.h>
#include "protokol.h"

QString getPemakaian(int tipe)
{
	switch (tipe)
	{
	/* should be flexible ??? any idea ?? */
		case 0:
			return "10";
		case 1:
			return "20";
		case 2:
			return "30";
		case 3:
			return "40";
		case 4: 
			return "50";
		case 5:
			return "60";
		case 6: 
			return "70";
		case 7:
			return "80";
		case 8: 
			return "90";
		case 9:
			return "100";
		case 10:
			return "110";
		case 11:
			return "120";
		case 12: 
			return "0";
	}

}

QString getService(int tipe)
{
	switch (tipe)
	{
		case 0:
			return "INT";
		break;
		case 1:
			return "KTK";
		break;
	}

}

MyThread::MyThread()
: QThread(0)
{

}

MyThread::~MyThread()
{

}

void MyThread::setDialog(QDialog *dialog)
{
	this->dialog = dialog;
}

void MyThread::run()
{
//	setenv("FREEBILLING_SESSION","konsole",0);
	QString session = getenv("FREEBILLING_SESSION");
	system(session);
	QEvent *e = new QEvent((QEvent::Type)1100);
	qApp->postEvent(dialog,e);
	this->exit();
	

}

FormLogin::FormLogin(QWidget *parent, const char *name)
    :_FormLogin(parent, name)
{
//	setenv("FREEBILLING_CLIENT_TYPE","client",0);
//	setenv("FREEBILLING_SERVER","127.0.0.1",0);

	FILE *f;
	char line[1024];
	this->user = QString(getenv("LOGNAME"));
	clienttype = getenv("FREEBILLING_CLIENT_TYPE");
	QString command;
	if (clienttype=="ltsp")
	{
		command = "who | grep " + this->user +  " | grep -e \"[0-9]*\\.[0-9]*\\.[0-9]*\\.[0-9]*\" | wc -l";
		f = popen(command,"r");
		fgets(line,1024,f);
		if (QString(line).toInt()>1)
		{ 
			pclose(f);
			QMessageBox::information(this,"Informasi","Nama user "+this->user+" telah dipakai \nsilahkan login menggunakan nama user lain");
			exit(0);
		}		
		pclose(f);
		//command = "who | grep " + this->user + " | cut -d \" \" -f 3 | grep -e \"^[0-9]*\\.[0-9]*\\.[0-9]*\\.[0-9]*\"";
		command = "who | grep " + this->user + " | grep -e \"[0-9]*\\.[0-9]*\\.[0-9]*\\.[0-9]*\" | gawk -F \" \" '{ print $2 }' | gawk -F : '{ print $1 }'";
		qDebug(command);	
		f = popen(command,"r");
		if (fgets(line,1024,f))
		{
			this->ipaddress = line;
			this->ipaddress.remove("\n");
			qDebug(ipaddress);
		} else 
		{
			QMessageBox::information(this,"Peringatan","Tidak bisa menemukan IP address LTSP client");
			pclose(f);
		}
		pclose(f);
		this->host = "localhost";
	} 
	else 
	if (clienttype=="client")
	{
	//	this->user = "--none--";
		char *server;
		if (server = getenv("FREEBILLING_SERVER"))
			this->host = server;
		else
		{
			QMessageBox::information(this,"Peringatan","Variabel environmen FREEBILLING_SERVER belum di set");
			exit(0);
		}	
	
	} else 
	{
		QMessageBox::warning(this,"Peringatan","Variabel environmen FREEBILLING_CLIENT_TYPE belum diset\nSilahkan set dengan cara:\n- echo \"export FREEBILLING_CLIENT_TYPE=ltsp\" (untuk client LTSP)\n- echo \"export FREEBILLING_CLIENT_TYPE=client\" (untuk client normal)");
		exit(0);
	
	}
	
	
	comboPemakaian->setCurrentItem(12);
	connect(&socket,SIGNAL(connected()),this,SLOT(socketConnected()));
	connect(&socket,SIGNAL(error(int)),this,SLOT(socketError(int)));
	connect(&socket,SIGNAL(readyRead()),this,SLOT(socketReadyRead()));
	char *port;

	if (port = getenv("FREEBILLING_SERVER_PORT"))
		serverport = port;
	else
		serverport = "5000";
	fclient = NULL;
	connect(&pingTimer,SIGNAL(timeout()),this,SLOT(onPingTimer()));
	connect(&waitTimer,SIGNAL(timeout()),this,SLOT(onWaitTimer()));
		
}


void FormLogin::onPingTimer()
{
	this->writeLineToSocket("ping");
	this->waitTimer.start(20000,TRUE);
}

void FormLogin::onWaitTimer()
{
	QEvent *e = new QEvent((QEvent::Type)1101);
	if (this->fclient)
		qApp->postEvent(this->fclient,e);
	this->pingTimer.stop();

}

void FormLogin::btnStart_clicked()
{
	this->socket.connectToHost(this->host, serverport.toInt());
}

void FormLogin::socketReadyRead()
{
	while ( socket.canReadLine() ) 
	{
		QString line = socket.readLine();
		qDebug(line);	
		line.remove("\n");
		line.remove("\r");	
		QEvent *e = NULL;
#ifdef DREAMWARE_DEBUG
		printf("%s\n",line);
#endif
		if (line == "pong")
			this->waitTimer.stop();
		switch (convert(line.left(4))) 
		{
		case OK_1 : 
			// STARTING BILLING	
#ifdef DREAMWARE_DEBUG
		qDebug("Billing start\n");
#endif
			break;
		case ER_1 :
			// ERROR STARTING BILLING
#ifdef DREAMWARE_DEBUG
		qDebug("Billing Error\n");
#endif
			exit(0);
			break;
		case SN_2 :
			system("/opt/freebilling/fbmessage \"<b>PERINGATAN!!</b><br> Batas waktu yang telah anda tentukan hampir habis\" &");

			break;
		case SN_3 :
			system("/opt/freebilling/fbmessage \"<b>PERINGATAN!!</b><br> \
						Batas waktu yang telah anda tentukan telah habis<br>Abaikan peringatan ini jika anda masih ingin melanjutkan\" &");
			// Warning dari server waktu habis	
			break;
		case CS_1 :
			e = new QEvent((QEvent::Type)1101);
			if (this->fclient)
				qApp->postEvent(this->fclient,e);
			break;
		}
		
		
	}
}




void FormLogin::socketConnected()
{
	QString line = "SN01 ";
	line += ((clienttype == "client") ? ((QSocket *)sender())->address().toString() : (this->user + "@" + this->ipaddress)) + " "+ getPemakaian(this->comboPemakaian->currentItem()) + " " + getService(this->comboConnectionType->currentItem());
	
	this->writeLineToSocket(line);
	QString command = "ps -U " + this->user + " | cut -d \" \" -f 1 | grep -e [1-9][0-9]*";
	FILE *f = popen(command,"r");

	int temp[1024];
	char buff[1024];
	int count = 0;
	
	while (fgets(buff,1024,f))
	{
		temp[count++] = atoi(buff);
	}
		
	pclose(f);
	
	fclient = new FormClient(this,"formclient");
	session.setDialog(fclient);
	session.start();
	
	pingTimer.start(60000);
		
	this->hide();
	fclient->exec();

	writeLineToSocket("SN04");
	
	pingTimer.stop();

	if (!session.finished())
		session.terminate();
	delete fclient;

//this->show();
	
	f = popen(command,"r");
	
	while (fgets(buff,1024,f))
	{
		int i;
		bool flag = false;
		for (i=0;i<count;i++)
		{
		 	if (temp[i] == atoi(buff))
		 	{
		 		flag = true;
		 	}
		}
		if (!flag) kill(atoi(buff),9);
	}

	pclose(f);
		
	fclient = NULL;
	
	qApp->quit();
		
}

void FormLogin::socketError(int errcode)
{
	FormRestriction res;
	if (res.exec() == QDialog::Rejected)
		this->socket.connectToHost(this->host, serverport.toInt());
}

void FormLogin::writeLineToSocket(QString line)
{
	qDebug(line);
	QTextStream tx(&socket);
	tx << line << "\n";
}


void FormLogin::connectionClosed()
{
#ifdef DREAMWARE_DEBUG
	qDebug("closed here");
#endif
	QEvent *e = new QEvent((QEvent::Type)1101);
	if (this->fclient)
		qApp->postEvent(this->fclient,e);
}

