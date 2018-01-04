#include "precomp.h"
#include "cni_xml_parse.h"

CniXmlParse::CniXmlParse(QObject *parent) :
    QObject(parent)
{
    setObjectName("CniXmlParse");
}

CniXmlParse::~CniXmlParse()
{
}

bool CniXmlParse::parse(const QString & path, CNI_PACKAGE_INFO *package)
{
    if (path.isEmpty()) {
        qDebug() << QString("path can not be empty");
        Q_ASSERT(false);
        return false;
    }

    if (package->Initialized) {
        return true;
    }

    QString fileName(QApplication::applicationDirPath() + CNI_DATA_PATH + "cni_data_defaults.xml");
    TiXmlDocument doc(fileName.toLocal8Bit().data());

    if(!doc.LoadFile(TIXML_ENCODING_UTF8))
    {
        QMessageBox::critical(Q_NULLPTR, tr("Open CNI config file"),
                              tr("Open file %1 failed! \n\n"
                                 "error information:\n\n"
                                 "errorId: %2\n"
                                 "errorDesc: %3 \n\n"
                                 "coordinate: (%4,%5)")
                              .arg(fileName).arg(doc.ErrorId()).arg(doc.ErrorDesc())
                              .arg(doc.ErrorRow()).arg(doc.ErrorCol()),
                              QMessageBox::Ok);
        exit(-1);
        return false;
    }

    TiXmlElement* root = doc.RootElement();
    if (!root) {
        QMessageBox::critical(Q_NULLPTR, tr("Open CNI config file"),
                              tr("root node is not found in file %1").arg(fileName),
                              QMessageBox::Ok);
        exit(-1);
        return false;
    }

    QStringList paths = path.split('/', QString::SkipEmptyParts);
    if (paths.isEmpty()) {
        qDebug() << QString("have no '/' in path '[%1]'").arg(path);
        Q_ASSERT(false);
        exit(-1);
        return false;
    }

    if (paths.count() < 2) {
        qDebug() << QString("the size of paths '[%1]' can't be less than 2").arg(paths.count());
        Q_ASSERT(false);
        exit(-1);
        return false;
    }

    // load protocol
    TiXmlElement* protocol = root->FirstChildElement(paths[0].toStdString());
    if (!protocol) {
        QMessageBox::critical(Q_NULLPTR, tr("Parse CNI config file"),
                              tr("protocol '%1' is not found in file %2").arg(paths[0]).arg(fileName),
                              QMessageBox::Ok);
        exit(-1);
        return false;
    }

    if (paths[0] == "network") {

        // load module
        TiXmlElement* module = protocol->FirstChildElement(paths[1].toStdString());
        if (!module) {
            QMessageBox::critical(Q_NULLPTR, tr("Parse CNI config file"),
                                  tr("module '%1' is not found in file %2").arg(paths[1]).arg(fileName),
                                  QMessageBox::Ok);
            exit(-1);
            return false;
        }

        if (paths.count() == 3) {

            Q_ASSERT(package->inherits("CNI_NETWORK_CONTROL"));

            package->setObjectName(QString::fromUtf8(module->Attribute("name")));
            package->Header[0] = QString(module->Attribute("header0")).toInt(Q_NULLPTR, 16);
            package->Header[1] = QString(module->Attribute("header1")).toInt(Q_NULLPTR, 16);
            package->Header[2] = QString(module->Attribute("header2")).toInt(Q_NULLPTR, 16);
            package->DeviceId = QString(module->Attribute("deviceid")).toInt(Q_NULLPTR, 16);

            // load submodule
            TiXmlElement* submodule = module->FirstChildElement(paths[2].toStdString());
            if (!submodule) {
                QMessageBox::critical(Q_NULLPTR, tr("Parse CNI config file"),
                                      tr("submodule '%1' is not found in file %2").arg(paths[2]).arg(fileName),
                                      QMessageBox::Ok);
                exit(-1);
                return false;
            }

            if (!parse(submodule, qobject_cast<CNI_NETWORK_CONTROL*>(package))) {
                return false;
            }

        } else if (paths[1] == "monitor") {

            Q_ASSERT(package->inherits("CNI_NETWORK_MONITOR"));

            package->setObjectName(QString::fromUtf8(module->Attribute("name")));
            qobject_cast<CNI_NETWORK_MONITOR*>(package)->frame.header = QString(module->Attribute("header")).toInt(Q_NULLPTR, 16);

            if (!parse(module, qobject_cast<CNI_NETWORK_MONITOR*>(package))) {
                return false;
            }

        } else {
            QMessageBox::critical(Q_NULLPTR, tr("Parse CNI config file"),
                                  tr("not supported path '%1'").arg(path), QMessageBox::Ok);
            exit(-1);
            return false;
        }

    } else if (paths[0] == "serialport") {

        Q_ASSERT(package->inherits("CNI_ALT_PACKAGE_INFO"));

        // load module
        TiXmlElement* module = protocol->FirstChildElement(paths[1].toStdString());
        if (!module) {
            QMessageBox::critical(Q_NULLPTR, tr("Parse CNI config file"),
                                  tr("module '%1' is not found in file %2").arg(paths[1]).arg(fileName),
                    QMessageBox::Ok);
            exit(-1);
            return false;
        }

        package->setObjectName(QString::fromUtf8(module->Attribute("name")));

        if (!parse(module, qobject_cast<CNI_ALT_PACKAGE_INFO*>(package))) {
            return false;
        }

    } else {
        QMessageBox::critical(Q_NULLPTR, tr("Parse CNI config file"),
                              tr("not supported protocol '%1'").arg(paths[0]), QMessageBox::Ok);
        exit(-1);
        return false;
    }

    package->Initialized = true;
    package->Changed = false;

    return true;
}

bool CniXmlParse::parse(const TiXmlElement *node, CNI_ITEM_INFO &item, CNI_ITEM_ENUM_INFO &itemEnum)
{
    itemEnum.Offset = QString(node->Attribute("offset")).toInt();
    itemEnum.Width = QString(node->Attribute("width")).toInt();

    // load option
    const TiXmlElement* option = node->FirstChildElement("option");
    if (!option) {
        QMessageBox::critical(Q_NULLPTR, tr("Parse CNI config file"),
                              tr("node '%1/option' is not found").arg(CniXmlParse::elementPath(node)),
                              QMessageBox::Ok);
        return false;
    }
    itemEnum.Default = QString::fromUtf8(option->Attribute("default"));
    itemEnum.CurrentValue = itemEnum.Default;
    const TiXmlElement* elementOption;
    for (elementOption = option->FirstChildElement();
         elementOption != NULL;
         elementOption = elementOption->NextSiblingElement())
    {
        CNI_ITEM_ENUM_ITEM_INFO itemOption =  { Q_NULLPTR };
        itemOption.Text = QString::fromUtf8(elementOption->GetText());
        itemOption.Encode = elementOption->Attribute("encode") ?
                    QString::fromUtf8(elementOption->Attribute("encode")).toULong(Q_NULLPTR, 16) :
                    item.ItemEnum.Options.count();
        itemEnum.Options << itemOption;
        if (itemOption.Text == itemEnum.CurrentValue) {
            item.saveData(itemOption.Encode);
        }
    }

    return true;
}

bool CniXmlParse::parse(const TiXmlElement *node, CNI_ITEM_NUMERIC_INFO &itemNumeric)
{
    // load numeric
    const TiXmlElement* numeric = node->FirstChildElement("numeric");
    if (!numeric) {
        QMessageBox::critical(Q_NULLPTR, tr("Parse CNI config file"),
                              tr("node '%1/numeric' is not found").arg(CniXmlParse::elementPath(node)),
                              QMessageBox::Ok);
        return false;
    }
    itemNumeric.Unit = QString::fromUtf8(numeric->Attribute("unit"));
    itemNumeric.Offset = QString::fromUtf8(numeric->Attribute("offset")).toInt();
    itemNumeric.Related = QString::fromUtf8(numeric->Attribute("related"));
    const TiXmlElement* elementNumeric;
    for (elementNumeric = numeric->FirstChildElement();
         elementNumeric != NULL;
         elementNumeric = elementNumeric->NextSiblingElement())
    {
        CNI_ITEM_NUMERIC_FORMAT itemFormat = { Q_NULLPTR };
        itemFormat.Name = QString::fromUtf8(elementNumeric->Attribute("name"));
        if (itemNumeric.Unit.isEmpty()) {  // is an ascii
            itemFormat.Default = QString(elementNumeric->Attribute("default"))[0].toLatin1();
        } else {
            itemFormat.Default = QString(elementNumeric->Attribute("default")).toDouble();
        }
        itemFormat.CurrentValue = itemFormat.Default;
        itemFormat.Decimals = QString(elementNumeric->Attribute("decimals")).toInt();
        itemFormat.Ratio = QString(elementNumeric->Attribute("ratio")).toDouble();
        itemFormat.Step = QString(elementNumeric->Attribute("step")).toDouble();
        itemFormat.Offset = QString(elementNumeric->Attribute("offset")).toDouble();
        if (elementNumeric->Attribute("default")) {
            if (!HR_NUMERIC_RANGE::parseRange(itemFormat.Range, elementNumeric->Attribute("range"), !itemNumeric.Unit.isEmpty())) {
                QMessageBox::critical(Q_NULLPTR,tr("Parse CNI config file"),tr("parse range failed '%2' in '%1/item.range'")
                                      .arg(CniXmlParse::elementPath(elementNumeric)).arg(elementNumeric->Attribute("range")),
                                      QMessageBox::Ok);
                return false;
            }
            if (!itemFormat.inRange(itemFormat.Default)) {
                QMessageBox::critical(Q_NULLPTR, tr("Parse CNI config file"),
                                      tr("Default value '%2' (Range: '%3') is invalid '%1/numeric'")
                                      .arg(CniXmlParse::elementPath(elementNumeric))
                                      .arg(elementNumeric->Attribute("default"))
                                      .arg(itemFormat.Range.toString()),
                                      QMessageBox::Ok);
                return false;
            }
            const TiXmlElement* elementWithout;
            for (elementWithout = elementNumeric->FirstChildElement();
                 elementWithout != NULL;
                 elementWithout = elementWithout->NextSiblingElement())
            {
                HR_NUMERIC_RANGE itemWithout;
                if (!HR_NUMERIC_RANGE::parseRange(itemWithout, elementWithout->GetText())) {
                    QMessageBox::critical(Q_NULLPTR, tr("Parse CNI config file"), tr("parse range '%2' failed in '%1/without->text'")
                                          .arg(CniXmlParse::elementPath(elementWithout)).arg(elementWithout->GetText()),
                                          QMessageBox::Ok);
                    return false;
                }
                itemFormat.Withouts << itemWithout;
            }
        }
        itemNumeric.Formats << itemFormat;
    }

    return true;
}

bool CniXmlParse::parse(const TiXmlElement *node, CNI_ITEM_BUTTON_INFO &itemButton)
{
    itemButton.Default = (QString::fromUtf8(node->Attribute("default"))
                               .toLower() == "true") ? true : false;
    return true;
}

bool CniXmlParse::parse(const TiXmlElement *node, QVector<CNI_ITEM_INFO> &items)
{
    const TiXmlElement* element;
    for (element = node->FirstChildElement();
         element != Q_NULLPTR;
         element = element->NextSiblingElement())
    {
        CNI_ITEM_INFO item = { Q_NULLPTR };
        item.Name = QString::fromUtf8(element->Attribute("name"));
        item.Function = QString(element->Attribute("function")).toInt();
        item.Type = QString::fromUtf8(element->Attribute("type"));
        QString checkable = QString::fromUtf8(element->Attribute("checkable"));
        item.Checkable = (checkable.isEmpty() || checkable.toLower() == "true") ? true : false;
        QString editable = QString::fromUtf8(element->Attribute("editable"));
        item.Editable = (editable.isEmpty() || editable.toLower() == "true") ? true : false;
        item.Checked = true;
        if (item.Type == "enum") {
            if (!parse(element, item, item.ItemEnum)) {
                return false;
            }
        } else if (item.Type == "numeric") {
            if (!parse(element, item.ItemNumeric)) {
                return false;
            }
            item.saveData(item.ItemNumeric.currentFormat().CurrentValue);
        } else if (item.Type == "button") {
            if (!parse(element, item.ItemButton)) {
                return false;
            }
            item.ItemButton.CurrentValue = item.ItemButton.Default;
        } else {
            QMessageBox::critical(Q_NULLPTR, tr("Parse CNI config file"), tr("Unsupported type '%1'").arg(item.Type), QMessageBox::Ok);
            return false;
        }
        items << item;
    }

    return true;
}

bool CniXmlParse::parse(const TiXmlElement *node, CNI_NETWORK_CONTROL *package)
{
    if (!parse(node, package->items)) {
        return false;
    }

    for (int i = 1; i < package->items.count(); i++) {
        package->items[i - 1].Next = &package->items[i];
    }

    return true;
}

bool CniXmlParse::parse(const TiXmlElement *node, CNI_NETWORK_MONITOR *package)
{
    package->frame.tail = QString(node->Attribute("tail")).toInt(Q_NULLPTR, 16);

    if (!parse(node, package->items)) {
        return false;
    }

    return true;
}

bool CniXmlParse::parse(const TiXmlElement *node, CNI_ALT_PACKAGE_INFO *package)
{
    const TiXmlElement* elementFrame;

    //
    // frame-workMode
    //

    elementFrame = node->FirstChildElement();

    package->workMode.name = (QString::fromUtf8(elementFrame->Attribute("name")));
    package->workMode.frame.header = QString(elementFrame->Attribute("header")).toInt(Q_NULLPTR, 16);
    package->workMode.frame.id = QString(elementFrame->Attribute("id")).toInt(Q_NULLPTR, 16);
    package->workMode.frame.length = QString(elementFrame->Attribute("length")).toInt();
    package->workMode.frame.function = QString(elementFrame->Attribute("function")).toInt();
    package->workMode.frame.tail = QString(elementFrame->Attribute("tail")).toInt(Q_NULLPTR, 16);

    if (!parse(elementFrame, package->workMode.items)) {
        return false;
    }

    //
    // frame-attenuate
    //

    elementFrame = elementFrame->NextSiblingElement();

    package->attenuate.name = (QString::fromUtf8(elementFrame->Attribute("name")));
    package->attenuate.frame.header = QString(elementFrame->Attribute("header")).toInt(Q_NULLPTR, 16);
    package->attenuate.frame.id = QString(elementFrame->Attribute("id")).toInt(Q_NULLPTR, 16);
    package->attenuate.frame.length = QString(elementFrame->Attribute("length")).toInt();
    package->attenuate.frame.crc16 = QString(elementFrame->Attribute("crc16")).toInt();
    package->attenuate.frame.tail = QString(elementFrame->Attribute("tail")).toInt(Q_NULLPTR, 16);

    if (!parse(elementFrame, package->attenuate.items)) {
        return false;
    }

    //
    // frame-busAck
    //

    elementFrame = elementFrame->NextSiblingElement();

    package->busAck.name = (QString::fromUtf8(elementFrame->Attribute("name")));
    package->busAck.frame.header = QString(elementFrame->Attribute("header")).toInt(Q_NULLPTR, 16);
    package->busAck.frame.id = QString(elementFrame->Attribute("id")).toInt(Q_NULLPTR, 16);
    package->busAck.frame.length = QString(elementFrame->Attribute("length")).toInt();
    package->busAck.frame.crc16 = QString(elementFrame->Attribute("crc16")).toInt();
    package->busAck.frame.tail = QString(elementFrame->Attribute("tail")).toInt(Q_NULLPTR, 16);

    if (!parse(elementFrame, package->busAck.items)) {
        return false;
    }

    return true;
}

bool CniXmlParse::save(const QString &path, const CNI_PACKAGE_INFO *package)
{
    if (path.isEmpty()) {
        qDebug() << QString("path can not be empty");
        Q_ASSERT(false);
        return false;
    }

    if (!package->Initialized) {
        return false;
    }

    QString fileName(QApplication::applicationDirPath() + CNI_DATA_PATH + "cni_data_defaults.xml");
    TiXmlDocument doc(fileName.toLocal8Bit().data());

    QString tempText;

    if(!doc.LoadFile(TIXML_ENCODING_UTF8))
    {
        tempText = tr("Open file %1 failed! \n\n"
                      "error information:\n\n"
                      "errorId: %2\n"
                      "errorDesc: %3 \n\n"
                      "coordinate: (%4,%5)")
                .arg(fileName).arg(doc.ErrorId()).arg(doc.ErrorDesc()).arg(doc.ErrorRow()).arg(doc.ErrorCol());
        QMessageBox::critical(Q_NULLPTR, tr("Open CNI config file"), tempText, QMessageBox::Ok);
        exit(-1);
        return false;
    }

    TiXmlElement* root = doc.RootElement();
    if (!root) {
        tempText = tr("root node is not found in file %1").arg(fileName);
        QMessageBox::critical(Q_NULLPTR, tr("Open CNI config file"), tempText, QMessageBox::Ok);
        exit(-1);
        return false;
    }

    QStringList pathList = path.split('/', QString::SkipEmptyParts);
    if (pathList.isEmpty()) {
        qDebug() << QString("has no '/' in path '[%1]'").arg(path);
        Q_ASSERT(false);
        exit(-1);
        return false;
    }

    if (pathList.count() < 2) {
        qDebug() << QString("the size of pathList'[%1]' can't' be less than 2").arg(pathList.count());
        Q_ASSERT(false);
        exit(-1);
        return false;
    }

    // load protocol
    TiXmlElement* protocol = root->FirstChildElement(pathList[0].toStdString());
    if (!protocol) {
        tempText = tr("protocol '%1' is not found in file %2").arg(pathList[0]).arg(fileName);
        QMessageBox::critical(Q_NULLPTR, tr("Parse CNI config file"), tempText, QMessageBox::Ok);
        exit(-1);
        return false;
    }

    // load module
    TiXmlElement* module = protocol->FirstChildElement(pathList[1].toStdString());
    if (!module) {
        tempText = tr("module '%1' is not found in file %2").arg(pathList[1]).arg(fileName);
        QMessageBox::critical(Q_NULLPTR, tr("Parse CNI config file"), tempText, QMessageBox::Ok);
        exit(-1);
        return false;
    }

    // load submodule
    TiXmlElement* submodule = module->FirstChildElement(pathList[2].toStdString());
    if (!submodule) {
        tempText = tr("submodule '%1' is not found in file %2").arg(pathList[2]).arg(fileName);
        QMessageBox::critical(Q_NULLPTR, tr("Save CNI config file"), tempText, QMessageBox::Ok);
        exit(-1);
        return false;
    }

    TiXmlElement* element;
    QVectorIterator<CNI_ITEM_INFO> iteratorItem(package->items);
    for (element = submodule->FirstChildElement();
         element != Q_NULLPTR && iteratorItem.hasNext();
         element = element->NextSiblingElement())
    {
        const CNI_ITEM_INFO &item = iteratorItem.next();
        if (item.Type == "enum") {
            // load option
            TiXmlElement* option = element->FirstChildElement("option");
            if (!option) {
                tempText = tr("node '%1/option' is not found").arg(CniXmlParse::elementPath(element));
                QMessageBox::critical(Q_NULLPTR, tr("Save CNI config file"), tempText, QMessageBox::Ok);
                return false;
            }
            option->SetAttribute("default", item.ItemEnum.CurrentValue.toUtf8().constData());
        } else if (item.Type == "numeric") {
            // load numeric
            TiXmlElement* numeric = element->FirstChildElement("numeric");
            if (!numeric) {
                tempText = tr("node '%1/numeric' is not found").arg(CniXmlParse::elementPath(element));
                QMessageBox::critical(Q_NULLPTR, tr("Save CNI config file"), tempText, QMessageBox::Ok);
                return false;
            }
            TiXmlElement* elementNumeric;
            QVectorIterator<CNI_ITEM_NUMERIC_FORMAT> iteratorFormat(item.ItemNumeric.Formats);
            for (elementNumeric = numeric->FirstChildElement();
                 elementNumeric != NULL && iteratorFormat.hasNext();
                 elementNumeric = elementNumeric->NextSiblingElement())
            {
                const CNI_ITEM_NUMERIC_FORMAT &itemFormat = iteratorFormat.next();
                if (item.ItemNumeric.Unit.isEmpty()) {  // is an ascii
                    elementNumeric->SetAttribute("default", QString((char(itemFormat.CurrentValue))).toStdString());
                } else {
                    elementNumeric->SetDoubleAttribute("default", itemFormat.CurrentValue);
                }
            }
        } else if (item.Type == "button") {
            element->SetAttribute("default", item.ItemButton.CurrentValue ? "true" : "false");
        } else {
            tempText = tr("Unsupported type '%1'").arg(item.Type);
            QMessageBox::critical(Q_NULLPTR, tr("Parse CNI config file"), tempText, QMessageBox::Ok);
            return false;
        }
    }

    doc.SaveFile();

    return true;
}

bool CniXmlParse::save(TiXmlElement *node, const CNI_NETWORK_CONTROL *package)
{
    Q_UNUSED(node)
    Q_UNUSED(package);
    return true;
}

bool CniXmlParse::save(TiXmlElement *node, const CNI_NETWORK_MONITOR *package)
{
    Q_UNUSED(node)
    Q_UNUSED(package);
    return true;
}

bool CniXmlParse::save(TiXmlElement *node, const CNI_ALT_PACKAGE_INFO *package)
{
    Q_UNUSED(node)
    Q_UNUSED(package);
    return true;
}

QString CniXmlParse::elementPath(const TiXmlNode* element)
{
    Q_ASSERT(element);
    QString path;
    for ( ; element != NULL; element = element->Parent()) {
        path.insert(0, QString("/%1(%2,%3)").arg(element->Value()).arg(element->Row()).arg(element->Column()));
    }

    return path;
}
