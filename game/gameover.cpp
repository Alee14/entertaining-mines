#include "gameover.h"
#include "ui_gameover.h"

#include <pauseoverlay.h>
#include <musicengine.h>
#include <QKeyEvent>

struct GameOverPrivate {
    PauseOverlay* overlay;
};

GameOver::GameOver(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::GameOver)
{
    ui->setupUi(this);
    d = new GameOverPrivate();

    MusicEngine::setBackgroundMusic("pianobetween");
    MusicEngine::playBackgroundMusic();

    this->setFocusProxy(ui->playAgainButton);

    ui->focusBarrierTop->setBounceWidget(ui->playAgainButton);
    ui->focusBarrierBottom->setBounceWidget(ui->mainMenuButton);

    ui->gamepadHud->setButtonText(QGamepadManager::ButtonA, tr("Select"));
    ui->gamepadHud->setButtonText(QGamepadManager::ButtonB, tr("Main Menu"));

    ui->gamepadHud->setButtonAction(QGamepadManager::ButtonA, [=] {
        QKeyEvent event(QKeyEvent::KeyPress, Qt::Key_Space, Qt::NoModifier);
        QApplication::sendEvent(QApplication::focusWidget(), &event);

        QKeyEvent event2(QKeyEvent::KeyRelease, Qt::Key_Space, Qt::NoModifier);
        QApplication::sendEvent(QApplication::focusWidget(), &event2);
    });
    ui->gamepadHud->setButtonAction(QGamepadManager::ButtonB, [=] {
        ui->mainMenuButton->click();
    });

    d->overlay = new PauseOverlay(parent);
    d->overlay->pushOverlayWidget(this);

    ui->mainMenuButton->setProperty("type", "destructive");
}

GameOver::~GameOver()
{
    delete d;
    delete ui;
}

void GameOver::on_mainMenuButton_clicked()
{
    MusicEngine::pauseBackgroundMusic();
    d->overlay->popOverlayWidget([=] {
        emit mainMenu();
    });
}

void GameOver::on_playAgainButton_clicked()
{
    d->overlay->popOverlayWidget([=] {
        emit playAgain();
    });
}

void GameOver::on_saveButton_clicked()
{
    d->overlay->popOverlayWidget([=] {
        emit review();
    });
}