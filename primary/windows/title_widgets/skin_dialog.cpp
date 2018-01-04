#include "skin_dialog.h"
#include <QtWidgets>
#include "kskinbutton.h"
#include "kstatusbar.h"
#include <QDebug>

SkinDialog::SkinDialog(QWidget *parent) :
    KDialog(QString(), QMargins(), 0, KTitle::Normal, KTitle::Default, parent)
{
    setMinimumSize(800, 550);
    setWindowTitle(tr("Set skin"));

    // Initialize client area
    QStringList skins;
    skins << ":/images/skins/0_big" << ":/images/skins/1_big" << ":/images/skins/2_big"
          << ":/images/skins/3_big" << ":/images/skins/4_big" << ":/images/skins/5_big"
          << ":/images/skins/6_big" << ":/images/skins/7_big" << ":/images/skins/8_big"
          << ":/images/skins/9_big" << ":/images/skins/10_big" << ":/images/skins/11_big"
          << ":/images/skins/12_big" << ":/images/skins/13_big" << ":/images/skins/14_big"
          << ":/images/skins/15_big" << ":/images/skins/16_big" << ":/images/skins/17_big"
          << ":/images/skins/18_big" << ":/images/skins/19_big" << ":/images/skins/20_big"
          << ":/images/skins/21_big" << ":/images/skins/22_big" << ":/images/skins/23_big"
          << ":/images/skins/24"     << ":/images/skins/25";
    QStringList titles;
    titles << tr("Profound life")       << tr("Blue sea")       << tr("Red heart")      << tr("Lovely baby")
           << tr("Transparent water")   << tr("Flower")         << tr("Great sunshine") << tr("Shadow amazement")
           << tr("Green leaf")          << tr("360 pet")        << tr("Beautiful stone")<< tr("Yellow energy")
           << tr("Magic world")         << tr("Intense emotion")<< tr("Dream sky")      << tr("Angry bird")
           << tr("Graceful jazz")       << tr("Card")           << tr("Summer cool")    << tr("Blue world")
           << tr("Wood grain")          << tr("Pink mood")      << tr("Across time")    << tr("Six year")
           << tr("Android")             << tr("Elegant time");

    QWidget* clientWidget = new QWidget(this);
    clientWidget->setObjectName("ClientWidget");
    clientWidget->setStyleSheet("QWidget#ClientWidget { background-color: rgba(0,0,0,0);border: 0px; }");

    QGridLayout* gridLayoutClient = new QGridLayout(clientWidget);

    QProgressDialog progressDialog(tr("Loadding images, Please wait..."),
                                   tr("Cancel"), 0, skins.count() - 1, parent);
    progressDialog.setCancelButton(Q_NULLPTR);
    progressDialog.setAutoClose(false);
    progressDialog.setAutoReset(false);
    progressDialog.show();

    QSignalMapper* signalMapper = new QSignalMapper(this);
    for (int i = 0; i < skins.count(); i++) {
        // respond another message
        progressDialog.setLabelText(tr("Loadding image ") + skins[i]);
        progressDialog.setValue(i);
        QApplication::processEvents();

        KSkinButton* skinButton = new KSkinButton(titles[i], QPixmap(skins[i]), clientWidget);
        q_skinButtons.append(skinButton);
        gridLayoutClient->addWidget(skinButton, i / 5, i % 5);
        connect(skinButton, &KSkinButton::clicked, signalMapper,
                static_cast<void (QSignalMapper::*)()>(&QSignalMapper::map));
        signalMapper->setMapping(skinButton, skins[i]);
    }

    QScrollArea* scrollArea = new QScrollArea(this);
    scrollArea->setObjectName("scrollAreaClient");
    scrollArea->setStyleSheet("QScrollArea#scrollAreaClient { background-color: rgba(0,0,0,0);border: 0px; }");
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setWidget(clientWidget);
    clientWidget->adjustSize();

    QLayout *mainLayout = new QHBoxLayout(KDialog::clientWidget());
    mainLayout->addWidget(scrollArea);
    setCenter(QApplication::desktop());

    statusWidget()->setEnabled(true);

    ////////////////////////////////////////////////////////////////////////

    connect(signalMapper, static_cast<void (QSignalMapper::*)(const QString &)>(&QSignalMapper::mapped),
            this, &SkinDialog::selectionChanged);
    connect(this, &SkinDialog::selectionChanged, this, &KDialog::setSkin);

    ////////////////////////////////////////////////////////////////////////
}
