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

#include "fonteditor.h"
#include "ui_fonteditor.h"

fontEditor::fontEditor(QWidget *parent) :
        QDialog(parent),
        m_ui(new Ui::fontEditor)
{
    m_ui->setupUi(this);
    this->pars = new settingsFile("./settings.xml");
    this->charEd = new clickLabel("",this);
    //this->charEd->setScaledContents(true);
    this->fontF = new fontFile(this->pars->readParam("fontfile").isNull()?"./font.xml":this->pars->readParam("fontfile"));
    int initialX = 10;
    int initialY = 10;
    this->m_ui->fHeight->setValue(initialX);
    this->m_ui->fWidth->setValue(initialX);
    generateMap(initialX,initialY,this->charEd);
    this->m_ui->fontSelect->disconnect(this,SLOT(on_fontSelect_currentIndexChanged(int)));

    this->m_ui->fontSelect->insertItems(-1,this->fontF->listFonts());
    this->m_ui->fontSelect->connect(this->m_ui->fontSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(on_fontSelect_currentIndexChanged(int)));
    this->m_ui->fontSelect->setCurrentIndex(1);
    this->m_ui->fontSelect->setCurrentIndex(0);


    this->isEditing = false;
    this->charEd->setObjectName(QString::fromUtf8("charEd"));
    this->charEd->setMinimumSize(QSize(200, 200));
    this->charEd->setAlignment(Qt::AlignTop);
    this->m_ui->labelLayout->addWidget(this->charEd);
    this->charEd->connect(this->charEd,SIGNAL(pressed(QMouseEvent*)),this,SLOT(setPressed(QMouseEvent*)));
    this->charEd->connect(this->charEd,SIGNAL(moved(QMouseEvent*,bool)),this,SLOT(picClick(QMouseEvent*,bool)));
    this->charEd->connect(this->charEd,SIGNAL(released(QMouseEvent*)),this,SLOT(setReleased(QMouseEvent*)));
    this->eltColor = (pars->readParam("eltColor").isNull()?"#FFFF00":pars->readParam("eltColor"));




    QSize resultSize(16,16);
    QImage image(":/Elements/pastille_large.png");
    this->elementAdd = QImage(resultSize, QImage::Format_RGB32);


    QPainter painter(&elementAdd);
    QColor color(this->eltColor);
    painter.fillRect(elementAdd.rect(),color);

    painter.drawImage(0,0,image);
    painter.end();

    QImage image_r(":/Elements/fond_pastille_large.png");
    this->elementRem = QImage(resultSize, QImage::Format_RGB32);

    QPainter painte(&elementRem);
    painte.fillRect(elementRem.rect(),Qt::black);

    painte.drawImage(0,0,image_r);
    painte.end();

}

fontEditor::~fontEditor()
{
    delete m_ui;
}

void fontEditor::changeEvent(QEvent *e)
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
void fontEditor::generateMap(int height, int width, QLabel *dest)
{
    QSize resultSize(16 * width, 16 * height);
    QImage element(":/Elements/fond_pastille_large.png");
    QImage image(resultSize, QImage::Format_RGB32);

    QRect lab = dest->geometry();
    lab.setHeight(16 * height);
    lab.setWidth(16 * width);
    dest->setGeometry(lab);

    QPainter painter(&image);
    QRectF rect = element.rect();
    painter.fillRect(image.rect(), Qt::black);

    int x = 0;
    int y = 0;
    QPixmap pix;
    while(y < height)
    {
        while(x < width)
        {
            rect.setCoords(16 * x, 16 * y, -16, -16);
            painter.drawImage(rect, element, image.rect());
            pix = pix.fromImage(image);
            dest->setPixmap(pix);
            x++;
        }
        x = 0;
        y++;
    }
    painter.end();
    QStringList list = this->colored.split(";",QString::SkipEmptyParts);

    foreach(QString elt, list)
    {
        QStringList coords = elt.split(",");
        drawElement(coords[0].toInt(),coords[1].toInt(),elementAdd,dest);
    }
}
void fontEditor::drawElement(int x, int y, QImage element, QLabel *dest)
{
    QImage source = dest->pixmap()->toImage();
    QPainter painter(&source);
    painter.drawImage(16 * x ,16 * y,element);
    painter.end();
    QPixmap pix;
    pix = pix.fromImage(source);

    dest->setPixmap(pix);
}
void fontEditor::picClick(QMouseEvent* event, bool changeMode)
{
    int x= event->x();
    int y= event->y();

    double calculatedX = (x / 16);
    double calculatedY = (y / 16);

    x= calculatedX;
    y = calculatedY;

    this->m_ui->curpos->setText("X:"+parseInt(x)+", Y:"+parseInt(y));
    if(this->isEditing == true)
    {


        if(x <= this->m_ui->fWidth->value() -1 && y <= this->m_ui->fHeight->value() -1 && x >=0 && y >=0)
        {
            if((colored.indexOf(";"+parseInt(x)+","+parseInt(y)+";") == -1) && (!colored.startsWith(parseInt(x)+","+parseInt(y)+";")) && changeMode)
            {

                this->editMode = EDITMODE_ADD;
            }
            else if(changeMode)
            {
                this->editMode = EDITMODE_REM;
            }




            if(this->editMode == EDITMODE_ADD)
            {


                if((colored.indexOf(";"+parseInt(x)+","+parseInt(y)+";") == -1) && (!colored.startsWith(parseInt(x)+","+parseInt(y)+";")))
                {
                    drawElement(x, y, elementAdd, this->charEd);
                    this->colored.append(parseInt(x)+","+parseInt(y)+";");
                }

            }
            else
            {

                drawElement(x, y, elementRem, this->charEd);
                while(colored.indexOf(";"+parseInt(x)+","+parseInt(y)+";") != -1)
                {
                    this->colored.replace(";"+parseInt(x)+","+parseInt(y)+";",";");
                }
                if(colored.startsWith(parseInt(x)+","+parseInt(y)+";"))
                {
                    QString start = parseInt(x)+","+parseInt(y)+";";
                    int len = start.length();
                    colored = colored.right(colored.length() - len);
                }

            }



        }
        this->fontF->editCharData(this->currentCharacter,colored);
    }
}
void fontEditor::setPressed(QMouseEvent *event)
{
    this->isEditing = true;
    this->picClick(event,true);
}

void fontEditor::setReleased(QMouseEvent *event)
{
    this->isEditing = false;
    event->accept();
}

void fontEditor::on_close_clicked()
{
    this->close();
}

void fontEditor::on_fHeight_editingFinished()
{
    generateMap(this->m_ui->fHeight->value(),this->m_ui->fWidth->value(),this->charEd);
}

void fontEditor::on_fWidth_editingFinished()
{
    generateMap(this->m_ui->fHeight->value(),this->m_ui->fWidth->value(),this->charEd);
}

void fontEditor::on_fontSelect_currentIndexChanged(int index)
{

    if(index == this->m_ui->fontSelect->count() -1)
    {
        QInputDialog dialog;
        dialog.setLabelText(tr("Enter the font's name..."));
        dialog.setInputMode(QInputDialog::TextInput);
        this->m_ui->fontSelect->setCurrentIndex(index - 1);
        int dialogValue = dialog.exec();

        if(dialogValue == QDialog::Accepted)
        {
            QString fontName = dialog.textValue();
            if(fontF->createFont(fontName))
            {

                this->m_ui->fontSelect->insertItem(index,fontName);

                this->charEd->setEnabled(true);
                this->m_ui->charList->setEnabled(true);
                this->m_ui->fHeight->setEnabled(true);
                this->m_ui->fWidth->setEnabled(true);
                this->m_ui->nChar->setEnabled(true);
                this->m_ui->charName->setEnabled(true);
                this->m_ui->l_height->setEnabled(true);
                this->m_ui->l_width->setEnabled(true);
                this->m_ui->l_avChars->setEnabled(true);
                this->m_ui->newChar->setEnabled(true);
                this->m_ui->remove->setEnabled(true);
                this->m_ui->removeChar->setEnabled(true);
                this->m_ui->rename->setEnabled(true);
                this->m_ui->fillStd->setEnabled(true);
                this->regenerateFontList();
            }
            else
            {
                QMessageBox::critical(this,tr("Error"),tr("Error creating the font!"));
            }

        }
        else if(this->m_ui->fontSelect->count() == 1)
        {
            QMessageBox::information(this,tr("Information"),tr("You must create or load a font before editing anything!"));
            this->charEd->setEnabled(false);
            this->m_ui->charList->setEnabled(false);
            this->m_ui->fHeight->setEnabled(false);
            this->m_ui->fWidth->setEnabled(false);
            this->m_ui->nChar->setEnabled(false);
            this->m_ui->charName->setEnabled(false);
            this->m_ui->l_height->setEnabled(false);
            this->m_ui->l_width->setEnabled(false);
            this->m_ui->l_avChars->setEnabled(false);
            this->m_ui->newChar->setEnabled(false);
            this->m_ui->remove->setEnabled(false);
            this->m_ui->removeChar->setEnabled(false);
            this->m_ui->rename->setEnabled(false);
            this->m_ui->fillStd->setEnabled(false);
        }

        this->m_ui->fontSelect->setCurrentIndex(this->m_ui->fontSelect->count() -2);

    }
    if(this->m_ui->fontSelect->currentIndex() != this->m_ui->fontSelect->count() -1)
    {
        this->fontF->currentFont = this->m_ui->fontSelect->currentText();
        int fontX = this->fontF->readFontX(this->m_ui->fontSelect->currentText());
        int fontY = this->fontF->readFontY(this->m_ui->fontSelect->currentText());
        this->m_ui->fHeight->setValue(fontY);
        this->m_ui->fWidth->setValue(fontX);

        this->refreshCharList();
    }



}

void fontEditor::on_fHeight_valueChanged(int y)
{
    generateMap(y,this->m_ui->fWidth->value(),this->charEd);
    this->fontF->setFontY(this->fontF->currentFont,y);
}

void fontEditor::on_fWidth_valueChanged(int x)
{
    generateMap(this->m_ui->fHeight->value(),x,this->charEd);
    this->fontF->setFontX(this->fontF->currentFont,x);
}

void fontEditor::on_remove_clicked()
{
    if(QMessageBox::warning(this,tr("Warning"),tr("Do you really want to delete the font ")+this->m_ui->fontSelect->currentText()+tr("?"),QMessageBox::Yes,QMessageBox::No) == QMessageBox::Yes)
    {
        if(this->fontF->removeFont(this->m_ui->fontSelect->currentText()))
        {
            int toRem = this->m_ui->fontSelect->currentIndex();
            this->m_ui->fontSelect->setCurrentIndex(0);
            this->m_ui->fontSelect->removeItem(toRem);
        }
    }
}

void fontEditor::on_newChar_clicked()
{
    QString character = this->m_ui->charName->text();
    this->fontF->addChar(character);
    this->refreshCharList();

}
void fontEditor::refreshCharList()
{
    QStringList list = this->fontF->listChars(this->fontF->currentFont);
    this->m_ui->charList->clear();
    this->m_ui->charList->addItems(list);
}

void fontEditor::on_charList_currentTextChanged(QString currentText)
{
    this->currentCharacter = currentText;
    this->colored = this->fontF->readChar(this->currentCharacter);
    int fontX = this->fontF->readFontX(this->fontF->currentFont);
    int fontY = this->fontF->readFontY(this->fontF->currentFont);
    this->m_ui->fHeight->setValue(fontY);
    this->m_ui->fWidth->setValue(fontX);
    this->generateMap(fontY,fontX,this->charEd);
    QStringList toColor = this->colored.split(";",QString::SkipEmptyParts);
    foreach(QString coords, toColor)
    {
        int x = coords.split(",")[0].toInt();
        int y = coords.split(",")[1].toInt();

        drawElement(x, y, elementAdd, this->charEd);

    }
}

void fontEditor::on_removeChar_clicked()
{
    this->fontF->removeChar(this->currentCharacter);
    this->refreshCharList();
}

void fontEditor::on_load_clicked()
{
    QFileDialog fd;
    fd.setAcceptMode(QFileDialog::AcceptOpen);
    fd.setFileMode(QFileDialog::ExistingFile);
    if(fd.exec() == QFileDialog::Accepted)
    {
        fontMgmt *fgm = new fontMgmt(this,fd.selectedFiles()[0],this->pars->readParam("fontfile").isNull()?"./font.xml":this->pars->readParam("fontfile"),"import");
        fgm->exec();
        this->fontF->end();
        this->fontF = new fontFile(this->pars->readParam("fontfile").isNull()?"./font.xml":this->pars->readParam("fontfile"));
        this->regenerateFontList();
        if(this->m_ui->fontSelect->count() > 1)
        {
            this->charEd->setEnabled(true);
            this->m_ui->charList->setEnabled(true);
            this->m_ui->fHeight->setEnabled(true);
            this->m_ui->fWidth->setEnabled(true);
            this->m_ui->nChar->setEnabled(true);
            this->m_ui->charName->setEnabled(true);
            this->m_ui->l_height->setEnabled(true);
            this->m_ui->l_width->setEnabled(true);
            this->m_ui->l_avChars->setEnabled(true);
            this->m_ui->newChar->setEnabled(true);
            this->m_ui->remove->setEnabled(true);
            this->m_ui->removeChar->setEnabled(true);
            this->m_ui->rename->setEnabled(true);
            this->m_ui->fillStd->setEnabled(true);
            this->regenerateFontList();
        }
    }
}

void fontEditor::on_save_clicked()
{
    QFileDialog fd;
    fd.setAcceptMode(QFileDialog::AcceptSave);
    fd.setFileMode(QFileDialog::AnyFile);
    if(fd.exec() == QFileDialog::Accepted)
    {
        fontMgmt *fgm = new fontMgmt(this,this->pars->readParam("fontfile").isNull()?"./font.xml":this->pars->readParam("fontfile"),fd.selectedFiles()[0],"export");
        fgm->exec();
    }
}

void fontEditor::on_rename_clicked()
{
    QInputDialog dialog;
    dialog.setLabelText(tr("Enter the font's name..."));
    dialog.setTextValue(this->m_ui->fontSelect->currentText());
    dialog.setInputMode(QInputDialog::TextInput);
    int dialogValue = dialog.exec();

    if(dialogValue == QDialog::Accepted && dialog.textValue() != "" && dialog.textValue() != this->m_ui->fontSelect->currentText())
    {
        //Check if the font exists or not...
        QStringList existingFonts = this->fontF->listFonts();
        int retval = QDialog::Accepted;
        while(existingFonts.contains(dialog.textValue()) && retval == QDialog::Accepted)
        {
            dialog.setLabelText(tr("There's already a font with this name! Please enter another..."));
            retval = dialog.exec();
        }
        if(retval == QDialog::Accepted)
        {
            int index = this->m_ui->fontSelect->currentIndex();
            this->fontF->renameFont(this->m_ui->fontSelect->currentText(),dialog.textValue());
            this->regenerateFontList();
            this->m_ui->fontSelect->setCurrentIndex(index);

        }
    }

}

void fontEditor::regenerateFontList()
{
    this->m_ui->fontSelect->disconnect(this,SLOT(on_fontSelect_currentIndexChanged(int)));
    this->m_ui->fontSelect->clear();
    this->m_ui->fontSelect->insertItems(-1,this->fontF->listFonts());
    this->m_ui->fontSelect->addItem(tr("<New...>"));
    this->m_ui->fontSelect->connect(this->m_ui->fontSelect,SIGNAL(currentIndexChanged(int)),this,SLOT(on_fontSelect_currentIndexChanged(int)));
    this->m_ui->fontSelect->setCurrentIndex(1);
    this->m_ui->fontSelect->setCurrentIndex(0);
}

void fontEditor::on_fillStd_clicked()
{
    //Majuscules
    this->fontF->addChar("A");
    this->fontF->addChar("B");
    this->fontF->addChar("C");
    this->fontF->addChar("D");
    this->fontF->addChar("E");
    this->fontF->addChar("F");
    this->fontF->addChar("G");
    this->fontF->addChar("H");
    this->fontF->addChar("I");
    this->fontF->addChar("J");
    this->fontF->addChar("K");
    this->fontF->addChar("L");
    this->fontF->addChar("M");
    this->fontF->addChar("N");
    this->fontF->addChar("O");
    this->fontF->addChar("P");
    this->fontF->addChar("Q");
    this->fontF->addChar("R");
    this->fontF->addChar("S");
    this->fontF->addChar("T");
    this->fontF->addChar("U");
    this->fontF->addChar("V");
    this->fontF->addChar("W");
    this->fontF->addChar("X");
    this->fontF->addChar("Y");
    this->fontF->addChar("Z");
    //Minuscules
    this->fontF->addChar("a");
    this->fontF->addChar("b");
    this->fontF->addChar("c");
    this->fontF->addChar("d");
    this->fontF->addChar("e");
    this->fontF->addChar("f");
    this->fontF->addChar("g");
    this->fontF->addChar("h");
    this->fontF->addChar("i");
    this->fontF->addChar("j");
    this->fontF->addChar("k");
    this->fontF->addChar("l");
    this->fontF->addChar("m");
    this->fontF->addChar("n");
    this->fontF->addChar("o");
    this->fontF->addChar("p");
    this->fontF->addChar("q");
    this->fontF->addChar("r");
    this->fontF->addChar("s");
    this->fontF->addChar("t");
    this->fontF->addChar("u");
    this->fontF->addChar("v");
    this->fontF->addChar("w");
    this->fontF->addChar("x");
    this->fontF->addChar("y");
    this->fontF->addChar("z");
    //Chiffres
    this->fontF->addChar("0");
    this->fontF->addChar("1");
    this->fontF->addChar("2");
    this->fontF->addChar("3");
    this->fontF->addChar("4");
    this->fontF->addChar("5");
    this->fontF->addChar("6");
    this->fontF->addChar("7");
    this->fontF->addChar("8");
    this->fontF->addChar("9");
    //Special
    this->fontF->addChar("é");
    this->fontF->addChar("è");
    this->fontF->addChar("à");
    this->fontF->addChar("'");
    this->fontF->addChar("\"");
    this->fontF->addChar("&");
    this->fontF->addChar("(");
    this->fontF->addChar(")");
    this->fontF->addChar("+");
    this->fontF->addChar("-");
    this->fontF->addChar("*");
    this->fontF->addChar("/");
    this->fontF->addChar("_");
    this->refreshCharList();
}

void fontEditor::on_correctErrors_clicked()
{
    QString font = this->m_ui->fontSelect->currentText();
    int fontX = this->fontF->readFontX(font);
    int fontY = this->fontF->readFontY(font);
    QStringList charList = this->fontF->listChars(font);
    int errCount = 0;
    foreach(QString character, charList)
    {

        //qDebug("Now we're on char %s",character.toStdString().c_str());
        bool recommence = true;
        QString data = this->fontF->readChar(character);
        QStringList spl;
        while(recommence && data != "" && !data.isNull())
        {

            spl = data.split(";",QString::SkipEmptyParts);

            foreach(QString point, spl)
            {

                QString pointStr = ";" + point + ";";
                recommence = false;
                if(spl.first() != point)
                {

                    if(spl.count(point) > 1)
                    {


                        data.replace(pointStr,";");
                        data.append(point + ";");
                        //qDebug("corrected char %s, eliminated %s",character.toStdString().c_str(),point.toStdString().c_str());
                        // qDebug("Data: %s",data.toStdString().c_str());
                        errCount++;
                        recommence = true;
                        break;
                    }
                }
                else
                {
                    if(spl.count(point) > 1)
                    {


                        data.replace(pointStr,";");
                        //qDebug("corrected char %s, eliminated %s",character.toStdString().c_str(),point.toStdString().c_str());
                        //qDebug("Data: %s",data.toStdString().c_str());
                        errCount++;
                        recommence = true;
                        break;
                    }
                }



                int charX = point.split(",")[0].toInt();
                int charY = point.split(",")[1].toInt();

                if((charX > fontX) || (charY > fontY) || (charY < 0) || (charX <0))
                {

                    errCount++;
                    data.replace(pointStr,";");
                    recommence = true;
                    break;
                }


            }


        }
        this->fontF->editCharData(character,data);

    }
    QMessageBox::information(this,"Info","Cleared "+parseInt(errCount)+" errors.");
}
/*
void fontEditor::on_importFromImage_clicked()
{
    QFileDialog dialog;
    dialog.setFileMode(QFileDialog::ExistingFile);
    dialog.setAcceptMode(QFileDialog::AcceptOpen);
    if(dialog.exec() == QDialog::Accepted)
    {
        QImage img(dialog.selectedFiles()[0]);
        img = img.convertToFormat(QImage::Format_Mono);
        QImage scaled = img.scaledToWidth(this->m_ui->fWidth->value());
        //if(img->width() > 16 * this->m_ui->fWidth->value())
        //{
        //QPainter pt(&scaled);
        //pt.drawImage(0,0,img.scaledToWidth(this->m_ui->fWidth->value()));
        //pt.end();
        //this->generateMap(this->m_ui->fHeight->value(),this->m_ui->fWidth->value(),this->charEd);
        int cx = 0;
        int cy = 0;
        QColor color;
        QRgb pixcolor;
        while(cx < scaled.width())
        {
            while(cy < scaled.height())
            {
                pixcolor = scaled.pixel(cx,cy);
                color.setRgb(pixcolor);
                if(color == Qt::white)
                {
                    //White
                    //qDebug("white");
                    this->drawElement(cx,cy,this->elementRem,this->charEd);
                }
                else
                {
                    //Black
                    //qDebug("black");
                    this->drawElement(cx,cy,this->elementAdd,this->charEd);
                }
                cy++;
            }
            cy = 0;
            cx++;
        }
        / *
        QPixmap pixmap;
        pixmap = pixmap.fromImage(scaled);
        this->charEd->setPixmap(pixmap);
* /

    }
}
*/
