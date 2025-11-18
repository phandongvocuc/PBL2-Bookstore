#include "ui/LoginDialog.h"
#include "ui/MainWindow.h"
#include "datafile/DataPaths.h"
#include "service/AccountService.h"
#include "QtBridge.h"
#include "ui/DialogTheme.h"

#include <QApplication>
#include <QTimer>
#include <QString>
#include <QStyleFactory>
#include <QColor>
#include <QPalette>

#include <functional>
#include <iostream>


using namespace std;  // project-wide request
using namespace pbl2;

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setStyle(QStyleFactory::create(QStringLiteral("Fusion")));

    // Apply an application-wide stylesheet and palette to enforce the light dialog theme
    app.setStyleSheet(pbl2::ui::dialogStylesheet());
    QPalette appPal = app.palette();
    appPal.setColor(QPalette::Window, QColor(0xF8, 0xFA, 0xFC));
    appPal.setColor(QPalette::Base, QColor(0xFF, 0xFF, 0xFF));
    appPal.setColor(QPalette::AlternateBase, QColor(0xF1, 0xF5, 0xF9));
    appPal.setColor(QPalette::WindowText, QColor(0x0F, 0x17, 0x2A));
    appPal.setColor(QPalette::Text, QColor(0x0F, 0x17, 0x2A));
    appPal.setColor(QPalette::Button, QColor(0xE2, 0xE8, 0xF0));
    appPal.setColor(QPalette::ButtonText, QColor(0x0F, 0x17, 0x2A));
    appPal.setColor(QPalette::Highlight, QColor(0xDB, 0xEA, 0xFE));
    appPal.setColor(QPalette::HighlightedText, QColor(0x0F, 0x17, 0x2A));
    app.setPalette(appPal);


    const auto dataDir = util::locateDataDir();
    const QString dataDirPath = ::pbl2::bridge::toQString(dataDir);
    service::AccountService accountService(dataDir);

    unique_ptr<ui::MainWindow> window;
    function<void(bool)> showLogin;

    showLogin = [&](bool allowExit) {
        ui::LoginDialog loginDialog(accountService);
        // Ensure the dialog theme overrides any UI-generated stylesheet
        pbl2::ui::applyDialogTheme(&loginDialog);

        if (int result = loginDialog.exec(); result != QDialog::Accepted) {
            if (loginDialog.exitRequested()) {
                if (allowExit) {
                    QTimer::singleShot(0, [&app]() { app.quit(); });
                    return;
                }
            }
            QTimer::singleShot(0, [&showLogin, allowExit]() { showLogin(allowExit); });
            return;
        }

        const auto acct = loginDialog.account();
        window = make_unique<ui::MainWindow>(dataDirPath, acct);
        // Apply dialog theme to the main window as well so child dialogs inherit consistent palette
        pbl2::ui::applyDialogTheme(window.get());
        QObject::connect(window.get(), &ui::MainWindow::logoutRequested, &app, [&window, &showLogin]() {
            // Close and destroy current window, then restart login flow immediately
            if (window) {
                window->close();
                window.reset();
            }
            showLogin(false);
        }, Qt::QueuedConnection);
        window->show();
    };

    // At application startup allow the login dialog to quit the app.
    showLogin(true);
   
    return app.exec();
}
