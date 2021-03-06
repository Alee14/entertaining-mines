/****************************************
 *
 *   INSERT-PROJECT-NAME-HERE - INSERT-GENERIC-NAME-HERE
 *   Copyright (C) 2019 Victor Tran
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * *************************************/
#ifndef ONLINELOBBYSCREEN_H
#define ONLINELOBBYSCREEN_H

#include <QWidget>
#include <QStyledItemDelegate>

namespace Ui {
    class OnlineLobbyScreen;
}

struct OnlineLobbyScreenPrivate;
class OnlineLobbyScreen : public QWidget
{
        Q_OBJECT

    public:
        explicit OnlineLobbyScreen(QWidget *parent = nullptr);
        ~OnlineLobbyScreen();

    private slots:
        void on_backButton_clicked();

        void on_startButton_clicked();

        void on_widthBox_valueChanged(int arg1);

        void on_heightBox_valueChanged(int arg1);

        void on_minesBox_valueChanged(int arg1);

        void on_gamemodeButton_clicked();

    private:
        Ui::OnlineLobbyScreen *ui;
        OnlineLobbyScreenPrivate* d;

        void sendBoardParams();
};

class LobbyListDelegate : public QStyledItemDelegate {
    Q_OBJECT
    public:
        explicit LobbyListDelegate(QWidget* parent);


        void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
};

#endif // ONLINELOBBYSCREEN_H
