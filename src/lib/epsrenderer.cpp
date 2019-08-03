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
    Copyright (C) 2012 Martin Kuettler <martin.kuettler@gmail.com>
 */

#include "epsrenderer.h"

#include <config-cantorlib.h>

#ifdef LIBSPECTRE_FOUND
  #include "libspectre/spectre.h"
#endif

#include <QUuid>
#include <QDebug>

using namespace Cantor;

class Cantor::EpsRendererPrivate{
    public:
        double scale{1};
        bool useHighRes{false};
};

EpsRenderer::EpsRenderer() : d(new EpsRendererPrivate())
{
}

void EpsRenderer::setScale(qreal scale)
{
    d->scale = scale;
}

qreal EpsRenderer::scale()
{
    return d->scale;
}

void EpsRenderer::useHighResolution(bool b)
{
    d->useHighRes = b;
}

QTextImageFormat EpsRenderer::render(QTextDocument *document, const QUrl &url)
{
    QTextImageFormat epsCharFormat;

    QUrl internal;
    internal.setScheme(QLatin1String("internal"));
    QString path = QUuid::createUuid().toString();
    // Remove { and }
    path.remove(0, 1);
    path.chop(1);
    internal.setPath(path);

    QSizeF s = renderToResource(document, url, internal);

    if(s.isValid())
    {
        epsCharFormat.setName(internal.url());
        epsCharFormat.setWidth(s.width());
        epsCharFormat.setHeight(s.height());
    }

    return epsCharFormat;
}

QTextImageFormat EpsRenderer::render(QTextDocument *document,
                                     const Cantor::LatexRenderer* latex)
{
    QTextImageFormat format = render(document, QUrl::fromLocalFile(latex->imagePath()));

    if (!format.name().isEmpty()) {
        format.setProperty(CantorFormula, latex->method());
        format.setProperty(ImagePath, latex->imagePath());
        format.setProperty(Code, latex->latexCode());
    }

    return format;
}

QSizeF EpsRenderer::renderToResource(QTextDocument *document, const QUrl &url, const QUrl& internal)
{
    QSizeF size;
    QImage img = renderToImage(url, &size);

    qDebug() << internal;
    document->addResource(QTextDocument::ImageResource, internal, QVariant(img) );
    return size;
}

QImage EpsRenderer::renderToImage(const QUrl& url, double scale, bool useHighRes, QSizeF* size)
{
#ifdef LIBSPECTRE_FOUND
    SpectreDocument* doc = spectre_document_new();
    SpectreRenderContext* rc = spectre_render_context_new();

    qDebug() << "rendering eps file: " << url;
    QByteArray local_file = url.toLocalFile().toUtf8();
    spectre_document_load(doc, local_file.data());

    bool isEps = spectre_document_is_eps(doc);
    if (!isEps)
        qDebug() << "Error: spectre document is not eps! It means, that url is invalid";

    int wdoc, hdoc;
    qreal w, h;
    double realScale;
    spectre_document_get_page_size(doc, &wdoc, &hdoc);
    if(useHighRes) {
        realScale = 1.2*4.0; //1.2 scaling factor, to make it look nice, 4x for high resolution
        w = 1.2 * wdoc;
        h = 1.2 * hdoc;
    } else {
        realScale=1.8*scale;
        w = 1.8 * wdoc;
        h = 1.8 * hdoc;
    }

    qDebug()<<"scale: "<<realScale;

    qDebug()<<"dimension: "<<w<<"x"<<h;
    unsigned char* data;
    int rowLength;

    spectre_render_context_set_scale(rc, realScale, realScale);
    spectre_document_render_full( doc, rc, &data, &rowLength);

    QImage img(data, wdoc*realScale, hdoc*realScale, rowLength, QImage::Format_RGB32);
    spectre_document_free(doc);
    spectre_render_context_free(rc);
    img = img.convertToFormat(QImage::Format_ARGB32);

    if (size)
        *size = QSizeF(w,h);
    return img;
#else
    Q_UNUSED(url);
    Q_UNUSED(size);
    return QImage();
#endif
}

QImage EpsRenderer::renderToImage(const QUrl& url, QSizeF* size)
{
    return renderToImage(url, d->scale, d->useHighRes, size);
}