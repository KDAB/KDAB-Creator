/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2012 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact: Nokia Corporation (qt-info@nokia.com)
**
**
** GNU Lesser General Public License Usage
**
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this file.
** Please review the following information to ensure the GNU Lesser General
** Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** Other Usage
**
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
** If you have questions regarding the use of this file, please contact
** Nokia at qt-info@nokia.com.
**
**************************************************************************/

#ifndef REMOTEDIALOG_H
#define REMOTEDIALOG_H

#include <QDialog>

namespace Git {
namespace Internal {

namespace Ui {
class RemoteDialog;
class RemoteAdditionDialog;
}

class GitClient;
class RemoteModel;

// --------------------------------------------------------------------------
// RemoteAdditionDialog:
// --------------------------------------------------------------------------

class RemoteAdditionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RemoteAdditionDialog(QWidget *parent = 0);
    ~RemoteAdditionDialog();

    QString remoteName() const;
    QString remoteUrl() const;

    void clear();

private:
    Ui::RemoteAdditionDialog *m_ui;
};

// --------------------------------------------------------------------------
// RemoteDialog:
// --------------------------------------------------------------------------

class RemoteDialog : public QDialog {
    Q_OBJECT

public:
    explicit RemoteDialog(QWidget *parent = 0);
    ~RemoteDialog();

public slots:
    void refresh(const QString &repository, bool force);

    void refreshRemotes();
    void addRemote();
    void removeRemote();
    void pushToRemote();
    void fetchFromRemote();

    void updateButtonState();

private slots:

protected:
    virtual void changeEvent(QEvent *e);

private:
    Ui::RemoteDialog *m_ui;

    RemoteModel *m_remoteModel;
    RemoteAdditionDialog *m_addDialog;

    QString m_repository;
};

} // namespace Internal
} // namespace Git

#endif // REMOTEDIALOG_H
