#include "kptrootviewmodel.h"
#include <QtMvvmCore/Messages>
#include <QtMvvmCore/SettingsViewModel>
#include "kptsettingsviewmodel.h"

KptRootViewModel::KptRootViewModel(QObject *parent) :
	ViewModel{parent}
{}

void KptRootViewModel::showSettings()
{
	show<KptSettingsViewModel>();
}

void KptRootViewModel::about()
{
	QtMvvm::about(tr("A tool to securely transfer passwords and other credentials from any "
					 "device to a remote computer for easy access anywhere."),
				  QStringLiteral("https://github.com/Skycoder42/KeepassTransfer"),
				  tr("GPL-3.0"),
				  QStringLiteral("https://github.com/Skycoder42/KeepassTransfer/blob/master/LICENSE"),
				  {}, true, {},
				  tr("Attributions:<ul>"
					 "<li><a href=\"https://github.com/weidai11/cryptopp\">CryptoPP (BSL-1.0)</a></li>"
					 "<li><a href=\"https://github.com/nayuki/QR-Code-generator\">QR-Code-generator (MIT)</a></li>"
					 "<li><a href=\"https://github.com/iamantony/qtcsv\">qtcsv (MIT)</a></li>"
					 "<li><a href=\"https://github.com/PhilippC/keepass2android\">Keepass2Android Plugin SDK (GPL-3.0)</a></li>"
					 "<li><a href=\"https://material.io/tools/icons\">MaterialIO Icons (Apache-2.0)</a></li>"
					 "</ul>"));
}
