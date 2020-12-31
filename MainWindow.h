/**
 * @file MainWindow.h
 * @author Paul Thomson
 * @copyright Copyright (c) 2020 Paul Thomson. All rights reserved.
 */

#pragma once

#include <QDialog>
#include <QDialogButtonBox>
#include <QLineEdit>
#include <QLabel>
#include <QPlainTextEdit>
#include <QPushButton>

/**
 * @brief Application main window
 */
class MainWindow : public QDialog
{
    Q_OBJECT

public:

    /**
     * @brief Constructor
     * @param fileName Path to the original commit message
     * @param parent Optional parent widget
     * @param flags Optional window flags
     */
    MainWindow(const QString& fileName,
               QWidget *parent = nullptr,
               Qt::WindowFlags flags = Qt::WindowFlags());

private:

    static constexpr qsizetype MaximumLineLength { 72 };

    void updateSubjectLengthLabel(const QString& text);

    void updateCommitButton(const QString& text);

    void loadCommitMessage();

    void saveCommitMessage();

    QString _fileName;

    QLineEdit *_subjectLineEdit { nullptr };

    QLabel *_subjectLengthLabel { nullptr };

    QPlainTextEdit *_bodyTextEdit { nullptr };

    QDialogButtonBox *_buttons { nullptr };

};
