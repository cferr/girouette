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

#include "generalsettings.h"
#include "ui_generalsettings.h"
#include <QColorDialog>

GeneralSettings::GeneralSettings(QWidget *parent) :
        QDialog(parent),
        m_ui(new Ui::GeneralSettings)
{
    m_ui->setupUi(this);
    this->settings = new settingsFile("./settings.xml");
    this->m_ui->fontLocation->setText(this->settings->readParam("fontfile").isNull()?"./font.xml":this->settings->readParam("fontfile"));
    this->eltColor = (this->settings->readParam("eltColor").isNull()?"#FFFF00":this->settings->readParam("eltColor"));
    this->valign = (this->settings->readParam("vAlign").isNull()?"center":this->settings->readParam("vAlign"));
    this->eltColor_led_std = (this->settings->readParam("eltColor_led_std").isNull()?"#FF8C00":this->settings->readParam("eltColor_led_std"));
    this->eltColor_led_bg = (this->settings->readParam("eltColor_led_bg").isNull()?"#000000":this->settings->readParam("eltColor_led_bg"));
    this->eltColor_led_line = (this->settings->readParam("eltColor_led_line").isNull()?"#FF8C00":this->settings->readParam("eltColor_led_line"));
    this->eltType = (this->settings->readParam("eltType").isNull()?"matrix":this->settings->readParam("eltType"));
    if(eltType == "matrix")
    {
        this->m_ui->matrix_radio->setChecked(true);
    }
    else
    {
        this->m_ui->leds_radio->setChecked(true);
    }
    if(valign == "top")
    {
        this->m_ui->valign_top->setChecked(true);
    }
    else if(valign == "center")
    {
        this->m_ui->valign_center->setChecked(true);
    }
    else if(valign == "bottom")
    {
        this->m_ui->valign_bottom->setChecked(true);
    }
    QSize resultSize(50,16);
    this->eltSampleImg = QImage(resultSize, QImage::Format_RGB32);

    this->destLabel = this->m_ui->color_sample;
    this->color = QColor(this->eltColor);
    this->changeElementColor(this->color);

    this->destLabel = this->m_ui->color_sample_led_bg;
    this->color = QColor(this->eltColor_led_bg);
    this->changeElementColor(this->color);

    this->destLabel = this->m_ui->color_sample_led_std;
    this->color = QColor(this->eltColor_led_std);
    this->changeElementColor(this->color);

    this->destLabel = this->m_ui->color_sample_led_line;
    this->color = QColor(this->eltColor_led_line);
    this->changeElementColor(this->color);

}

GeneralSettings::~GeneralSettings()
{
    delete m_ui;
}

void GeneralSettings::changeEvent(QEvent *e)
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

void GeneralSettings::on_buttonBox_accepted()
{
    this->settings->saveParam("fontfile",this->m_ui->fontLocation->text());
    this->settings->saveParam("eltColor",this->eltColor);
    this->settings->saveParam("eltColor_led_std",this->eltColor_led_std);
    this->settings->saveParam("eltColor_led_line",this->eltColor_led_line);
    this->settings->saveParam("eltColor_led_bg",this->eltColor_led_bg);
    this->settings->saveParam("eltType",this->eltType);
    this->settings->saveParam("vAlign",this->valign);

}

void GeneralSettings::on_chooseColor_clicked()
{

    this->eltColor = this->chooseColor(this->m_ui->color_sample,this->eltColor);
}

QString GeneralSettings::chooseColor(QLabel *dest, QString previous)
{
    QColorDialog *dialog = new QColorDialog(this);
    QColor color(previous);
    dialog->setCurrentColor(color);
    this->destLabel = dest;
    dialog->connect(dialog,SIGNAL(colorSelected(QColor)),this,SLOT(changeElementColor(QColor)));
    dialog->exec();
    return dialog->currentColor().name();
}

void GeneralSettings::changeElementColor(QColor color)
{
    QPainter painter(&eltSampleImg);
    QString cname = color.name();
    this->color.setNamedColor(this->eltColor);
    painter.fillRect(eltSampleImg.rect(),color);
    painter.end();
    QPixmap pix;
    pix = pix.fromImage(this->eltSampleImg);
    this->destLabel->setPixmap(pix);
}

void GeneralSettings::on_matrix_radio_toggled(bool checked)
{
    //Only do if checked. If unchecked, then it is the other choice, nothing else possible!
    if(checked)
    {
        this->eltType = "matrix";
    }
}

void GeneralSettings::on_leds_radio_toggled(bool checked)
{
    //Only do if checked.
    if(checked)
    {
        this->eltType ="leds";
    }
}

void GeneralSettings::on_chooseColor_leds_std_clicked()
{

    this->eltColor_led_std = this->chooseColor(this->m_ui->color_sample_led_std,this->eltColor_led_std);
}

void GeneralSettings::on_chooseColor_leds_bg_clicked()
{

    this->eltColor_led_bg = this->chooseColor(this->m_ui->color_sample_led_bg,this->eltColor_led_bg);
}

void GeneralSettings::on_chooseColor_leds_line_clicked()
{

    this->eltColor_led_line = this->chooseColor(this->m_ui->color_sample_led_line,this->eltColor_led_line);
}

void GeneralSettings::on_valign_top_toggled(bool checked)
{
    if(checked)
    {
        this->valign="top";
    }
}

void GeneralSettings::on_valign_center_toggled(bool checked)
{
    if(checked)
    {
        this->valign="center";
    }
}

void GeneralSettings::on_valign_bottom_toggled(bool checked)
{
    if(checked)
    {
        this->valign="bottom";
    }
}
