#include "precomp.h"
#include "main_widget.h"
#include "cniconsole/version.h"
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // load font
    /*
    int index = QFontDatabase::addApplicationFont(app.applicationDirPath() + "/font/MyYahei/msyh.ttf");
    index = QFontDatabase::addApplicationFont(app.applicationDirPath() + "/font/MyYahei/msyhbd.ttf");
    */
    QFile file(":/qss/stylesheet-1");
    file.open(QFile::ReadOnly);
    app.setStyleSheet(file.readAll());
    app.setFont(QFont("微软雅黑", 10));

    //
    KwtCore kwtCore;
    bool result = kwtCore.load(QLocale::system().name());
    QTranslator translator;
    result = translator.load(":/lang/" + QString("zh_CN"));
    app.installTranslator(&translator);

    //
    app.setOrganizationDomain("crdtech.cn");
    app.setOrganizationName("CRDTech");
    app.setApplicationName("CniConsole");
#if (QT_VERSION < 0x050000)
    app.setProperty("displayName", QObject::tr("CNI System Test"));
#else
    app.setApplicationDisplayName(QObject::tr("CNI System Test"));
#endif
    app.setApplicationVersion(QString("%1.%2.%3")
                                   .arg(ME_MAJOR_VERSION)
                                   .arg(ME_MINOR_VERSION)
                                   .arg(ME_PATCH_VERSION));
    //
    MainWidget mainWidget;
    mainWidget.show();

    return app.exec();
}
