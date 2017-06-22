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

#include "fontmgmt.h"
#include "ui_fontmgmt.h"

fontMgmt::fontMgmt(QWidget *parent, QString source, QString dest, QString mode) :
        QDialog(parent),
        m_ui(new Ui::fontMgmt)
{
    m_ui->setupUi(this);
    this->sourceFile = new fontFile(source);
    this->destFile = new fontFile(dest);

    this->destinationFile = dest;
    this->mode = mode;

    if(this->mode == "import")
    {
        this->m_ui->info_red_text->setText(tr("indicates this font cannot be imported."));
        this->m_ui->info_label->setText(tr("Select which fonts will be imported:"));
        this->setWindowTitle(tr("Import fonts"));
    }
    else if(this->mode == "export")
    {
        this->m_ui->info_red_text->setText(tr("indicates this font cannot be exported."));
        this->m_ui->info_label->setText(tr("Select which fonts will be exported:"));
        this->setWindowTitle(tr("Export fonts"));
    }
    else
    {
        QMessageBox::critical(this,tr("Error"),tr("The mode you selected for this dialog is invalid."));
    }
    QStringList sourceFonts = sourceFile->listFonts();
    QStringList destFonts = destFile->listFonts();
    foreach(QString font, sourceFonts)
    {
        QListWidgetItem *item = new QListWidgetItem(font);
        QImage green(":/Elements/feu_vert.png");
        QPixmap greenPix;
        greenPix = greenPix.fromImage(green);
        QIcon greenIcon(greenPix);

        QImage yellow(":/Elements/feu_jaune.png");
        QPixmap yellowPix;
        yellowPix = yellowPix.fromImage(yellow);
        QIcon yellowIcon(yellowPix);

        QImage red(":/Elements/feu_rouge.png");
        QPixmap redPix;
        redPix = redPix.fromImage(red);
        QIcon redIcon(redPix);
        item->setFlags(Qt::ItemIsEditable|Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        if(sourceFile->listChars(font).count() == 0)
        {
            item->setIcon(redIcon);
            item->setCheckState(Qt::Unchecked);
            item->setFlags(Qt::ItemIsEnabled);
        }
        else if(destFonts.contains(font))
        {
            item->setIcon(yellowIcon);
            item->setCheckState(Qt::Unchecked);

        }
        else
        {
            item->setIcon(greenIcon);
            item->setCheckState(Qt::Checked);
        }


        item->setWhatsThis(font); //We use the "What's this?" property to set the former name of the font, as renaming is possible.
        //TODO: Find something else than the "What's this?" property in order to store the name, without creating a QListWidgetItem subclass...
        this->m_ui->fontList->addItem(item);

    }
}

fontMgmt::~fontMgmt()
{
    delete m_ui;
}

void fontMgmt::changeEvent(QEvent *e)
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

void fontMgmt::on_buttonBox_accepted()
{
    //foreach(QListWidgetItem *item, this->m_ui->fontList->item()
    //{
    QStringList destFonts = this->destFile->listFonts();
    for(int i = 0; i<this->m_ui->fontList->count(); i++)
    {
        QListWidgetItem *item = this->m_ui->fontList->item(i);
        if(item->checkState() == Qt::Checked)
        {
            QString font = item->text();
            QString formerName =  item->whatsThis();
            this->sourceFile->currentFont = formerName;

            if(destFonts.contains(font))
            {
                this->destFile->removeFont(font);
            }
            /*
            this->destFile->createFont(font);
            this->destFile->setFontX(font,this->sourceFile->readFontX(formerName));
            this->destFile->setFontY(font,this->sourceFile->readFontY(formerName));
            this->destFile->currentFont = font;
            QStringList availChars = this->sourceFile->listChars(formerName);

            foreach(QString character, availChars)
            {
                QString data = this->sourceFile->readChar(character);
                this->destFile->addChar(character);
                this->destFile->editCharData(character,data);
            }
            */

            if(!destFile->newFromNode(sourceFile->getfontNode(font)))
            {
                QMessageBox::information(this,tr("Error"),tr("Could not import fonts."));
            }
        }
    }

}


