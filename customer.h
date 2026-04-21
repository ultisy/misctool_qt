#pragma once
#include <QString>
#include <QDate>
#include <QJsonObject>
#include <QUuid>

struct Customer {
    QString id;           // 내부 고유 ID (UUID)
    QString name;
    QString loginId;
    QString password;
    int     balance;
    QString phone;
    QDate   birthday;
    QString email;
    QString address;
    bool    favorite;

    Customer()
        : id(QUuid::createUuid().toString(QUuid::WithoutBraces))
        , balance(0)
        , favorite(false)
    {}

    // JSON 직렬화
    QJsonObject toJson() const {
        QJsonObject obj;
        obj["id"]       = id;
        obj["name"]     = name;
        obj["loginId"]  = loginId;
        obj["password"] = password;
        obj["balance"]  = balance;
        obj["phone"]    = phone;
        obj["birthday"] = birthday.toString(Qt::ISODate);
        obj["email"]    = email;
        obj["address"]  = address;
        obj["favorite"] = favorite;
        return obj;
    }

    // JSON 역직렬화
    static Customer fromJson(const QJsonObject &obj) {
        Customer c;
        c.id       = obj["id"].toString();
        c.name     = obj["name"].toString();
        c.loginId  = obj["loginId"].toString();
        c.password = obj["password"].toString();
        c.balance  = obj["balance"].toInt();
        c.phone    = obj["phone"].toString();
        c.birthday = QDate::fromString(obj["birthday"].toString(), Qt::ISODate);
        c.email    = obj["email"].toString();
        c.address  = obj["address"].toString();
        c.favorite = obj["favorite"].toBool();
        return c;
    }
};