/*
    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA  02110-1301, USA.

    ---
    Copyright (C) 2013 Filipe Saraiva <filipe@kde.org>
 */

#ifndef SCILABEXTENSIONS_H
#define SCILABEXTENSIONS_H

#include <extension.h>

#define SCILAB_EXT_CDTOR_DECL(name) Scilab##name##Extension(QObject* parent); \
                                     ~Scilab##name##Extension();

class ScilabScriptExtension : public Cantor::ScriptExtension
{
    public:
        SCILAB_EXT_CDTOR_DECL(Script)
        QString scriptFileFilter() override;
        QString highlightingMode() override;
        QString runExternalScript(const QString& path) override;
        QString commandSeparator() override;
};

class ScilabVariableManagementExtension : public Cantor::VariableManagementExtension
{
    public:
        SCILAB_EXT_CDTOR_DECL(VariableManagement)
        QString addVariable(const QString& name, const QString& value) override;
        QString setValue(const QString& name, const QString& value) override;
        QString removeVariable(const QString& name) override;
        QString saveVariables(const QString& fileName) override;
        QString loadVariables(const QString& fileName) override;
        QString clearVariables() override;
};

#endif // SCILABEXTENSIONS_H
