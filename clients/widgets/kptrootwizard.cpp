#include "kptrootwizard.h"
#include <QStringListModel>
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
	setOptions(QWizard::IndependentPages |
			   QWizard::NoBackButtonOnStartPage |
			   QWizard::HaveHelpButton);
	setButtonText(QWizard::HelpButton, tr("About"));
	connect(this, &QWizard::helpRequested,
			_viewModel, &KptRootViewModel::about);
#ifdef Q_OS_LINUX
	setWizardStyle(ModernStyle);
#endif
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
	if(currentPage() && currentPage()->isFinalPage())
		return QWizard::validateCurrentPage();
	else if(currentId() == topStack()) {
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
	if(id != topStack())
		qWarning() << "Wizard corrupted - invalid id in initializePage - can crash anytime now";
	QWizard::initializePage(id);
}

void KptRootWizard::cleanupPage(int id)
{
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
