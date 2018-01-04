#include "precomp.h"
#include "about_dialog.h"
#include "../../cniconsole/version.h"

AboutDialog::AboutDialog(QWidget *parent) :
    KDialog(QString(), QMargins(), 0, KTitle::Normal, KTitle::Default, parent)
{
    Q_ASSERT(parent);
#if (QT_VERSION < 0x050000)
    setWindowTitle(tr("About ") + qApp->property("displayName").toString());
#else
    setWindowTitle(tr("About ") + qApp->applicationDisplayName());
#endif
    setFixedSize(520, 300);

    //
    QVBoxLayout* infoLayout = new QVBoxLayout();
    infoLayout->setSpacing(5);
    infoLayout->setContentsMargins(0, 15, 0, 0);

    QLabel* titleInfoLabel = new QLabel(tr("CNI Console"), this);
#if (QT_VERSION < 0x050000)
    QLabel* infoLabel = new QLabel(qApp->property("displayName").toString(), this);
#else
    QLabel* infoLabel = new QLabel(qApp->applicationDisplayName(), this);
#endif
    QLabel* versionLabel = new QLabel(tr("Version %1")
                                .arg(qApp->applicationVersion()),this);
    QLabel* copyrightLabel = new QLabel(tr("Copyright (c) 2011-2014 CRDTech Inc."), this);
    QLabel* dateLabel = new QLabel(tr("Inner version"), this);

    titleInfoLabel->setStyleSheet("color:rgb(30,170,60);");
    infoLabel->setStyleSheet("color:rgb(30,170,60);");
    copyrightLabel->setStyleSheet("color:gray;");
    infoLabel->setContentsMargins(0, 0, 0, 40);
    infoLayout->addWidget(titleInfoLabel);
    infoLayout->addWidget(infoLabel);
    infoLayout->addStretch();
    infoLayout->addWidget(versionLabel);
    infoLayout->addWidget(dateLabel);
    infoLayout->addWidget(copyrightLabel);
    infoLayout->addStretch();

    QHBoxLayout *bottoMayout = new QHBoxLayout();
    bottoMayout->setSpacing(0);
    bottoMayout->setContentsMargins(0, 0, 30, 20);
    bottoMayout->addStretch();
    QPushButton* buttonOk = new QPushButton(tr("Ok"), this);
    buttonOk->setFixedSize(75, 25);
    buttonOk->setStyleSheet(QString("QPushButton { border:1px solid lightgray; background:rgb(230,230,230);}"
        "QPushButton:hover{border-color:green; background:transparent;} "));
    bottoMayout->addWidget(buttonOk);

    QHBoxLayout *horiLayout = new QHBoxLayout();
    horiLayout->setSpacing(0);
    horiLayout->setContentsMargins(20, 0, 20, 10);
    horiLayout->addLayout(infoLayout);
    //horiLayout->addStretch();
    QLabel* iconLabel = new QLabel(this);
    iconLabel->setPixmap(QPixmap(":/images/global/crdtech"));
    iconLabel->setFixedSize(200, 128);
    iconLabel->setScaledContents(true);
    horiLayout->addWidget(iconLabel);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setSpacing(0);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->addLayout(horiLayout);
    mainLayout->addLayout(bottoMayout);

    setLayout(mainLayout);

    QFont titleInfoFont("微软雅黑", 14, QFont::Bold, false);
    titleInfoLabel->setFont(titleInfoFont);

    QFont infoFont = infoLabel->font();
    infoFont.setBold(true);
    infoLabel->setFont(infoFont);

    /////////////////////////////////////////////////////////////////////////

    connect(buttonOk, SIGNAL(clicked()), this, SLOT(accept()));

    /////////////////////////////////////////////////////////////////////////
}
