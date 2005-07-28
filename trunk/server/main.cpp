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

#include <iostream>
#include <qapplication.h>
#include "formserver.h"


using namespace std;

int main(int argc, char **argv)
{
	
	FILE *f = fopen("/var/run/fbserver.pid","w");
	int pid = getpid();
	if (f)
	{
		fprintf(f,"%d",pid);
		fclose(f);
	}	

	QApplication a(argc,argv);	
	FormServer formmain;
	formmain.show();
	a.setMainWidget(&formmain);
	return a.exec();

}
