#ifndef WINDOW_H
#define WINDOW_H

#include "settings.h"
#include "detector.h"
#include <QWidget>
#include <QPaintEvent>
#include <QPainter>
#include <QPixmap>
#include <QPen>
#include <QBasicTimer>
#include <QVector>
#include <QTemporaryFile>
#include <QProcess>
#include <QHash>
#include <QRect>
#include <QMainWindow>
#include <QStatusBar>
#include <QApplication>
#include <QtNetwork/QHostAddress>
#include <QDebug>
#include "ofqf/qoscclient.h"

class Window : public QWidget {
    Q_OBJECT

  public:
    Window(QWidget *);
    ~Window();
  private:
    enum Note { WHOLE, HALF, QUARTER, EIGHTH, SIXTEENTH, THIRTY_SECOND };
    struct NoteAndPosition {
        Note n;
        int pos;
    };
    struct Chord {
        QVector<NoteAndPosition> notes;
    };
    Settings *settings;
    QOscClient *oscClient;
    bool isDrawing;
    QPixmap *image;
    QPoint last;
    QPen pen;
    QBasicTimer *timer;
    NoteAndPosition getNote(QString, int);
    void clear();
    void setupImage();
    void setupTimer();
    void analyzeImage();
    void showMessage(QString);
    bool isChordValid(Chord, NoteAndPosition);
    QString getName(Note);
    QString getShortName(Note note);
    QString toString(Chord);
    int getMidiPos(int pos);
    void fileInit(QTemporaryFile &, QString);
    void fileCopy(QTemporaryFile &, QString, QString);
    void initPng(QTemporaryFile &);
    void initPng(QTemporaryFile &, cv::Mat);
    void initPy(QTemporaryFile &);
    void initXml(QTemporaryFile &);
    void drawLines(QPainter &, int, int);
    void drawStave(QPainter &);
    void drawNotes(QPainter &);
    void drawPoints(int, int, int, QPainter &);
    void paintEvent(QPaintEvent *);
    void resizeEvent(QResizeEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void timerEvent(QTimerEvent *);
    void keyPressEvent(QKeyEvent *);
public slots:
    void oscBack();
    void oscNext();
    void oscPrev();
    void oscNextMeasure();
    void oscPrevMeasure();
};

#endif // WINDOW_H
