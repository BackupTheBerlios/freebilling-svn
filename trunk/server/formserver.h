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
#ifndef FORMSERVER_H
#define FORMSERVER_H

#include "_server.h"
#include "bserversocket.h"

class FormServer: public _FormServer {
Q_OBJECT
public:
    FormServer(QWidget *parent = 0, const char *name = 0);
public slots:
    virtual void btnStop_clicked();
public slots:
    virtual void btnStart_clicked();
private:
    BServerSocket *ss;

};

#endif
