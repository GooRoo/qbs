/****************************************************************************
**
** Copyright (C) 2019 Denis Shienkov <denis.shienkov@gmail.com>
** Contact: http://www.qt.io/licensing
**
** This file is part of Qbs.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms and
** conditions see http://www.qt.io/terms-conditions. For further information
** use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file.  Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, The Qt Company gives you certain additional
** rights.  These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
****************************************************************************/

#include "avrarchiversettingsgroup_v7.h"
#include "avrassemblersettingsgroup_v7.h"
#include "avrbuildconfigurationgroup_v7.h"
#include "avrcompilersettingsgroup_v7.h"
#include "avrgeneralsettingsgroup_v7.h"
#include "avrlinkersettingsgroup_v7.h"

#include "../../iarewtoolchainpropertygroup.h"
#include "../../iarewutils.h"

namespace qbs {
namespace v7 {

AvrBuildConfigurationGroup::AvrBuildConfigurationGroup(
        const Project &qbsProject,
        const ProductData &qbsProduct,
        const std::vector<ProductData> &qbsProductDeps)
    : IarewPropertyGroup("configuration")
{
    // Append configuration name item.
    const QString cfgName = IarewUtils::buildConfigurationName(qbsProject);
    appendProperty("name", cfgName);

    // Apend toolchain name group item.
    appendChild<IarewToolchainPropertyGroup>("AVR");

    // Append debug info item.
    const int debugBuild = IarewUtils::debugInformation(qbsProduct);
    appendProperty("debug", debugBuild);

    // Append settings group items.
    appendChild<AvrArchiverSettingsGroup>(
                qbsProject, qbsProduct, qbsProductDeps);
    appendChild<AvrAssemblerSettingsGroup>(
                qbsProject, qbsProduct, qbsProductDeps);
    appendChild<AvrCompilerSettingsGroup>(
                qbsProject, qbsProduct, qbsProductDeps);
    appendChild<AvrGeneralSettingsGroup>(
                qbsProject, qbsProduct, qbsProductDeps);
    appendChild<AvrLinkerSettingsGroup>(
                qbsProject, qbsProduct, qbsProductDeps);
}

bool AvrBuildConfigurationGroupFactory::canCreate(
        IarewUtils::Architecture architecture,
        const Version &version) const
{
    return architecture == IarewUtils::Architecture::AvrArchitecture
            && version.majorVersion() == 7;
}

std::unique_ptr<IarewPropertyGroup> AvrBuildConfigurationGroupFactory::create(
        const Project &qbsProject,
        const ProductData &qbsProduct,
        const std::vector<ProductData> &qbsProductDeps) const
{
    const auto group = new AvrBuildConfigurationGroup(
                qbsProject, qbsProduct, qbsProductDeps);
    return std::unique_ptr<AvrBuildConfigurationGroup>(group);
}

} // namespace v7
} // namespace qbs
