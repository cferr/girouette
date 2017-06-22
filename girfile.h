//    girouette - Bus sign editor
//    Copyright (C) 2011 Corentin FERRY
//
//    This program is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with this program.  If not, see <http://www.gnu.org/licenses/>.

#ifndef GIRFILE_H
#define GIRFILE_H

#include <QFile>
#include <QtXml>
#include <QStringList>
#include <QTextStream>

class girFile
{
public:
    girFile(QString location="./gir.temp.xml");
    QFile *girF;
    QDomDocument *dom;
    QStringList listMessages();
    QStringList listDisplays(QString msgName);
    QStringList listSeq(QString msgName, QString dispName);
    QStringList listLines(QString msgName, QString dispName, QString seqEltName);
    QString readDisplayProp(QString msgName, QString displayName, QString param);
    QString readLineParam(QString msgName, QString dispName, QString seqEltName, QString lineName, QString param);
    QString readLine(QString msgName, QString dispName, QString seqEltName, QString lineName); //Deprecated
    QString readLineFont(QString msgName, QString dispName, QString seqEltName, QString lineName); //Deprecated
    QString readSeqProp(QString msgName, QString dispName, QString seqName, QString param);

    bool createMessage(QString msgName);
    bool renameMessage(QString oldName, QString newname);
    bool removeMessage(QString msgName);

    bool createDisplay(QString msgName, QString displayName);
    bool renameDisplay(QString msgName, QString oldName, QString newname);
    bool setDisplayProp(QString msgName, QString displayName, QString param, QString value);
    bool removeDisplay(QString msgName, QString displayName);

    bool createSeq(QString msgName, QString dispName, QString seqName);
    bool renameSeq(QString msgName, QString dispName, QString oldName, QString newname);
    bool setSeqProp(QString msgName, QString dispName, QString seqName, QString param, QString value);
    bool removeSeq(QString msgName, QString dispName, QString seqName);

    bool setLineParam(QString msgName, QString dispName, QString seqEltName, QString lineName, QString param, QString value);
    bool setLine(QString msgName, QString dispName, QString seqEltName, QString lineName, QString data); //Deprecated
    bool setLineFont(QString msgName, QString dispName, QString seqEltName, QString lineName, QString fontName); //Deprecated

    void saveFile();
    void saveFileAs(QString location);
    void reload();
    void end();
    QTextStream textStream;
};

#endif // GIRFILE_H
