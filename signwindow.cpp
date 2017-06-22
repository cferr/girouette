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

#include "signwindow.h"
#include "ui_signwindow.h"
#include "fontfile.h"
#include <QMessageBox>
#include <QImageWriter>

signWindow::signWindow(QWidget *parent)
        : QMainWindow(parent), ui(new Ui::signWindow)
{
    ui->setupUi(this);
    this->pars = new settingsFile("./settings.xml");
    // This will connect  the file to stdout
    this->fontF = new fontFile(this->pars->readParam("fontfile").isNull()?"./font.xml":this->pars->readParam("fontfile"));

    this->customs = new customSettings();

    this->defaultY = (pars->readParam("defWidth").isNull())?200:pars->readParam("defWidth").toInt();
    this->defaultX = (pars->readParam("defHeight").isNull())?20:pars->readParam("defHeight").toInt();
    this->defaultLineW = (pars->readParam("defLineWidth").isNull())?20:pars->readParam("defLineWidth").toInt();
    this->vAlign = (pars->readParam("vAlign").isNull()?"center":pars->readParam("vAlign"));
    this->eltType =  (pars->readParam("eltType").isNull()?"matrix":pars->readParam("eltType"));
    this->eltColor = (pars->readParam("eltColor").isNull()?"#FFFF00":pars->readParam("eltColor"));
    this->eltColor_led_std = (pars->readParam("eltColor_led_std").isNull()?"#FF8C00":pars->readParam("eltColor_led_std"));
    this->eltColor_led_bg = (pars->readParam("eltColor_led_bg").isNull()?"#000000":pars->readParam("eltColor_led_bg"));
    this->eltColor_led_line = (pars->readParam("eltColor_led_line").isNull()?"#FF8C00":pars->readParam("eltColor_led_line"));
    this->customs->vAlign = this->vAlign;
    this->customs->width = this->defaultY;
    this->customs->height = this->defaultX;
    this->customs->lineWidth = this->defaultLineW;
    this->customs->eltType = this->eltType;
    this->customs->eltColor = this->eltColor;
    this->customs->eltColor_led_std = this->eltColor_led_std;
    this->customs->eltColor_led_bg = this->eltColor_led_bg;
    this->customs->eltColor_led_line = this->eltColor_led_line;

    QImage img = generateMapEx(defaultX,defaultY);
    this->drawImage(img,this->ui->sign);
    this->generateFontList();

    this->girF = new girFile("./gir.tmp.xml");
    //Temporary
    this->refreshGirList();
    this->girIndex = this->ui->girList->currentRow();
    this->dispIndex = this->ui->dispList->currentRow();
    this->seqIndex = this->ui->seqList->currentRow();
    if(this->girIndex != -1)
    {
        this->girName = this->ui->girList->currentItem()->text();

    }
    else
    {
        this->ui->dispList->setEnabled(false);
        this->ui->newDisp->setEnabled(false);
        this->ui->deleteDisp->setEnabled(false);
        this->ui->edit_disp->setEnabled(false);
    }
    if(this->seqIndex != -1)
    {
        this->seqName = this->ui->seqList->currentItem()->text();
    }
    else
    {
        toggleElements(false);
    }
    if(this->dispIndex != -1)
    {
        this->dispName = this->ui->dispList->currentItem()->text();
    }
    else
    {
        this->ui->seqList->setEnabled(false);
        this->ui->addSeq->setEnabled(false);
        this->ui->delSeq->setEnabled(false);
        this->ui->edit_seq->setEnabled(false);
    }
    this->ui->girList->connect(this->ui->girList->itemDelegate(),SIGNAL(closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint)),this, SLOT(girRename(QWidget*,QAbstractItemDelegate::EndEditHint)));

    this->ui->dispList->connect(this->ui->dispList->itemDelegate(),SIGNAL(closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint)),this, SLOT(dispRename(QWidget*,QAbstractItemDelegate::EndEditHint)));

    this->ui->seqList->connect(this->ui->seqList->itemDelegate(),SIGNAL(closeEditor(QWidget*,QAbstractItemDelegate::EndEditHint)),this, SLOT(seqRename(QWidget*,QAbstractItemDelegate::EndEditHint)));

}

signWindow::~signWindow()
{
    delete ui;
}

QImage signWindow::generateMapEx(int height, int width)
{
    QSize resultSize(4 * width, 4 * height);
    QImage element(":/Elements/fond_pastille.png");
    QImage image(resultSize, QImage::Format_RGB32);

    QPainter painter(&image);
    QRectF rect = element.rect();
    painter.fillRect(image.rect(), Qt::black);

    if(this->customs->eltType == "leds")
    {
        int lineWidth = customs->lineWidth;
        QRect linerect(0,0,4 * lineWidth, 4 * height);
        QRectF linerectf(linerect);
        QColor linecolor(this->customs->eltColor_led_bg);
        painter.fillRect(linerectf,linecolor);
    }
    int x = 0;
    int y = 0;

    while(y < height)
    {
        while(x < width)
        {
            rect.setCoords(4 * x, 4 * y, -4, -4);
            painter.drawImage(rect, element, image.rect());


            x++;
        }
        x = 0;
        y++;
    }

    return image;
}

QImage signWindow::drawElementEx(int x, int y, QImage element, QImage source)
{
    QPainter painter(&source);
    painter.drawImage(4 * x ,4 * y,element);
    painter.end();
    return source;
}

void signWindow::on_actionQuit_triggered()
{
    exit(0);
}

void signWindow::on_actionFont_editor_triggered()
{
    fontEditor *fedit = new fontEditor();
    fedit->exec();
    this->fontF->end();
    this->fontF = new fontFile(this->pars->readParam("fontfile").isNull()?"./font.xml":this->pars->readParam("fontfile"));
    this->generateFontList();

}

void signWindow::generateFontList()
{
    QStringList fontList = this->fontF->listFonts();
    this->ui->fontSelect->clear();
    this->ui->fontSelect->insertItems(0,fontList);

    this->ui->fontSelect_l1->clear();
    this->ui->fontSelect_l1->insertItems(0,fontList);

    this->ui->fontSelect_l2->clear();
    this->ui->fontSelect_l2->insertItems(0,fontList);

    this->ui->fontSelect_l3->clear();
    this->ui->fontSelect_l3->insertItems(0,fontList);
    if(fontList.count() == 0)
    {
        toggleElements(false);
        QMessageBox::warning(this,"Warning","No font is currently available. Please go to the Font Editor (Font > Font editor) to create or import fonts. You won't be able to edit signs until this is done.");
    }
    else if(this->ui->seqList->currentRow() != -1)
    {
        toggleElements(true);
    }
}


QImage signWindow::regenerateTextEx(QString text, QString align, int x, int y, QImage work, bool isLineNumber)
{
    QString font = this->ui->fontSelect->currentText();
    int fontX = this->fontF->readFontX(this->fontF->currentFont);

    int length = text.length();
    int i = 0;

    int currentPos = x;

    int h = y;
    QSize resultSize(4,4);
    QImage image(":/Elements/pastille.png");
    QImage element(resultSize, QImage::Format_RGB32);


    QPainter painter(&element);
    QColor color;
    if(this->customs->eltType == "leds")
    {
        if(isLineNumber)
        {
            color.setNamedColor(this->customs->eltColor_led_line);
        }
        else
        {
            color.setNamedColor(this->customs->eltColor_led_std);
        }
    }
    else
    {
        color.setNamedColor(this->customs->eltColor);
    }

    painter.fillRect(element.rect(),color);

    painter.drawImage(0,0,image);
    painter.end();

    if(align == "center")
    {
        if(!isLineNumber)
        {
            int totalLength = 0;
            //First we calculate...
            while(i<length)
            {
                QString character = text.mid(i,1);
                if(character == " ")
                {
                    totalLength = totalLength + 3;
                }
                else
                {
                    totalLength = totalLength + fontX;
                }

                i++;
            }
            int usableLength = this->customs->width - this->customs->lineWidth;
            int start = (usableLength - totalLength) / 2;
            start = start + this->customs->lineWidth;
            currentPos = start;
            i = 0;
        }
        else
        {
            int totalLength = 0;
            //First we calculate...
            while(i<length)
            {
                QString character = text.mid(i,1);
                if(character == " ")
                {
                    totalLength = totalLength + 3;
                }
                else
                {
                    totalLength = totalLength + fontX;
                }

                i++;
            }
            int usableLength = this->customs->lineWidth;
            int start = (usableLength - totalLength) / 2;
            currentPos = start;
            i = 0;
        }
    }
    else if(align == "left")
    {
        i = 0;

    }
    else if(align == "right")
    {
        if(!isLineNumber)
        {
            int totalLength = 0;
            //First we calculate...
            while(i<length)
            {
                QString character = text.mid(i,1);
                if(character == " ")
                {
                    totalLength = totalLength + 3;
                }
                else
                {
                    totalLength = totalLength + fontX;
                }

                i++;
            }
            int usableLength = this->customs->width - this->customs->lineWidth;
            int start = (usableLength - totalLength);
            start = start + this->customs->lineWidth;
            currentPos = start;
            i = 0;
        }
        else
        {
            int totalLength = 0;
            //First we calculate...
            while(i<length)
            {
                QString character = text.mid(i,1);
                if(character == " ")
                {
                    totalLength = totalLength + 3;
                }
                else
                {
                    totalLength = totalLength + fontX;
                }

                i++;
            }
            int usableLength = this->customs->lineWidth;
            int start = (usableLength - totalLength);
            currentPos = start;
            i = 0;
        }
    }
    else if(align == "justify")
    {
        //Uniplemented yet!
    }

    //Then we draw
    while(i<length)
    {
        QString character = text.mid(i,1);
        int pos = this->characters.indexOf(character,0);

        if(pos != -1)
        {
            QString graphics = this->datas[pos];
            QStringList splitted = graphics.split(";",QString::SkipEmptyParts);
            foreach(QString coords, splitted)
            {
                int x= coords.split(",")[0].toInt() + currentPos;
                int y= coords.split(",")[1].toInt() + h;
                work = drawElementEx(x,y,element,work);
            }
        }
        if(character == " ")
        {
            currentPos = currentPos + 3;
        }
        else
        {
            currentPos = currentPos + fontX;
        }

        i++;
    }
    return work;
}

void signWindow::on_fontSelect_currentIndexChanged(QString font)
{
    if(this->ui->girList->currentRow() != -1 && this->ui->dispList->currentRow() != -1 && this->ui->seqList->currentRow() != -1)
    {
        QString msg = this->ui->girList->currentItem()->text();
        QString dis = this->ui->dispList->currentItem()->text();
        QString seq = this->ui->seqList->currentItem()->text();
        this->girF->setLineFont(msg,dis,seq,"linenum",font);
    }


    this->refreshDisplay();

}

void signWindow::on_fontSelect_l1_currentIndexChanged(QString font)
{

    if(this->ui->girList->currentRow() != -1 && this->ui->dispList->currentRow() != -1 && this->ui->seqList->currentRow() != -1)
    {
        QString msg = this->ui->girList->currentItem()->text();
        QString dis = this->ui->dispList->currentItem()->text();
        QString seq = this->ui->seqList->currentItem()->text();
        this->girF->setLineFont(msg,dis,seq,"line_1",font);
    }
    this->refreshDisplay();
}

void signWindow::on_fontSelect_l2_currentIndexChanged(QString font)
{
    if(this->ui->girList->currentRow() != -1 && this->ui->dispList->currentRow() != -1 && this->ui->seqList->currentRow() != -1)
    {
        QString msg = this->ui->girList->currentItem()->text();
        QString dis = this->ui->dispList->currentItem()->text();
        QString seq = this->ui->seqList->currentItem()->text();
        this->girF->setLineFont(msg,dis,seq,"line_2",font);
    }
    this->refreshDisplay();
}

void signWindow::on_fontSelect_l3_currentIndexChanged(QString font)
{
    if(this->ui->girList->currentRow() != -1 && this->ui->dispList->currentRow() != -1 && this->ui->seqList->currentRow() != -1)
    {
        QString msg = this->ui->girList->currentItem()->text();
        QString dis = this->ui->dispList->currentItem()->text();
        QString seq = this->ui->seqList->currentItem()->text();
        this->girF->setLineFont(msg,dis,seq,"line_3",font);
    }
    this->refreshDisplay();
}

void signWindow::on_linenum_editingFinished()
{
    if(this->ui->girList->currentRow() != -1 && this->ui->dispList->currentRow() != -1 && this->ui->seqList->currentRow() != -1)
    {
        QString msg = this->ui->girList->currentItem()->text();
        QString dis = this->ui->dispList->currentItem()->text();
        QString seq = this->ui->seqList->currentItem()->text();
        this->girF->setLine(msg,dis,seq,"linenum",this->ui->linenum->text());
        this->girF->setLineParam(msg,dis,seq,"linenum","font",this->ui->fontSelect->currentText());
    }
    this->refreshDisplay();
}

void signWindow::on_text1_editingFinished()
{
    if(this->ui->girList->currentRow() != -1 && this->ui->dispList->currentRow() != -1 && this->ui->seqList->currentRow() != -1)
    {
        QString msg = this->ui->girList->currentItem()->text();
        QString dis = this->ui->dispList->currentItem()->text();
        QString seq = this->ui->seqList->currentItem()->text();
        this->girF->setLine(msg,dis,seq,"line_1",this->ui->text1->text());
        this->girF->setLineParam(msg,dis,seq,"line_1","font",this->ui->fontSelect_l1->currentText());
    }
    this->refreshDisplay();
}

void signWindow::on_text2_editingFinished()
{
    if(this->ui->girList->currentRow() != -1 && this->ui->dispList->currentRow() != -1 && this->ui->seqList->currentRow() != -1)
    {
        QString msg = this->ui->girList->currentItem()->text();
        QString dis = this->ui->dispList->currentItem()->text();
        QString seq = this->ui->seqList->currentItem()->text();
        this->girF->setLine(msg,dis,seq,"line_2",this->ui->text2->text());
        this->girF->setLineParam(msg,dis,seq,"line_2","font",this->ui->fontSelect_l2->currentText());
    }
    this->refreshDisplay();
}

void signWindow::on_text3_editingFinished()
{
    if(this->ui->girList->currentRow() != -1 && this->ui->dispList->currentRow() != -1 && this->ui->seqList->currentRow() != -1)
    {
        QString msg = this->ui->girList->currentItem()->text();
        QString dis = this->ui->dispList->currentItem()->text();
        QString seq = this->ui->seqList->currentItem()->text();
        this->girF->setLine(msg,dis,seq,"line_3",this->ui->text3->text());
        this->girF->setLineParam(msg,dis,seq,"line_3","font",this->ui->fontSelect_l3->currentText());
    }
    this->refreshDisplay();
}

void signWindow::loadFont(QString fontName)
{
    this->fontF->currentFont = fontName;
    this->characters = this->fontF->listChars(fontName);
    this->datas.clear();
    foreach(QString character, this->characters)
    {
        this->datas.append(this->fontF->readChar(character));
    }
}
void signWindow::drawImage(QImage img, QLabel *dest)
{
    QPixmap pix;
    pix = pix.fromImage(img);

    dest->setPixmap(pix);
}

void signWindow::on_actionDisplay_Setup_triggered()
{
    DisplaySettings ds;
    ds.exec();
    this->pars->reload();
    this->defaultY = (pars->readParam("defWidth").isNull())?200:pars->readParam("defWidth").toInt();
    this->defaultX = (pars->readParam("defHeight").isNull())?20:pars->readParam("defHeight").toInt();
    this->defaultLineW = (pars->readParam("defLineWidth").isNull())?200:pars->readParam("defLineWidth").toInt();

    this->refreshDisplay();
}

void signWindow::refreshDisplay()
{
    int currHeight = 0;
    int lineWidth = this->customs->lineWidth;
    QImage img;
    if(this->customs->vAlign == "center")
    {
        int totalHeight = 0;
        if(this->ui->text1->text() != "" && !this->ui->text1->text().isNull())
        {

            totalHeight += this->fontF->readFontY(this->ui->fontSelect_l1->currentText());
        }

        if(this->ui->text2->text() != "" && !this->ui->text2->text().isNull())
        {

            totalHeight += this->fontF->readFontY(this->ui->fontSelect_l2->currentText());
        }

        if(this->ui->text3->text() != "" && !this->ui->text3->text().isNull())
        {

            totalHeight += this->fontF->readFontY(this->ui->fontSelect_l3->currentText());
        }

        int startHeight = (this->customs->height - totalHeight) / 2;
        currHeight = startHeight;
    }
    else if(this->customs->vAlign == "top")
    {
        currHeight = 0;
    }
    else if(this->customs->vAlign == "bottom")
    {
        int totalHeight = 0;
        if(this->ui->text1->text() != "" && !this->ui->text1->text().isNull())
        {

            totalHeight += this->fontF->readFontY(this->ui->fontSelect_l1->currentText());
        }

        if(this->ui->text2->text() != "" && !this->ui->text2->text().isNull())
        {

            totalHeight += this->fontF->readFontY(this->ui->fontSelect_l2->currentText());
        }

        if(this->ui->text3->text() != "" && !this->ui->text3->text().isNull())
        {

            totalHeight += this->fontF->readFontY(this->ui->fontSelect_l3->currentText());
        }

        int startHeight = (this->customs->height - totalHeight);
        currHeight = startHeight;

    }

    img = this->generateMapEx(customs->height, customs->width);
    QString align;
    if(this->ui->girList->currentRow() != -1 && this->ui->dispList->currentRow() != -1 && this->ui->seqList->currentRow() != -1)
    {
        QString seq = this->ui->seqList->currentItem()->text();
        QString gir = this->ui->girList->currentItem()->text();
        QString disp = this->ui->dispList->currentItem()->text();
        if(this->ui->text1->text() != "" && !this->ui->text1->text().isNull())
        {
            this->loadFont(this->ui->fontSelect_l1->currentText());
            align = (this->girF->readLineParam(gir,disp,seq,"line_1","align").isNull())?"center":this->girF->readLineParam(gir,disp,seq,"line_1","align");
            img = this->regenerateTextEx(this->ui->text1->text(),align,lineWidth,currHeight,img,false);
            currHeight += this->fontF->readFontY(this->fontF->currentFont);
        }

        if(this->ui->text2->text() != "" && !this->ui->text2->text().isNull())
        {
            this->loadFont(this->ui->fontSelect_l2->currentText());
            align = (this->girF->readLineParam(gir,disp,seq,"line_2","align").isNull())?"center":this->girF->readLineParam(gir,disp,seq,"line_2","align");
            img = this->regenerateTextEx(this->ui->text2->text(),align,lineWidth,currHeight,img,false);
            currHeight += this->fontF->readFontY(this->fontF->currentFont);
        }

        if(this->ui->text3->text() != "" && !this->ui->text3->text().isNull())
        {
            this->loadFont(this->ui->fontSelect_l3->currentText());
            align = (this->girF->readLineParam(gir,disp,seq,"line_3","align").isNull())?"center":this->girF->readLineParam(gir,disp,seq,"line_3","align");
            img = this->regenerateTextEx(this->ui->text3->text(),align,lineWidth,currHeight,img,false);
            currHeight += this->fontF->readFontY(this->fontF->currentFont);
        }

        this->loadFont(this->ui->fontSelect->currentText());
        int lineY =  (customs->height - this->fontF->readFontY(this->fontF->currentFont)) / 2;
        align = (this->girF->readLineParam(gir,disp,seq,"linenum","align").isNull())?"center":this->girF->readLineParam(gir,disp,seq,"linenum","align");
        img = this->regenerateTextEx(this->ui->linenum->text(),align,0,lineY,img,true);
    }
    this->drawImage(img, this->ui->sign);
}



void signWindow::on_actionAbout_triggered()
{
    QMessageBox::information(this,tr("About"),tr("Girouette, bus sign editor\nCreated by Cocodidou, 2011\nTHIS IS FREE SOFTWARE! SEE THE GPLv3 LICENSE FOR DETAILS."));
}

void signWindow::on_fontSelect_l3_activated(QString )
{

}

void signWindow::on_actionGeneral_Setup_triggered()
{
    GeneralSettings genset;
    genset.exec();
    this->pars->reload();
    this->fontF->end();
    this->fontF = new fontFile(this->pars->readParam("fontfile").isNull()?"./font.xml":this->pars->readParam("fontfile"));
    this->eltColor = (pars->readParam("eltColor").isNull()?"#FFFF00":pars->readParam("eltColor"));
    this->eltType = (pars->readParam("eltType").isNull()?"matrix":pars->readParam("eltType"));
    this->eltColor_led_std = (pars->readParam("eltColor_led_std").isNull()?"#FFFF00":pars->readParam("eltColor_led_std"));
    this->eltColor_led_bg = (pars->readParam("eltColor_led_bg").isNull()?"#FFFF00":pars->readParam("eltColor_led_bg"));
    this->eltColor_led_line = (pars->readParam("eltColor_led_line").isNull()?"#FFFF00":pars->readParam("eltColor_led_line"));
    this->vAlign = (pars->readParam("vAlign").isNull()?"center":pars->readParam("vAlign"));
    this->generateFontList();

}

void signWindow::on_actionOpen_triggered()
{
    QFileDialog fd;
    fd.setAcceptMode(QFileDialog::AcceptOpen);
    fd.setFileMode(QFileDialog::ExistingFile);
    fd.setWindowTitle("Open...");
    if(fd.exec() == QDialog::Accepted)
    {
        //Load file
        this->girF = new girFile(fd.selectedFiles()[0]);
        this->currentFile = fd.selectedFiles()[0];
        this->setWindowTitle(this->currentFile + tr(" - Sign Editor"));

        this->refreshGirList();
    }


}


void signWindow::on_newGir_clicked()
{
    int count = this->ui->girList->count();
    QString newName = tr("Message ")+parseInt(count+1);
    if(this->girF->createMessage(newName))
    {
        this->girF->createDisplay(newName,tr("Front"));
        this->girF->createSeq(newName,tr("Front"),tr("First"));
        this->setDefaults(newName,tr("Front"));

        this->girF->createDisplay(newName,tr("Rear"));
        this->girF->createSeq(newName,tr("Rear"),tr("First"));
        this->setDefaults(newName,tr("Rear"));

        this->girF->createDisplay(newName,tr("Side"));
        this->girF->createSeq(newName,tr("Side"),tr("First"));
        this->setDefaults(newName,tr("Side"));


    }
    else
    {
        QInputDialog dialog;
        dialog.setLabelText(tr("Enter a new message name..."));
        dialog.setInputMode(QInputDialog::TextInput);
        int dialogValue = dialog.exec();
        newName = dialog.textValue();
        while(!this->girF->createMessage(newName) && dialogValue == QDialog::Accepted)
        {
            dialogValue = dialog.exec();
            newName = dialog.textValue();
        }
        // QMessageBox::critical(this,"Error","Could not create a new message. For the moment, no way to fix this is implemnted. By the future, this dialog box will be replaced with a prompt.");
    }
    this->refreshGirList();

    this->ui->girList->setCurrentRow(this->ui->girList->count()-1);
    this->ui->girList->editItem(this->ui->girList->currentItem());
}
//WARNING: The following 3 functions do not check their return value nor their input. Some checks must be implemnted before calling them (i.e. in lists, selected indexes)
void signWindow::refreshGirList()
{
    QStringList girs = this->girF->listMessages();

    this->ui->girList->clear();
    foreach(QString gir, girs)
    {
        QListWidgetItem *item = new QListWidgetItem(gir);
        item->setFlags(Qt::ItemIsEditable|Qt::ItemIsSelectable|Qt::ItemIsEnabled);

        this->ui->girList->addItem(item);
    }


}
void signWindow::refreshDispList(QString msgName)
{
    this->ui->dispList->clear();
    QStringList girs = this->girF->listDisplays(msgName);

    foreach(QString gir, girs)
    {
        QListWidgetItem *item = new QListWidgetItem(gir);
        item->setFlags(Qt::ItemIsEditable|Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        this->ui->dispList->addItem(item);
    }
    //this->ui->dispList->addItems(girs);
}
void signWindow::refreshSeqList(QString msgName, QString dispName)
{
    this->ui->seqList->clear();
    QStringList girs = this->girF->listSeq(msgName,dispName);

    foreach(QString gir, girs)
    {
        QListWidgetItem *item = new QListWidgetItem(gir);
        item->setFlags(Qt::ItemIsEditable|Qt::ItemIsSelectable|Qt::ItemIsEnabled);
        this->ui->seqList->addItem(item);
    }

}


void signWindow::on_girList_currentTextChanged(QString currentText)
{

    if(this->ui->girList->currentRow() != -1)
    {

        //Index change
        this->girName = currentText;
        this->girIndex = this->ui->girList->currentRow();
        this->ui->dispList->clear();
        this->ui->seqList->clear();
        this->refreshDispList(currentText);
        this->ui->dispList->setEnabled(true);
        this->ui->newDisp->setEnabled(true);
        this->ui->deleteDisp->setEnabled(true);
        this->ui->edit_disp->setEnabled(true);

    }
    else
    {
        this->ui->dispList->setEnabled(false);
        this->ui->seqList->setEnabled(false);
        this->ui->newDisp->setEnabled(false);
        this->ui->deleteDisp->setEnabled(false);
        this->ui->addSeq->setEnabled(false);
        this->ui->delSeq->setEnabled(false);
        this->ui->edit_seq->setEnabled(false);
        this->ui->edit_disp->setEnabled(false);

        this->ui->dispList->clear();
        this->ui->seqList->clear();
    }
    if(this->fontF->listFonts().count() > 0)
    {
        this->ui->linenum->setText("");
        this->ui->fontSelect->setCurrentIndex(0);
        this->ui->text1->setText("");
        this->ui->fontSelect_l1->setCurrentIndex(0);
        this->ui->text2->setText("");
        this->ui->fontSelect_l2->setCurrentIndex(0);
        this->ui->text3->setText("");
        this->ui->fontSelect_l3->setCurrentIndex(0);
        refreshDisplay();
    }
}

void signWindow::on_dispList_currentTextChanged(QString currentText)
{

    if(this->ui->girList->currentRow() != -1 && this->ui->dispList->currentRow() != -1)
    {
        //Index change
        this->dispName = currentText;
        this->dispIndex = this->ui->dispList->currentRow();
        this->ui->seqList->clear();
        this->refreshSeqList(this->ui->girList->currentItem()->text(),currentText);
        this->ui->seqList->setEnabled(true);
        this->ui->addSeq->setEnabled(true);
        this->ui->delSeq->setEnabled(true);
        this->ui->edit_seq->setEnabled(true);

    }
    else
    {

        this->ui->seqList->setEnabled(false);
        this->ui->addSeq->setEnabled(false);
        this->ui->delSeq->setEnabled(false);
        this->ui->edit_seq->setEnabled(false);
        this->ui->seqList->clear();
    }

    if(this->fontF->listFonts().count() > 0)
    {
        this->ui->linenum->setText("");
        this->ui->fontSelect->setCurrentIndex(0);
        this->ui->text1->setText("");
        this->ui->fontSelect_l1->setCurrentIndex(0);
        this->ui->text2->setText("");
        this->ui->fontSelect_l2->setCurrentIndex(0);
        this->ui->text3->setText("");
        this->ui->fontSelect_l3->setCurrentIndex(0);
        refreshDisplay();
    }
}

void signWindow::on_delGir_clicked()
{
    if(this->ui->girList->currentRow() != -1)
    {
        this->girF->removeMessage(this->ui->girList->currentItem()->text());
    }
    this->refreshGirList();

    if(this->ui->girList->currentRow() != -1)
    {
        this->refreshDispList(this->ui->girList->currentItem()->text());
    }
    else
    {
        this->ui->dispList->clear();
    }

    if(this->ui->girList->currentRow() != -1 && this->ui->dispList->currentRow() != -1)
    {
        this->refreshSeqList(this->ui->girList->currentItem()->text(),this->ui->dispList->currentItem()->text());
    }
    else
    {
        this->ui->seqList->clear();
    }
}

void signWindow::on_newDisp_clicked()
{

    if(this->ui->girList->currentRow() != -1)
    {
        int count = this->ui->dispList->count();
        QString newName = tr("Display ")+parseInt(count);
        if(!this->girF->createDisplay(this->ui->girList->currentItem()->text(), newName))
        {
            QInputDialog dialog;
            dialog.setLabelText(tr("Enter a new display name..."));
            dialog.setInputMode(QInputDialog::TextInput);
            int dialogValue = dialog.exec();
            newName = dialog.textValue();
            while(!this->girF->createDisplay(this->ui->girList->currentItem()->text(), newName) && dialogValue == QDialog::Accepted)
            {
                dialogValue = dialog.exec();
                newName = dialog.textValue();
            }
            this->refreshDispList(this->ui->girList->currentItem()->text());
        }
        else
        {
            this->setDefaults(this->ui->girList->currentItem()->text(), newName);
            this->refreshDispList(this->ui->girList->currentItem()->text());
            this->ui->dispList->setCurrentRow(this->ui->dispList->count()-1);
            this->ui->dispList->editItem(this->ui->dispList->currentItem());
        }


    }

}

void signWindow::on_deleteDisp_clicked()
{
    if(this->ui->girList->currentRow() != -1 && this->ui->dispList->currentRow() != -1)
    {
        this->girF->removeDisplay(this->ui->girList->currentItem()->text(),this->ui->dispList->currentItem()->text());
        this->ui->dispList->clear();
        this->ui->seqList->clear();
        this->refreshDispList(this->ui->girList->currentItem()->text());
    }

}

void signWindow::on_addSeq_clicked()
{
    if(this->ui->girList->currentRow() != -1 && this->ui->dispList->currentRow() != -1)
    {
        int count = this->ui->seqList->count();
        QString newName = "";
        switch(count)
        {
        case 0:
            newName = tr("First");
            break;
        case 1:
            newName = tr("Second");
            break;
        case 2:
            newName = tr("Third");
            break;
        default:
            newName = parseInt(count+1) + tr("th");
            break;

        }
        if(!this->girF->createSeq(this->ui->girList->currentItem()->text(), this->ui->dispList->currentItem()->text(), newName))
        {
            QInputDialog dialog;
            dialog.setLabelText(tr("Enter a new sequence element name..."));
            dialog.setInputMode(QInputDialog::TextInput);
            int dialogValue = dialog.exec();
            newName = dialog.textValue();
            while(!this->girF->createSeq(this->ui->girList->currentItem()->text(), this->ui->dispList->currentItem()->text(), newName) && dialogValue == QDialog::Accepted)
            {
                dialogValue = dialog.exec();
                newName = dialog.textValue();
            }
            if(dialogValue == QDialog::Accepted)
            {
                //Seq elem has been created.
                this->girF->setSeqProp(this->ui->girList->currentItem()->text(),this->ui->dispList->currentItem()->text(),newName,"vAlign",this->vAlign);
            }
            this->refreshSeqList(this->ui->girList->currentItem()->text(), this->ui->dispList->currentItem()->text());
        }
        else
        {
            this->refreshSeqList(this->ui->girList->currentItem()->text(), this->ui->dispList->currentItem()->text());
            this->ui->seqList->setCurrentRow(this->ui->seqList->count()-1);
            this->ui->seqList->editItem(this->ui->seqList->currentItem());
        }

    }

}

void signWindow::on_delSeq_clicked()
{
    if(this->ui->girList->currentRow() != -1 && this->ui->dispList->currentRow() != -1 && this->ui->seqList->currentRow() != -1)
    {
        this->girF->removeSeq(this->ui->girList->currentItem()->text(),this->ui->dispList->currentItem()->text(), this->ui->seqList->currentItem()->text());
        this->ui->seqList->clear();
        this->refreshSeqList(this->ui->girList->currentItem()->text(),this->ui->dispList->currentItem()->text());
    }
}

void signWindow::on_actionExport_current_view_triggered()
{
    QFileDialog dlg;
    QStringList filters;
    filters.append("PNG (*.png)");
    filters.append(tr("All file types(*)"));
    dlg.setAcceptMode(QFileDialog::AcceptSave);
    dlg.setFileMode(QFileDialog::AnyFile);
    dlg.setNameFilters(filters);
    if(dlg.exec() == QDialog::Accepted)
    {
        const QPixmap *pix = this->ui->sign->pixmap();
        pix->save(dlg.selectedFiles()[0],"png",100);
    }
}

void signWindow::on_seqList_currentTextChanged(QString currentText)
{

    if(this->ui->girList->currentRow() != -1 && this->ui->dispList->currentRow() != -1 && this->ui->seqList->currentRow() != -1)
    {

        this->seqName = currentText;
        this->seqIndex = this->ui->seqList->currentRow();

        QString msg = this->ui->girList->currentItem()->text();
        QString dis = this->ui->dispList->currentItem()->text();
        QString seq = currentText;
        readCustomSettings(msg,dis,seq);
        updateCustomFields();
        QStringList availLines = this->girF->listLines(msg,dis,seq);
        QStringList availFonts = this->fontF->listFonts();
        if(this->customs->vAlign == "top")
        {
            this->ui->valign_top->setChecked(true);
        }
        else if(this->customs->vAlign == "center")
        {
            this->ui->valign_center->setChecked(true);
        }
        else if(this->customs->vAlign == "bottom")
        {
            this->ui->valign_bottom->setChecked(true);
        }
        if(availFonts.count() > 0)
        {
            if(availLines.contains("linenum"))
            {
                QString line_num = this->girF->readLine(msg,dis,seq,"linenum");
                QString line_num_font = this->girF->readLineFont(msg,dis,seq,"linenum");
                this->ui->linenum->setText(line_num);
                if(availFonts.contains(line_num_font))
                {
                    int index = this->ui->fontSelect->findText(line_num_font);
                    this->ui->fontSelect->setCurrentIndex(index);
                }
            }
            else
            {
                this->ui->linenum->setText("");
                this->ui->fontSelect->setCurrentIndex(0);
            }

            if(availLines.contains("line_1"))
            {
                QString line = this->girF->readLine(msg,dis,seq,"line_1");
                QString line_font = this->girF->readLineFont(msg,dis,seq,"line_1");
                this->ui->text1->setText(line);
                if(availFonts.contains(line_font))
                {
                    int index = this->ui->fontSelect_l1->findText(line_font);
                    this->ui->fontSelect_l1->setCurrentIndex(index);
                }
            }
            else
            {
                this->ui->text1->setText("");
                this->ui->fontSelect_l1->setCurrentIndex(0);
            }

            if(availLines.contains("line_2"))
            {
                QString line = this->girF->readLine(msg,dis,seq,"line_2");
                QString line_font = this->girF->readLineFont(msg,dis,seq,"line_2");
                this->ui->text2->setText(line);
                if(availFonts.contains(line_font))
                {
                    int index = this->ui->fontSelect_l2->findText(line_font);
                    this->ui->fontSelect_l2->setCurrentIndex(index);
                }
            }
            else
            {
                this->ui->text2->setText("");
                this->ui->fontSelect_l2->setCurrentIndex(0);
            }

            if(availLines.contains("line_3"))
            {
                QString line = this->girF->readLine(msg,dis,seq,"line_3");
                QString line_font = this->girF->readLineFont(msg,dis,seq,"line_3");
                this->ui->text3->setText(line);
                if(availFonts.contains(line_font))
                {
                    int index = this->ui->fontSelect_l3->findText(line_font);
                    this->ui->fontSelect_l3->setCurrentIndex(index);
                }
            }
            else
            {
                this->ui->text3->setText("");
                this->ui->fontSelect_l3->setCurrentIndex(0);
            }
            toggleElements(true);
            this->refreshDisplay();
        }



    }
    else
    {
        toggleElements(false);
    }
}

void signWindow::on_actionSave_triggered()
{
    this->girF->saveFile();
}

void signWindow::on_actionNew_triggered()
{
    this->girF->end();
    QFile::remove("./gir.tmp.xml");
    this->girF = new girFile("./gir.tmp.xml");
    this->ui->linenum->clear();
    this->ui->text1->clear();
    this->ui->text2->clear();
    this->ui->text3->clear();
    this->ui->seqList->clear();
    this->ui->dispList->clear();
    this->ui->girList->clear();
    this->generateFontList();
    this->refreshDisplay();
    this->setWindowTitle(tr("Sign Editor"));
}

void signWindow::on_actionSave_as_triggered()
{
    QFileDialog dlg;
    dlg.setAcceptMode(QFileDialog::AcceptSave);
    dlg.setFileMode(QFileDialog::AnyFile);
    if(dlg.exec() == QDialog::Accepted)
    {
        this->girF->saveFileAs(dlg.selectedFiles()[0]);
        this->setWindowTitle(dlg.selectedFiles()[0] + tr(" - Sign Editor"));
    }

}

void signWindow::girRename(QWidget * widget, QAbstractItemDelegate::EndEditHint hint)
{
    widget = widget; //So as the compiler does not complain...
    hint = hint;
    QString newGir = this->ui->girList->currentItem()->text();
    QString oldGir = this->girName;
    this->girF->renameMessage(oldGir,newGir);
    this->girName = newGir;
}

void signWindow::dispRename(QWidget * widget, QAbstractItemDelegate::EndEditHint hint)
{
    widget = widget;
    hint = hint;
    QString gir = this->girName;
    QString oldDisp = this->dispName;
    QString newDisp = this->ui->dispList->currentItem()->text();
    this->girF->renameDisplay(gir,oldDisp,newDisp);
    this->dispName = newDisp;
}

void signWindow::seqRename(QWidget * widget, QAbstractItemDelegate::EndEditHint hint)
{
    widget = widget;
    hint = hint;
    QString gir = this->girName;
    QString disp = this->dispName;
    QString oldSeq = this->seqName;
    QString newSeq = this->ui->seqList->currentItem()->text();
    this->girF->renameSeq(gir,disp,oldSeq,newSeq);
    this->seqName = newSeq;
}

void signWindow::toggleElements(bool enabled)
{
    this->ui->height->setEnabled(enabled);
    this->ui->width->setEnabled(enabled);
    this->ui->linewidth->setEnabled(enabled);
    this->ui->eltType_leds->setEnabled(enabled);
    this->ui->eltType_matrix->setEnabled(enabled);
    this->ui->eltColor_bg_btn->setEnabled(enabled);
    this->ui->eltColor_btn->setEnabled(enabled);
    this->ui->eltColor_leds_btn->setEnabled(enabled);
    this->ui->eltColor_leds_std_btn->setEnabled(enabled);
    this->ui->linenum->setEnabled(enabled);
    this->ui->text1->setEnabled(enabled);
    this->ui->text2->setEnabled(enabled);
    this->ui->text3->setEnabled(enabled);
    this->ui->fontSelect->setEnabled(enabled);
    this->ui->fontSelect_l1->setEnabled(enabled);
    this->ui->fontSelect_l2->setEnabled(enabled);
    this->ui->fontSelect_l3->setEnabled(enabled);
    this->ui->valign_bottom->setEnabled(enabled);
    this->ui->valign_center->setEnabled(enabled);
    this->ui->valign_top->setEnabled(enabled);

    this->ui->align_left_linenum->setEnabled(enabled);
    this->ui->align_center_linenum->setEnabled(enabled);
    this->ui->align_right_linenum->setEnabled(enabled);
    this->ui->align_justify_linenum->setEnabled(enabled);

    this->ui->align_left_line_1->setEnabled(enabled);
    this->ui->align_center_line_1->setEnabled(enabled);
    this->ui->align_right_line_1->setEnabled(enabled);
    this->ui->align_justify_line_1->setEnabled(enabled);

    this->ui->align_left_line_2->setEnabled(enabled);
    this->ui->align_center_line_2->setEnabled(enabled);
    this->ui->align_right_line_2->setEnabled(enabled);
    this->ui->align_justify_line_2->setEnabled(enabled);

    this->ui->align_left_line_3->setEnabled(enabled);
    this->ui->align_center_line_3->setEnabled(enabled);
    this->ui->align_right_line_3->setEnabled(enabled);
    this->ui->align_justify_line_3->setEnabled(enabled);

}

void signWindow::setDefaults(QString girName, QString dispName)
{
    this->girF->setDisplayProp(girName,dispName,"eltColor",this->eltColor);
    this->girF->setDisplayProp(girName,dispName,"eltColor_led_std",this->eltColor_led_std);
    this->girF->setDisplayProp(girName,dispName,"eltColor_led_bg",this->eltColor_led_bg);
    this->girF->setDisplayProp(girName,dispName,"eltColor_led_line",this->eltColor_led_line);
    this->girF->setDisplayProp(girName,dispName,"eltType",this->eltType);
    this->girF->setDisplayProp(girName,dispName,"height",parseInt(this->defaultX));
    this->girF->setDisplayProp(girName,dispName,"width",parseInt(this->defaultY));
    this->girF->setDisplayProp(girName,dispName,"lineWidth",parseInt(this->defaultLineW));
}

void signWindow::on_eltType_matrix_clicked()
{

}

void signWindow::on_eltType_leds_clicked()
{

}

void signWindow::on_eltColor_btn_clicked()
{
    this->customs->eltColor = this->chooseColor(this->customs->eltColor);
    this->drawColorSample(this->customs->eltColor,this->ui->eltColor_btn);
    this->girF->setDisplayProp(this->girName,this->dispName,"eltColor",this->customs->eltColor);
    this->refreshDisplay();
}

void signWindow::on_eltColor_leds_std_btn_clicked()
{
    this->customs->eltColor_led_std = this->chooseColor(this->customs->eltColor_led_std);
    this->drawColorSample(this->customs->eltColor_led_std,this->ui->eltColor_leds_std_btn);
    this->girF->setDisplayProp(this->girName,this->dispName,"eltColor_led_std",this->customs->eltColor_led_std);
    this->refreshDisplay();
}

void signWindow::on_eltColor_leds_btn_clicked()
{
    this->customs->eltColor_led_line= this->chooseColor(this->customs->eltColor_led_line);
    this->drawColorSample(this->customs->eltColor_led_line,this->ui->eltColor_leds_btn);
    this->girF->setDisplayProp(this->girName,this->dispName,"eltColor_led_line",this->customs->eltColor_led_line);
    this->refreshDisplay();
}

void signWindow::on_eltColor_bg_btn_clicked()
{
    this->customs->eltColor_led_bg= this->chooseColor(this->customs->eltColor_led_bg);
    this->drawColorSample(this->customs->eltColor_led_bg,this->ui->eltColor_bg_btn);
    this->girF->setDisplayProp(this->girName,this->dispName,"eltColor_led_bg",this->customs->eltColor_led_bg);
    this->refreshDisplay();
}

void signWindow::on_height_valueChanged(int height)
{
    this->customs->height = height;
    this->girF->setDisplayProp(this->girName,this->dispName,"height",parseInt(height));
    this->refreshDisplay();
}

void signWindow::on_width_valueChanged(int width)
{
    this->customs->width = width;
    this->girF->setDisplayProp(this->girName,this->dispName,"width",parseInt(width));
    this->refreshDisplay();
}

void signWindow::on_linewidth_valueChanged(int lineWidth)
{
    this->customs->lineWidth = lineWidth;
    this->girF->setDisplayProp(this->girName,this->dispName,"lineWidth",parseInt(lineWidth));
    this->refreshDisplay();
}

void signWindow::readCustomSettings(QString msgName, QString dispName, QString seqName)
{
    this->customs = new customSettings();
    this->customs->eltColor = (this->girF->readDisplayProp(msgName, dispName,"eltColor").isNull())?this->eltColor:this->girF->readDisplayProp(msgName, dispName,"eltColor");
    this->customs->eltColor_led_bg = (this->girF->readDisplayProp(msgName, dispName,"eltColor_led_bg").isNull())?this->eltColor_led_bg:this->girF->readDisplayProp(msgName, dispName,"eltColor_led_bg");
    this->customs->eltColor_led_line = (this->girF->readDisplayProp(msgName, dispName,"eltColor_led_line").isNull())?this->eltColor_led_line:this->girF->readDisplayProp(msgName, dispName,"eltColor_led_line");
    this->customs->eltColor_led_std = (this->girF->readDisplayProp(msgName, dispName,"eltColor_led_std").isNull())?this->eltColor_led_std:this->girF->readDisplayProp(msgName, dispName,"eltColor_led_std");
    this->customs->eltType = (this->girF->readDisplayProp(msgName, dispName,"eltType").isNull())?this->eltType:this->girF->readDisplayProp(msgName, dispName,"eltType");
    this->customs->height = (this->girF->readDisplayProp(msgName, dispName,"height").isNull())?this->defaultX:this->girF->readDisplayProp(msgName, dispName,"height").toInt();
    this->customs->width = (this->girF->readDisplayProp(msgName, dispName,"width").isNull())?this->defaultY:this->girF->readDisplayProp(msgName, dispName,"width").toInt();
    this->customs->lineWidth = (this->girF->readDisplayProp(msgName, dispName,"lineWidth").isNull())?this->defaultLineW:this->girF->readDisplayProp(msgName, dispName,"lineWidth").toInt();
    this->customs->vAlign = (this->girF->readSeqProp(msgName, dispName, seqName,"vAlign").isNull())?this->vAlign:this->girF->readSeqProp(msgName, dispName, seqName, "vAlign");

}

void signWindow::drawColorSample(QString color, QPushButton *button)
{
    QColor clr(color);
    QSize resultSize(40, 16);
    QImage image(resultSize, QImage::Format_RGB32);
    QPainter painter(&image);
    painter.fillRect(image.rect(),clr);
    painter.end();
    QPixmap pix;
    pix = pix.fromImage(image);
    QIcon icon(pix);
    button->setIcon(icon);
}

QString signWindow::chooseColor(QString previous)
{
    QColorDialog *dialog = new QColorDialog(this);
    QColor color(previous);
    dialog->setCurrentColor(color);
    if(dialog->exec() == QDialog::Accepted)
    {
        return dialog->currentColor().name();
    }
    else
    {
        return previous;
    }
}

void signWindow::updateCustomFields()
{
    drawColorSample(this->customs->eltColor,this->ui->eltColor_btn);
    drawColorSample(this->customs->eltColor_led_bg,this->ui->eltColor_bg_btn);
    drawColorSample(this->customs->eltColor_led_line,this->ui->eltColor_leds_btn);
    drawColorSample(this->customs->eltColor_led_std,this->ui->eltColor_leds_std_btn);
    this->ui->height->setValue(this->customs->height);
    this->ui->width->setValue(this->customs->width);
    this->ui->linewidth->setValue(this->customs->lineWidth);
    if(this->customs->eltType == "leds")
    {
        this->ui->eltType_leds->setChecked(true);
    }
    else
    {
        this->ui->eltType_matrix->setChecked(true);
    }

}

void signWindow::on_eltType_matrix_toggled(bool checked)
{
    if(checked)
    {
        this->customs->eltType = "matrix";
        this->girF->setDisplayProp(this->girName,this->dispName,"eltType","matrix");
        this->refreshDisplay();
    }
}

void signWindow::on_eltType_leds_toggled(bool checked)
{
    if(checked)
    {
        this->customs->eltType = "leds";
        this->girF->setDisplayProp(this->girName,this->dispName,"eltType","leds");
        this->refreshDisplay();
    }
}

void signWindow::on_edit_msg_clicked()
{
    if(this->ui->girList->currentRow() != -1)
    {
        this->ui->girList->editItem(this->ui->girList->currentItem());
    }
}

void signWindow::on_edit_disp_clicked()
{
    if(this->ui->dispList->currentRow() != -1)
    {
        this->ui->dispList->editItem(this->ui->dispList->currentItem());
    }
}

void signWindow::on_edit_seq_clicked()
{
    if(this->ui->seqList->currentRow() != -1)
    {
        this->ui->seqList->editItem(this->ui->seqList->currentItem());
    }
}

void signWindow::on_valign_top_toggled(bool checked)
{
    if(checked)
    {
        this->customs->vAlign = "top";
        this->girF->setSeqProp(this->ui->girList->currentItem()->text(),this->ui->dispList->currentItem()->text(),this->ui->seqList->currentItem()->text(),"vAlign","top");
        this->refreshDisplay();
    }
}

void signWindow::on_valign_center_toggled(bool checked)
{
    if(checked)
    {
        this->customs->vAlign = "center";
        this->girF->setSeqProp(this->ui->girList->currentItem()->text(),this->ui->dispList->currentItem()->text(),this->ui->seqList->currentItem()->text(),"vAlign","center");
        this->refreshDisplay();
    }

}

void signWindow::on_valign_bottom_toggled(bool checked)
{

    if(checked)
    {
        this->customs->vAlign = "bottom";
        this->girF->setSeqProp(this->ui->girList->currentItem()->text(),this->ui->dispList->currentItem()->text(),this->ui->seqList->currentItem()->text(),"vAlign","bottom");
        this->refreshDisplay();
    }
}



void signWindow::on_align_left_linenum_clicked()
{
    this->alignText("linenum","left");
}

void signWindow::on_align_center_linenum_clicked()
{
    this->alignText("linenum","center");
}

void signWindow::on_align_right_linenum_clicked()
{
    this->alignText("linenum","right");
}

void signWindow::on_align_justify_linenum_clicked()
{
    this->alignText("linenum","justify");
}

void signWindow::on_align_left_line_1_clicked()
{
    this->alignText("line_1","left");
}

void signWindow::on_align_center_line_1_clicked()
{
    this->alignText("line_1","center");
}

void signWindow::on_align_right_line_1_clicked()
{
    this->alignText("line_1","right");
}

void signWindow::on_align_justify_line_1_clicked()
{
    this->alignText("line_1","justify");
}

void signWindow::on_align_left_line_2_clicked()
{
    this->alignText("line_2","left");
}

void signWindow::on_align_center_line_2_clicked()
{
    this->alignText("line_2","center");
}

void signWindow::on_align_right_line_2_clicked()
{
    this->alignText("line_2","right");
}

void signWindow::on_align_justify_line_2_clicked()
{
    this->alignText("line_2","justify");
}

void signWindow::on_align_left_line_3_clicked()
{
    this->alignText("line_3","left");
}

void signWindow::on_align_right_line_3_clicked()
{
    this->alignText("line_3","right");
}

void signWindow::on_align_center_line_3_clicked()
{
    this->alignText("line_3","center");
}

void signWindow::on_align_justify_line_3_clicked()
{
    this->alignText("line_3","justify");
}

void signWindow::alignText(QString line, QString align)
{
    if(this->ui->girList->currentRow() != -1 && this->ui->dispList->currentRow() != -1 && this->ui->seqList->currentRow() != -1)
    {
        QString seq = this->ui->seqList->currentItem()->text();
        QString gir = this->ui->girList->currentItem()->text();
        QString disp = this->ui->dispList->currentItem()->text();
        this->girF->setLineParam(gir,disp,seq,line,"align",align);
        this->refreshDisplay();
    }
}
