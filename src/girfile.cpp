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

#include "girfile.h"

girFile::girFile(QString location)
{
    if(!QFile::exists(location))
    {
        this->girF = new QFile(location);
        this->girF->open(QFile::WriteOnly);
        this->girF->write("<!DOCTYPE girouette>\n<girset>\n</girset>");
        this->girF->close();
    }
    if(QFile::exists(location))
    {

        this->dom = new QDomDocument();
        this->girF = new QFile(location);
        this->girF->open(QFile::ReadWrite);
        this->textStream.setDevice(this->girF);
        this->dom->setContent(girF);

    }
}


QStringList girFile::listMessages()
{

    QStringList ret;
    QDomElement docElem = this->dom->documentElement();
    QDomNode msgNode = docElem.firstChild();
    while(!msgNode.isNull())
    {
        if(msgNode.toElement().nodeName() == "message")
        {
            ret.append(msgNode.toElement().attribute("name"));
        }

        msgNode = msgNode.nextSibling();
    }

    return ret;
}

QStringList girFile::listDisplays(QString msgName)
{
    QStringList ret;
    QDomElement docElem = this->dom->documentElement();
    QDomNode msgNode = docElem.firstChild();
    while(!msgNode.isNull())
    {
        if(msgNode.toElement().nodeName() == "message" && msgNode.toElement().attribute("name") == msgName)
        {
            QDomNode disNode = msgNode.firstChild();
            while(!disNode.isNull())
            {
                if(disNode.toElement().nodeName() == "display")
                {
                    ret.append(disNode.toElement().attribute("name"));

                }
                disNode = disNode.nextSibling();
            }
        }

        msgNode = msgNode.nextSibling();
    }

    return ret;
}

QStringList girFile::listSeq(QString msgName, QString dispName)
{
    QStringList ret;
    QDomElement docElem = this->dom->documentElement();
    QDomNode msgNode = docElem.firstChild();
    while(!msgNode.isNull())
    {
        if(msgNode.toElement().nodeName() == "message" && msgNode.toElement().attribute("name") == msgName)
        {
            QDomNode disNode = msgNode.firstChild();
            while(!disNode.isNull())
            {
                if(disNode.toElement().nodeName() == "display" && disNode.toElement().attribute("name") == dispName)
                {
                    QDomNode seqNode = disNode.firstChild();
                    while(!seqNode.isNull())
                    {
                        if(seqNode.toElement().nodeName() == "seqElem")
                        {
                            ret.append(seqNode.toElement().attribute("name"));
                        }
                        seqNode = seqNode.nextSibling();
                    }

                }
                disNode = disNode.nextSibling();
            }
        }

        msgNode = msgNode.nextSibling();
    }

    return ret;
}

QStringList girFile::listLines(QString msgName, QString dispName, QString seqEltName)
{
    QStringList ret;
    QDomElement docElem = this->dom->documentElement();
    QDomNode msgNode = docElem.firstChild();
    while(!msgNode.isNull())
    {
        if(msgNode.toElement().nodeName() == "message" && msgNode.toElement().attribute("name") == msgName)
        {
            QDomNode disNode = msgNode.firstChild();
            while(!disNode.isNull())
            {
                if(disNode.toElement().nodeName() == "display" && disNode.toElement().attribute("name") == dispName)
                {
                    QDomNode seqNode = disNode.firstChild();
                    while(!seqNode.isNull())
                    {
                        if(seqNode.toElement().nodeName() == "seqElem" && seqNode.toElement().attribute("name") == seqEltName)
                        {
                            QDomNode liNode = seqNode.firstChild();
                            while(!liNode.isNull())
                            {
                                if(liNode.toElement().nodeName() == "line")
                                {
                                    ret.append(liNode.toElement().attribute("name"));
                                }
                                liNode = liNode.nextSibling();
                            }

                        }
                        seqNode = seqNode.nextSibling();
                    }

                }
                disNode = disNode.nextSibling();
            }
        }

        msgNode = msgNode.nextSibling();
    }

    return ret;
}
QString girFile::readLine(QString msgName, QString dispName, QString seqEltName, QString lineName) //Deprecated
{
    return readLineParam(msgName, dispName, seqEltName, lineName, "data");
}
QString girFile::readLineParam(QString msgName, QString dispName, QString seqEltName, QString lineName, QString param)
{
    QString ret;
    QDomElement docElem = this->dom->documentElement();
    QDomNode msgNode = docElem.firstChild();
    while(!msgNode.isNull())
    {
        if(msgNode.toElement().nodeName() == "message" && msgNode.toElement().attribute("name") == msgName)
        {
            QDomNode disNode = msgNode.firstChild();
            while(!disNode.isNull())
            {
                if(disNode.toElement().nodeName() == "display" && disNode.toElement().attribute("name") == dispName)
                {
                    QDomNode seqNode = disNode.firstChild();
                    while(!seqNode.isNull())
                    {
                        if(seqNode.toElement().nodeName() == "seqElem" && seqNode.toElement().attribute("name") == seqEltName)
                        {
                            QDomNode liNode = seqNode.firstChild();
                            while(!liNode.isNull())
                            {
                                if(liNode.toElement().nodeName() == "line" && liNode.toElement().attribute("name") == lineName)
                                {
                                    ret = liNode.toElement().attribute(param);
                                }
                                liNode = liNode.nextSibling();

                            }

                        }
                        seqNode = seqNode.nextSibling();

                    }
                }
                disNode = disNode.nextSibling();
            }
        }

        msgNode = msgNode.nextSibling();
    }

    return ret;
}
QString girFile::readLineFont(QString msgName, QString dispName, QString seqEltName, QString lineName) //Deprecated
{
    return readLineParam(msgName, dispName, seqEltName, lineName, "font");
}

bool girFile::createMessage(QString msgName)
{

    bool ret;
    bool msgAlreadyExists = false;
    QDomElement docElem = this->dom->documentElement(); //The document
    QDomNode msgNode = docElem.firstChild();
    while(!msgNode.isNull())
    {
        QDomElement msg = msgNode.toElement();
        if(msg.nodeName() == "message" && msg.attribute("name") == msgName)
        {
            msgAlreadyExists = true;

        }
        msgNode = msgNode.nextSibling();
    }

    if(!msgAlreadyExists)
    {
        QDomElement msgElement = this->dom->createElement("message");


        msgElement.setAttribute("name",msgName);

        if(!this->dom->firstChild().appendChild(msgElement).isNull())
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

    return ret;
}

bool girFile::createDisplay(QString msgName, QString displayName)
{

    bool ret;
    bool dispAlreadyExists = false;
    QDomElement docElem = this->dom->documentElement(); //The document
    QDomNode msgNode = docElem.firstChild();
    while(!msgNode.isNull())
    {
        QDomElement msg = msgNode.toElement();
        if(msg.nodeName() == "message" && msg.attribute("name") == msgName)
        {
            QDomNode dispNode = msgNode.firstChild();
            while(!dispNode.isNull())
            {
                if(dispNode.toElement().nodeName() == "display" && dispNode.toElement().attribute("name") == displayName)
                {
                    dispAlreadyExists = true;
                }
                dispNode = dispNode.nextSibling();
            }
            if(!dispAlreadyExists)
            {
                QDomElement msgElement = this->dom->createElement("display");
                msgElement.setAttribute("name",displayName);

                if(!msgNode.appendChild(msgElement).isNull())
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

        }
        msgNode = msgNode.nextSibling();
    }



    return ret;
}

bool girFile::createSeq(QString msgName, QString displayName,QString seqName)
{

    bool ret;
    bool seqAlreadyExists = false;
    QDomElement docElem = this->dom->documentElement(); //The document
    QDomNode msgNode = docElem.firstChild();
    while(!msgNode.isNull())
    {
        QDomElement msg = msgNode.toElement();
        if(msg.nodeName() == "message" && msg.attribute("name") == msgName)
        {
            QDomNode dispNode = msgNode.firstChild();
            while(!dispNode.isNull())
            {
                if(dispNode.toElement().nodeName() == "display" && dispNode.toElement().attribute("name") == displayName)
                {
                    QDomNode seqNode = dispNode.firstChild();
                    while(!seqNode.isNull())
                    {
                        if(seqNode.toElement().nodeName() == "seqElem" && seqNode.toElement().attribute("name") == seqName)
                        {
                            seqAlreadyExists = true;
                        }
                        seqNode = seqNode.nextSibling();
                    }
                    if(!seqAlreadyExists)
                    {
                        QDomElement msgElement = this->dom->createElement("seqElem");
                        msgElement.setAttribute("name",seqName);

                        if(!dispNode.appendChild(msgElement).isNull())
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
                }
                dispNode = dispNode.nextSibling();
            }


        }
        msgNode = msgNode.nextSibling();
    }



    return ret;
}

void girFile::saveFile()
{
    this->girF->resize(0);
    this->dom->save(this->textStream,2);
}

bool girFile::removeMessage(QString msgName)
{
    bool ret = false;
    QDomElement docElem = this->dom->documentElement();
    QDomNode msgNode = docElem.firstChild();
    while(!msgNode.isNull())
    {
        if(msgNode.toElement().nodeName() == "message" && msgNode.toElement().attribute("name") == msgName)
        {
            docElem.removeChild(msgNode);
            ret = true;
        }

        msgNode = msgNode.nextSibling();
    }

    return ret;
}

bool girFile::removeDisplay(QString msgName, QString displayName)
{
    bool ret = false;
    QDomElement docElem = this->dom->documentElement();
    QDomNode msgNode = docElem.firstChild();
    while(!msgNode.isNull())
    {
        if(msgNode.toElement().nodeName() == "message" && msgNode.toElement().attribute("name") == msgName)
        {
            QDomNode dispNode = msgNode.firstChild();
            while(!dispNode.isNull())
            {
                if(dispNode.toElement().nodeName() == "display" && dispNode.toElement().attribute("name") == displayName)
                {
                    msgNode.removeChild(dispNode);
                    ret = true;
                }
                dispNode = dispNode.nextSibling();
            }

        }

        msgNode = msgNode.nextSibling();
    }

    return ret;
}

bool girFile::removeSeq(QString msgName, QString dispName, QString seqName)
{
    bool ret = false;
    QDomElement docElem = this->dom->documentElement();
    QDomNode msgNode = docElem.firstChild();
    while(!msgNode.isNull())
    {
        if(msgNode.toElement().nodeName() == "message" && msgNode.toElement().attribute("name") == msgName)
        {
            QDomNode dispNode = msgNode.firstChild();
            while(!dispNode.isNull())
            {
                if(dispNode.toElement().nodeName() == "display" && dispNode.toElement().attribute("name") == dispName)
                {
                    QDomNode seqNode = dispNode.firstChild();
                    while(!seqNode.isNull())
                    {
                        if(seqNode.toElement().nodeName() == "seqElem" && seqNode.toElement().attribute("name") == seqName)
                        {
                            dispNode.removeChild(seqNode);
                            ret = true;
                        }
                        seqNode = seqNode.nextSibling();
                    }


                }
                dispNode = dispNode.nextSibling();
            }

        }

        msgNode = msgNode.nextSibling();
    }

    return ret;
}
bool girFile::setLine(QString msgName, QString dispName, QString seqEltName, QString lineName, QString data)
{
    return setLineParam(msgName, dispName, seqEltName, lineName, "data", data);
}
bool girFile::setLineParam(QString msgName, QString dispName, QString seqEltName, QString lineName, QString param, QString value)
{
    bool ret = false;
    bool lineExists = false;
    QDomElement docElem = this->dom->documentElement();
    QDomNode msgNode = docElem.firstChild();
    while(!msgNode.isNull())
    {
        if(msgNode.toElement().nodeName() == "message" && msgNode.toElement().attribute("name") == msgName)
        {
            QDomNode disNode = msgNode.firstChild();
            while(!disNode.isNull())
            {
                if(disNode.toElement().nodeName() == "display" && disNode.toElement().attribute("name") == dispName)
                {
                    QDomNode seqNode = disNode.firstChild();
                    while(!seqNode.isNull())
                    {
                        if(seqNode.toElement().nodeName() == "seqElem" && seqNode.toElement().attribute("name") == seqEltName)
                        {
                            QDomNode liNode = seqNode.firstChild();
                            while(!liNode.isNull())
                            {
                                if(liNode.toElement().nodeName() == "line" && liNode.toElement().attribute("name") == lineName)
                                {
                                    lineExists = true;
                                    liNode.toElement().setAttribute(param,value);
                                    ret = true;
                                }
                                liNode = liNode.nextSibling();

                            }
                            if(!lineExists)
                            {
                                QDomElement lineElt = this->dom->createElement("line");
                                lineElt.setAttribute("name",lineName);
                                lineElt.setAttribute(param,value);
                                seqNode.appendChild(lineElt);
                                ret = true;
                            }

                        }
                        seqNode = seqNode.nextSibling();

                    }
                }
                disNode = disNode.nextSibling();
            }
        }

        msgNode = msgNode.nextSibling();
    }

    return ret;
}

bool girFile::setLineFont(QString msgName, QString dispName, QString seqEltName, QString lineName, QString fontName)
{
    return setLineParam(msgName, dispName, seqEltName, lineName, "font", fontName);
}
void girFile::end()
{
    this->girF->close();
}
void girFile::saveFileAs(QString location)
{
    this->girF = new QFile(location);
    this->girF->open(QFile::ReadWrite);
    this->textStream.setDevice(this->girF);

    this->girF->resize(0);
    this->dom->save(this->textStream,2);
}

bool girFile::renameMessage(QString oldName, QString newname)
{
    bool ret = false;
    QDomElement docElem = this->dom->documentElement();
    QDomNode msgNode = docElem.firstChild();
    while(!msgNode.isNull())
    {
        if(msgNode.toElement().nodeName() == "message" && msgNode.toElement().attribute("name") == oldName)
        {
            msgNode.toElement().setAttribute("name",newname);
            ret = true;
        }

        msgNode = msgNode.nextSibling();
    }

    return ret;
}
bool girFile::renameDisplay(QString msgName, QString oldName, QString newname)
{
    bool ret = false;
    QDomElement docElem = this->dom->documentElement();
    QDomNode msgNode = docElem.firstChild();
    while(!msgNode.isNull())
    {
        if(msgNode.toElement().nodeName() == "message" && msgNode.toElement().attribute("name") == msgName)
        {
            QDomNode dispNode = msgNode.firstChild();
            while(!dispNode.isNull())
            {
                if(dispNode.toElement().nodeName() == "display" && dispNode.toElement().attribute("name") == oldName)
                {
                    dispNode.toElement().setAttribute("name",newname);
                    ret = true;
                }
                dispNode = dispNode.nextSibling();
            }

        }

        msgNode = msgNode.nextSibling();
    }

    return ret;
}
bool girFile::renameSeq(QString msgName, QString dispName, QString oldName, QString newname)
{
    bool ret = false;
    QDomElement docElem = this->dom->documentElement();
    QDomNode msgNode = docElem.firstChild();
    while(!msgNode.isNull())
    {
        if(msgNode.toElement().nodeName() == "message" && msgNode.toElement().attribute("name") == msgName)
        {
            QDomNode dispNode = msgNode.firstChild();
            while(!dispNode.isNull())
            {
                if(dispNode.toElement().nodeName() == "display" && dispNode.toElement().attribute("name") == dispName)
                {
                    QDomNode seqNode = dispNode.firstChild();
                    while(!seqNode.isNull())
                    {
                        if(seqNode.toElement().nodeName() == "seqElem" && seqNode.toElement().attribute("name") == oldName)
                        {
                            seqNode.toElement().setAttribute("name",newname);
                            ret = true;
                        }
                        seqNode = seqNode.nextSibling();
                    }


                }
                dispNode = dispNode.nextSibling();
            }

        }

        msgNode = msgNode.nextSibling();
    }

    return ret;
}
bool girFile::setDisplayProp(QString msgName, QString displayName, QString param, QString value)
{
    bool ret = false;
    QDomElement docElem = this->dom->documentElement();
    QDomNode msgNode = docElem.firstChild();
    while(!msgNode.isNull())
    {
        if(msgNode.toElement().nodeName() == "message" && msgNode.toElement().attribute("name") == msgName)
        {
            QDomNode dispNode = msgNode.firstChild();
            while(!dispNode.isNull())
            {
                if(dispNode.toElement().nodeName() == "display" && dispNode.toElement().attribute("name") == displayName)
                {
                    dispNode.toElement().setAttribute(param,value);
                    ret = true;
                }
                dispNode = dispNode.nextSibling();
            }

        }

        msgNode = msgNode.nextSibling();
    }

    return ret;
}

QString girFile::readDisplayProp(QString msgName, QString displayName, QString param)
{
    QString ret;
    QDomElement docElem = this->dom->documentElement();
    QDomNode msgNode = docElem.firstChild();
    while(!msgNode.isNull())
    {
        if(msgNode.toElement().nodeName() == "message" && msgNode.toElement().attribute("name") == msgName)
        {
            QDomNode dispNode = msgNode.firstChild();
            while(!dispNode.isNull())
            {
                if(dispNode.toElement().nodeName() == "display" && dispNode.toElement().attribute("name") == displayName)
                {
                    ret = dispNode.toElement().attribute(param);

                }
                dispNode = dispNode.nextSibling();
            }

        }

        msgNode = msgNode.nextSibling();
    }

    return ret;
}

QString girFile::readSeqProp(QString msgName, QString dispName, QString seqName, QString param)
{
    QString ret;
    QDomElement docElem = this->dom->documentElement();
    QDomNode msgNode = docElem.firstChild();
    while(!msgNode.isNull())
    {
        if(msgNode.toElement().nodeName() == "message" && msgNode.toElement().attribute("name") == msgName)
        {
            QDomNode disNode = msgNode.firstChild();
            while(!disNode.isNull())
            {
                if(disNode.toElement().nodeName() == "display" && disNode.toElement().attribute("name") == dispName)
                {
                    QDomNode seqNode = disNode.firstChild();
                    while(!seqNode.isNull())
                    {
                        if(seqNode.toElement().nodeName() == "seqElem" && seqNode.toElement().attribute("name") == seqName)
                        {
                            ret = seqNode.toElement().attribute(param);
                        }
                        seqNode = seqNode.nextSibling();
                    }

                }
                disNode = disNode.nextSibling();
            }
        }

        msgNode = msgNode.nextSibling();
    }

    return ret;
}

bool girFile::setSeqProp(QString msgName, QString dispName, QString seqName, QString param, QString value)
{
    bool ret = false;
    QDomElement docElem = this->dom->documentElement();
    QDomNode msgNode = docElem.firstChild();
    while(!msgNode.isNull())
    {
        if(msgNode.toElement().nodeName() == "message" && msgNode.toElement().attribute("name") == msgName)
        {
            QDomNode disNode = msgNode.firstChild();
            while(!disNode.isNull())
            {
                if(disNode.toElement().nodeName() == "display" && disNode.toElement().attribute("name") == dispName)
                {
                    QDomNode seqNode = disNode.firstChild();
                    while(!seqNode.isNull())
                    {
                        if(seqNode.toElement().nodeName() == "seqElem" && seqNode.toElement().attribute("name") == seqName)
                        {

                            seqNode.toElement().setAttribute(param,value);
                            ret = true;
                        }
                        seqNode = seqNode.nextSibling();
                    }

                }
                disNode = disNode.nextSibling();
            }
        }

        msgNode = msgNode.nextSibling();
    }

    return ret;
}
