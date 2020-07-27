#ifndef TTKPAINTAREA_H
#define TTKPAINTAREA_H

/* =================================================
 * This file is part of the TTKLinksGame project
 * Copyright (C) 2017 - 2020 Greedysky Studio

 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License along
 * with this program; If not, see <http://www.gnu.org/licenses/>.
 ================================================= */

#include <QWidget>

#define ROWCOUNT        14
#define COLCOUNT        20

class TTKPaintArea : public QWidget
{
    Q_OBJECT
public:
    explicit TTKPaintArea(QWidget *parent = nullptr);

protected Q_SLOTS:
    void nextPair();
    void newGame();
    void reset();

protected:
    virtual void paintEvent(QPaintEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;

private:
    bool checkLink(int x1, int y1, int x2, int y2);
    bool checkXLink(int x1, int y1, int x2, int y2);
    bool checkYLink(int x1, int y1, int x2, int y2);
    bool checkOneLink(int x1, int y1, int x2, int y2);
    bool checkTwoLink(int x1, int y1, int x2, int y2);
    bool checkWin();
    bool findPair();

    QSize m_size;
    QPoint m_selectPoint;
    QPoint m_startPoint;
    QPoint m_endPoint;

    QList<QPixmap> m_pixs;
    int m_data[ROWCOUNT][COLCOUNT];

};

#endif 
