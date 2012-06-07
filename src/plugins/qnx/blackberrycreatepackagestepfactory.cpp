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

#include "blackberrycreatepackagestepfactory.h"

#include "qnxconstants.h"
#include "blackberrycreatepackagestep.h"

#include <projectexplorer/buildsteplist.h>
#include <projectexplorer/projectexplorerconstants.h>
#include <projectexplorer/target.h>

using namespace Qnx;
using namespace Qnx::Internal;

BlackBerryCreatePackageStepFactory::BlackBerryCreatePackageStepFactory(QObject *parent) :
    ProjectExplorer::IBuildStepFactory(parent)
{
}

BlackBerryCreatePackageStepFactory::~BlackBerryCreatePackageStepFactory()
{
}

QStringList BlackBerryCreatePackageStepFactory::availableCreationIds(
        ProjectExplorer::BuildStepList *parent) const
{
    if (parent->id() != QLatin1String(ProjectExplorer::Constants::BUILDSTEPS_DEPLOY))
        return QStringList();
    if (parent->target()->id() == QLatin1String(Constants::QNX_BB_X86_TARGET_ID)
            || parent->target()->id() == QLatin1String(Constants::QNX_BB_ARMLEV7_TARGET_ID))
        return QStringList() << QLatin1String(Constants::QNX_CREATE_PACKAGE_BS_ID);
    return QStringList();
}

QString BlackBerryCreatePackageStepFactory::displayNameForId(const QString &id) const
{
    if (id == QLatin1String(Constants::QNX_CREATE_PACKAGE_BS_ID))
        return tr("Create Package");
    return QString();
}

bool BlackBerryCreatePackageStepFactory::canCreate(ProjectExplorer::BuildStepList *parent,
                                            const QString &id) const
{
    if (parent->id() != QLatin1String(ProjectExplorer::Constants::BUILDSTEPS_DEPLOY))
        return false;
    if (parent->target()->id() != QLatin1String(Constants::QNX_BB_X86_TARGET_ID)
            && parent->target()->id() != QLatin1String(Constants::QNX_BB_ARMLEV7_TARGET_ID))
        return false;
    return (id == QLatin1String(Constants::QNX_CREATE_PACKAGE_BS_ID));
}

ProjectExplorer::BuildStep *BlackBerryCreatePackageStepFactory::create(
        ProjectExplorer::BuildStepList *parent, const QString &id)
{
    if (!canCreate(parent, id))
        return 0;
    return new BlackBerryCreatePackageStep(parent);
}

bool BlackBerryCreatePackageStepFactory::canRestore(ProjectExplorer::BuildStepList *parent,
                                             const QVariantMap &map) const
{
    return canCreate(parent, ProjectExplorer::idFromMap(map));
}

ProjectExplorer::BuildStep *BlackBerryCreatePackageStepFactory::restore(
        ProjectExplorer::BuildStepList *parent, const QVariantMap &map)
{
    if (!canRestore(parent, map))
        return 0;
    BlackBerryCreatePackageStep *bs = new BlackBerryCreatePackageStep(parent);
    if (bs->fromMap(map))
        return bs;
    delete bs;
    return 0;
}

bool BlackBerryCreatePackageStepFactory::canClone(ProjectExplorer::BuildStepList *parent,
                                           ProjectExplorer::BuildStep *source) const
{
    return canCreate(parent, source->id());
}

ProjectExplorer::BuildStep *BlackBerryCreatePackageStepFactory::clone(ProjectExplorer::BuildStepList *parent,
                                                               ProjectExplorer::BuildStep *source)
{
    if (!canClone(parent, source))
        return 0;
    return new BlackBerryCreatePackageStep(parent, static_cast<BlackBerryCreatePackageStep *>(source));
}
