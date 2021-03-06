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
    Copyright (C) 2009 Alexander Rieder <alexanderrieder@gmail.com>
    Copyright (C) 2019 Alexander Semke <alexander.semke@web.de>
 */

#include "sagebackend.h"
#include "sageextensions.h"
#include "sagesession.h"
#include "settings.h"
#include "ui_settings.h"

SageBackend::SageBackend( QObject* parent,const QList<QVariant>& args ) : Cantor::Backend( parent,args )
{
    //initialize the supported extensions
    new SageHistoryExtension(this);
    new SageScriptExtension(this);
    new SageCASExtension(this);
    new SageCalculusExtension(this);
    new SageLinearAlgebraExtension(this);
    new SagePlotExtension(this);
    new SagePackagingExtension(this);
}

SageBackend::~SageBackend()
{
    qDebug()<<"Destroying SageBackend";
}

QString SageBackend::id() const
{
    return QLatin1String("sage");
}

QString SageBackend::version() const
{
    return QLatin1String("8.1, 8.2");
}

Cantor::Session* SageBackend::createSession()
{
    qDebug()<<"Spawning a new Sage session";

    return new SageSession(this);
}

Cantor::Backend::Capabilities SageBackend::capabilities() const
{
    Cantor::Backend::Capabilities caps = Cantor::Backend::SyntaxHighlighting|Cantor::Backend::Completion;

    // Latex output from sage sometimes correct, sometimes not, so allow disable typesetting, if user want it
    if (SageSettings::self()->allowLatex())
        caps |= Cantor::Backend::LaTexOutput;

    return caps;
}

bool SageBackend::requirementsFullfilled(QString* const reason) const
{
    const QString& path = SageSettings::self()->path().toLocalFile();
    return Cantor::Backend::checkExecutable(QLatin1String("Sage"), path, reason);
}

QWidget* SageBackend::settingsWidget(QWidget* parent) const
{
    QWidget* widget=new QWidget(parent);
    Ui::SageSettingsBase s;
    s.setupUi(widget);
    return widget;
}

KConfigSkeleton* SageBackend::config() const
{
    return SageSettings::self();
}

QUrl SageBackend::helpUrl() const
{
    const QUrl& localDoc = SageSettings::self()->localDoc();
    if (!localDoc.isEmpty())
        return localDoc;
    else
    return QUrl(i18nc("the url to the documentation of Sage, please check if there is a translated version and use the correct url",
                 "https://doc.sagemath.org/html/en/reference/index.html"));
}

QString SageBackend::description() const
{
    return i18n("<b>Sage</b> is a free open-source mathematics software system licensed under the GPL. <br/>" \
                "It combines the power of many existing open-source packages into a common Python-based interface.");
}

K_PLUGIN_FACTORY_WITH_JSON(sagebackend, "sagebackend.json", registerPlugin<SageBackend>();)
#include "sagebackend.moc"
