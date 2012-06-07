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

#include "blackberrydebugsupport.h"
#include "blackberryapplicationrunner.h"
#include "blackberryconnect.h"

#include <debugger/debuggerrunner.h>
#include <debugger/debuggerengine.h>
#include <debugger/debuggerstartparameters.h>

using namespace Qnx;
using namespace Qnx::Internal;

BlackBerryDebugSupport::BlackBerryDebugSupport(BlackBerryRunConfiguration *runConfig,
                                 Debugger::DebuggerRunControl *runControl)
    : QObject(runControl->engine())
    , m_engine(runControl->engine())
{
    m_runner = new BlackBerryApplicationRunner(true, runConfig, this);
    m_connector = BlackBerryConnect::instance(runConfig);

    connect(m_engine, SIGNAL(requestRemoteSetup()), this, SLOT(launchRemoteApplication()));
    connect(m_engine, SIGNAL(stateChanged(Debugger::DebuggerState)),
            this, SLOT(handleDebuggerStateChanged(Debugger::DebuggerState)));

    connect(m_connector, SIGNAL(error(QString)), this, SLOT(handleConnectorError(QString)));
    connect(m_connector, SIGNAL(connected()), m_runner, SLOT(start()));
    connect(m_connector, SIGNAL(output(QString,Utils::OutputFormat)),
            runControl, SLOT(appendMessage(QString,Utils::OutputFormat)));

    connect(m_runner, SIGNAL(started()), this, SLOT(handleStarted()));
    connect(m_runner, SIGNAL(started()), m_runner, SLOT(tailApplicationLog()));
    connect(m_runner, SIGNAL(startFailed(QString)), this, SLOT(handleStartFailed(QString)));
    connect(m_runner, SIGNAL(output(QString,Utils::OutputFormat)),
            runControl, SLOT(appendMessage(QString,Utils::OutputFormat)));
    connect(m_runner, SIGNAL(finished()), m_connector, SLOT(disconnectFromDevice()));

    connect(this, SIGNAL(output(QString,Utils::OutputFormat)),
            runControl, SLOT(appendMessage(QString,Utils::OutputFormat)));
}

void BlackBerryDebugSupport::launchRemoteApplication()
{
    m_connector->connectToDevice();
}

void BlackBerryDebugSupport::handleStarted()
{
    m_engine->startParameters().attachPID = m_runner->pid();
    m_engine->handleRemoteSetupDone(-1, -1);
}

void BlackBerryDebugSupport::handleStartFailed(const QString &message)
{
    m_engine->handleRemoteSetupFailed(message);
}

void BlackBerryDebugSupport::handleDebuggerStateChanged(Debugger::DebuggerState state)
{
    if (state == Debugger::EngineShutdownOk || state == Debugger::DebuggerFinished) {
        if (m_runner->isRunning())
            m_runner->stop();
    }
}

void BlackBerryDebugSupport::handleConnectorError(const QString &message)
{
    m_engine->handleRemoteSetupFailed(message);
}
