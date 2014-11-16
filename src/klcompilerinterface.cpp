/***************************************************************************
 *   Copyright (C) 2006 by Martin Strasser                                 *
 *   strasser  a t  cadmaniac  d o t  org                                  *
 *   Special thanks to Mario Boikov                                        *
 *   squeeze  a t  cadmaniac  d o t  org                                   *
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
#include "klcompilerinterface.h"
#include "klproject.h"
#include "kldocument.h"
#include "kldocumentview.h"


KLCompilerInterface::KLCompilerInterface( KLProject* project,
                                          KLProcessManager* processManager ) :
    KLProcessHandler( processManager )
{
    m_project = project;
}


KLCompilerInterface::~KLCompilerInterface()
{
}




QString KLCompilerInterface::getHierarchyName(const KUrl & filename) const
{
    int cutTheFirst = 0;
    if ( m_project )
        cutTheFirst = m_project->projectBaseURL().path().length() + 1;
    return filename.path().right( filename.path().length() - cutTheFirst );
}
