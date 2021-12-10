/*
 * Copyright (C) 2021 ~ 2021 Uniontech Software Technology Co., Ltd.
 *
 * Author:     wangchunlin<wangchunlin@uniontech.com>
 *
 * Maintainer: wangchunlin<wangchunlin@uniontech.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "picscreensaver.h"
#include "picscreensaverconfig.h"

#include <QLabel>
#include <QtDebug>
#include <QDir>
#include <QPaintEvent>
#include <QMimeType>
#include <QPixmap>
#include <QPainter>
#include <QScreen>
#include <QWindow>
#include <QRandomGenerator>

PicScreenSaver::PicScreenSaver(bool subWindow, QWidget *parent)
    : QWidget(parent)
    , m_subWindow(subWindow)
{
    if (m_subWindow)
        setWindowFlag(Qt::WindowTransparentForInput, true);

    QString path = PicScreenSaverConfig::instance()->imagePath();
    QDir dir(path);
    QDir::Filters filters = QDir::Files | QDir::NoDotAndDotDot | QDir::Readable;
    QFileInfoList infoList = dir.entryInfoList(filters);
    if (infoList.isEmpty()) {
        qWarning()<<"Error:no image file!";
        return;
    }

    for (auto info : infoList) {
        QMimeType mime = m_mimeDataBase.mimeTypeForFile(info);
        if (mime.name().startsWith("image/")) {
            // 记录图片列表
            m_imagefiles.append(info.absoluteFilePath());

            if (m_pixmap.isNull()) {
                m_pixmap.reset(new QPixmap(info.absoluteFilePath()));
                m_currentImage = 0;
            }
        }
    }

    m_interval = PicScreenSaverConfig::instance()->isRotate();
    m_intervalTime = PicScreenSaverConfig::instance()->intervalTime();
    m_intervalMode = PicScreenSaverConfig::instance()->intervalMode();

    if (m_interval) {

        m_timer.reset(new QTimer);
        connect(m_timer.get(), &QTimer::timeout, this, &PicScreenSaver::onUpdateImage);

        m_timer->setInterval(m_intervalTime*1000);
        m_timer->start();
    }
}

PicScreenSaver::~PicScreenSaver()
{

}

void PicScreenSaver::playScreenSaver()
{
    if (m_play)
        return;
    m_play = true;

    show();
}

void PicScreenSaver::onUpdateImage()
{
    if (!m_interval)
        return;

    switch (m_intervalMode) {
    case 0:
        {
            // 正序
            if (m_currentImage < m_imagefiles.count() - 1)
                m_currentImage++;
            else
                m_currentImage = 0;

            break;
        }
    case 1:
        {
            // 逆序
            if (m_currentImage > 0)
                m_currentImage--;
            else
                m_currentImage = m_imagefiles.count() - 1;

            break;
        }
    case 2:
        {
            // 随机
            m_currentImage = int(QRandomGenerator::global()->generate() / uint((m_imagefiles.count() - 1)));

            break;
        }
    default:
        {
            // 保持不切换
            return;
        }
    }
    m_pixmap.reset(new QPixmap(m_imagefiles.at(m_currentImage)));
    update();
}

void PicScreenSaver::paintEvent(QPaintEvent *event)
{
    if (m_pixmap) {

        qreal scale = devicePixelRatioF();

        QSize trueSize(this->windowHandle()->screen()->geometry().size());
        auto pix = m_pixmap->scaled(trueSize, Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);

        if (pix.width() > trueSize.width() || pix.height() > trueSize.height()) {
            pix = pix.copy(QRect(static_cast<int>((pix.width() - trueSize.width()) / 2.0),
                                 static_cast<int>((pix.height() - trueSize.height()) / 2.0),
                                 trueSize.width(),
                                 trueSize.height()));
        }



        QPainter pa(this);
        pa.drawPixmap(event->rect().topLeft(), pix, QRectF(QPointF(event->rect().topLeft()) * scale, QSizeF(event->rect().size()) * scale));
    }

    return QWidget::paintEvent(event);
}

