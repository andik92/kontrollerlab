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
#include "kldocumentview.h"
#include "kldocument.h"
#include <qlayout.h>
#include "klproject.h"
#include <kxmlguifactory.h>
#include <kactioncollection.h>
#include <kactionmenu.h>

#include <ktexteditor/configinterface.h>
#include <ktexteditor/cursor.h>
#include <kmessagebox.h>



KLDocumentView::KLDocumentView( KLDocument *doc, KontrollerLab* parent ) : QMdiSubWindow( parent)
{
    // Store the document for this view:
    setObjectName(doc->name());
    setAttribute(Qt::WA_DeleteOnClose);

    m_document = doc;
    m_parent = parent;

    // The document only represents the document, to view
    // the document's content
    // we have to create a view for the document.

    m_view = (KTextEditor::View *) doc->kateDoc()->createView( this);

    KTextEditor::ConfigInterface *configIf = qobject_cast<KTextEditor::ConfigInterface*>( m_view );



    setWidget(m_view);

    parent->m_mdiArea->addSubWindow( this );
    setWindowTitle(doc->name());

    // remove the unwanted actions

    // qDebug() << m_view->actionCollection()->actions();

    QAction *a = m_view->actionCollection()->action( "file_export" );

    if (a)
        m_view->actionCollection()->removeAction(a);

    a =  m_view->actionCollection()->action( "file_save" );
    if (a)
        m_view->actionCollection()->removeAction(a);

    a = m_view->actionCollection()->action( "file_save_as" );
    if (a)
        m_view->actionCollection()->removeAction(a);
    
    a = m_view->actionCollection()->action( "file_reload" );
    
    if (a)
        connect( a, SIGNAL(activated()), this, SLOT(slotCheckForModifiedFiles()) );
    // m_view->actionCollection()->take(a);
    
    a = m_view->actionCollection()->action( "edit_undo" );
    if (a)
        connect( a, SIGNAL(activated()), this, SLOT(slotCheckForModifiedFiles()) );
    //m_view->actionCollection()->takeAction(a);
/*
    a = m_view->actionCollection()->action( "view_folding_markers" );
    if (a)
        m_view->actionCollection()->removeAction(a);

    a = m_view->actionCollection()->action( "switch_to_cmd_line" );
    if (a)
        m_view->actionCollection()->removeAction(a);
*/
    //because they are not implemented in VPL
    //! \todo still in kde4?
    /*
    a = m_view->actionCollection()->action( "edit_copy" );
    if (a)
        m_view->actionCollection()->takeAction(a);
    a = m_view->actionCollection()->action( "edit_cut" );
    if (a)
        m_view->actionCollection()->takeAction(a);
    a = m_view->actionCollection()->action( "edit_paste" );
    if (a)
        m_view->actionCollection()->takeAction(a);

    a = m_view->actionCollection()->action( "edit_paste" );
    if (a)
        m_view->actionCollection()->takeAction(a);
        */

    if( configIf )
    {
        configIf->setConfigValue("line-numbers",true);
        configIf->setConfigValue("icon-bar",true);
    }

    KActionMenu *bookmarkAction = dynamic_cast<KActionMenu*>(m_view->actionCollection()->action( "bookmarks" ));
    if (bookmarkAction)
    {
        m_view->actionCollection()->removeAction(bookmarkAction);
        //kdDebug(24000) << "Bookmarks found!" << endl;
        //bookmarkAction->insert(quantaApp->actionCollection()->action( "file_quit" ));
    }
    //    viewCursorIf = dynamic_cast<KTextEditor::ViewCursorInterface *>(m_view);
    codeCompletionIf = dynamic_cast<KTextEditor::CodeCompletionInterface *>(m_view);

    //get the context-menu of ktexteditor
    if (parent->factory()) {
        QList<QWidget*> conts = parent->factory()->containers("menu");
        foreach (QWidget *w, conts) {
            if (w->objectName() == "ktexteditor_popup") {
                QMenu *men = dynamic_cast<QMenu*>(w);
                men->addAction(parent->debugToggleBreakpoint());
                break;
            }
        }
    }

    setFocusProxy( m_view );
    m_view->setFocusPolicy(Qt::WheelFocus);
    m_parent->slotNewPart(doc->kateDoc(), true);
    // add the view's XML GUI Client
    if ( !m_parent->kateGuiClientAdded() )
    {
        m_parent->setUpdatesEnabled(false);
        m_parent->guiFactory()->addClient( m_view );
        m_parent->setKateGuiClientAdded( (KXMLGUIClient*) m_view );
        m_parent->setUpdatesEnabled(true);
    }

    doc->registerKLDocumentView( this );
    m_view->show();
    show();

    connect( this,SIGNAL(aboutToActivate()),this,SLOT(mdiViewActivated()));

    m_inhibitFocusRecursion = false;
    mdiViewActivated(this);
}


KLDocumentView::~KLDocumentView()
{
    // qDebug() << "removing" << this;

    if ( m_parent->oldKTextEditor() == m_view )
    {
        if ( m_parent->guiFactory()->clients().indexOf( m_view ) >= 0 )
        {
            // qDebug("REMOVE: %d", m_oldKTextEditor);
            m_parent->guiFactory()->removeClient( m_view );
        }
        m_parent->setOldKTextEditor( 0L );
    }
    
    if (m_document)
        m_document->unregisterKLDocumentView( this );
    if ( m_parent->kateGuiClientAdded() )
    {
        m_parent->guiFactory()->removeClient( m_parent->kateGuiClientAdded() );
        m_parent->setKateGuiClientAdded( 0L );
    }

    delete m_view;
}


void KLDocumentView::mdiViewActivated(QMdiSubWindow * )
{
    if ( m_document )
    {
        m_parent->partManager()->setActivePart(m_document->kateDoc(), m_view);
        m_document->setActiveView( this );
    }
}

void KLDocumentView::mdiViewActivated( )
{
    if ( m_inhibitFocusRecursion )
        return;

    m_inhibitFocusRecursion = true;
    setFocus();
    m_view->setFocus();
    m_inhibitFocusRecursion = false;

    if ( m_document )
    {
        m_parent->partManager()->setActivePart(m_document->kateDoc(), m_view);
        m_document->setActiveView( this );
    }
}


void KLDocumentView::activated( )
{
    // if ( m_document )
    // m_parent->m_mdiArea->setActiveSubWindow();
}


void KLDocumentView::setCursorToLine( int lineNr )
{
    m_view->setCursorPosition(KTextEditor::Cursor( lineNr-1, 0 ));
}

void KLDocumentView::closeEvent(QCloseEvent *e)
{
    if ( !m_document )
    {
        m_parent->setOldKTextEditor( m_view );
        e->accept();
    }
    else if ( ( !m_document->isModified() ) ||
              ( m_document->kateDoc() &&
                ( m_document->kateDoc()->views().count() > 1 ) ) )
    {
        m_parent->setOldKTextEditor( m_view );
        e->accept();
    }
    else
    {
        int retVal = KMessageBox::questionYesNoCancel( this,
                                                       i18n("Do you want to save the document before closing?"),
                                                       i18n("Close document") );

        if ( retVal == KMessageBox::No )
        {
            m_parent->setOldKTextEditor( m_view );
            m_document->revert();
            e->accept();
        }
        else if ( retVal == KMessageBox::Yes )
        {
            if (m_document->save())
            {
                m_parent->setOldKTextEditor( m_view );
                e->accept();
            }
        }
        else if ( retVal == KMessageBox::Cancel )
            e->ignore();
    }
}


void KLDocumentView::slotCheckForModifiedFiles()
{
    m_document->project()->checkForModifiedFiles();
}







