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

#include "qnxdeploystep.h"

#include "qnxconstants.h"
#include "qnxdeployconfiguration.h"
#include "qnxdeploystepconfigwidget.h"
#include "qnxutils.h"

#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/target.h>
#include <qt4projectmanager/qt4buildconfiguration.h>

#include <QDir>

using namespace Qnx;
using namespace Qnx::Internal;

namespace {
const char DEPLOY_CMD[] = "blackberry-deploy";

int parseProgress(const QString &line)
{
    const QString startOfLine = QLatin1String("Info: Progress ");
    if (!line.startsWith(startOfLine))
        return -1;

    int percentPos = line.indexOf(QLatin1Char('%'));
    const QString progressStr = line.mid(startOfLine.length(), percentPos - startOfLine.length());

    bool ok;
    int progress = progressStr.toInt(&ok);
    if (!ok)
        return -1;

    return progress;
}
}

QnxDeployStep::QnxDeployStep(ProjectExplorer::BuildStepList *bsl)
    : ProjectExplorer::AbstractProcessStep(bsl, QLatin1String(Constants::QNX_DEPLOY_PACKAGE_BS_ID))
    , m_futureInterface(0)
{
}

QnxDeployStep::QnxDeployStep(ProjectExplorer::BuildStepList *bsl, QnxDeployStep *bs)
    : ProjectExplorer::AbstractProcessStep(bsl, bs)
    , m_futureInterface(0)
{
}

QnxDeployStep::~QnxDeployStep()

{
}

bool QnxDeployStep::immutable() const
{
    return false;
}

bool QnxDeployStep::init()
{
    QString deployCmd = qt4BuildConfiguration()->environment().searchInPath(QLatin1String(DEPLOY_CMD));
    if (deployCmd.isEmpty()) {
        raiseError(tr("Could not find deploy command '%1' in the build environment")
                       .arg(QLatin1String(DEPLOY_CMD)));
        return false;
    }

    if (deviceHost().isEmpty()) {
        raiseError(tr("No hostname specified for device"));
        return false;
    }

    m_barPackage = barPackage();
    QStringList args;
    args << QLatin1String("-installApp");
    args << QLatin1String("-device") << deviceHost();
    if (!password().isEmpty())
        args << QLatin1String("-password") << password();
    args << QnxUtils::addQuotes(barPackage());

    processParameters()->setCommand(deployCmd);
    processParameters()->setArguments(args.join(" "));
    processParameters()->setEnvironment(qt4BuildConfiguration()->environment());
    processParameters()->setWorkingDirectory(qt4BuildConfiguration()->buildDirectory());
    return ProjectExplorer::AbstractProcessStep::init();
}

void QnxDeployStep::run(QFutureInterface<bool> &fi)
{
    if (!QFileInfo(m_barPackage).exists()) {
        raiseError(tr("Package '%1' does not exist. Create the package first.").arg(barPackage()));
        fi.reportResult(false);
        return;
    }

    m_futureInterface = &fi;
    m_futureInterface->setProgressRange(0, 100);
    ProjectExplorer::AbstractProcessStep::run(fi);
    m_futureInterface = 0;
}

void QnxDeployStep::stdOutput(const QString &line)
{
    int progress = parseProgress(line);
    if (progress > -1)
        m_futureInterface->setProgressValue(progress);

    ProjectExplorer::AbstractProcessStep::stdOutput(line);
}

void QnxDeployStep::processStarted()
{
    QString arguments = processParameters()->prettyArguments();
    if (!password().isEmpty()) {
        QString passwordLine = QLatin1String(" -password ") + password();
        QString hiddenPasswordLine = QLatin1String(" -password <hidden>");
        arguments.replace(passwordLine, hiddenPasswordLine);
    }

    emit addOutput(tr("Starting: \"%1\" %2")
                   .arg(QDir::toNativeSeparators(processParameters()->effectiveCommand()),
                        arguments),
                   BuildStep::MessageOutput);
}

ProjectExplorer::BuildStepConfigWidget *QnxDeployStep::createConfigWidget()
{
    return new QnxDeployStepConfigWidget();
}

Qt4ProjectManager::Qt4BuildConfiguration *QnxDeployStep::qt4BuildConfiguration() const
{
    return static_cast<Qt4ProjectManager::Qt4BuildConfiguration *>(target()->activeBuildConfiguration());
}


QString QnxDeployStep::barPackage() const
{
    QnxDeployConfiguration *dc = static_cast<QnxDeployConfiguration *>(deployConfiguration());
    return dc->barPackage();
}

QString QnxDeployStep::deviceHost() const
{
    QnxDeployConfiguration *dc = static_cast<QnxDeployConfiguration *>(deployConfiguration());
    return dc->deviceHost();
}

QString QnxDeployStep::password() const
{
    QnxDeployConfiguration *dc = static_cast<QnxDeployConfiguration *>(deployConfiguration());
    return dc->password();
}

void QnxDeployStep::raiseError(const QString &errorMessage)
{
    emit addOutput(errorMessage, BuildStep::ErrorMessageOutput);
    emit addTask(ProjectExplorer::Task(ProjectExplorer::Task::Error, errorMessage, Utils::FileName(), -1,
                      Core::Id(ProjectExplorer::Constants::TASK_CATEGORY_BUILDSYSTEM)));
}
