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

#ifndef PROTOKOL_H_
#define PROTOKOL_H_

#include <qstring.h>

enum RETTYPE { OK_1 = 1, OK_2, OK_3, OK_4, SN_1, SN_2, SN_3, SN_4, ER_1, ER_2, ER_3, ER_4, CS_1 }; 


RETTYPE convert(QString substr)
{
	if (substr=="OK01") return OK_1;
	if (substr=="OK02") return OK_2;
	if (substr=="OK03") return OK_3;
	if (substr=="OK04") return OK_4;
	if (substr=="SN01") return SN_1;
	if (substr=="SN02") return SN_2;
	if (substr=="SN03") return SN_3;
	if (substr=="SN04") return SN_4;
	if (substr=="CS01") return CS_1;
	if (substr=="ER01") return ER_1;
	if (substr=="ER02") return ER_2;
	if (substr=="ER03") return ER_3;
	if (substr=="ER04") return ER_4;
}

#endif //PROTOKOL_H_