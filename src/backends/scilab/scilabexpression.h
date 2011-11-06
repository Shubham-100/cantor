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
    Copyright (C) 2011 Filipe Saraiva <filip.saraiva@gmail.com>
 */

#ifndef _SCILABEXPRESSION_H
#define _SCILABEXPRESSION_H

#include "expression.h"
#include <QStringList>

class QTimer;

class ScilabExpression : public Cantor::Expression
{
  Q_OBJECT
  public:
    ScilabExpression( Cantor::Session* session);
    ~ScilabExpression();

    void evaluate();
    void interrupt();
    void parseOutput(QString output);
    void parseError(QString error);
    void parsePlotFile();
    void setPlotPending(bool plot);

  public slots:
    void evalFinished();

  private:
    QTimer* m_timer;
    bool m_finished;
    bool m_plotPending;
    int numPlot;
};

#endif /* _SCILABEXPRESSION_H */
