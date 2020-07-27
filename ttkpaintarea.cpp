#include "ttkpaintarea.h"

#include <QPainter>
#include <QDateTime>
#include <QMouseEvent>
#include <QMessageBox>

#define FRONTWIDTH      35
#define FRONTHEIGHT     35
#define BKWIDTH         35
#define BKHEIGHT        35
#define INIT_STATE      -1
#define PHOTOCOUNT      6

TTKPaintArea::TTKPaintArea(QWidget *parent)
    : QWidget(parent)
{
    m_size.setWidth(COLCOUNT);
    m_size.setHeight(ROWCOUNT);

    m_selectPoint.setX(INIT_STATE);
    m_selectPoint.setY(INIT_STATE);

    m_startPoint.setX(INIT_STATE);
    m_startPoint.setY(INIT_STATE);

    m_endPoint.setX(INIT_STATE);
    m_endPoint.setY(INIT_STATE);

    for(int y=0; y<m_size.height(); y++)
    {
        for(int x=0; x<m_size.width(); x++)
        {
            m_data[y][x] = INIT_STATE;
        }
    }

    qsrand(QDateTime::currentMSecsSinceEpoch());
    setFixedSize(m_size.width() * FRONTWIDTH + 1, m_size.height() * FRONTHEIGHT + 1);
}

void TTKPaintArea::newGame()
{
    for(int y=0; y<m_size.height(); y++)
    {
        for(int x=0; x<m_size.width(); x++)
        {
            m_data[y][x] = INIT_STATE;
        }
    }

    QList<int> data;
    for(int i=0; i<((m_size.width() - 2) * (m_size.height() - 2)) / PHOTOCOUNT; i++)
    {
        QPixmap pixmap(":/data/images/" + QString::number(i, 10) + ".gif");
        m_pixs.append(pixmap);
        for(int j=0; j<PHOTOCOUNT; j++)
        {
            data.append(i);
        }
    }

    for(int y=0; y<m_size.height(); y++)
    {
        for(int x=0; x<m_size.width(); x++)
        {
            if(!(x == 0 || x == (m_size.width() - 1) || y== 0 || y== (m_size.height() - 1)))
            {
                const qint64 index = (qint64(qrand() * 0.1 + qrand() * 0.01 + qrand())) % data.size();
                m_data[y][x] = data.at(index);
                data.removeAt(index);
            }
        }
    }
    update();
}

void TTKPaintArea::nextPair()
{
    if(m_pixs.isEmpty())
    {
        newGame();
        return;
    }

    if(checkWin())
    {
        QMessageBox::information(this, "Congratulations", tr("you are win!"));
        newGame();
        return;
    }

    if(!findPair())
    {
        reset();
    }
    else
    {
        m_data[m_startPoint.y()][m_startPoint.x()] = INIT_STATE;
        m_data[m_endPoint.y()][m_endPoint.x()] = INIT_STATE;
        update();
    }
}

void TTKPaintArea::reset()
{
    QList<int> data;
    for(int i=0; i<m_size.height(); i++)
    {
        for(int j=0; j<m_size.width(); j++)
        {
            if(m_data[i][j] != INIT_STATE)
            {
                data.append(m_data[i][j]);
            }
        }
    }

    for(int i=0; i<m_size.height(); i++)
    {
        for(int j=0; j<m_size.width(); j++)
        {
            if(m_data[i][j] != INIT_STATE)
            {
                const int index = (int(rand() * 0.1 + rand() * 0.01 + rand())) % data.size();
                m_data[i][j] = data.at(index);
                data.removeAt(index);
            }
        }
    }
    update();
}

void TTKPaintArea::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setBrush(QBrush(Qt::white));
    painter.drawRect(0, 0, m_size.width() * FRONTWIDTH, m_size.height() * FRONTHEIGHT);

    for(int i=0; i<m_size.height(); i++)
    {
        for(int j=0; j<m_size.width(); j++)
        {
            if(m_data[i][j] == INIT_STATE)
            {
                continue;
            }

            painter.drawPixmap(j * FRONTWIDTH, i * FRONTHEIGHT, BKWIDTH, BKHEIGHT, m_pixs.at(m_data[i][j]));
            if(m_selectPoint.x() != INIT_STATE)
            {
                painter.setPen(QPen(Qt::red, 2));
                painter.drawLine(m_selectPoint.x() * FRONTWIDTH, m_selectPoint.y() * FRONTHEIGHT, m_selectPoint.x() * FRONTWIDTH, (m_selectPoint.y() + 1) * FRONTHEIGHT);
                painter.drawLine(m_selectPoint.x() * FRONTWIDTH, (m_selectPoint.y() + 1) * FRONTHEIGHT, (m_selectPoint.x() + 1) * FRONTWIDTH, (m_selectPoint.y() + 1) * FRONTHEIGHT);
                painter.drawLine((m_selectPoint.x() + 1) * FRONTWIDTH, (m_selectPoint.y() + 1) * FRONTHEIGHT, (m_selectPoint.x() + 1) * FRONTWIDTH, m_selectPoint.y() * FRONTHEIGHT);
                painter.drawLine((m_selectPoint.x() + 1) * FRONTWIDTH, m_selectPoint.y() * FRONTHEIGHT, m_selectPoint.x() * FRONTWIDTH, m_selectPoint.y() * FRONTHEIGHT);
            }
        }
    }
}

void TTKPaintArea::mousePressEvent(QMouseEvent *event)
{
    if(m_pixs.isEmpty())
    {
        newGame();
        return;
    }

    const int x = event->x() / FRONTWIDTH + (event->x() % FRONTWIDTH ? 1 : 0) - 1;
    const int y = event->y() / FRONTHEIGHT + (event->y() % FRONTHEIGHT ? 1 : 0) - 1;

    if(x < m_size.width() && y < m_size.height() && m_data[y][x] != INIT_STATE)
    {
        if(m_selectPoint.x() == INIT_STATE)
        {
            m_selectPoint.setX(x);
            m_selectPoint.setY(y);
        }
        else
        {
            if((m_selectPoint.x() != x || m_selectPoint.y() != y) && m_data[m_selectPoint.y()][m_selectPoint.x()] == m_data[y][x])
            {
                if(checkLink(m_selectPoint.x(), m_selectPoint.y(), x, y))
                {
                    m_data[m_selectPoint.y()][m_selectPoint.x()] = INIT_STATE;
                    m_data[y][x] = INIT_STATE;
                    m_selectPoint.setX(INIT_STATE);
                    m_selectPoint.setY(INIT_STATE);
                }
                else
                {
                    m_selectPoint.setX(x);
                    m_selectPoint.setY(y);
                }
            }
            else
            {
                m_selectPoint.setX(x);
                m_selectPoint.setY(y);
            }
        }
        update();
    }

    if(checkWin())
    {
        QMessageBox::information(this, "Congratulations", tr("you are win!"));
        newGame();
        update();
    }
    else
    {
        if(!findPair())
        {
            reset();
        }
    }
}

bool TTKPaintArea::checkLink(int x1, int y1, int x2, int y2)
{  
    if(checkXLink(x1, y1, x2, y2))
    {
        return true;
    }

    if(checkYLink(x1, y1, x2, y2))
    {
        return true;
    }

    if(checkOneLink(x1, y1, x2, y2))
    {
        return true;
    }
    else if(checkTwoLink(x1, y1, x2, y2))
    {
        return true;
    }
    return false;
}

bool TTKPaintArea::checkXLink(int x1, int y1, int x2, int y2)
{
    if(y1 != y2)
    {
        return false;
    }

    int direct = 1;
    if(x1 > x2)
    {
        direct = -1;
    }

    for(int x=x1 + direct; x!=x2 && x<m_size.width() && x>=0; x+=direct)
    {
        if(m_data[y1][x] != INIT_STATE)
        {
            return false;
        }
    }
    return true;
}

bool TTKPaintArea::checkYLink(int x1, int y1, int x2, int y2)
{
    if(x1 != x2)
    {
        return false;
    }

    int direct = 1;
    if(y1 > y2)
    {
        direct = -1;
    }

    for(int y=y1 + direct; y!=y2 && y<m_size.width() && y>=0; y+=direct)
    {
        if(m_data[y][x1] != INIT_STATE)
        {
            return false;
        }
    }
    return true;
}

bool TTKPaintArea::checkOneLink(int x1, int y1, int x2, int y2)
{
    const int xy = m_data[y1][x2];
    if(xy == INIT_STATE && checkXLink(x1, y1, x2, y1) && checkYLink(x2, y1, x2, y2))
    {
        return true;
    }

    const int yx = m_data[y2][x1];
    if(yx == INIT_STATE && checkYLink(x1, y1, x1, y2) && checkXLink(x1, y2, x2, y2))
    {
        return true;
    }
    return false;

}

bool TTKPaintArea::checkTwoLink(int x1, int y1, int x2, int y2)
{
    int xy = INIT_STATE;
    int yx = INIT_STATE;
    //up
    for(int y=y1-1; y>=0; y--)
    {
        xy = m_data[y][x1];
        yx = m_data[y][x2];

        if(xy != INIT_STATE)
        {
            break;
        }

        if(yx == INIT_STATE && checkXLink(x1, y, x2, y) && checkYLink(x2, y, x2, y2))
        {
            return true;
        }
    }
    //down
    for(int y=y1 + 1; y<m_size.height(); y++)
    {
        xy = m_data[y][x1];
        yx = m_data[y][x2];

        if(xy != INIT_STATE)
        {
            break;
        }

        if(yx == INIT_STATE && checkXLink(x1, y, x2, y) && checkYLink(x2, y, x2, y2))
        {
            return true;
        }
    }
    //left
    for(int x=x1 - 1; x>=0; x--)
    {
        yx = m_data[y1][x];
        xy = m_data[y2][x];

        if(yx != INIT_STATE)
        {
            break;
        }

        if(xy == INIT_STATE && checkYLink(x, y1, x, y2) && checkXLink(x, y2, x2, y2))
        {
            return true;
        }
    }
    //right
    for(int x=x1 + 1; x<m_size.width(); x++)
    {
        yx = m_data[y1][x];
        xy = m_data[y2][x];

        if(yx != INIT_STATE)
        {
            break;
        }

        if(xy == INIT_STATE && checkYLink(x, y1, x, y2) && checkXLink(x, y2, x2, y2))
        {
            return true;
        }
    }
    return false;
}

bool TTKPaintArea::checkWin()
{
    for(int i=0; i<m_size.height(); i++)
    {
        for(int j=0; j<m_size.width(); j++)
        {
            if(m_data[i][j] != INIT_STATE)
            {
                return false;
            }
        }
    }
    return true;
}

bool TTKPaintArea::findPair()
{
    for(int sy=1; sy<m_size.height() - 1; sy++)
    {
        for(int sx=1; sx<m_size.width() - 1; sx++)
        {
            if(m_data[sy][sx] == INIT_STATE)
            {
                continue;
            }

            for(int ey=sy; ey<m_size.height() - 1; ey++)
            {
                for(int ex=1; ex<m_size.width(); ex++)
                {
                    if(m_data[ey][ex] == INIT_STATE || (ey == sy && ex == sx))
                    {
                        continue;
                    }

                    if(m_data[sy][sx] == m_data[ey][ex])
                    {
                        if(checkLink(sx, sy, ex, ey))
                        {
                            m_startPoint.setX(sx);
                            m_startPoint.setY(sy);
                            m_endPoint.setX(ex);
                            m_endPoint.setY(ey);
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;
}
