#include "kptrootviewmodel.h"
#include <QtMvvmCore/Messages>
#include "credentialseditviewmodel.h"
#include "transferselectionviewmodel.h"

KptRootViewModel::KptRootViewModel(QObject *parent) :
	ViewModel{parent}
{}

void KptRootViewModel::about()
{
	QtMvvm::about(tr("A tool to securely transfer passwords and other credentials from any "
					 "device to a remote computer for easy access anywhere."),
				  QStringLiteral("https://github.com/Skycoder42/KeepassTransfer"),
				  tr("GPL v3"),
				  QStringLiteral("https://github.com/Skycoder42/KeepassTransfer/blob/master/LICENSE"),
				  {}, true, {},
				  tr("Attributions:<ul>"
					 "<li><a href=\"https://github.com/weidai11/cryptopp\">CryptoPP (BSL-1.0)</a></li>"
					 "<li><a href=\"https://github.com/nayuki/QR-Code-generator\">QR-Code-generator (MIT)</a></li>"
					 "<li><a href=\"https://material.io/tools/icons\">MaterialIO Icons (Apache-2.0)</a></li>"
					 "</ul>"));
}

void KptRootViewModel::onInit(const QVariantHash &params)
{
	Q_UNUSED(params);
	if(params.contains(TransferSelectionViewModel::paramCred))
		show<TransferSelectionViewModel>(params);
	else
		show<CredentialsEditViewModel>();
}
