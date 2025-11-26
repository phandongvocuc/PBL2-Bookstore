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


using namespace std;
using namespace pbl2;

// Định nghĩa biến toàn cục cho các service
pbl2::service::ReaderService *readerService = nullptr;
pbl2::service::StaffService *staffService = nullptr;

namespace pbl2 {
namespace ui {
    service::StaffService *staffService = nullptr;
    service::ReaderService *readerService = nullptr;
}
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    // Force a simple, light theme so it does not
    // switch to macOS dark mode.
    app.setStyle(QStyleFactory::create(QStringLiteral("Fusion")));

    QPalette pal;
    pal.setColor(QPalette::Window, Qt::white);
    pal.setColor(QPalette::WindowText, Qt::black);
    pal.setColor(QPalette::Base, Qt::white);
    pal.setColor(QPalette::AlternateBase, QColor(0xF0, 0xF0, 0xF0));
    pal.setColor(QPalette::Text, Qt::black);
    pal.setColor(QPalette::Button, QColor(0xE0, 0xE0, 0xE0));
    pal.setColor(QPalette::ButtonText, Qt::black);
    pal.setColor(QPalette::Highlight, QColor(0x80, 0x80, 0x80));
    pal.setColor(QPalette::HighlightedText, Qt::white);
    app.setPalette(pal);

    const auto dataDir = util::locateDataDir();
    const QString dataDirPath = bridge::toQString(dataDir);
    service::AccountService accountService(dataDir);

    // Khởi tạo các service toàn cục
    pbl2::ui::readerService = new pbl2::service::ReaderService(dataDir);
    pbl2::ui::staffService = new pbl2::service::StaffService(dataDir);

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
