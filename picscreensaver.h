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
#ifndef PICSCREENSAVER_H
#define PICSCREENSAVER_H

#include <QWidget>
#include <QPixmap>
#include <QTimer>
#include <QScopedPointer>
#include <QSettings>
#include <QMimeDatabase>

class PicScreenSaver : public QWidget
{
    Q_OBJECT

public:
    PicScreenSaver(bool subWindow = false, QWidget *parent = nullptr);
    ~PicScreenSaver() override;

    void playScreenSaver();

private slots:
    void onUpdateImage();

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QScopedPointer<QPixmap> m_pixmap;
    QScopedPointer<QTimer> m_timer;
    QScopedPointer<QSettings> m_settings;

    QString m_path;
    QStringList m_imagefiles;
    int m_currentImage = -1;

    bool m_play = false;
    bool m_interval = true;
    int m_intervalTime = 0;
    int m_intervalMode = 0;

    bool m_subWindow = false;

    QMimeDatabase m_mimeDataBase;
};

#endif // PICSCREENSAVER_H
