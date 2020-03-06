/*
 * Copyright (C) 2013 ~ 2020 National University of Defense Technology(NUDT) & Tianjin Kylin Ltd.
 *
 * Authors:
 *  Kobe Lee    lixiang@kylinos.cn/kobe24_lixiang@126.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef INDEXITEMWIDGET_H
#define INDEXITEMWIDGET_H

#include <QWidget>

class IndexItemWidget : public QWidget
{
    Q_OBJECT

public:
    explicit IndexItemWidget(const QString &name, const QString &iconPath, QWidget *parent = 0);

    void refreshLifeStyle(const QString &brf, const QString &txt);

    void setDayStyleSheets();
    void setNightStyleSheets();

signals:
    void requestShowMsg(const QString &name) const;

protected:
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    bool m_mouseHover;
    QString m_indexName;
    QString m_iconPath;
    QString m_brf;
    QString m_txt;
    QColor m_txtColor;
    QColor m_titleColor;
};

#endif // INDEXITEMWIDGET_H
