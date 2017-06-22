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

#include "displaysettings.h"
#include "ui_displaysettings.h"

DisplaySettings::DisplaySettings(QWidget *parent) :
        QDialog(parent),
        m_ui(new Ui::DisplaySettings)
{
    m_ui->setupUi(this);
    this->setF = new settingsFile("./settings.xml");
    this->m_ui->defHeight->setValue((this->setF->readParam("defHeight").isNull())?20:this->setF->readParam("defHeight").toInt());
    this->m_ui->defWidth->setValue((this->setF->readParam("defWidth").isNull())?20:this->setF->readParam("defWidth").toInt());
    this->m_ui->defLineNWidth->setValue((this->setF->readParam("defLineWidth").isNull())?20:this->setF->readParam("defLineWidth").toInt());
}

DisplaySettings::~DisplaySettings()
{
    delete m_ui;
}

void DisplaySettings::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void DisplaySettings::on_buttonBox_accepted()
{
    QString defHeight;
    QString defWidth;
    QString defLineNWidth;

    this->setF->saveParam("defHeight",defHeight.setNum(this->m_ui->defHeight->value()));


    this->setF->saveParam("defWidth",defWidth.setNum(this->m_ui->defWidth->value()));
    this->setF->saveParam("defLineWidth",defLineNWidth.setNum(this->m_ui->defLineNWidth->value()));

}
