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

#ifndef LIGHTANIMATION_H
#define LIGHTANIMATION_H

#include <QList>
#include <QSettings>
#include <QStringList>

#include "key.h"

struct AnimationSettings
{
    enum Pattern {
        Sequence,
        WaveLeftRight,
        WaveTopBottom,
        Ripple,
        Breathing,
        Twinkle
    };

    enum Loop {
        Repeat,
        BackAndForth,
        Once
    };

    bool enabled{false};
    int pattern{Sequence};
    int stepMs{120};
    int fadeMs{600};
    int brightness{250};
    int loop{Repeat};
    int pauseMs{0};

    // Read and write the "animation" subgroup of the current settings group
    void write(QSettings& settings) const;
    static AnimationSettings read(QSettings& settings);
};

// Renders host driven animations as a brightness level (0 - MaxLevel) per key
class LightAnimation
{
public:
    static constexpr int MaxLevel = 250;

    // selectedKeys are indexes into keys, in the order the user picked them.
    // sequence is the steps of a Sequence pattern, each a group of key indexes;
    // a key may appear in any number of steps, and an empty step is a pause.
    void prepare(const QList<Key>& keys, const QList<int>& selectedKeys,
                 const QList<QList<int>>& sequence, const AnimationSettings& settings);

    // Level for every key (indexed like keys) at the given time
    QList<int> render(qint64 ms) const;

    // True once a Loop::Once animation has played through
    bool finished(qint64 ms) const;

    // Sequences are saved as one string per step, keys joined by '+'.
    // A step with no keys is a pause and is saved as "-".
    static QStringList sequenceToStrings(const QList<QList<int>>& sequence);
    static QList<QList<int>> sequenceFromStrings(const QStringList& strings);

private:
    QList<Key> m_keys;
    AnimationSettings m_settings;

    // A key can take part more than once, e.g. a repeated sequence step
    QList<int> m_participants;
    QList<double> m_offsets;
    double m_span{0};

    qint64 cycleLength() const;
    int envelope(double age) const;
};

#endif // LIGHTANIMATION_H
