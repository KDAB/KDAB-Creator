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

#include "qnxplugin.h"

#include "blackberrydeviceconfigurationfactory.h"
#include "qnxconstants.h"
#include "blackberryqtversionfactory.h"
#include "qnxtargetfactory.h"
#include "qnxtoolchainfactory.h"
#include "blackberrydeployconfigurationfactory.h"
#include "blackberrycreatepackagestepfactory.h"
#include "blackberrydeploystepfactory.h"
#include "blackberryrunconfigurationfactory.h"
#include "blackberryruncontrolfactory.h"
#include "qnxdeviceconfigurationfactory.h"
#include "qnxruncontrolfactory.h"
#include "qnxdeploystepfactory.h"
#include "qnxdeployconfigurationfactory.h"
#include "qnxrunconfigurationfactory.h"
#include "qnxqtversionfactory.h"
#include "blackberrywizardextension.h"

#include <QtPlugin>

using namespace Qnx::Internal;

QNXPlugin::QNXPlugin()
{
}

QNXPlugin::~QNXPlugin()
{
}

bool QNXPlugin::initialize(const QStringList &arguments, QString *errorString)
{
    Q_UNUSED(arguments)
    Q_UNUSED(errorString)

    // Handles QNX and BlackBerry
    addAutoReleasedObject(new QnxTargetFactory);
    addAutoReleasedObject(new QnxToolChainFactory);

    // Handles BlackBerry
    addAutoReleasedObject(new BlackBerryQtVersionFactory);
    addAutoReleasedObject(new BlackBerryDeployConfigurationFactory);
    addAutoReleasedObject(new BlackBerryDeviceConfigurationFactory);
    addAutoReleasedObject(new BlackBerryCreatePackageStepFactory);
    addAutoReleasedObject(new BlackBerryDeployStepFactory);
    addAutoReleasedObject(new BlackBerryRunConfigurationFactory);
    addAutoReleasedObject(new BlackBerryRunControlFactory);
    addAutoReleasedObject(new BlackBerryWizardExtension);

    // Handles QNX
    addAutoReleasedObject(new QnxQtVersionFactory);
    addAutoReleasedObject(new QnxDeviceConfigurationFactory);
    addAutoReleasedObject(new QnxRunControlFactory);
    addAutoReleasedObject(new QnxDeployStepFactory);
    addAutoReleasedObject(new QnxDeployConfigurationFactory);
    addAutoReleasedObject(new QnxRunConfigurationFactory);

    return true;
}

void QNXPlugin::extensionsInitialized()
{
}

ExtensionSystem::IPlugin::ShutdownFlag QNXPlugin::aboutToShutdown()
{
    return SynchronousShutdown;
}

Q_EXPORT_PLUGIN2(QNX, QNXPlugin)
