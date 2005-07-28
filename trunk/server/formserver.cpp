/***************************************************************************
 *   Copyright (C) 2005 by Dreamware   *
 *   eddy.setyawan@dreamware.web.id   *
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
#include "formserver.h"
#include "qpushbutton.h"
#include <sys/types.h>
#include <unistd.h>


FormServer::FormServer(QWidget *parent, const char *name)
    :_FormServer(parent, name)
{
}

void FormServer::btnStart_clicked()
{
	char *temp;
	int iPort, iMaxClient;
	if (temp = getenv("FREEBILLING_SERVER_PORT"))
		iPort = atoi(temp);
	else
		iPort = 5000;
		
	if (temp = getenv("FREEBILLING_MAX_CLIENT"))
		iMaxClient = atoi(temp);
	else
		iMaxClient = 20;
	if ( getenv("FREEBILLING_IFACE") == 0 )
	{
		printf("Warning Variabel Environment FREEBILLING_IFACE not set, changing to default eth0\n");
		setenv("FREEBILLING_IFACE","eth0",1);
	}
	this->ss = new BServerSocket(iPort,iMaxClient);
	if (!this->ss->ok()) 
	{
		delete this->ss;
		this->ss = NULL;
	} 
	else
	{
		btnStop->setEnabled(TRUE);
		btnStart->setEnabled(FALSE);
	}
}

void FormServer::btnStop_clicked()
{
	if (!this->ss)
		return;
	if (ss->ok())
	{
		delete this->ss;
		this->ss = NULL;
	}
	btnStop->setEnabled(FALSE);
	btnStart->setEnabled(TRUE);



	

}


