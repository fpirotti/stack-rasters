#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileInfo>
#include <QDebug>

#include <QMainWindow>
#include <QMessageBox>
#include <QThread>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDropEvent>
#include <QTableView>
#include <QAbstractTableModel>

#include <worker.h>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE


class FileModel : public QAbstractTableModel {
   QFileInfoList m_data;
public:
   FileModel(QObject * parent = {}) : QAbstractTableModel{parent} {}
   int rowCount(const QModelIndex &) const override { return m_data.count(); }
   int rowCount() { return m_data.count(); }
   int columnCount(const QModelIndex &) const override { return 4; }
   QVariant data(const QModelIndex &index, int role) const override {
      if (role != Qt::DisplayRole && role != Qt::EditRole) return {};
      const QFileInfo & fileInfo = m_data[index.row()];
      switch (index.column()) {
      case 0: return fileInfo.fileName();
      case 1: { int sz = round(fileInfo.size()/1000); if(sz < 1) return "0"; else return sz;  }
      case 2: return fileInfo.birthTime();
      case 3: return fileInfo.absolutePath();
      default: return {};
      };
   }
   QVariant headerData(int section, Qt::Orientation orientation, int role) const override {
      if (orientation != Qt::Horizontal || role != Qt::DisplayRole) return {};
      switch (section) {
      case 0: return "File Name";
      case 1: return "Size";
      case 2: return "Creation Time";
      case 3: return "Directory";
      default: return {};
      }
   }
   void append(const QFileInfo & file) {
      beginInsertRows({}, m_data.count(), m_data.count());
      m_data.append(file);
      endInsertRows();
   }
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QFileInfoList files;
    FileModel model;
    int masterImageIdx;

protected:
    void dragEnterEvent(QDragEnterEvent *e);
    void dropEvent(QDropEvent *e);

public slots:
    void processFinished();
    void launchRegistration();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
