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

#ifndef PERFORCESUBMITEDITORWIDGET_H
#define PERFORCESUBMITEDITORWIDGET_H

#include "ui_submitpanel.h"
#include <utils/submiteditorwidget.h>

namespace Perforce {
namespace Internal {

/* Submit editor widget with additional information pane
 * at the top. */
class PerforceSubmitEditorWidget : public Utils::SubmitEditorWidget
{

public:
    explicit PerforceSubmitEditorWidget(QWidget *parent = 0);

    void setData(const QString &change, const QString &client, const QString &userName);

private:
    QGroupBox *m_submitPanel;
    Ui::SubmitPanel m_submitPanelUi;
};

} // namespace Internal
} // namespace Perforce

#endif // PERFORCESUBMITEDITORWIDGET_H
