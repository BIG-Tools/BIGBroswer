#ifndef SETTINGSWIDGET_H
#define SETTINGSWIDGET_H

#include <QWidget>
#include <QDialog>
#include <QHBoxLayout>
#include <QTabWidget>
#include <QList>
#include <QStackedWidget>
#include <QListWidget>
#include <QDialogButtonBox>
#include "abstractsettingswidget.h"
namespace big {
namespace gui {
class SettingsDialog : public QDialog
{
    Q_OBJECT
public:
    explicit SettingsDialog(QWidget *parent = 0);
    void addWidget(AbstractSettingsWidget * widget,const QString& categorie = QString("extra"));

public Q_SLOTS:
    bool save();
    bool load();

protected Q_SLOTS:
    void updateTab(int row);

private:
    void createList();

private:
    QHash<QString, QList<AbstractSettingsWidget*> > mWidgets;

    QTabWidget * mTabWidget;
    QListWidget * mListWidget;
    QDialogButtonBox * mButtonBox;

};

}}
#endif // SETTINGSWIDGET_H
