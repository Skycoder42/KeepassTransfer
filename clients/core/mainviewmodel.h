#ifndef MAINVIEWMODEL_H
#define MAINVIEWMODEL_H

#include <QtMvvmCore/ViewModel>

class MainViewModel : public QtMvvm::ViewModel
{
	Q_OBJECT

	Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)

public:
	Q_INVOKABLE explicit MainViewModel(QObject *parent = nullptr);

	QString text() const;

public Q_SLOTS:
	void showSettings();

	void setText(const QString &text);

Q_SIGNALS:
	void textChanged(const QString &text);

private:
	QString _text;
};

#endif // MAINVIEWMODEL_H
