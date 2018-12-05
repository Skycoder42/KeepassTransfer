#include "kptrootwizard.h"
#include <QStringListModel>
#include <QIcon>
#include <QDebug>

namespace {

class ScopedBlocker
{
	Q_DISABLE_COPY(ScopedBlocker)
public:
	inline ScopedBlocker(bool &target) :
		_target{target} {
		_target = true;
	}
	inline ~ScopedBlocker() {
		_target = false;
	}
private:
	bool &_target;
};

}

KptRootWizard::KptRootWizard(QtMvvm::ViewModel *viewModel, QWidget *parent) :
	QWizard{parent, Qt::Window | Qt::WindowCloseButtonHint | Qt::WindowMinMaxButtonsHint},
	_viewModel{static_cast<KptRootViewModel*>(viewModel)}
{
#ifdef Q_OS_LINUX
	setWizardStyle(ModernStyle);
#endif

	setOptions(NoBackButtonOnStartPage |
			   HaveHelpButton |
			   HaveCustomButton1);
	setPixmap(LogoPixmap, windowIcon().pixmap(windowHandle(), {64, 64}));

	setButtonText(FinishButton, tr("Transfer"));
	setButtonText(HelpButton, tr("About"));
	setButtonText(CustomButton1, tr("Settings"));
	connect(this, &KptRootWizard::helpRequested,
			_viewModel, &KptRootViewModel::about);
	connect(this, &KptRootWizard::customButtonClicked,
			this, &KptRootWizard::customAction);
}

bool KptRootWizard::tryPresent(QWidget *view)
{
	auto page = qobject_cast<QWizardPage*>(view);
	if(!page)
		return false;

	const auto needsRestart = _pageStack.isEmpty();
	_pageStack.push(++_pageCounter);
	setPage(_pageCounter, page);
	if(needsRestart) {
		setStartId(_pageCounter);
		restart();
	} else
		next();
	return true;
}

bool KptRootWizard::validateCurrentPage()
{
	/*if(currentPage() && currentPage()->isFinalPage())
		return QWizard::validateCurrentPage();
	else*/ if(currentId() == topStack()) {
		QWizard::validateCurrentPage();
		return false;
	} else
		return true;
}

int KptRootWizard::nextId() const
{
	if(_recurseBlocker)
		return 0;

	ScopedBlocker _{_recurseBlocker};
	if(currentPage() && currentPage()->isFinalPage())
		return -1;
	else
		return nextStackId();
}

void KptRootWizard::initializePage(int id)
{
	qDebug() << Q_FUNC_INFO << id;
	if(id != topStack())
		qWarning() << "Wizard corrupted - invalid id in initializePage - can crash anytime now";
	QWizard::initializePage(id);
}

void KptRootWizard::cleanupPage(int id)
{
	qDebug() << Q_FUNC_INFO << id;
	if(id != topStack())
		qWarning() << "Wizard corrupted - invalid id in cleanupPage - can crash anytime now";

	_pageStack.pop();
	QWizard::cleanupPage(id);
	QMetaObject::invokeMethod(this, "dropPage", Qt::QueuedConnection,
							  Q_ARG(int, id));
}

void KptRootWizard::dropPage(int id)
{
	auto cPage = page(id);
	removePage(id);
	cPage->deleteLater();
}

void KptRootWizard::customAction(int button)
{
	switch (button) {
	case CustomButton1:
		_viewModel->showSettings();
		break;
	default:
		break;
	}
}

int KptRootWizard::topStack() const
{
	return _pageStack.isEmpty() ? 0 : _pageStack.top();
}

int KptRootWizard::nextStackId() const
{
	if(_pageStack.isEmpty())
		return 0;

	auto sIndex = _pageStack.indexOf(currentId());
	if(sIndex < 0)
		return _pageStack.first();
	else if(sIndex == _pageStack.size() - 1)
		return 0;
	else
		return _pageStack.value(sIndex + 1);
}
