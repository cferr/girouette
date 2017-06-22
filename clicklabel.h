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

#ifndef CLICKLABEL_H
#define CLICKLABEL_H
#include <QLabel>

class clickLabel : public QLabel
{
    Q_OBJECT
public:
    clickLabel( const QString & text, QWidget * parent = 0 );
    ~clickLabel(){}

signals:
    void clicked(QMouseEvent * event);
    void pressed(QMouseEvent * event);
    void released(QMouseEvent * event);
    void moved(QMouseEvent * event, bool changeMode = false);

protected:
    void mousePressEvent ( QMouseEvent * event) ;
    void mouseReleaseEvent(QMouseEvent * event);
    void mouseMoveEvent(QMouseEvent * event);

};


#endif // CLICKLABEL_H
