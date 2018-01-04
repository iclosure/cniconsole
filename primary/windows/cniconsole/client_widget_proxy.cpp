#include "precomp.h"
#include "client_widget_proxy.h"
#include "toolbar_proxy.h"
#include "output_widget.h"
#include "modules/tacanes_widget.h"
#include "modules/tacanss_widget.h"
#include "modules/mwpdmd_widget.h"
#include "modules/instrument_widget.h"
#include "modules/tacanes_advance_widget.h"
#include "modules/tacanss_advance_widget.h"
#include "modules/mwpdmd_advance_widget.h"
#include "modules/instrument_advance_widget.h"
#include "modules/altimeter_widget.h"

ClientWidgetProxy::ClientWidgetProxy(QWidget *parent) :
    QWidget(parent)
{
    setObjectName("ClientWidgetProxy");

    ToolBarProxy* toolBarProxy = new ToolBarProxy(this);
    OutputWidget* outputWidget = new OutputWidget(this);

    QHBoxLayout *horiLayoutMain = new QHBoxLayout(parent);
    horiLayoutMain->setSpacing(0);
    horiLayoutMain->setContentsMargins(0, 0, 0, 0);
    horiLayoutMain->addWidget(toolBarProxy);
    horiLayoutMain->addWidget(outputWidget);

    //////////////////////////////////////////////////////////////////////////

    ClientWidgetProxy* _this = this;

    connect(toolBarProxy, &ToolBarProxy::clicked, this, [=](int id) {

        ModuleWidgetProxy** moduleGeneral = &q_moduleWidgetProxys[id];
        int oldSel = toolBarProxy->property("selected").toInt();
        if (oldSel == id) {  // select the same module
            qDebug() << QString("Select the same module");
            return;
        }

        toolBarProxy->setEnabled(false);

        bool justBack = toolBarProxy->property("justBack").toBool();

        switch (id) {
        case 0: *moduleGeneral = new TacanEsWidget(justBack, this); break;
        case 1: *moduleGeneral = new TacanSsWidget(justBack, this); break;
        case 2: *moduleGeneral = new MwPdmdWidget(justBack, this); break;
        case 3: *moduleGeneral = new InstrumentWidget(justBack, this); break;
        case 4: *moduleGeneral = new AltimeterWidget(justBack, this); break;
        default: Q_ASSERT(false); break;
        }
        toolBarProxy->setProperty("justBack", false);
        delete (*moduleGeneral)->findChild<QWidget*>("buttonBack");
        horiLayoutMain->insertWidget(1, *moduleGeneral);

        QObject::connect(*moduleGeneral, &ModuleWidgetProxy::selectAdvance, this, [=]() {

            toolBarProxy->setEnabled(false);
            (*moduleGeneral)->setEnabled(false);

            ModuleWidgetProxy** moduleAdvance = &q_moduleWidgetProxys[id + 4];

            switch (id) {
            case 0:
                TacanEsAdvanceWidget::q_package.RelatedPkg = TacanEsWidget::package();
                *moduleAdvance = new TacanEsAdvanceWidget(_this);
                break;
            case 1:
                TacanSsAdvanceWidget::q_package.RelatedPkg = TacanSsWidget::package();
                *moduleAdvance = new TacanSsAdvanceWidget(_this);
                break;
            case 2:
                MwPdmdAdvanceWidget::q_package.RelatedPkg = MwPdmdWidget::package();
                *moduleAdvance = new MwPdmdAdvanceWidget(_this);
                break;
            case 3:
                InstrumentAdvanceWidget::q_package.RelatedPkg = InstrumentWidget::package();
                *moduleAdvance = new InstrumentAdvanceWidget(_this);
                break;
            case 4:
                Q_ASSERT(false);
                break;
            default: Q_ASSERT(false); break;
            }
            delete (*moduleAdvance)->findChild<QWidget*>("buttonAdvance");
            delete (*moduleAdvance)->findChild<QWidget*>("buttonReset");
            horiLayoutMain->insertWidget(1, *moduleAdvance);
            toolBarProxy->setProperty("selected", id + 4);

            // delete old module
            delete *moduleGeneral;
            q_moduleWidgetProxys.remove(id);

            ////////////////////////////////////////////////////////////////////

            QObject::connect((*moduleAdvance)->findChild<QPushButton*>("buttonBack"), &QPushButton::clicked, _this, [=]() {
                toolBarProxy->setProperty("justBack", true);
                emit toolBarProxy->clicked(id);
            });

            ////////////////////////////////////////////////////////////////////

            toolBarProxy->setEnabled(true);

            qDebug() << QString("current selected advance module: %1").arg((*moduleAdvance)->windowTitle());

        });
        toolBarProxy->setProperty("selected", id);
        qDebug() << QString("current selected module: %1").arg((*moduleGeneral)->windowTitle());

        // delete old module
        if (oldSel >= 0) {
            delete q_moduleWidgetProxys[oldSel];
            q_moduleWidgetProxys.remove(oldSel);
        }

        toolBarProxy->setEnabled(true);

    });
    //////////////////////////////////////////////////////////////////////////

    // Initialize

    toolBarProxy->setProperty("selected", -1);
    emit toolBarProxy->clicked(0);
}
