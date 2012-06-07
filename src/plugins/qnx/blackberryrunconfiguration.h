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

#ifndef QNX_INTERNAL_BLACKBERRYRUNCONFIGURATION_H
#define QNX_INTERNAL_BLACKBERRYRUNCONFIGURATION_H

#include <projectexplorer/runconfiguration.h>

namespace Qt4ProjectManager {
class Qt4BaseTarget;
class Qt4BuildConfiguration;
class Qt4ProFileNode;
}

namespace Qnx {
namespace Internal {

class QnxTarget;
class BlackBerryDeployConfiguration;

class BlackBerryRunConfiguration : public ProjectExplorer::RunConfiguration
{
    Q_OBJECT
    friend class BlackBerryRunConfigurationFactory;

public:
    explicit BlackBerryRunConfiguration(Qt4ProjectManager::Qt4BaseTarget *parent, const QString &path);

    QWidget *createConfigurationWidget();

    QString proFilePath() const;

    QString deviceName() const;
    QString barPackage() const;

    QString localExecutableFilePath() const;

    bool fromMap(const QVariantMap &map);
    QVariantMap toMap() const;

    BlackBerryDeployConfiguration *deployConfiguration() const;
    Qt4ProjectManager::Qt4BuildConfiguration *activeQt4BuildConfiguration() const;

    QString key() const;

signals:
    void targetInformationChanged();

protected:
    BlackBerryRunConfiguration(Qt4ProjectManager::Qt4BaseTarget *parent, BlackBerryRunConfiguration *source);

private:
    void init();
    void updateDisplayName();

    QnxTarget *qt4Target() const;

    QString m_proFilePath;
};

} // namespace Internal
} // namespace Qnx

#endif // QNX_INTERNAL_BLACKBERRYRUNCONFIGURATION_H
