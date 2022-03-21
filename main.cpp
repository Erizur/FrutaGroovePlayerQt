#include "musicplayer.h"
#include "windowhandler.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "FrutaGroovePlayerQt_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }
    MusicPlayer *musPlayer = new MusicPlayer;
    WindowHandler winHandler;
    winHandler.setAttribute(Qt::WA_QuitOnClose, true);
    winHandler.setTitle(musPlayer->windowTitle());
    winHandler.setIcon(musPlayer->windowIcon());

    winHandler.setContent(musPlayer);
    winHandler.setSize(musPlayer->size());
    winHandler.activateWindow();
    winHandler.show();
    return a.exec();
}
