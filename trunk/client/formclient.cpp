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
#include "formclient.h"
#include <qtextstream.h>
#include "_restriction.h"
#include <qapplication.h>
#include <qevent.h>
#include <iostream>
#include <qdatetime.h>
#include <qlabel.h>
#include <qmessagebox.h>
#include <qobject.h>

using namespace std;

FormClient::FormClient(QWidget *parent, const char *name)
    :_FormClient(parent, name, WStyle_StaysOnTop | WX11BypassWM  )
{
	connect(&localtimer,SIGNAL(timeout()),this,SLOT(onLocalTimer()));
	connect(&evilTimer,SIGNAL(timeout()),this,SLOT(onEvilTimer()));
	this->startTime = QDateTime::currentDateTime();	
	localtimer.start(1000);
}

bool FormClient::event(QEvent *e)
{
	if (e->type() == 1100)
		this->accept();
	if (e->type() == 1101)
	{
		this->evilTimer.start(120000, TRUE);
	/* perlu perbaikan pemanggilan aplikasinya */	
		system("/opt/freebilling/fbmessage \"<b>PERINGATAN!!</b><br>Koneksi anda terputus <br> dalam waktu 2 menit semua aplikasi akan tertutup!\" &");
	}
	_FormClient::event(e);
}

void FormClient::closeEvent(QCloseEvent *e)
{
	e->ignore();
}

void FormClient::btnLogout_clicked()
{
	this->accept();
}



void FormClient::onLocalTimer()
{
	this->currentTime = QDateTime::currentDateTime();
	int numDate = this->startTime.daysTo(currentTime);
	int numSecond = this->startTime.secsTo(currentTime) % 86400;
	QTime time(0,0,0,0);
	time = time.addSecs(numSecond);
	this->labelTime->setText( (numDate) ? QString::number(numDate) + " days " : "" + time.toString("hh:mm:ss"));
	this->setCaption(labelTime->text() + "::FREEbilling");
}

void FormClient::onEvilTimer()
{
	this->accept();
}




