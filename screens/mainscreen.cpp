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
#include "mainscreen.h"
#include "ui_mainscreen.h"

#include <QApplication>
#include <QKeyEvent>
#include <gamepadbuttons.h>
#include <textinputoverlay.h>
#include <pauseoverlay.h>
#include <loadoverlay.h>
#include <musicengine.h>
#include <QSvgRenderer>
#include <QPainter>
#include <the-libs_global.h>

MainScreen::MainScreen(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainScreen)
{
    ui->setupUi(this);

    ui->focusBarrierTop->setBounceWidget(ui->startEasy);
    ui->focusBarrierBottom->setBounceWidget(ui->exitButton);

    ui->gamepadHud->setButtonText(QGamepadManager::ButtonA, tr("Select"));
    ui->gamepadHud->setButtonText(QGamepadManager::ButtonB, tr("Exit"));

    ui->gamepadHud->setButtonAction(QGamepadManager::ButtonA, [=] {
        QKeyEvent event(QKeyEvent::KeyPress, Qt::Key_Space, Qt::NoModifier);
        QApplication::sendEvent(QApplication::focusWidget(), &event);

        QKeyEvent event2(QKeyEvent::KeyRelease, Qt::Key_Space, Qt::NoModifier);
        QApplication::sendEvent(QApplication::focusWidget(), &event2);
    });
    ui->gamepadHud->setButtonAction(QGamepadManager::ButtonB, [=] {
        ui->exitButton->click();
    });

    ui->exitButton->setProperty("type", "destructive");
}

MainScreen::~MainScreen()
{
    delete ui;
}

void MainScreen::resizeEvent(QResizeEvent*event)
{
    ui->leftSpacing->changeSize(this->width() * 0.4, 0, QSizePolicy::Fixed, QSizePolicy::Fixed);
    this->layout()->invalidate();
}

void MainScreen::paintEvent(QPaintEvent*event)
{
    QPainter painter(this);
    QSvgRenderer renderer(QStringLiteral(":/icons/background.svg"));

    QSize size = renderer.defaultSize();
    size.scale(this->size(), Qt::KeepAspectRatioByExpanding);

    QRect geometry;
    geometry.setSize(size);
    geometry.moveCenter(QPoint(this->width() / 2, this->height() / 2));
    renderer.render(&painter, geometry);

    QLinearGradient grad(QPoint(0, this->height()), QPoint(0, this->height() - SC_DPI(50)));
    grad.setColorAt(0, QColor(0, 0, 0, 127));
    grad.setColorAt(1, QColor(0, 0, 0, 0));

    painter.setBrush(grad);
    painter.setPen(Qt::transparent);
    painter.drawRect(0, 0, this->width(), this->height());
}

void MainScreen::on_startEasy_clicked()
{
    emit startGame(9, 9, 10);
}

void MainScreen::on_startIntermediate_clicked()
{
    emit startGame(16, 16, 40);
}

void MainScreen::on_startDifficult_clicked()
{
    emit startGame(30, 16, 99);
}

void MainScreen::on_exitButton_clicked()
{
    QApplication::exit();
}

#include <QGraphicsBlurEffect>
void MainScreen::on_startCustom_clicked()
{
    QScopedPointer<PauseOverlay> overlay(new PauseOverlay(this));
    bool canceled;

    int width = 15;
    int height = 15;
    int mines = 60;

    askWidth:
    width = TextInputOverlay::getInt(this, tr("How wide is the board?"), &canceled, width, 5, 99, QLineEdit::Normal, overlay.data());
    if (canceled) return;

    askHeight:
    height = TextInputOverlay::getInt(this, tr("How high is the board?"), &canceled, height, 5, 99, QLineEdit::Normal, overlay.data());
    if (canceled) goto askWidth;

    mines = TextInputOverlay::getInt(this, tr("How many mines are on this board?"), &canceled, mines, 1, static_cast<int>(width * height * 0.9) - 1, QLineEdit::Normal, overlay.data());
    if (canceled) goto askHeight;

    emit startGame(width, height, mines);
}

void MainScreen::on_loadButton_clicked()
{
//    QFile* f = new QFile("/home/victor/.local/share/theSuite/Entertaining Mines/saves/coolsave");
//    f->open(QFile::ReadWrite);
//    QDataStream str(f);
//    int magic;
//    QString string;
//    str >> magic >> magic;
//    str >> string;
//    QVariantMap meta;
//    str >> meta;

//    emit loadGame(&str);

    MusicEngine::playSoundEffect(MusicEngine::Selection);
    LoadOverlay* load = new LoadOverlay(this);
    connect(load, &LoadOverlay::loadData, this, [=](QDataStream* stream) {
        loadGame(stream);
    });
    load->load();
}
