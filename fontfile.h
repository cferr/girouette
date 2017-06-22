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

#ifndef FONTFILE_H
#define FONTFILE_H
#include <QFile>
#include <QtXml>
#include <QStringList>
#include <QTextStream>

class fontFile
{
public:
    fontFile(QString location = "./font.xml");
    QFile *fontF;
    QDomDocument *dom;
    QString currentFont;
    QString readChar(QString character);
    QTextStream textStream;
    void saveFile();
    void end();
    bool addChar(QString character);
    bool editCharData(QString character, QString data);
    bool removeChar(QString character);
    bool createFont(QString fontName);
    bool removeFont(QString fontName);
    bool renameFont(QString oldName, QString newName);
    QStringList listFonts();
    QStringList listChars(QString font);
    int readFontX(QString font);
    int readFontY(QString font);
    bool setFontX(QString font, int x);
    bool setFontY(QString font, int y);
    bool newFromNode(QDomElement fontNode);
    QDomElement getfontNode(QString fontName);
};

#endif // FONTFILE_H
