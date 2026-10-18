/*
 * Copyright (C) 2026 Rangoli contributors
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#ifndef CUSTOMEFFECTMODEL_H
#define CUSTOMEFFECTMODEL_H

#include <QAbstractListModel>
#include <QColor>
#include <QHash>

struct CustomEffect
{
    QString id;
    QString name;

    // LED buffer index -> colour
    QHash<int, QColor> keys;
};

// User created custom light mode effects, saved per keyboard model
class CustomEffectModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum CustomEffectModelRoles {
        IDRole = Qt::UserRole + 1,
        NameRole
    };

    explicit CustomEffectModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    void load(const QString& keyboardID);

    int createEffect(const QString& name, const QHash<int, QColor>& keys);
    void setEffectName(const int& row, const QString& name);
    void setEffectKeys(const int& row, const QHash<int, QColor>& keys);
    void deleteEffect(const int& row);

    const CustomEffect& effect(const int& row) const;
    int indexOf(const QString& id) const;

    Q_INVOKABLE QString effectName(const int& row) const;

private:
    QList<CustomEffect> m_effects;
    QString m_keyboardID;

    void writeEffect(const CustomEffect& effect);
    void writeOrder();
};

#endif // CUSTOMEFFECTMODEL_H
