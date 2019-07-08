#ifndef SETTINGS_H
#define SETTINGS_H

#include <QSettings>

class Settings {
private:
    void initialize(QString, int &);
    static const int NUM_LINES = 5; // liczba lini na pięciolnii
    int NUM_ADDITIONAL_LINES = 5; // liczba dodatkowych linii
    int POINT_SIZE = 5; // rozmiar pędzla
    int DISTANCE = 20; // odległość pomiędzy liniami na pięciolinii (parzysta, podzielna przez 4)
    int DELAY = 1000; // liczba milisekund do analizy symbolu
    int MESSAGE_TIMEOUT = 2000; // timeout pokazywania wiadomości
    int OSC_PORT = 5282;
public:
    static QSettings *s;
    Settings();
    int get(QString);
    void set(QString, int &);
};

#endif // SETTINGS_H
