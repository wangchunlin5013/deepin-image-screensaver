/*
 * Copyright (C) 2021 Uniontech Software Technology Co., Ltd.
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
#ifndef PICSCREENSAVERCONFIG_H
#define PICSCREENSAVERCONFIG_H

#include <QObject>
#include <QSettings>
#include <QScopedPointer>

class PicScreenSaverConfig : public QObject
{
    Q_OBJECT
public:
    static PicScreenSaverConfig *instance();

    QString imagePath() const;
    void setImagePath(const QString &path);

    bool isRotate() const;
    void setRotate(const bool rotate);

    int intervalTime() const;
    void setIntervalTime(const int time);

    int intervalMode() const;
    void setIntervalMode(const int mode);

private:
    explicit PicScreenSaverConfig(QObject *parent = nullptr);

    QString path() const;

private:
    QScopedPointer<QSettings> m_settings;
};

#endif // PICSCREENSAVERCONFIG_H
