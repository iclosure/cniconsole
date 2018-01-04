#ifndef CNI_XML_PARSE_H
#define CNI_XML_PARSE_H

#include <QObject>
#include "../protocol/protocol.h"

class TiXmlNode;

class CniXmlParse : public QObject
{
    Q_OBJECT
public:
    explicit CniXmlParse(QObject *parent = 0);
    virtual ~CniXmlParse();

    static bool parse(const QString &path, CNI_PACKAGE_INFO *package);
    static bool save(const QString &path, const CNI_PACKAGE_INFO *package);

private:
    static bool parse(const TiXmlElement *node, CNI_ITEM_INFO &item, CNI_ITEM_ENUM_INFO &itemEnum);
    static bool parse(const TiXmlElement *node, CNI_ITEM_NUMERIC_INFO &itemNumeric);
    static bool parse(const TiXmlElement *node, CNI_ITEM_BUTTON_INFO &itemButton);
    static bool parse(const TiXmlElement *node, QVector<CNI_ITEM_INFO> &items);
    static bool parse(const TiXmlElement *node, CNI_NETWORK_CONTROL *package);
    static bool parse(const TiXmlElement *node, CNI_NETWORK_MONITOR *package);
    static bool parse(const TiXmlElement *node, CNI_ALT_PACKAGE_INFO *package);

    static bool save(TiXmlElement *node, const CNI_NETWORK_CONTROL *package);
    static bool save(TiXmlElement *node, const CNI_NETWORK_MONITOR *package);
    static bool save(TiXmlElement *node, const CNI_ALT_PACKAGE_INFO *package);

signals:

public slots:

protected:
    static QString elementPath(const TiXmlNode* element);
};

#endif // CNI_XML_PARSE_H
