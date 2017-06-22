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

#ifndef FONTEDITOR_H
#define FONTEDITOR_H

#include <QDialog>
#include <QLabel>
#include <QPainter>
#include <QImage>
#include <QIcon>
#include <QGroupBox>
#include <QMouseEvent>
#include <QMessageBox>
#include <QInputDialog>
#include <QStringList>
#include <QFile>
#include <QImage>
#include <QFileDialog>
#include "clicklabel.h"
#include "fontfile.h"
#include "fontmgmt.h"
#include "settingsfile.h"
#include "common.h"
#define EDITMODE_ADD 0
#define EDITMODE_REM 1

namespace Ui {
    class fontEditor;
}

class fontEditor : public QDialog {
    Q_OBJECT
public:
    fontEditor(QWidget *parent = 0);
    ~fontEditor();
    void generateMap(int height, int width, QLabel* dest);
    void drawElement(int x, int y, QImage element, QLabel *dest);
    clickLabel *charEd;
    QString colored;
    QFile *fileHandler;
    fontFile *fontF;
    QString currentCharacter;
    void refreshCharList();
    void regenerateFontList();
    settingsFile *pars;
    bool isEditing;
    int editMode;
    QString eltColor;
    QImage elementAdd;
    QImage elementRem;

public slots:
    void picClick(QMouseEvent *event, bool changeMode);
    void setPressed(QMouseEvent *event);
    void setReleased(QMouseEvent *event);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::fontEditor *m_ui;


private slots:
    //void on_importFromImage_clicked(); Will be implemented later
    void on_correctErrors_clicked();
    void on_fillStd_clicked();
    void on_rename_clicked();
    void on_save_clicked();
    void on_load_clicked();
    void on_removeChar_clicked();
    void on_charList_currentTextChanged(QString currentText);
    void on_newChar_clicked();
    void on_remove_clicked();
    void on_fWidth_valueChanged(int );
    void on_fHeight_valueChanged(int );
    void on_fontSelect_currentIndexChanged(int index);
    void on_fWidth_editingFinished();
    void on_fHeight_editingFinished();
    void on_close_clicked();

};

#endif // FONTEDITOR_H
