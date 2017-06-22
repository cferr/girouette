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

#ifndef FONTMGMT_H
#define FONTMGMT_H

#include <QDialog>
#include <QMessageBox>
#include <QListWidgetItem>
#include "fontfile.h"

namespace Ui {
    class fontMgmt;
}

class fontMgmt : public QDialog {
    Q_OBJECT
public:
    fontMgmt(QWidget *parent = 0, QString source = "./.font.xml", QString dest = "./font.xml", QString mode = "import");
    ~fontMgmt();
    fontFile *sourceFile;
    fontFile *destFile;
    QString destinationFile;
    QString mode;

protected:
    void changeEvent(QEvent *e);

private:
    Ui::fontMgmt *m_ui;

private slots:
    void on_buttonBox_accepted();
};

#endif // FONTMGMT_H
