/**************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (C) 2011 - 2012 Research In Motion
**
** Contact: Research In Motion (blackberry-qt@qnx.com)
** Contact: KDAB (info@kdab.com)
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
** Nokia at info@qt.nokia.com.
**
**************************************************************************/

#include "blackberryruncontrol.h"
#include "blackberryapplicationrunner.h"
#include "blackberryrunconfiguration.h"
#include "blackberryconnect.h"

#include <QIcon>
#include <QDebug>

using namespace Qnx;
using namespace Qnx::Internal;

BlackBerryRunControl::BlackBerryRunControl(BlackBerryRunConfiguration *runConfiguration)
    : ProjectExplorer::RunControl(runConfiguration, ProjectExplorer::NormalRunMode)
{
    m_runner = new BlackBerryApplicationRunner(false, runConfiguration, this);
    m_connector = BlackBerryConnect::instance(runConfiguration);

    connect(m_runner, SIGNAL(started()), this, SIGNAL(started()));
    connect(m_runner, SIGNAL(started()), m_connector, SLOT(connectToDevice()));
    connect(m_runner, SIGNAL(finished()), this, SIGNAL(finished()));
    connect(m_runner, SIGNAL(finished()), m_connector, SLOT(disconnectFromDevice()));
    connect(m_runner, SIGNAL(output(QString,Utils::OutputFormat)),
            this, SLOT(appendMessage(QString,Utils::OutputFormat)));
    connect(m_runner, SIGNAL(startFailed(QString)), this, SLOT(handleStartFailed(QString)));

    connect(m_connector, SIGNAL(connected()), this, SLOT(launchTailProcess()));
    connect(m_connector, SIGNAL(output(QString,Utils::OutputFormat)),
            this, SLOT(appendMessage(QString,Utils::OutputFormat)));
}

void BlackBerryRunControl::start()
{
    m_runner->start();
}

ProjectExplorer::RunControl::StopResult BlackBerryRunControl::stop()
{
    return m_runner->stop();
}

bool BlackBerryRunControl::isRunning() const
{
    return m_runner->isRunning();
}

QIcon BlackBerryRunControl::icon() const
{
    return QIcon(ProjectExplorer::Constants::ICON_RUN_SMALL);
}

void BlackBerryRunControl::handleStartFailed(const QString &message)
{
    appendMessage(message, Utils::StdErrFormat);
}

void BlackBerryRunControl::launchTailProcess()
{
    disconnect(m_connector, SIGNAL(connected()), this, SLOT(launchTailProcess()));
    m_runner->tailApplicationLog();
}
