#pragma once

#include <QLabel>
#include <QResizeEvent>

namespace Ui {

class MarkdownLabel : public QLabel
{
public:
    MarkdownLabel(const std::string& content, QWidget *parent);
};

} // namespace Ui
