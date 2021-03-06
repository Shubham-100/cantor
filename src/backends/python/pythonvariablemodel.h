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
    Copyright (C) 2019 Nikita Sirgienko <warquark@gmail.com>
*/

#ifndef _PYTHONVARIABLEMODEL_H
#define _PYTHONVARIABLEMODEL_H

#include "defaultvariablemodel.h"

class PythonSession;
class QDBusInterface;

class PythonVariableModel : public Cantor::DefaultVariableModel
{
  public:
    PythonVariableModel( PythonSession* session);
    ~PythonVariableModel() override;

    void update() override;

  private:
    Cantor::Expression* m_expression{nullptr};

  private Q_SLOTS:
    void extractVariables(Cantor::Expression::Status status);
};

#endif /* _PYTHONVARIABLEMODEL_H */
