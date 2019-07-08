#include "window.h"

Window::Window(QWidget * parent) :
    QWidget(parent) {
    setFocusPolicy(Qt::StrongFocus);
    settings = new Settings;
    oscClient = new QOscClient(QHostAddress::LocalHost, settings->get("osc_port"), qApp);
    isDrawing = false;
    pen.setWidth(settings->get("point_size"));
    timer = new QBasicTimer;
    setupImage();
    clear();
}

Window::~Window() {
    delete timer;
}

Window::NoteAndPosition Window::getNote(QString name, int y) {
    Note n;
    int y0 = y - this->height() / 2;
    int sign = y0 > 0 ? 1 : -1;
    int pos = (y0 + sign * settings->get("distance") / 4) / (settings->get("distance") / 2);
    if (name == "whole") {
        n = WHOLE;
    } else if (name == "half") {
        n = HALF;
    } else if (name == "quarter") {
        n = QUARTER;
    } else if (name == "eighth") {
        n = EIGHTH;
    } else if (name == "sixteenth") {
        n = SIXTEENTH;
    } else if (name == "thirty-second") {
        n = THIRTY_SECOND;
    }
    return NoteAndPosition{n, pos};
}

void Window::clear() {
    QPainter painter(image);
    painter.fillRect(0, 0, this->width(), this->height(), Qt::white);
}

void Window::showMessage(QString message) {
    QMainWindow *mw = qobject_cast<QMainWindow *>(parent()->parent());
    mw->statusBar()->showMessage(message, settings->get("message_timeout"));
}

void Window::setupImage() {
    image = new QPixmap(this->width(), this->height());
}

void Window::setupTimer() {
    delete timer;
    timer = new QBasicTimer;
}

void Window::fileInit(QTemporaryFile &file, QString extension) {
    file.open();
    file.close();
    QString path = file.fileName() + "." + extension;
    QFile::remove(path);
    file.rename(path);
    QFile::remove(file.fileName());
}

void Window::fileCopy(QTemporaryFile &file, QString sourcePath, QString extension) {
    fileInit(file, extension);
    QFile source(sourcePath);
    source.copy(file.fileName());
}

void Window::initPy(QTemporaryFile &py) {
    fileCopy(py, ":/qt/classifier.py", "py");
}

void Window::initPng(QTemporaryFile &img) {
    fileInit(img, "png");
    image->save(img.fileName(), "png");
}

void Window::initPng(QTemporaryFile &img, cv::Mat m) {
    fileInit(img, "png");
    imwrite(img.fileName().toStdString(), m);
}

void Window::initXml(QTemporaryFile &xml) {
    fileCopy(xml, ":/qt/classifier.xml", "xml");
}

bool Window::isChordValid(Chord chord, NoteAndPosition note) {
    bool result = true;
    foreach (NoteAndPosition n, chord.notes) {
        if (n.n != note.n) {
            result = false;
        }
    }
    return result;
}

QString Window::getName(Note note) {
    switch (note) {
        case WHOLE:
            return "whole";
        case HALF:
            return "half";
        case QUARTER:
            return "quarter";
        case EIGHTH:
            return "eighth";
        case SIXTEENTH:
            return "sixteenth";
        case THIRTY_SECOND:
            return "thirty-second";
        default:
            return "unknown";
    }
}

QString Window::getShortName(Note note) {
    switch (note) {
        case WHOLE:
            return "1";
        case HALF:
            return "2";
        case QUARTER:
            return "4";
        case EIGHTH:
            return "8";
        case SIXTEENTH:
            return "16";
        case THIRTY_SECOND:
            return "32";
        default:
            return "unknown";
    }
}

QString Window::toString(Chord chord) {
    QString result = "";
    foreach (NoteAndPosition note, chord.notes) {
        result += "(" + getName(note.n) + ", " + QString::number(note.pos) + ")\n";
    }
    return result;
}

void Window::analyzeImage() {
    QTemporaryFile img, py, xml;
    initPy(py);
    initXml(xml);
    initPng(img);
    QVector<Mat> parts = Detector::parts(img.fileName());
    Chord chord;
    QVector<QString> names;
    QVector<QTemporaryFile*> images;
    for (int i = 0; i < parts.size(); i++) {
        images << new QTemporaryFile;
    }
    for (int i = 0; i < parts.size(); i++) {
        Mat m = parts.at(i);
        QTemporaryFile *img = images.at(i);
        initPng(*img, m);
        QProcess p;
        QString command = "python " + py.fileName() + " " + img->fileName() + " " + xml.fileName();
        p.start(command);
        p.waitForFinished();
        QString name = p.readAll().trimmed();
        names << name;
        if (name == "error") {
            showMessage("Classifier returned error");
            return;
        }
        QString errors = p.readAllStandardError();
        if (errors.size() != 0) {
            qDebug() << errors;
            return;
        }
    }
    for (int i = 0; i < parts.size(); i++) {
        QTemporaryFile *img = images.at(i);
        QString name = names.at(i);
        Detector d(img->fileName(), name);
        QVector<QPoint> centers = d.getCenters();
        if (centers.size() == 1) {
            NoteAndPosition note = getNote(name, centers.first().y());
            if (!isChordValid(chord, note)) {
                showMessage("Chord is invalid");
                return;
            }
            chord.notes << note;
        } else {
            showMessage("Too many centers detected");
            return;
        }
    }
    if (chord.notes.size() != 0) {
        QString action = "/actions/pad-note-" + getShortName(chord.notes.first().n);
        oscClient->sendData(action);
        for (int i = 0; i < chord.notes.size(); i++) {
            NoteAndPosition note = chord.notes.at(i);
            oscClient->sendData("/addpitch", getMidiPos(note.pos));
        }
        oscNext();
        showMessage(toString(chord));
    }
}

int Window::getMidiPos(int pos) {
    static int spacing_forward[7] = {1, 2, 2, 1, 2, 2, 2};
    static int spacing_backward[7] = {2, 2, 2, 1, 2, 2, 1};
    int *spacing, p, sign;
    if (pos > 0) {
        spacing = spacing_backward;
        p = pos;
        sign = -1;
    } else if (pos < 0){
        spacing = spacing_forward;
        p = -pos;
        sign = 1;
    } else {
        return 71;
    }
    int result = 0, i = 0;
    while (p > 0) {
        result += spacing[i % 8];
        p--;
        i++;
    }
    return 71 + sign * result;
}

void Window::drawLines(QPainter &painter, int start, int end) {
    int w = this->width();
    int h = this->height();
    int h2 = h / 2;
    for (int i = start; i < end + 1; i++) {
        int dy = i * settings->get("distance");
        painter.drawLine(0, h2 + dy, w, h2 + dy);
    }
}

void Window::drawStave(QPainter &painter) {
    int NUM_LINES = settings->get("num_lines");
    int NUM_ADDITIONAL_LINES = settings->get("num_additional_lines");
    drawLines(painter, -NUM_LINES / 2, NUM_LINES / 2);
    QPen pen, oldPen;
    oldPen = painter.pen();
    pen.setStyle(Qt::DotLine);
    painter.setPen(pen);
    drawLines(painter, -NUM_LINES / 2 - NUM_ADDITIONAL_LINES, -NUM_LINES / 2);
    drawLines(painter, NUM_LINES / 2 , NUM_LINES / 2 + NUM_ADDITIONAL_LINES);
    painter.setPen(oldPen);
}

void Window::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.drawPixmap(0, 0, *image);
    drawStave(painter);
}

void Window::mousePressEvent(QMouseEvent *event) {
    isDrawing = true;
    last = event->pos();
    timer->stop();
}

void Window::mouseReleaseEvent(QMouseEvent *event) {
    isDrawing = false;
    last = event->pos();
    if (!timer->isActive()) {
        setupTimer();
    }
    timer->start(settings->get("delay"), this);
}

void Window::mouseMoveEvent(QMouseEvent *event) {
    if (isDrawing) {
        QPainter painter(image);
        painter.setPen(pen);
        painter.drawLine(last, event->pos());
        update();
    }
    last = event->pos();
}

void Window::resizeEvent(QResizeEvent *) {
    QPixmap pixmap = image->copy();
    setupImage();
    clear();
    QPainter painter(image);
    painter.drawPixmap(0, 0, pixmap);
}

void Window::timerEvent(QTimerEvent *) {
    setupTimer();
    try {
        analyzeImage();
    } catch(cv::Exception &e) {
        qDebug() << e.what();
    }
    setupImage();
    clear();
    update();
}

void Window::oscBack() {
    oscClient->sendData("/actions/backspace", QVariant::Invalid);
}

void Window::oscNext() {
    oscClient->sendData("/actions/next-chord", QVariant::Invalid);
}

void Window::oscPrev() {
    oscClient->sendData("/actions/prev-chord", QVariant::Invalid);
}

void Window::oscNextMeasure() {
    oscClient->sendData("/actions/next-measure", QVariant::Invalid);
}

void Window::oscPrevMeasure() {
    oscClient->sendData("/actions/prev-measure", QVariant::Invalid);
}

void Window::keyPressEvent(QKeyEvent *event) {
    if (event->key() == Qt::Key_Backspace) {
        oscBack();
    }
    if (event->key() == Qt::Key_Left) {
        if (event->modifiers().testFlag(Qt::ControlModifier)) {
            oscPrevMeasure();
        } else {
            oscPrev();
        }
    }
    if (event->key() == Qt::Key_Right) {
        if (event->modifiers().testFlag(Qt::ControlModifier)) {
            oscNextMeasure();
        } else {
            oscNext();
        }
    }
    if (event->key() == Qt::Key_Space) {
        timerEvent(0);
    }
}
