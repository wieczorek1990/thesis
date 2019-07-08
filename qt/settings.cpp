#include "settings.h"

void Settings::initialize(QString key, int &value) {
    if (!s->contains(key)) {
        s->setValue(key, value);
    }
}

Settings::Settings() {
    s->setValue("num_lines", NUM_LINES);
    initialize("num_additional_lines", NUM_ADDITIONAL_LINES);
    initialize("point_size", POINT_SIZE);
    initialize("distance", DISTANCE);
    initialize("delay", DELAY);
    initialize("message_timeout", MESSAGE_TIMEOUT);
    initialize("osc_port", OSC_PORT);
}
int Settings::get(QString key) {
    return s->value(key).toInt();
}
void Settings::set(QString key, int &value) {
    s->setValue(key, value);
}

QSettings *Settings::s = new QSettings("luke", "qt");
