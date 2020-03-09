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

#ifndef TRANLUCENTLABEL_H
#define TRANLUCENTLABEL_H

#include <QLabel>

class TranslucentLabel : public QLabel {
    Q_OBJECT
public:
    TranslucentLabel(QWidget *parent = 0);
    TranslucentLabel(const QString &tiptxt, QWidget *parent = 0);

    void setLabelIcon(const QString &iconPath);
    void setLabelText(const QString &text);

    void showTooltip(const QPoint &pos);

signals:
    void clicked();
    void requestHideTip();


protected:
//    virtual void focusInEvent(QFocusEvent *event) Q_DECL_OVERRIDE;
//    virtual void focusOutEvent(QFocusEvent *event) Q_DECL_OVERRIDE;
    bool event(QEvent *event) Q_DECL_OVERRIDE;
    void enterEvent(QEvent *event) Q_DECL_OVERRIDE;
    void leaveEvent(QEvent *event) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    //bool eventFilter(QObject *obj, QEvent *event) Q_DECL_OVERRIDE;

private:
    QLabel *m_icon = nullptr;
    QLabel *m_text = nullptr;
    bool m_showTip;
    bool m_isPressed;
    QString m_tipTxt;
};
#endif // TRANLUCENTLABEL_H
