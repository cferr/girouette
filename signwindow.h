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

#ifndef SIGNWINDOW_H
#define SIGNWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QLabel>
#include <QColor>
#include <QColorDialog>
#include <QRect>
#include "fonteditor.h"
#include "fontfile.h"
#include "girfile.h"
#include "displaysettings.h"
#include "generalsettings.h"
#include "customsettings.h"

namespace Ui
{
    class signWindow;
}

class signWindow : public QMainWindow
{
    Q_OBJECT

public:
    signWindow(QWidget *parent = 0);
    ~signWindow();
    QImage generateMapEx(int height, int width);
    QImage drawElementEx(int x,int y, QImage element, QImage source);
    fontFile *fontF;
    void generateFontList();
    QImage regenerateTextEx(QString text, QString align, int x, int y, QImage work, bool isLineNumber);
    QStringList characters;
    QStringList datas;
    void loadFont(QString fontName);
    void drawImage(QImage img, QLabel *dest);
    settingsFile *pars;
    void refreshDisplay();
    QString vAlign;
    QString eltColor;
    QString eltType;
    QString eltColor_led_std;
    QString eltColor_led_bg;
    QString eltColor_led_line;
    girFile *girF;
    QString currentFile;
    void refreshGirList();
    void refreshSeqList(QString msgName, QString dispName);
    void refreshDispList(QString msgName);
    void refreshLines();
    int seqIndex;
    int girIndex;
    int dispIndex;
    QString girName;
    QString dispName;
    QString seqName;
    void toggleElements(bool enabled);
    void setDefaults(QString girName, QString dispName);
    int defaultX;
    int defaultY;
    int defaultLineW;
    customSettings *customs;
    void readCustomSettings(QString msgName, QString dispName, QString seqName);
    void drawColorSample(QString color, QPushButton *button);
    QString chooseColor(QString previous);
    void updateCustomFields();
    void alignText(QString line, QString align);

public slots:
    void seqRename(QWidget*,QAbstractItemDelegate::EndEditHint);
    void dispRename(QWidget*,QAbstractItemDelegate::EndEditHint);
    void girRename(QWidget*,QAbstractItemDelegate::EndEditHint);
private:
    Ui::signWindow *ui;

private slots:
    void on_align_justify_line_3_clicked();
    void on_align_center_line_3_clicked();
    void on_align_left_line_3_clicked();
    void on_align_justify_line_2_clicked();
    void on_align_right_line_2_clicked();
    void on_align_center_line_2_clicked();
    void on_align_left_line_2_clicked();
    void on_align_justify_line_1_clicked();
    void on_align_right_line_1_clicked();
    void on_align_center_line_1_clicked();
    void on_align_left_line_1_clicked();
    void on_align_justify_linenum_clicked();
    void on_align_right_linenum_clicked();
    void on_align_center_linenum_clicked();
    void on_align_left_linenum_clicked();
    void on_align_right_line_3_clicked();
    void on_valign_bottom_toggled(bool checked);
    void on_valign_center_toggled(bool checked);
    void on_valign_top_toggled(bool checked);
    void on_edit_seq_clicked();
    void on_edit_disp_clicked();
    void on_edit_msg_clicked();
    void on_eltType_leds_toggled(bool checked);
    void on_eltType_matrix_toggled(bool checked);
    void on_linewidth_valueChanged(int );
    void on_width_valueChanged(int );
    void on_height_valueChanged(int );
    void on_eltColor_bg_btn_clicked();
    void on_eltColor_leds_btn_clicked();
    void on_eltColor_leds_std_btn_clicked();
    void on_eltColor_btn_clicked();
    void on_eltType_leds_clicked();
    void on_eltType_matrix_clicked();
    void on_actionSave_as_triggered();
    void on_actionNew_triggered();
    void on_actionSave_triggered();
    void on_seqList_currentTextChanged(QString currentText);
    void on_actionExport_current_view_triggered();
    void on_delSeq_clicked();
    void on_addSeq_clicked();
    void on_deleteDisp_clicked();
    void on_newDisp_clicked();
    void on_delGir_clicked();
    void on_dispList_currentTextChanged(QString currentText);
    void on_girList_currentTextChanged(QString currentText);
    void on_newGir_clicked();
    void on_actionOpen_triggered();
    void on_actionGeneral_Setup_triggered();
    void on_fontSelect_l3_activated(QString );
    void on_actionAbout_triggered();
    void on_fontSelect_l3_currentIndexChanged(QString );
    void on_fontSelect_l2_currentIndexChanged(QString );
    void on_fontSelect_l1_currentIndexChanged(QString );
    void on_actionDisplay_Setup_triggered();
    void on_text3_editingFinished();
    void on_text2_editingFinished();
    void on_text1_editingFinished();
    void on_linenum_editingFinished();
    void on_fontSelect_currentIndexChanged(QString );
    void on_actionFont_editor_triggered();
    void on_actionQuit_triggered();
};

#endif // SIGNWINDOW_H
