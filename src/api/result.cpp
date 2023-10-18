#include "result.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStringList>
#include "../defined.h"

struct DictionaryEntry {
    QString query;
    QString explain;
};

QList<DictionaryEntry> parseDictionary(const QString& jsonString)
{
    QList<DictionaryEntry> entries;

    QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonString.toUtf8());
    if (jsonDoc.isNull()) {
        return entries;
    }

    QJsonObject jsonObj = jsonDoc.object();
    QJsonObject dataObj = jsonObj["data"].toObject();

    if (!dataObj.contains("entries")) {
        return entries;
    }

    QJsonArray entriesArray = dataObj["entries"].toArray();

    QString query = jsonObj["query"].toString();
    for (int i = 0; i < entriesArray.size(); i++) {
        QJsonObject entryObj = entriesArray[i].toObject();
        QString explain = entryObj["explain"].toString();
        DictionaryEntry entry = {query, explain};
        entries.append(entry);
    }

    return entries;
}

Result::Result(QByteArray res)
{
#if 0
    QString res_str = QString(res);
    INFO << "reply of server:" << res_str;
    QJsonDocument json_doc = QJsonDocument::fromJson(res);
    QJsonObject json_obj = json_doc.object();

    this->query = json_obj.take("query").toString();
    this->explain = json_obj.take("explain").toString();
    this->translation = json_obj.take("translation").toString();
    this->uk_phonetic = json_obj.take("uk-phonetic").toString();
    this->us_phonetic = json_obj.take("us-phonetic").toString();
    this->error_code = json_obj.take("errorCode").toInt();
    this->error_msg = json_obj.take("errorMsg").toString();
#endif

// youdaoapi result
#if 1
    this->error_code = -1;
    
    QString res_str = QString(res);
    INFO << "reply of server:" << res_str;
    QList<DictionaryEntry> entries = parseDictionary(res_str);

    bool get_query = false;
    QString explains;
    for (int i = 0; i < entries.size(); i++ ) {
        DictionaryEntry entry = entries[i];
        if (!get_query) {
            this->query = entry.query;
            get_query = true; 
            this->error_code = 0;
        }
        explains += entry.explain + "\r\n";
    }
    this->explain = explains;
#endif

#if 0
    QList<DictionaryEntry> entries = parseDictionary(res_str);

    bool get_query = false;
    QString explains;
    for (int i = 0; i < entries.size(); i++ ) {
        DictionaryEntry entry = entries[i];
        if (!get_query) {
            this->query = entry.query;
            get_query = true; 
            this->error_code = 0;
        }
        explains += entry.explain + "\r\n";
    }
    this->explain = explains;
#endif

    
// iciba result


//    QStringList L1 = json_obj.keys();
//    INFO << "the reply keys:" << L1;

//    //Get the translation
//    QJsonArray translation_array = json_obj.take("translation").toArray();
//    int counter = translation_array.count();
//    for (int i = 0; i < counter; i++)
//    {
//        translation.append(translation_array.at(i).toString() + "\n");
//    }

//    //Get the basic：contain almost information we need
//    QJsonObject basic_obj = json_obj.take("basic").toObject();

//    us_phonetic = basic_obj.take("us-phonetic").toString();
//    uk_phonetic = basic_obj.take("uk-phonetic").toString();

//        //Get the explains in basic_obj
//    QJsonArray explains_array = basic_obj.take("explains").toArray();
//    counter = explains_array.count();
//    for (int i = 0; i < counter; i++){
//        explain.append(explains_array.at(i).toString() + "\n");
//    }
//    INFO << "explains" << ":" << explain;

//    //Get the way of translation：from English to Chinese-simple as the default way
//    QString language = json_obj.take("l").toString();
//    INFO << "way of translation:" << language;

//    // Get the erroCode：0 means everything on it's way.You can get more from
//    // http://ai.youdao.com/docs/api.s
//    error_code = json_obj.take("erroCode").toInt();
}

Result::~Result() {

}
