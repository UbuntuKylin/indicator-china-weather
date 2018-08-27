/*
 * Copyright (C) 2013 ~ 2018 National University of Defense Technology(NUDT) & Tianjin Kylin Ltd.
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

#ifndef SEARCHTITLEBAR_H
#define SEARCHTITLEBAR_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QHBoxLayout>
#include <QTimer>

#include "data.h"

class SearchInputEdit;
class LocationWorker;

class SearchTitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit SearchTitleBar(QWidget *parent = 0);
    ~SearchTitleBar();

    void initLeftContent();
    void initMiddleContent();
    void initRightContent();
    void initBottomContent();
    void initWidgets();

    void resetSearchInputEdit() const;
    void setSearchResult(const QList<LocationData> data);

public slots:
    void onSearchTimerOut();

signals:
    void requestCloseDialog();
    void requestShowNoResultLabel();
    void requestResetViewAndModel();
    void sendSearchResults(QList<LocationData> results);

protected:
    void paintEvent(QPaintEvent *event) Q_DECL_OVERRIDE;

private:
    QVBoxLayout *m_layout = nullptr;
    QHBoxLayout *m_toplayout = nullptr;
    QHBoxLayout *m_lLayout = nullptr;
    QHBoxLayout *m_mLayout = nullptr;
    QHBoxLayout *m_rLayout = nullptr;
    QHBoxLayout *m_bLayout = nullptr;
    QPushButton *m_closeBtn = nullptr;

    QColor m_borderColor;
    int m_borderRadius;
    int m_borderWidth;
    QBrush m_bgBrush;

    SearchInputEdit *m_searchInputEdit = nullptr;
    QTimer *m_searchTimer = nullptr;
    LocationWorker *m_locationWorker = nullptr;
};

#endif // SEARCHTITLEBAR_H
