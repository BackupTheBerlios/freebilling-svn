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

#include "bserversocket.h"
#include <qsocket.h>
#include <qsqlquery.h>
#include <qiodevice.h>
#include "protokol.h"



BServerSocket::BServerSocket(Q_UINT16 port, int maxclient ) 
:QServerSocket(port,maxclient)
{
	db = QSqlDatabase::addDatabase("QPSQL7");
	db->setDatabaseName("freebilling");
	db->setUserName("freebilling");
	db->setPassword("");
	db->setHostName("localhost");
	
	if (!db->open())
	{
		qDebug("Error Open Database");
		exit(0);
	}
	
	QSqlQuery query("SELECT cleanup();",db);
	
	connect(&timer,SIGNAL(timeout()),this,SLOT(onTimer()));
	timer.start(60000);
}

BServerSocket::~BServerSocket()
{

}

void BServerSocket::newConnection( int socket )
{
	QSocket *s = new QSocket(this);
	connect(s,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
	connect(s,SIGNAL(connectionClosed()),this,SLOT(onConnectionClosed()));
	s->setSocket(socket);

}

void BServerSocket::onReadyRead()
{
	QSocket *s = (QSocket *)sender();
	if (s->canReadLine())
	{
		QString line = s->readLine();
		qDebug(line);
		line.remove("\n");
		line.remove("\r");
		if (line == "ping")
			writeLineToSocket("pong",s);

		QSqlQuery query(db);
		QString sql;
		QString ipaddress;
		QString username;
		QStringList tokens = QStringList::split(" ",line);
		QString iptablescmd = "iptables ";
		switch (convert(tokens[0]))
		{
		case SN_1 :
			// tokens[1] -> ip / user@ip, tokens[2]-> menit (15,30,60,90,120,99999999), tokens[3]-> tipe (INT,KTK)
			//masukkan ke database karena ada yang login
			//cek disini validitas data klo sala kembalikan pesan error ER_1 / "ER01"
			//send ke client OK_1 / "OK01"
			
			if (tokens[1].contains("@"))
			{
				username = tokens[1].section("@",0,0);
				ipaddress = tokens[1].section("@",1,1); 
				if (tokens[3]=="INT")
				{	
					//iptablescmd += "-D -o $FREEBILLING_IFACE -p tcp -m owner --uid-owner=";
					//iptablescmd += username; /* ganti ke uid */ 
					//iptablescms += " -j DROP";
					//printf("%s\n",iptablescmd.ascii());
					//system (iptablescmd);
				} else
				{
					
					//iptablescmd += "-A -o $FREEBILLING_IFACE -p tcp -m owner --uid-owner=";
					//iptablescmd += username; /* ganti ke uid */ 
					//iptablescmd += " -j DROP";	
					//printf("%s\n",iptablescmd.ascii());
					//system (iptablescmd);
				}


			} else
			{
				
				ipaddress = tokens[1];
				if (tokens[3]=="INT")
				{	
					iptablescmd += "-A POSTROUTING -t nat -s ";
					iptablescmd += ipaddress;
					iptablescmd += " -o $FREEBILLING_IFACE -j MASQUERADE";
					printf("%s\n",iptablescmd.ascii());
					system (iptablescmd);
				} else
				{
					iptablescmd += "-D POSTROUTING -t nat -s ";
					iptablescmd += ipaddress;
					iptablescmd += " -o $FREEBILLING_IFACE -j MASQUERADE";	
					printf("%s\n",iptablescmd.ascii());
					system (iptablescmd);
				}
			}
			
			s->setName(ipaddress);
			
			sql = "SELECT computer_login('" + ipaddress + "','" + tokens[3] + "'," + tokens[2] + ");";

			query.exec(sql);	
			
			writeLineToSocket("OK01",s);
		break;
		case SN_4 :
			query.exec(QString("SELECT computer_logout('") + s->name() + QString("');"));
			s->close();
			s->deleteLater();
		break;
		case CS_1 :
			//resend this message to client
			this->writeLineToSocket("CS01",(QSocket *)this->child(tokens[1]));
			s->close();
			s->deleteLater();
		break;
		}
	}
}

void BServerSocket::writeLineToSocket(QString line, QSocket *s)
{
	QTextStream tx((QIODevice *)s);
	tx << line << "\n";
}


void BServerSocket::onConnectionClosed()
{
	QSocket *s = (QSocket *)sender();
	qDebug("Closed here");

//	system("echo testing > /tmp/serverini");
	QSqlQuery query(QString("SELECT computer_logout('") + s->name() + QString("');"), db);
	
	s->close();
	s->deleteLater();
	
}


void BServerSocket::onTimer()
{
	db->exec( "SELECT update_time(); " );
	QSqlQuery query("SELECT ipaddress FROM tbl_realtime WHERE \
			(timelimit - interval_to_seconds(currenttime - logintime)) < 6 AND warning1 = false AND timelimit > 0;",db);
	QSqlQuery target(db);
	if (query.isActive())
		while (query.next())
		{
			target.exec("UPDATE tbl_realtime SET warning1 = true WHERE ipaddress = '"+query.value(0).toString()+"'");
			this->writeLineToSocket("SN02",(QSocket *)this->child(query.value(0).toString()));	
			qDebug("updating");
		}
	
	QSqlQuery query2("SELECT ipaddress FROM tbl_realtime WHERE \
			(timelimit - interval_to_seconds(currenttime - logintime)) < 1 \
			AND warning1 = true AND warning2 = false AND timelimit > 0",db);
	if (query2.isActive())
		while (query2.next())
		{
			target.exec("UPDATE tbl_realtime SET warning2 = true WHERE ipaddress = '"+query2.value(0).toString()+"'");
			this->writeLineToSocket("SN03",(QSocket *)this->child(query2.value(0).toString()));	
			qDebug("updating");
		}
	
	
			
	//cek database apakah user ada yang udah waktunya hampir habis / sudah habis klo ada kabari update jam used time di database
}
