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
#include "onlinelobbyscreen.h"
#include "ui_onlinelobbyscreen.h"

#include <QJsonObject>
#include <QJsonArray>
#include <discordintegration.h>
#include "onlinecontroller.h"

struct OnlineLobbyScreenPrivate {
    int currentRoomId = -1;
    int currentRoomPin = -1;
};

OnlineLobbyScreen::OnlineLobbyScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OnlineLobbyScreen)
{
    ui->setupUi(this);
    d = new OnlineLobbyScreenPrivate();

    ui->gamepadHud->setButtonText(QGamepadManager::ButtonA, tr("Select"));
    ui->gamepadHud->setButtonText(QGamepadManager::ButtonB, tr("Leave Room"));

    ui->gamepadHud->setButtonAction(QGamepadManager::ButtonA, GamepadHud::standardAction(GamepadHud::SelectAction));
    ui->gamepadHud->setButtonAction(QGamepadManager::ButtonB, [=] {
        ui->backButton->click();
    });

    connect(OnlineController::instance(), &OnlineController::jsonMessage, this, [=](QJsonDocument doc) {
        QJsonObject obj = doc.object();
        if (obj.value("type").toString() == "roomUpdate") {
            ui->membersInRoom->clear();
            QJsonArray users = obj.value("users").toArray();
            for (QJsonValue val : users) {
                ui->membersInRoom->addItem(val.toString());
            }

            DiscordIntegration::instance()->setPresence({
                {"state", tr("In Lobby")},
                {"details", tr("Waiting for players")},
                {"partyId", QString::number(d->currentRoomId)},
                {"partySize", users.count()},
                {"partyMax", obj.value("maxUsers").toInt()},
                {"joinSecret", QStringLiteral("%1:%2").arg(d->currentRoomId).arg(d->currentRoomPin)}
            });
        } else if (obj.value("type").toString() == "lobbyChange") {
            d->currentRoomId = obj.value("lobbyId").toInt();
        }
    });
}

OnlineLobbyScreen::~OnlineLobbyScreen()
{
    delete d;
    delete ui;
}

void OnlineLobbyScreen::on_backButton_clicked()
{
    //Tell the server that we want to leave the room
    OnlineController::instance()->sendJsonO({
        {"type", "leaveRoom"}
    });
}