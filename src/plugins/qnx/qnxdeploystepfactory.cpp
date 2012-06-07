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

#include "qnxdeploystepfactory.h"
#include "qnxconstants.h"

#include <projectexplorer/buildsteplist.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/target.h>
#include <remotelinux/genericdirectuploadstep.h>

using namespace Qnx;
using namespace Qnx::Internal;

QnxDeployStepFactory::QnxDeployStepFactory()
    : ProjectExplorer::IBuildStepFactory()
{
}

QStringList QnxDeployStepFactory::availableCreationIds(ProjectExplorer::BuildStepList *parent) const
{
    if (parent->id() != QLatin1String(ProjectExplorer::Constants::BUILDSTEPS_DEPLOY))
        return QStringList();
    if (parent->target()->id() == QLatin1String(Constants::QNX_QNX_X86_TARGET_ID)
            || parent->target()->id() == QLatin1String(Constants::QNX_QNX_ARMLEV7_TARGET_ID))
        return QStringList() << RemoteLinux::GenericDirectUploadStep::stepId();
    return QStringList();
}

QString QnxDeployStepFactory::displayNameForId(const QString &id) const
{
    if (id == RemoteLinux::GenericDirectUploadStep::stepId())
        return RemoteLinux::GenericDirectUploadStep::displayName();
    return QString();
}

bool QnxDeployStepFactory::canCreate(ProjectExplorer::BuildStepList *parent, const QString &id) const
{
    if (parent->id() != QLatin1String(ProjectExplorer::Constants::BUILDSTEPS_DEPLOY))
        return false;
    if (parent->target()->id() != QLatin1String(Constants::QNX_QNX_X86_TARGET_ID)
            && parent->target()->id() != QLatin1String(Constants::QNX_QNX_ARMLEV7_TARGET_ID))
        return false;
    return id == RemoteLinux::GenericDirectUploadStep::stepId();
}

ProjectExplorer::BuildStep *QnxDeployStepFactory::create(ProjectExplorer::BuildStepList *parent, const QString &id)
{
    if (!canCreate(parent, id))
        return 0;
    return new RemoteLinux::GenericDirectUploadStep(parent, id);
}

bool QnxDeployStepFactory::canRestore(ProjectExplorer::BuildStepList *parent, const QVariantMap &map) const
{
    return canCreate(parent, ProjectExplorer::idFromMap(map));
}

ProjectExplorer::BuildStep *QnxDeployStepFactory::restore(ProjectExplorer::BuildStepList *parent, const QVariantMap &map)
{
    if (!canRestore(parent, map))
        return 0;
    ProjectExplorer::BuildStep * const bs = create(parent, ProjectExplorer::idFromMap(map));
    if (bs->fromMap(map))
        return bs;
    delete bs;
    return 0;
}

bool QnxDeployStepFactory::canClone(ProjectExplorer::BuildStepList *parent, ProjectExplorer::BuildStep *product) const
{
    return canCreate(parent, product->id());
}

ProjectExplorer::BuildStep *QnxDeployStepFactory::clone(ProjectExplorer::BuildStepList *parent, ProjectExplorer::BuildStep *product)
{
    if (!canClone(parent, product))
        return 0;
    return new RemoteLinux::GenericDirectUploadStep(parent, static_cast<RemoteLinux::GenericDirectUploadStep *>(product));
}
