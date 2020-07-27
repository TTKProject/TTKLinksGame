#include "ttkapplication.h"
#include "ttkpaintarea.h"

#include <QMenuBar>
#include <QBoxLayout>

TTKApplication::TTKApplication(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("TTKApplication");

    m_mainWidget = new QWidget(this);
    m_area = new TTKPaintArea(m_mainWidget);

    QHBoxLayout *hLayout = new QHBoxLayout(m_mainWidget);
    hLayout->setSpacing(20);
    hLayout->addWidget(m_area);
    m_mainWidget->setLayout(hLayout);

    QPalette p = palette();
    p.setColor(QPalette::Window, Qt::white);
    setPalette(p);

    setCentralWidget(m_mainWidget);

    initialize();
}

TTKApplication::~TTKApplication()
{
    delete m_area;
}

void TTKApplication::initialize()
{
    // exit Action
    QAction *exitAction = new QAction(QIcon(":/data/images/openfile.png"), tr("Exit"), this);
    exitAction->setShortcut(tr("Ctrl+X"));
    exitAction->setStatusTip(tr("Exit"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
    //newgame Action
    QAction *newAction = new QAction(QIcon(":/data/images/openfile.png"), tr("New"), this);
    newAction->setShortcut(tr("Ctrl+N"));
    newAction->setStatusTip(tr("New"));
    connect(newAction, SIGNAL(triggered()), m_area, SLOT(newGame()));
    //nextpair Action
    QAction *nextAction = new QAction(QIcon(":/data/images/openfile.png"), tr("Next"), this);
    nextAction->setShortcut(tr("Ctrl+D"));
    connect(nextAction, SIGNAL(triggered()), m_area, SLOT(nextPair()));
    //reset Action
    QAction *resetAction = new QAction(QIcon(":/data/images/openfile.png"), tr("Reset"), this);
    connect(resetAction, SIGNAL(triggered()), m_area, SLOT(reset()));

    QMenu *fileMenu = menuBar()->addMenu(tr("File"));
    QMenu *toolMenu = menuBar()->addMenu(tr("Tool"));

    fileMenu->addAction(newAction);
    fileMenu->addAction(exitAction);
    toolMenu->addAction(nextAction);
    toolMenu->addAction(resetAction);
}
