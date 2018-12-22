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
    Copyright (C) 2018 Nikita Sirgienko <warquark@gmail.com>
*/

#include "rvariablemodel.h"
#include "rsession.h"

using namespace Cantor;

RVariableModel::RVariableModel(RSession* session):
    DefaultVariableModel(session)
{
}

void RVariableModel::update()
{
    static_cast<RSession*>(session())->updateSymbols(this);
}

void RVariableModel::parseResult(const QStringList& names, const QStringList& values, const QStringList& funcs)
{
    QList<Variable> vars;
    for (int i = 0; i < names.size(); i++)
        vars.append(Variable{names[i], values[i]});
    setVariables(vars);

    // Handle functions
    QStringList addedFuncs;
    QStringList removedFuncs;

    int i = 0;
    while (i < m_functions.size())
        if (!funcs.contains(m_functions[i]))
        {
            removedFuncs << m_functions[i];
            m_functions.removeAt(i);
        }
        else
            i++;

    for (const QString newFunc : funcs)
        if(!m_functions.contains(newFunc))
        {
            addedFuncs << newFunc;
            m_functions << newFunc;
        }

    if (!addedFuncs.isEmpty())
        emit functionsAdded(addedFuncs);

    if (!removedFuncs.isEmpty())
        emit functionsRemoved(removedFuncs);
}

void RVariableModel::clearFunctions()
{
    emit functionsRemoved(m_functions);
    m_functions.clear();
}
