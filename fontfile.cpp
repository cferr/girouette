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

#include "fontfile.h"
#include <QMessageBox>

fontFile::fontFile(QString location)
{

    if(!QFile::exists(location))
    {
        this->fontF = new QFile(location);
        this->fontF->open(QFile::WriteOnly);
        this->fontF->write("<!DOCTYPE girouette-font>\n<fontlist>\n</fontlist>");
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

void fontFile::saveFile()
{
    this->fontF->resize(0);
    this->dom->save(this->textStream,2);
}

void fontFile::end()
{
    this->fontF->close();
}

QString fontFile::readChar(QString character)
{
    QString ret;
    QDomElement docElem = this->dom->documentElement(); //The document
    //Here we're gonna switch to the right font. If none is defined, then either the app crashes (in the case the name hasn't been previously defined), or either this function returns NULL.
    QDomNode fontNode = docElem.firstChild();
    while(!fontNode.isNull())
    {

        QDomElement font = fontNode.toElement();
        if(font.nodeName() == "font" && font.attribute("name") == this->currentFont)
        {
            //Here we're right;
            QDomNode charNode = font.firstChild();
            while(!charNode.isNull())
            {
                //Parcourir Z ou la liste des caractères...
                QDomElement currChar = charNode.toElement();
                if(currChar.nodeName() == "char" && currChar.attribute("name") == character)
                {
                    ret = currChar.attribute("data","");
                }
                charNode = charNode.nextSibling();
            }

        }
        fontNode = fontNode.nextSibling();
    }

    return ret;
}

QStringList fontFile::listFonts()
{

    QStringList ret;
    QDomElement docElem = this->dom->documentElement();
    QDomNode fontNode = docElem.firstChild();
    while(!fontNode.isNull())
    {
        //qDebug("found an element: %s",fontNode.toElement().tagName().toStdString().c_str());
        if(fontNode.toElement().nodeName() == "font")
        {


            ret.append(fontNode.toElement().attribute("name"));
        }

        fontNode = fontNode.nextSibling();
    }

    return ret;
}

QStringList fontFile::listChars(QString font)
{

    QStringList ret;
    QDomElement docElem = this->dom->documentElement();
    QDomNode fontNode = docElem.firstChild();
    while(!fontNode.isNull())
    {
        if(fontNode.toElement().nodeName() == "font" && fontNode.toElement().attribute("name") == font)
        {
            QDomNode charNode = fontNode.firstChild();
            while(!charNode.isNull())
            {
                if(charNode.toElement().tagName() == "char")
                {

                    ret.append(charNode.toElement().attribute("name"));
                }
                charNode = charNode.nextSibling();
            }

        }

        fontNode = fontNode.nextSibling();
    }

    return ret;
}

bool fontFile::addChar(QString character)
{
    bool ret;
    QDomElement docElem = this->dom->documentElement(); //The document
    //Here we're gonna switch to the right font. If none is defined, then either the app crashes (in the case the name hasn't been previously defined), or either this function returns NULL.
    QDomNode fontNode = docElem.firstChild();
    while(!fontNode.isNull())
    {
        QDomElement font = fontNode.toElement();
        if(font.nodeName() == "font" && font.attribute("name") == this->currentFont)
        {
            bool charAlreadyExists = false;
            QDomNode charNode = font.firstChild();
            while(!charNode.isNull())
            {
                QDomElement currChar = charNode.toElement();
                if(currChar.nodeName() == "char" && currChar.attribute("name") == character)
                {
                    charAlreadyExists = true;
                }
                charNode = charNode.nextSibling();
            }
            if(!charAlreadyExists)
            {
                QDomElement charElement = this->dom->createElement("char");


                charElement.setAttribute("name",character);


                font.appendChild(charElement);
                ret = true;
            }
            else
            {
                ret = false;
            }

        }
        fontNode = fontNode.nextSibling();
    }
    saveFile();
    return ret;
}

bool fontFile::editCharData(QString character, QString data)
{
    bool ret = false;
    QDomElement docElem = this->dom->documentElement(); //The document
    //Here we're gonna switch to the right font. If none is defined, then either the app crashes (in the case the name hasn't been previously defined), or either this function returns NULL.
    QDomNode fontNode = docElem.firstChild();
    while(!fontNode.isNull())
    {
        QDomElement font = fontNode.toElement();
        if(font.nodeName() == "font" && font.attribute("name") == this->currentFont)
        {

            QDomNode charNode = font.firstChild();
            while(!charNode.isNull())
            {
                QDomElement currChar = charNode.toElement();
                if(currChar.nodeName() == "char" && currChar.attribute("name") == character)
                {

                    currChar.setAttribute("data",data);
                    ret = true;
                }

                charNode = charNode.nextSibling();
            }

        }

        fontNode = fontNode.nextSibling();
    }
    saveFile();
    return ret;
}
bool fontFile::removeChar(QString character)
{
    bool ret = false;
    QDomElement docElem = this->dom->documentElement(); //The document
    //Here we're gonna switch to the right font. If none is defined, then either the app crashes (in the case the name hasn't been previously defined), or either this function returns NULL.
    QDomNode fontNode = docElem.firstChild();
    while(!fontNode.isNull())
    {
        QDomElement font = fontNode.toElement();
        if(font.nodeName() == "font" && font.attribute("name") == this->currentFont)
        {

            QDomNode charNode = font.firstChild();
            while(!charNode.isNull())
            {
                QDomElement currChar = charNode.toElement();
                if(currChar.nodeName() == "char" && currChar.attribute("name") == character)
                {
                    fontNode.removeChild(charNode);
                }

                charNode = charNode.nextSibling();
            }

        }

        fontNode = fontNode.nextSibling();
    }
    saveFile();
    return ret;
}
bool fontFile::createFont(QString fontName)
{
    bool ret;
    bool fontAlreadyExists = false;
    QDomElement docElem = this->dom->documentElement(); //The document
    //Here we're gonna switch to the right font. If none is defined, then either the app crashes (in the case the name hasn't been previously defined), or either this function returns NULL.
    QDomNode fontNode = docElem.firstChild();
    while(!fontNode.isNull())
    {

        QDomElement font = fontNode.toElement();
        if(font.nodeName() == "font" && font.attribute("name") == fontName)
        {
            fontAlreadyExists = true;

        }
        fontNode = fontNode.nextSibling();
    }

    if(!fontAlreadyExists)
    {
        QDomElement fontElement = this->dom->createElement("font");
        //fontElement.setTagName("font");
        
        fontElement.setAttribute("name",fontName);

        //qDebug("found %s",this->dom->firstChild().toElement().tagName().toStdString().c_str());
        if(!this->dom->firstChild().appendChild(fontElement).isNull())
        {
            ret = true;
        }
        else
        {
            ret = false;
        }


    }
    else
    {
        ret = false;
    }
    saveFile();
    return ret;
}

bool fontFile::removeFont(QString fontName)
{
    bool ret = false;
    QDomElement docElem = this->dom->documentElement(); //The document
    //Here we're gonna switch to the right font. If none is defined, then either the app crashes (in the case the name hasn't been previously defined), or either this function returns NULL.
    QDomNode fontNode = docElem.firstChild();
    while(!fontNode.isNull())
    {

        QDomElement font = fontNode.toElement();
        if(font.nodeName() == "font" && font.attribute("name") == fontName)
        {
            docElem.removeChild(fontNode);
            ret = true;
        }
        fontNode = fontNode.nextSibling();
    }
    saveFile();
    return ret;
}

bool fontFile::renameFont(QString oldName, QString newName)
{
    bool ret = false;
    QDomElement docElem = this->dom->documentElement(); //The document "fontlist" element (global...)
    //Here we're gonna switch to the right font. If none is defined, then either the app crashes (in the case the name hasn't been previously defined), or either this function returns NULL.
    QDomNode fontNode = docElem.firstChild();
    while(!fontNode.isNull())
    {

        QDomElement font = fontNode.toElement();
        if(font.nodeName() == "font" && font.attribute("name") == oldName)
        {
            font.setAttribute("name",newName);
            ret = true;
        }
        fontNode = fontNode.nextSibling();
    }
    saveFile();
    return ret;
}
int fontFile::readFontX(QString fontName)
{
    int ret = 10;
    QDomElement docElem = this->dom->documentElement(); //The document "fontlist" element (global...)
    //Here we're gonna switch to the right font. If none is defined, then either the app crashes (in the case the name hasn't been previously defined), or either this function returns NULL.
    QDomNode fontNode = docElem.firstChild();
    while(!fontNode.isNull())
    {

        QDomElement font = fontNode.toElement();
        if(font.nodeName() == "font" && font.attribute("name") == fontName)
        {
            ret = font.attribute("x").toInt();

        }
        fontNode = fontNode.nextSibling();
    }

    return ret;
}

int fontFile::readFontY(QString fontName)
{
    int ret = 10;
    QDomElement docElem = this->dom->documentElement(); //The document "fontlist" element (global...)
    //Here we're gonna switch to the right font. If none is defined, then either the app crashes (in the case the name hasn't been previously defined), or either this function returns NULL.
    QDomNode fontNode = docElem.firstChild();
    while(!fontNode.isNull())
    {

        QDomElement font = fontNode.toElement();
        if(font.nodeName() == "font" && font.attribute("name") == fontName)
        {
            ret = font.attribute("y").toInt();

        }
        fontNode = fontNode.nextSibling();
    }

    return ret;
}

bool fontFile::setFontX(QString fontName, int x)
{
    bool ret = false;
    QDomElement docElem = this->dom->documentElement(); //The document "fontlist" element (global...)
    //Here we're gonna switch to the right font. If none is defined, then either the app crashes (in the case the name hasn't been previously defined), or either this function returns NULL.
    QDomNode fontNode = docElem.firstChild();
    while(!fontNode.isNull())
    {

        QDomElement font = fontNode.toElement();
        if(font.nodeName() == "font" && font.attribute("name") == fontName)
        {
            font.setAttribute("x",x);
            saveFile();
            ret = true;
        }
        fontNode = fontNode.nextSibling();
    }

    return ret;
}

bool fontFile::setFontY(QString fontName, int y)
{
    bool ret = false;
    QDomElement docElem = this->dom->documentElement(); //The document "fontlist" element (global...)
    //Here we're gonna switch to the right font. If none is defined, then either the app crashes (in the case the name hasn't been previously defined), or either this function returns NULL.
    QDomNode fontNode = docElem.firstChild();
    while(!fontNode.isNull())
    {

        QDomElement font = fontNode.toElement();
        if(font.nodeName() == "font" && font.attribute("name") == fontName)
        {
            font.setAttribute("y", y);
            saveFile();
            ret = true;
        }
        fontNode = fontNode.nextSibling();
    }

    return ret;
}

QDomElement fontFile::getfontNode(QString fontName)
{
    QDomElement ret;
    QDomElement docElem = this->dom->documentElement(); //The document "fontlist" element (global...)
    //Here we're gonna switch to the right font. If none is defined, then either the app crashes (in the case the name hasn't been previously defined), or either this function returns NULL.
    QDomNode fontNode = docElem.firstChild();
    while(!fontNode.isNull())
    {

        QDomElement font = fontNode.toElement();
        if(font.nodeName() == "font" && font.attribute("name") == fontName)
        {
            ret = fontNode.toElement();
        }
        fontNode = fontNode.nextSibling();
    }
    return ret;
}

bool fontFile::newFromNode(QDomElement fontNode)
{

    QDomElement docElem = this->dom->documentElement(); //The document "fontlist" element (global...)

    bool ret =  docElem.appendChild(fontNode).isNull()?false:true;
    saveFile();
    return ret;

}
