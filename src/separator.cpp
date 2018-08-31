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

#include "separator.h"

VSeparator::VSeparator(QWidget *parent) :
    QFrame(parent)
{
    this->setStyleSheet("QFrame{background-color: rgba(238, 238, 238, 1.0);}");//#eeeeee 0:完全透明  1:完全不透明
    this->setFixedWidth(1);
    this->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
}

HSeparator::HSeparator(QWidget *parent) :
    QFrame(parent)
{
    this->setStyleSheet("QFrame{background-color: rgba(238, 238, 238, 0.5);margin-left:5px;margin-right:5px;}");//#eeeeee 0:完全透明  1:完全不透明
    this->setFixedHeight(1);
    this->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
}
