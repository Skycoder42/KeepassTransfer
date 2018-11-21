#include "mainviewmodel.h"

#include <QtMvvmCore/SettingsViewModel>

MainViewModel::MainViewModel(QObject *parent) :
	ViewModel(parent),
	_text(QStringLiteral("hello world"))
{}

QString MainViewModel::text() const
{
	return _text;
}

void MainViewModel::showSettings()
{
	show<QtMvvm::SettingsViewModel>();
}

void MainViewModel::setText(const QString &text)
{
	if (_text == text)
		return;

	_text = text;
	emit textChanged(_text);
}
