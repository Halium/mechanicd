#include <QCoreApplication>

#include "listeners/session.h"
#include "listeners/power.h"
#include "tuners/schedtune.h"

int main(int argc, char** argv)
{
    QCoreApplication app(argc, argv);

    SessionListener session;
    PowerListener power;

    return app.exec();
}
