#include "credential.h"

class CredentialData : public QSharedData
{
public:
	CredentialData(QString &&key, QString &&value, bool confidential) :
		key{std::move(key)},
		value{std::move(value)},
		confidential{confidential}
	{}
	CredentialData(const CredentialData &other) = default;

	QString key;
	QString value;
	bool confidential;
};

Credential::Credential(QString key, QString value, bool confidential) :
	d{new CredentialData{std::move(key), std::move(value), confidential}}
{}

void Credential::filterOutEmpty(QList<Credential> &credentials)
{
	for(auto it = credentials.begin(); it != credentials.end();) {
		if(it->isValid())
			++it;
		else
			it = credentials.erase(it);
	}
}

bool Credential::isValid() const
{
	return !d->key.isEmpty();
}

Credential::operator bool() const
{
	return isValid();
}

bool Credential::operator!() const
{
	return !isValid();
}

Credential::Credential(const Credential &other) = default;

Credential::Credential(Credential &&other) noexcept = default;

Credential &Credential::operator=(const Credential &other) = default;

Credential &Credential::operator=(Credential &&other) noexcept = default;

Credential::~Credential() = default;

QString Credential::key() const
{
	return d->key;
}

QString Credential::value() const
{
	return d->value;
}

bool Credential::confidential() const
{
	return d->confidential;
}

void Credential::setKey(QString key)
{
	d->key = std::move(key);
}

void Credential::setValue(QString value)
{
	d->value = std::move(value);
}

void Credential::setConfidential(bool confidential)
{
	d->confidential = confidential;
}

bool Credential::operator==(const Credential &other) const
{
	return d == other.d || (
				d->key == other.d->key &&
				d->value == other.d->value &&
				d->confidential == other.d->confidential);
}

bool Credential::operator!=(const Credential &other) const
{
	return d != other.d && (
				d->key != other.d->key ||
				d->value != other.d->value ||
				d->confidential != other.d->confidential);
}

QDataStream &operator<<(QDataStream &stream, const Credential &credential)
{
	stream << credential.d->key
		   << credential.d->value
		   << credential.d->confidential;
	return stream;
}

QDataStream &operator>>(QDataStream &stream, Credential &credential)
{
	stream >> credential.d->key
		   >> credential.d->value
		   >> credential.d->confidential;
	return stream;
}

QDebug operator<<(QDebug stream, const Credential &credential)
{
	QDebugStateSaver state{stream};
	stream.nospace().noquote() << "{"
							   << credential.key() << ": "
							   << (credential.confidential() ?
									   QString{credential.value().size(), QLatin1Char('*')} :
									   credential.value())
							   << "}";
	return stream;
}
