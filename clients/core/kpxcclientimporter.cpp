#include "kpxcclientimporter.h"
#include <QtMvvmCore/Messages>
#include <kpxcclient/defaultdatabaseregistry.h>
using namespace KPXCClient;

KPXCClientImporter::KPXCClientImporter(QObject *parent) :
	QObject{parent},
	_client{new Client{this}}
{
	_client->setOptions(Client::Option::Default | Client::Option::DisconnectOnClose);
	static_cast<DefaultDatabaseRegistry*>(_client->databaseRegistry())->setPersistent(true);

	connect(_client, &Client::disconnected,
			this, &KPXCClientImporter::disconnected);
	connect(_client, &Client::databaseOpened,
			this, &KPXCClientImporter::databaseOpen);
	connect(_client, &Client::loginsReceived,
			this, &KPXCClientImporter::loginsReceived);
	connect(_client, &Client::errorOccured,
			this, &KPXCClientImporter::onError);
}

void KPXCClientImporter::setKpxcPath(QString path)
{
	_kpxcPath = std::move(path);
}

void KPXCClientImporter::importCredentials()
{
	if(_currentProgress)
		return;

	QtMvvm::getInput<QUrl>(tr("Import from KeePassXC"),
						   tr("Enter a URL to search entries for in KeePassXC:"),
						   this, [this](QUrl url, bool ok) {
		if(_currentProgress)
			return;

		if(ok) {
			_fetchUrl = std::move(url);
			_currentProgress = QtMvvm::showIndeterminateProgress(this,
																 tr("Importing from KeePassXC"),
																 tr("Connecting to KeePassXC instance…"));
			connect(_currentProgress, &QtMvvm::ProgressControl::canceled,
					this, &KPXCClientImporter::onCancel);

			switch(_client->state()) {
			case Client::State::Disconnected:
				if(_kpxcPath.isEmpty())
					_client->connectToKeePass();
				else
					_client->connectToKeePass(_kpxcPath);
				break;
			case Client::State::Locked:
				_client->openDatabase();
				break;
			case Client::State::Unlocked:
				databaseOpen();
				break;
			default:
				break;
			}
		}
	});
}

void KPXCClientImporter::databaseOpen()
{
	if(_currentProgress) {
		_currentProgress->updateLabel(tr("Searching entries for URL \"%1\" in KeePassXC…")
									  .arg(_fetchUrl.toString()));
		_client->getLogins(_fetchUrl);
	}
}

void KPXCClientImporter::loginsReceived(const QList<Entry> &entries)
{
	if(_currentProgress) {
		_currentProgress->close();
		_currentProgress.clear();

		if(entries.isEmpty()) {
			QtMvvm::warning(tr("Import from KeePassXC failed"),
							tr("Unable to find any entries for the given URL!"));
		} else if(entries.size() == 1) {
			convertEntry(entries.first());
		} else {
			QVariantList selectList;
			selectList.reserve(entries.size());
			auto cIndex = 0;
			for(const auto &entry : entries) {
				selectList.append(QVariantMap{
									  {QStringLiteral("name"), tr("%1 (%2)").arg(entry.username(), entry.title())},
									  {QStringLiteral("value"), cIndex++}
								  });
			}
			QtMvvm::getInput(tr("Import from KeePassXC"),
							 tr("Select the Entrie you would like to transfer:"),
							 "selection",
							 this, [this, entries](const QVariant &result){
				if(result.isValid())
					convertEntry(entries.at(result.toInt()));
			}, {}, {
				{QStringLiteral("listElements"), QVariant{selectList}}
			});
		}
	}
}

void KPXCClientImporter::disconnected()
{
	if(_currentProgress) {
		_currentProgress->close();
		_currentProgress.clear();

		QtMvvm::critical(tr("Import from KeePassXC failed"),
						 tr("The connection to KeePassXC was unexpectedly closed!"));
	}
}

void KPXCClientImporter::onError(Client::Error error, const QString &message, const QString &action, bool unrecoverable)
{
	qWarning().noquote() << "KPXCClientImporter Error:" << error
						 << "on action" << action;

	if(_currentProgress) {
		_currentProgress->close();
		_currentProgress.clear();

		QtMvvm::critical(tr("Import from KeePassXC failed"),
						 tr("Failed to import data with error: %1").arg(message));
	}

	if(!unrecoverable) // close anyways
		_client->disconnectFromKeePass();
}

void KPXCClientImporter::onCancel()
{
	if(_currentProgress) {
		_currentProgress->close();
		_currentProgress.clear();
	}
	_client->disconnectFromKeePass();
}

void KPXCClientImporter::convertEntry(const Entry &entry)
{
	QList<Credential> credentials;
	credentials.append(Credential{QStringLiteral("Title"), entry.title()});
	credentials.append(Credential{QStringLiteral("UserName"), entry.username()});
	credentials.append(Credential{QStringLiteral("Password"), entry.password(), true});
	if(!entry.totp().isEmpty())
		credentials.append(Credential{QStringLiteral("TOPT"), entry.totp(), true});

	const auto extraFields = entry.extraFields();
	for(auto it = extraFields.constBegin(); it != extraFields.constEnd(); ++it)
		credentials.append(Credential{it.key(), it.value()});
	emit credentialsImported(credentials);
}
