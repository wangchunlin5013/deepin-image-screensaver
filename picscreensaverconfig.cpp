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
#include "picscreensaverconfig.h"

#include <QApplication>
#include <QStandardPaths>
#include <QDir>
#include <QFileInfo>

static const char *const kGroupWorkPath = "base.image_dir.select_work_dir";
static const char *const kGroupEnableRotate = "base.rotate_settings.enable_rotate";
static const char *const kGroupRotateMode = "base.rotate_settings.rotate_mode";
static const char *const kGroupRotateTime = "base.rotate_settings.rotate_time";
static const char *const kKeyValue = "value";

static const char *const kDeepinScreenSaver = "deepin-screensaver";

PicScreenSaverConfig::PicScreenSaverConfig(QObject *parent) : QObject(parent)
{
    QString configPath = path();

    m_settings.reset(new QSettings(configPath, QSettings::IniFormat));
}

PicScreenSaverConfig *PicScreenSaverConfig::instance()
{
    static PicScreenSaverConfig config;
    return &config;
}

QString PicScreenSaverConfig::imagePath() const
{
    m_settings->beginGroup(kGroupWorkPath);
    QString path = m_settings->value(kKeyValue).toString();
    m_settings->endGroup();

    return path;
}

void PicScreenSaverConfig::setImagePath(const QString &path)
{
    m_settings->beginGroup(kGroupWorkPath);
    m_settings->setValue(kKeyValue, path);
    m_settings->endGroup();
}

bool PicScreenSaverConfig::isRotate() const
{
    m_settings->beginGroup(kGroupEnableRotate);
    bool rotate = m_settings->value(kKeyValue).toBool();
    m_settings->endGroup();

    return rotate;
}

void PicScreenSaverConfig::setRotate(const bool rotate)
{
    m_settings->beginGroup(kGroupEnableRotate);
    m_settings->setValue(kKeyValue, rotate);
    m_settings->endGroup();
}

int PicScreenSaverConfig::intervalTime() const
{
    m_settings->beginGroup(kGroupRotateTime);
    bool ok = false;
    int time = m_settings->value(kKeyValue).toInt(&ok);
    m_settings->endGroup();

    if (ok)
        return time;
    return -1;
}

void PicScreenSaverConfig::setIntervalTime(const int time)
{
    m_settings->beginGroup(kGroupRotateTime);
    m_settings->setValue(kKeyValue, time);
    m_settings->endGroup();
}

int PicScreenSaverConfig::intervalMode() const
{
    m_settings->beginGroup(kGroupRotateMode);
    bool ok = false;
    int mode = m_settings->value(kKeyValue).toInt(&ok);
    m_settings->endGroup();

    if (ok)
        return mode;
    return -1;
}

void PicScreenSaverConfig::setIntervalMode(const int mode)
{
    m_settings->beginGroup(kGroupRotateMode);
    m_settings->setValue(kKeyValue, mode);
    m_settings->endGroup();
}

QString PicScreenSaverConfig::path() const
{
    auto configPaths = QStandardPaths::standardLocations(QStandardPaths::ConfigLocation);
    Q_ASSERT(!configPaths.isEmpty());

    QString configPath = configPaths.first();
    configPath = configPath
                 + "/" + QApplication::organizationName()
                 + "/" + kDeepinScreenSaver + "/"
                 + QApplication::applicationName()
                 + "/" + QApplication::applicationName() + ".conf";

    QFileInfo confFile(configPath);

    // 本地配置文件不存在
    if (!confFile.exists()) {

        QDir confDir = confFile.absoluteDir();
        if (!confDir.exists())
            confDir.mkpath(confDir.absolutePath());

        // 系统配置文件存在，则拷贝。否则不做处理，后面设置值时会自动创建
        QString pathGeneral("/etc");
        pathGeneral = pathGeneral
                      + "/" + kDeepinScreenSaver
                      + "/" + QApplication::applicationName()
                      + "/" + QApplication::applicationName() + ".conf";
        QFile confGeneralFile(pathGeneral);
        if (confGeneralFile.exists()) {
            confGeneralFile.copy(configPath);
        }
    }

    return configPath;
}
