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

#include <QApplication>
#include <QFile>
#include <QMessageBox>
#include "signwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QString locale = QLocale::system().name().section('_', 0, 0);
    qDebug("Language: %s",locale.toStdString().c_str());
    QTranslator trans;
    trans.load(QString(":/Elements/girouette_") + locale);

    a.installTranslator(&trans);

    QFile f("./IORWTest");
    bool exists = f.exists();
    int resultCode;
    if(!f.open(QFile::ReadWrite))
    {
        QMessageBox msg;
        msg.setText(a.tr("You are not allowed to read/write into this directory. Please run this program somewhere you are allowed to write into (i.e. an USB flash drive or your personal folder)."));
        msg.setWindowTitle(a.tr("Error"));
        msg.setStandardButtons(QMessageBox::Close);
        msg.exec();
        resultCode = 1;
    }
    else
    {
        f.close();
        if(!exists){f.remove();}
        signWindow w;
        w.show();
        resultCode = a.exec();
    }

    QFile::remove("./gir.tmp.xml");
    return resultCode;
}
