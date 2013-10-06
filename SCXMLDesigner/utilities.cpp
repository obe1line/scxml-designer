#include <QMessageBox>
#include "utilities.h"

Utilities::Utilities()
{
}

void Utilities::ShowWarning(QString msg)
{
    QMessageBox msgBox;
    msgBox.setModal(true);
    msgBox.setIcon(QMessageBox::Warning);
    msgBox.setText(msg);
    msgBox.exec();
}
