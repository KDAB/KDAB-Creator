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

#ifndef DISPLAYSETTINGSPAGE_H
#define DISPLAYSETTINGSPAGE_H

#include "texteditor_global.h"

#include "texteditoroptionspage.h"

namespace TextEditor {

class DisplaySettings;

class DisplaySettingsPageParameters
{
public:
    QString id;
    QString displayName;
    QString settingsPrefix;
};

class DisplaySettingsPage : public TextEditorOptionsPage
{
    Q_OBJECT

public:
    DisplaySettingsPage(const DisplaySettingsPageParameters &p, QObject *parent);
    virtual ~DisplaySettingsPage();

    // IOptionsPage
    QString id() const;
    QString displayName() const;

    QWidget *createPage(QWidget *parent);
    void apply();
    void finish();
    bool matches(const QString &s) const;

    const DisplaySettings &displaySettings() const;

signals:
    void displaySettingsChanged(const TextEditor::DisplaySettings &);

private:
    void settingsFromUI(DisplaySettings &displaySettings) const;
    void settingsToUI();
    void setDisplaySettings(const DisplaySettings &);

    struct DisplaySettingsPagePrivate;
    DisplaySettingsPagePrivate *d;
};

} // namespace TextEditor

#endif // DISPLAYSETTINGSPAGE_H
