#include "kptrootviewmodel.h"
#include <credentialseditviewmodel.h>

KptRootViewModel::KptRootViewModel(QObject *parent) :
	ViewModel{parent}
{}

void KptRootViewModel::onInit(const QVariantHash &params)
{
	Q_UNUSED(params);
	show<CredentialsEditViewModel>();
}
