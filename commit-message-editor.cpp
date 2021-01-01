/**
 * @file commit-message-editor.cpp
 * @author Paul Thomson
 * @copyright Copyright (c) 2021 Paul Thomson. All rights reserved.
 */

#include "MainWindow.h"

#include <QApplication>
#include <QCommandLineParser>
#include <QMessageBox>

/**
 * @brief Entry point
 * @param argc Number of command line arguments
 * @param argv Array of command line arguments
 * @return Application exit code
 */
int main(int argc, char *argv[])
{
  QApplication app(argc, argv);
  
  QApplication::setOrganizationName("pthomson.net");
  QApplication::setApplicationName("commit-message-editor");
  QApplication::setApplicationVersion("0.1.0");

  QCommandLineParser parser;
  parser.addVersionOption();
  parser.process(app);

  if (parser.positionalArguments().length() != 1)
  {
    QMessageBox::critical(nullptr, "Missing argument", "Please supply the commit message filename");
    return 2;
  }

  QString fileName = parser.positionalArguments().at(0);

  MainWindow dialog(fileName);

  return (dialog.exec() == QDialog::Accepted) ? 0 : 1;
}
