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
#include "commandlinemanager.h"
#include "picscreensaverconfig.h"
#include "picscreensaver.h"

#include <QApplication>
#include <QtDebug>
#include <QWindow>
#include <QMessageBox>
#include <QScreen>
#include <QStringList>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setOrganizationName("deepin");

    CommandLineManager::instance()->process(a.arguments());

    if (CommandLineManager::instance()->isSet("window-id")) {

        QString windowId = CommandLineManager::instance()->value("window-id");

        WId windowHwnd = WId(windowId.toULongLong());
        QWindow *window = QWindow::fromWinId(windowHwnd);

        if (!window) {
            qDebug() << "Error:not found QWindow by window id:" << windowId;
            return -1;
        }

        PicScreenSaver w(true);   // 自定义屏保的显示主界面
        w.setProperty("_q_embedded_native_parent_handle",QVariant(windowHwnd));
        w.winId();
        w.windowHandle()->setParent(window);
        QRect wRect = window->screen()->geometry();
        w.setGeometry(QRect(0, 0, wRect.width(), wRect.height()));

        w.playScreenSaver();

        return a.exec();

    } else if (CommandLineManager::instance()->isSet("d")) {

        QString path = CommandLineManager::instance()->value("d");
        PicScreenSaverConfig::instance()->setImagePath(path);

    } else if (CommandLineManager::instance()->isSet("r")) {

        QString rotate = CommandLineManager::instance()->value("r");
        if ("true" == rotate)
            PicScreenSaverConfig::instance()->setRotate(true);
        else
            PicScreenSaverConfig::instance()->setRotate(false);

    } else if (CommandLineManager::instance()->isSet("t")) {

        QString time = CommandLineManager::instance()->value("t");
        PicScreenSaverConfig::instance()->setIntervalTime(time.toInt());

    } else if (CommandLineManager::instance()->isSet("m")) {

        QString mode = CommandLineManager::instance()->value("m");
        PicScreenSaverConfig::instance()->setIntervalMode(mode.toInt());

    } else {

        PicScreenSaver w;
        w.playScreenSaver();

        return a.exec();
    }

    return 0;
}
