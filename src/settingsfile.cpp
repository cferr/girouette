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

#include "settingsfile.h"

settingsFile::settingsFile(QString location)
{

    if(!QFile::exists(location))
    {
        this->fontF = new QFile(location);
        this->fontF->open(QFile::WriteOnly);
        this->fontF->write("<!DOCTYPE girouette-settings>\n<settings>\n</settings>");
        this->fontF->close();
    }
    if(QFile::exists(location))
    {

        this->dom = new QDomDocument();
        this->fontF = new QFile(location);
        this->fontF->open(QFile::ReadWrite);
        this->textStream.setDevice(this->fontF);
        this->dom->setContent(fontF);

    }
}
void settingsFile::saveFile()
{
    this->fontF->resize(0);
    this->dom->save(this->textStream,2);
}

QString settingsFile::readParam(QString param)
{
    QString ret;
    QDomElement docElem = this->dom->documentElement();
    QDomNode fontNode = docElem.firstChild();
    while(!fontNode.isNull())
    {
        if(fontNode.toElement().nodeName() == "param" && fontNode.toElement().attribute("name") == param)
        {
            ret = fontNode.toElement().attribute("value");
        }

        fontNode = fontNode.nextSibling();
    }

    return ret;
}

bool settingsFile::saveParam(QString param, QString value)
{
    bool ret;
    QDomElement docElem = this->dom->documentElement();
    QDomNode fontNode = docElem.firstChild();
    bool paramExists = false;
    while(!fontNode.isNull())
    {
        if(fontNode.toElement().nodeName() == "param" && fontNode.toElement().attribute("name") == param)
        {
            paramExists = true;
            fontNode.toElement().setAttribute("value",value);
            ret = true;
        }

        fontNode = fontNode.nextSibling();
    }

    if(!paramExists)
    {
        QDomElement element = this->dom->createElement("param");
        element.setAttribute("name",param);
        element.setAttribute("value",value);
        docElem.appendChild(element);
        ret = true;

    }

    saveFile();
    return ret;
}
void settingsFile::reload()
{
    this->fontF->close();
    this->fontF->open(QFile::ReadWrite);
    this->textStream.setDevice(this->fontF);
    this->dom->setContent(fontF);
}
