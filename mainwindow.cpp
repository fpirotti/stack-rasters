#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

//    model.setHeaderData(0, Qt::Horizontal, QObject::tr("File"));
//    model.setHeaderData(1, Qt::Horizontal, QObject::tr("Size"));
//    model.setHeaderData(2, Qt::Horizontal, QObject::tr("CTime"));
//    model.setHeaderData(3, Qt::Horizontal, QObject::tr("Full Path"));

    ui->tableView->setModel(&model);

    QObject::connect( ui->pushButton , SIGNAL(released()),
                     this, SLOT(launchRegistration()));

}


void MainWindow::launchRegistration()
{
    if( model.rowCount() < 1 ){
        QMessageBox::warning(this,QString("Warning"),QString("Files not present: drag and drop files in program and select master image."),
                          QMessageBox::Ok);
        return;
    }
    ui->progressBar->setValue(0);

    QModelIndex selectedRow = ui->tableView->currentIndex();


    if( !selectedRow.isValid() ){
        QMessageBox::warning(this,QString("Warning"),QString("Select master image."),
                          QMessageBox::Ok);
        return;
    }

    QThread* thread = new QThread;
    Worker* workerThread = new Worker();
    workerThread->files = files;
    workerThread->selectedRow = selectedRow.row();
 //   this->ui->terminaButton->setEnabled(true);

    connect(workerThread, SIGNAL(finished()), workerThread, SLOT(deleteLater()));

    workerThread->moveToThread(thread);

//    connect(workerThread, SIGNAL(errMessage(QString, int)), this, SLOT(logit(QString, int)));
//    connect(workerThread, SIGNAL(resultReady(QStringList)), this, SLOT(setAddresses(QStringList)));
//    connect(workerThread, SIGNAL(valueChanged(int)),  this, SLOT(setProgress(int)));
    connect(workerThread, SIGNAL(finished()), this, SLOT(processFinished()));
//    connect(workerThread, SIGNAL(finished()), thread, SLOT(quit()));
    connect(thread, SIGNAL(started()), workerThread, SLOT(process()));
    thread->start();
}


void MainWindow::dragEnterEvent(QDragEnterEvent *e)
{
    if (e->mimeData()->hasUrls()) {
        e->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent *e)
{

    files.clear();
    // ui->tableView.mo;
    foreach (const QUrl &url, e->mimeData()->urls()) {
        qDebug() << url.path();
        model.append(QFileInfo(url.toLocalFile()));
       // files.append(QFileInfo(url.path()));
    }
}


void MainWindow::processFinished()
{
    ui->progressBar->setRange(0, 100);
    ui->progressBar->setValue(100);
    ui->textBrowser->append("Processo terminato");
}


MainWindow::~MainWindow()
{
    delete ui;
}



