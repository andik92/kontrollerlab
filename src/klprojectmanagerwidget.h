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
#ifndef KLPROJECTMANAGERWIDGET_H
#define KLPROJECTMANAGERWIDGET_H

#include "ui_klprojectmanagerwidgetbase.h"
#include <QDialog>
#include <QDateTime>
#include <q3listview.h>
#include <qtoolbutton.h>
#include <qlabel.h>
#include <klocale.h>
#include <kurl.h>
#include <kmenu.h>


#define KLQListViewDocumentItem_RTTI 1001

#define SOURCES_TXT "Sources"
#define HEADERS_TXT "Headers"
#define OTHERS_TXT "Others"
#define NOTES_TXT "Notes"


class KLProject;
class KLDocument;
class KIconLoader;
class KMenu;



class KLQListViewDocumentItem : public Q3ListViewItem
{
public:
    KLQListViewDocumentItem( Q3ListViewItem* parent, const QString& name );
    void setDoc(KLDocument* theValue) { m_doc = theValue; }
    KLDocument* doc() const { return m_doc; }
    virtual int rtti() const { return KLQListViewDocumentItem_RTTI; }
    void updateFileInfo();
    void updateModified();
protected:
    KLDocument* m_doc;
};

class KLProjectManagerWidget: public QDialog {
Q_OBJECT
public:
    KLProjectManagerWidget(KLProject* project, QWidget *parent = 0, const char* name = 0);
    virtual ~KLProjectManagerWidget();
    void addDocument( KLDocument* doc );
    void removeDocument( KLDocument* doc );
    void update();
    void update( KLDocument* doc );
    void addFileInfo( KLDocument* doc, Q3ListViewItem* item );
    void setConfigButtonVisible( bool val ) { val?ui->tbConfigure->show():ui->tbConfigure->hide(); }
    void setProjectName( const QString& name )
    {
        m_curName = name;
        ui->lblProjectName->setText( QString( i18n("%1 (Size: %2)").arg(name).arg(m_curHexSize) ) );
        if ( m_rootNode ) m_rootNode->setText( 0, name );
    }
    void setHexFileSize( const int size )
    {
        m_curHexSize = size;
        if ( m_rootNode )
            m_rootNode->setText( 1, QString("%1").arg( size ) );
        setProjectName( m_curName );
    }
    void updateModified();
    void clear();

private:
    Ui_KLProjectManagerWidgetBase *ui;
public slots:
    virtual void slotDoubleClicked( Q3ListViewItem* item );
    virtual void slotConfigure();
    virtual void slotTrash();
    virtual void slotCreateNewView();
    virtual void slotShowPopupMenu( Q3ListViewItem* item, const QPoint& pt, int col );
protected:
    Q3ListViewItem* getSourceParentFor( KLDocument* doc )
    { return getParentNamedFor(doc, SOURCES_TXT); }
    Q3ListViewItem* getHeaderParentFor( KLDocument* doc )
    { return getParentNamedFor(doc, HEADERS_TXT); }
    Q3ListViewItem* getNotesParentFor( KLDocument* doc )
    { return getParentNamedFor(doc, NOTES_TXT); }
    Q3ListViewItem* getOthersParentFor( KLDocument* doc )
    { return getParentNamedFor(doc, OTHERS_TXT); }
    Q3ListViewItem* getParentNamedFor( KLDocument* doc, const QString& name );

    // void registerDocHierarchically( KLDocument* doc );
    Q3ListViewItem* getDirectoryListViewItem( const KUrl& dir );
    Q3ListViewItem *m_rootNode, *m_sourcesNode, *m_headersNode, *m_notesNode, *m_othersNode;
    KLProject* m_project;
    KIconLoader *m_iconLoader;
    int m_curHexSize;
    QString m_curName;
    KMenu *m_projectManagerPopup;

};

#endif
