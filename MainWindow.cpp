/**
 * @file MainWindow.cpp
 * @author Paul Thomson
 * @copyright Copyright (c) 2020 Paul Thomson. All rights reserved.
 */

#include "MainWindow.h"

#include <QFile>
#include <QFileInfo>
#include <QVBoxLayout>
#include <QApplication>
#include <QTextStream>

/**
 *
 */
MainWindow::MainWindow(const QString& fileName,
                       QWidget *parent,
                       Qt::WindowFlags flags):
    QDialog(parent, flags),
    _fileName(fileName)
{

    _subjectLineEdit = new QLineEdit;
    _subjectLineEdit->setMinimumWidth(600);
    _subjectLineEdit->setFont(QFont("Roboto Mono"));

    _subjectLengthLabel = new QLabel(tr("0/50"));

    _bodyTextEdit = new QPlainTextEdit;
    _bodyTextEdit->setFont(QFont("Roboto Mono"));

    _buttons = new QDialogButtonBox({QDialogButtonBox::Save, QDialogButtonBox::Cancel});

    QPushButton *commitButton = _buttons->button(QDialogButtonBox::Save);
    commitButton->setText(tr("Commit"));
    commitButton->setEnabled(false);

    //

    QVBoxLayout *mainLayout = new QVBoxLayout;

    QHBoxLayout *subjectLayout = new QHBoxLayout;

    subjectLayout->addWidget(_subjectLineEdit);
    subjectLayout->addStretch(1);
    subjectLayout->addWidget(_subjectLengthLabel);

    mainLayout->addWidget(new QLabel(tr("Subject")));
    mainLayout->addLayout(subjectLayout);
    mainLayout->addWidget(new QLabel(tr("Body")));
    mainLayout->addWidget(_bodyTextEdit);

    mainLayout->addWidget(_buttons);

    setLayout(mainLayout);

    //

    connect(_subjectLineEdit, &QLineEdit::textChanged,
            this, &MainWindow::updateSubjectLengthLabel);

    connect(_subjectLineEdit, &QLineEdit::textChanged,
            this, &MainWindow::updateCommitButton);

    connect(_buttons, &QDialogButtonBox::accepted,
            this, &MainWindow::saveCommitMessage);

    connect(_buttons, &QDialogButtonBox::rejected,
            this, &MainWindow::reject);

    //

    setWindowTitle(tr("Commit Message Editor - v") + QApplication::applicationVersion());
    setMinimumSize(720, 480);

    //

    loadCommitMessage();
}

/*
 *
 * Private methods
 *
 */

/**
 *
 */
void MainWindow::updateSubjectLengthLabel(const QString& text)
{
    _subjectLengthLabel->setText(QString("%1/50").arg(text.size()));
}

/**
 *
 */
void MainWindow::updateCommitButton(const QString& text)
{
    QPushButton *commitButton = _buttons->button(QDialogButtonBox::Save);
    commitButton->setEnabled(text.size() > 0);
}

/**
 *
 */
void MainWindow::loadCommitMessage()
{
    if (_fileName.isEmpty() == false)
    {
        QFileInfo inputFileInfo(_fileName);
        if (inputFileInfo.exists())
        {
            QFile commitMessageFile(_fileName, this);
            if (commitMessageFile.open(QIODevice::ReadOnly))
            {
                QTextStream inputStream(&commitMessageFile);
                QString contents;
                while (!inputStream.atEnd())
                {
                    contents += inputStream.readLine() + QLatin1Char('\n');
                }

                _bodyTextEdit->setPlainText(contents);
            }
        }
    }
}

/**
 *
 */
void MainWindow::saveCommitMessage()
{
    QString content = _bodyTextEdit->toPlainText();

    QStringList sourceLines = content.split('\n');
    QStringList parsedLines;

    for (QString& line : sourceLines)
    {
        line = line.trimmed();

        // Ignore lines that start with a #

        if (false == line.startsWith('#'))
        {
            if (line.length() > MaximumLineLength)
            {
                while (line.length() > MaximumLineLength)
                {
                    const int p = line.lastIndexOf(' ', MaximumLineLength);

                    if (p >= 0)
                    {
                        QString partOfLine = line;
                        partOfLine.truncate(p);
                        line.remove(0, p);
                        line = line.trimmed();

                        parsedLines << (partOfLine);
                    }
                    else
                    {
                        // Couldn't break line on a space, so use it as-is
                        break;
                    }
                }

                if (line.length() > 0)
                {
                    parsedLines << (line);
                }
            }
            else
            {
                parsedLines << (line);
            }
        }
    }

    QFile commitMessageFile(_fileName, this);
    if (commitMessageFile.open(QIODevice::WriteOnly))
    {
        QTextStream outputStream(&commitMessageFile);
        outputStream << _subjectLineEdit->text() << '\n';
        outputStream << '\n';
        outputStream << parsedLines.join('\n') << '\n';
        outputStream.flush();

        commitMessageFile.close();
    }
    else
    {
        /// @todo Reject?
    }

    accept();
}
