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
#include "commandlinemanager.h"

CommandLineManager::CommandLineManager()
    : m_commandParser(new QCommandLineParser)
{
    // 必须设置为长选项模式，否则解析时 -window-id 将识别错误
    m_commandParser->setSingleDashWordOptionMode(QCommandLineParser::ParseAsLongOptions);

    initOptions();
    m_commandParser->addHelpOption();
    m_commandParser->addVersionOption();
}

CommandLineManager::~CommandLineManager()
{

}

void CommandLineManager::initOptions()
{
    QCommandLineOption subWindowOption("window-id", "displays as a child of the given ID window.\nNOTE:is '-window-id',not '--window-id'", "ID");
    QCommandLineOption directoryOption(QStringList() << "d" << "directory", "set the path to find pictures.", "PATH");
    QCommandLineOption rotateOption(QStringList() << "r" << "rotate", "whether to rotate.\n  true:rotate\n  false:no rotate", "ROTATE");
    QCommandLineOption intervalTimeOption(QStringList() << "t" << "interval-time", "set the number of seconds between switches.", "TIME");
    QCommandLineOption intervalModeOption(QStringList() << "m" << "interval-mode", "set the mode of switch.\n  0:random\n  1:positive\n  2:reverse", "MODE");

    addOption(subWindowOption);
    addOption(directoryOption);
    addOption(rotateOption);
    addOption(intervalTimeOption);
    addOption(intervalModeOption);
}

CommandLineManager *CommandLineManager::instance()
{
    static CommandLineManager instance;
    return &instance;
}

bool CommandLineManager::isSet(const QString &name) const
{
    return m_commandParser->isSet(name);
}

QString CommandLineManager::value(const QString &name) const
{
    return m_commandParser->value(name);
}

void CommandLineManager::process(const QStringList &arguments)
{
    return m_commandParser->process(arguments);
}

void CommandLineManager::addOption(const QCommandLineOption &option)
{
    m_commandParser->addOption(option);
}

void CommandLineManager::addOptions(const QList<QCommandLineOption> &options)
{
    for (auto option : options) {
        addOption(option);
    }
}

QStringList CommandLineManager::positionalArguments()
{
    return m_commandParser->positionalArguments();
}

QStringList CommandLineManager::unknownOptionNames()
{
    return m_commandParser->unknownOptionNames();
}


