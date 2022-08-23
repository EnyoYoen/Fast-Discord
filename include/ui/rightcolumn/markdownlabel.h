#pragma once

#include "api/ressourcemanager.h"

#include <QLabel>

namespace Ui {

class MarkdownLabel : public QLabel
{
public:
    MarkdownLabel(const QString& content, Api::RessourceManager *rm, QWidget *parent);

private:
    QString html;
    int usernamesOffset = 0;
    bool finished = false;
};

} // namespace Ui
