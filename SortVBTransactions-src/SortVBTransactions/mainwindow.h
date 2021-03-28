#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

	bool selectOutputPath();

public slots:
	void openTemplateTriggered();
	void openTransactionTriggered();
	void generateProcessedFile();
	void addItem();
	void removeItem();

private:
    Ui::MainWindow *ui;
	QString m_template_location;
	QString m_transaction_location;
	QString m_output_location;
};

#endif // MAINWINDOW_H
