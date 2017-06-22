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

#ifndef GENERALSETTINGS_H
#define GENERALSETTINGS_H

#include <QDialog>
#include <QImage>
#include <QPainter>
#include <QColor>
#include <QLabel>
#include "settingsfile.h"

namespace Ui {
    class GeneralSettings;
}

class GeneralSettings : public QDialog {
    Q_OBJECT
public:
    GeneralSettings(QWidget *parent = 0);
    ~GeneralSettings();
    settingsFile *settings;
    QString eltColor;
    QString eltColor_led_std;
    QString eltColor_led_bg;
    QString eltColor_led_line;
    QImage eltSampleImg;
    QColor color;
    QString chooseColor(QLabel *dest, QString previous);
    QLabel *destLabel;
    QString eltType;
    QString valign;

public slots:
    void changeElementColor(QColor color);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::GeneralSettings *m_ui;

private slots:
    void on_valign_bottom_toggled(bool checked);
    void on_valign_center_toggled(bool checked);
    void on_valign_top_toggled(bool checked);
    void on_chooseColor_leds_line_clicked();
    void on_chooseColor_leds_bg_clicked();
    void on_chooseColor_leds_std_clicked();
    void on_leds_radio_toggled(bool checked);
    void on_matrix_radio_toggled(bool checked);
    void on_chooseColor_clicked();
    void on_buttonBox_accepted();
};

#endif // GENERALSETTINGS_H
