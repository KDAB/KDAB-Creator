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

#ifndef PROGRESSVIEW_H
#define PROGRESSVIEW_H

#include "progressmanager.h"

#include <QFuture>
#include <QWidget>

QT_BEGIN_NAMESPACE
class QVBoxLayout;
QT_END_NAMESPACE

namespace Core {

class FutureProgress;

namespace Internal {

class ProgressView : public QWidget
{
    Q_OBJECT

public:
    ProgressView(QWidget *parent = 0);
    ~ProgressView();

    /** The returned FutureProgress instance is guaranteed to live till next main loop event processing (deleteLater). */
    FutureProgress *addTask(const QFuture<void> &future,
                            const QString &title,
                            const QString &type,
                            ProgressManager::ProgressFlags flags);

private slots:
    void slotRemoveTask();

private:
    void removeOldTasks(const QString &type, bool keepOne = false);
    void removeOneOldTask();
    void removeTask(FutureProgress *task);
    void deleteTask(FutureProgress *task);

    QVBoxLayout *m_layout;
    QList<FutureProgress *> m_taskList;
};

} // namespace Internal
} // namespace Core

#endif // PROGRESSVIEW_H
