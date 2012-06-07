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

#include "blackberryapplicationrunner.h"

#include "blackberrydeployconfiguration.h"
#include "blackberryrunconfiguration.h"

#include <projectexplorer/target.h>
#include <qt4projectmanager/qt4buildconfiguration.h>
#include <utils/ssh/sshremoteprocessrunner.h>
#include <utils/qtcassert.h>

#include <QtCore/QTimer>
#include <QtCore/QDir>

namespace {
const char DEPLOY_CMD[] = "blackberry-deploy";

qint64 parsePid(const QString &line)
{
    QTC_ASSERT(line.startsWith(QLatin1String("result::")), return -1);

    int pidIndex = -1;
    if (line.contains(QLatin1String("running"))) // "result::running,<pid>"
        pidIndex = 16;
    else // "result::<pid>"
        pidIndex = 8;

    bool ok;
    qint64 pid = line.mid(pidIndex).toInt(&ok);
    if (!ok)
        return -1;
    return pid;
}

QString parseAppId(const QString &line)
{
    QTC_ASSERT(line.startsWith(QLatin1String("Info: Launching")), return QString());

    int endOfId = line.indexOf("...");
    return line.mid(16, endOfId - 16);
}

bool parseRunningState(const QString &line)
{
    QTC_ASSERT(line.startsWith(QLatin1String("result::")), return false);
    return line.trimmed().mid(8) == QLatin1String("true");
}
}

using namespace Qnx;
using namespace Qnx::Internal;

BlackBerryApplicationRunner::BlackBerryApplicationRunner(bool debugMode, BlackBerryRunConfiguration *runConfiguration, QObject *parent)
    : QObject(parent)
    , m_debugMode(debugMode)
    , m_pid(-1)
    , m_appId(QString())
    , m_running(false)
    , m_stopping(false)
    , m_sshParams(Utils::SshConnectionParameters::NoProxy)
    , m_launchProcess(0)
    , m_stopProcess(0)
    , m_tailProcess(0)
    , m_runningStateTimer(new QTimer(this))
    , m_runningStateProcess(0)
{
    QTC_ASSERT(runConfiguration, return);

    Qt4ProjectManager::Qt4BuildConfiguration *qt4BuildConfig = runConfiguration->activeQt4BuildConfiguration();
    m_environment = qt4BuildConfig->environment();
    m_deployCmd = m_environment.searchInPath(QLatin1String(DEPLOY_CMD));

    m_deviceHost = runConfiguration->deployConfiguration()->deviceHost();
    m_password = runConfiguration->deployConfiguration()->password();
    m_barPackage = runConfiguration->deployConfiguration()->barPackage();

    m_sshParams = runConfiguration->deployConfiguration()->deviceConfiguration()->sshParameters();
    // The QNX device always uses key authentication
    m_sshParams.authenticationType = Utils::SshConnectionParameters::AuthenticationByKey;

    m_runningStateTimer->setInterval(3000);
    m_runningStateTimer->setSingleShot(true);
    connect(m_runningStateTimer, SIGNAL(timeout()), this, SLOT(determineRunningState()));
}

void BlackBerryApplicationRunner::start()
{
    QStringList args;
    args << QLatin1String("-launchApp");
    if (m_debugMode)
        args << QLatin1String("-debugNative");
    args << "-device" << m_deviceHost;
    if (!m_password.isEmpty())
        args << QLatin1String("-password") << m_password;
    args << QDir::toNativeSeparators(m_barPackage);

    if (!m_launchProcess)
        m_launchProcess = new QProcess(this);

    connect(m_launchProcess, SIGNAL(readyReadStandardError()), this, SLOT(readStandardError()));
    connect(m_launchProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readStandardOutput()));
    connect(m_launchProcess, SIGNAL(finished(int,QProcess::ExitStatus)),
            this, SLOT(startFinished(int,QProcess::ExitStatus)));

    m_launchProcess->setEnvironment(m_environment.toStringList());

    m_launchProcess->start(m_deployCmd, args);
    m_runningStateTimer->start();
    m_running = true;
}

void BlackBerryApplicationRunner::startFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    if (exitCode == 0 && exitStatus == QProcess::NormalExit && m_pid > -1) {
        emit started();
    } else {
        m_running = false;
        m_runningStateTimer->stop();

        QTC_ASSERT(m_launchProcess, return);
        const QString errorString = (m_launchProcess->error() != QProcess::UnknownError)
                ? m_launchProcess->errorString() : tr("Launching application failed");
        emit startFailed(errorString);
    }
}

ProjectExplorer::RunControl::StopResult BlackBerryApplicationRunner::stop()
{
    m_stopping = true;

    QStringList args;
    args << QLatin1String("-terminateApp");
    args << QLatin1String("-device") << m_deviceHost;
    if (!m_password.isEmpty())
        args << QLatin1String("-password") << m_password;
    args << m_barPackage;

    if (!m_stopProcess)
        m_stopProcess = new QProcess(this);

    connect(m_stopProcess, SIGNAL(readyReadStandardError()), this, SLOT(readStandardError()));
    connect(m_stopProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readStandardOutput()));
    connect(m_stopProcess, SIGNAL(finished(int,QProcess::ExitStatus)),
            this, SLOT(stopFinished(int,QProcess::ExitStatus)));

    m_stopProcess->setEnvironment(m_environment.toStringList());

    m_stopProcess->start(m_deployCmd, args);
    return ProjectExplorer::RunControl::AsynchronousStop;
}

bool BlackBerryApplicationRunner::isRunning() const
{
    return m_running && !m_stopping;
}

qint64 BlackBerryApplicationRunner::pid() const
{
    return m_pid;
}

void BlackBerryApplicationRunner::stopFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitCode);
    Q_UNUSED(exitStatus);

    reset();
}

void BlackBerryApplicationRunner::readStandardOutput()
{
    QProcess *process = qobject_cast<QProcess *>(sender());
    process->setReadChannel(QProcess::StandardOutput);
    while (process->canReadLine()) {
        QString line = QString::fromLocal8Bit(process->readLine());
        emit output(line, Utils::StdOutFormat);

        if (line.startsWith(QLatin1String("result::"))) {
            m_pid = parsePid(line);
        } else if (line.startsWith(QLatin1String("Info: Launching"))) {
            m_appId = parseAppId(line);
        }
    }
}

void BlackBerryApplicationRunner::readStandardError()
{
    QProcess *process = qobject_cast<QProcess *>(sender());
    process->setReadChannel(QProcess::StandardError);
    while (process->canReadLine()) {
        QString line = QString::fromLocal8Bit(process->readLine());
        emit output(line, Utils::StdErrFormat);
    }
}

void BlackBerryApplicationRunner::killTailProcess()
{
    Utils::SshRemoteProcessRunner *slayProcess = new Utils::SshRemoteProcessRunner(this);
    connect(slayProcess, SIGNAL(processClosed(int)), this, SIGNAL(finished()));

    slayProcess->run("slay tail", m_sshParams);

    // Not supported by OpenSSH server
    //m_tailProcess->sendSignalToProcess(Utils::SshRemoteProcess::KillSignal);
    m_tailProcess->cancel();

    delete m_tailProcess;
    m_tailProcess = 0;
}

void BlackBerryApplicationRunner::tailApplicationLog()
{
    // TODO: Reading the log using qconn instead?
    QTC_CHECK(!m_appId.isEmpty());
    QTC_CHECK(!m_tailProcess);

    m_tailProcess = new Utils::SshRemoteProcessRunner(this);

    connect(m_tailProcess, SIGNAL(processOutputAvailable(QByteArray)),
            this, SLOT(handleTailOutput(QByteArray)));
    connect(m_tailProcess, SIGNAL(processErrorOutputAvailable(QByteArray)),
            this, SLOT(handleTailError(QByteArray)));
    connect(m_tailProcess, SIGNAL(connectionError()),
            this, SLOT(handleTailConnectionError()));

    const QString command = QLatin1String("tail -c +1 -f /accounts/1000/appdata/") + m_appId
            + QLatin1String("/logs/log");

    m_tailProcess->run(command.toLatin1(), m_sshParams);
}

void BlackBerryApplicationRunner::handleTailOutput(const QByteArray &message)
{
    emit output(QString::fromAscii(message), Utils::StdOutFormat);
}

void BlackBerryApplicationRunner::handleTailError(const QByteArray &message)
{
    emit output(QString::fromAscii(message), Utils::StdErrFormat);
}

void BlackBerryApplicationRunner::handleTailConnectionError()
{
    emit output(tr("Cannot show debug output. Error: %1").arg(m_tailProcess->lastConnectionErrorString()),
                Utils::StdErrFormat);
}

void BlackBerryApplicationRunner::startRunningStateTimer()
{
    if (m_running)
        m_runningStateTimer->start();
}

void BlackBerryApplicationRunner::determineRunningState()
{
    QStringList args;
    args << QLatin1String("-isAppRunning");
    args << "-device" << m_deviceHost;
    if (!m_password.isEmpty())
        args << QLatin1String("-password") << m_password;
    args << m_barPackage;

    if (!m_runningStateProcess) {
        m_runningStateProcess = new QProcess(this);

        connect(m_runningStateProcess, SIGNAL(readyReadStandardOutput()), this, SLOT(readRunningStateStandardOutput()));
        connect(m_runningStateProcess, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(startRunningStateTimer()));
    }

    m_runningStateProcess->setEnvironment(m_environment.toStringList());

    m_runningStateProcess->start(m_deployCmd, args);
}

void BlackBerryApplicationRunner::readRunningStateStandardOutput()
{
    QProcess *process = qobject_cast<QProcess *>(sender());
    process->setReadChannel(QProcess::StandardOutput);
    while (process->canReadLine()) {
        QString line = QString::fromLocal8Bit(process->readLine());
        if (line.startsWith(QLatin1String("result"))) {
            m_running = parseRunningState(line);
            break;
        }
    }

    if (!m_running)
        reset();
}

void BlackBerryApplicationRunner::reset()
{
    m_pid = -1;
    m_appId = "";
    m_running = false;
    m_stopping = false;

    m_runningStateTimer->stop();
    m_runningStateProcess->terminate();
    if (!m_runningStateProcess->waitForFinished(1000))
        m_runningStateProcess->kill();

    if (m_tailProcess && m_tailProcess->isProcessRunning())
        killTailProcess();
    else
        emit finished();
}
