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

#ifndef QNX_INTERNAL_BLACKBERRYDEPLOYCONFIGURATIONWIDGET_H
#define QNX_INTERNAL_BLACKBERRYDEPLOYCONFIGURATIONWIDGET_H

#include <projectexplorer/deployconfiguration.h>

namespace Qnx {
namespace Internal {

namespace Ui {
class BlackBerryDeployConfigurationWidget;
}

class BlackBerryDeployConfiguration;

class BlackBerryDeployConfigurationWidget : public ProjectExplorer::DeployConfigurationWidget
{
    Q_OBJECT
public:
    explicit BlackBerryDeployConfigurationWidget(QWidget *parent = 0);
    ~BlackBerryDeployConfigurationWidget();

    void init(ProjectExplorer::DeployConfiguration *dc);

private slots:
    void showDeviceConfigurations();
    void updateUi();
    void updateBarPackage(const QString &barPackage);
    void handleDeviceConfigurationListChanged();

private:
    Ui::BlackBerryDeployConfigurationWidget *m_ui;

    BlackBerryDeployConfiguration *m_deployConfiguration;
};

} // namespace Internal
} // namespace Qnx

#endif // QNX_INTERNAL_BLACKBERRYDEPLOYCONFIGURATIONWIDGET_H
