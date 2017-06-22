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

#include "clicklabel.h"


clickLabel::clickLabel( const QString & text, QWidget * parent )
        :QLabel(parent)
{
    this->setText(text);
    //connect( this, SIGNAL( clicked() ), this, SLOT( slotClicked() ) );
}



void clickLabel::mousePressEvent ( QMouseEvent * event )
{
    emit pressed(event);
    //emit clicked(event);
}

void clickLabel::mouseReleaseEvent(QMouseEvent * event)
{
    emit released(event);
}

void clickLabel::mouseMoveEvent(QMouseEvent * event)
{
    emit moved(event,false);
}
