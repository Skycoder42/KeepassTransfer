#ifndef CREDENTIAL_H
#define CREDENTIAL_H

#include <QObject>
#include <QSharedData>

class CredentialData;
class Credential
{
	Q_GADGET

	Q_PROPERTY(bool valid READ isValid STORED false)

	Q_PROPERTY(QString key READ key WRITE setKey USER true)
	Q_PROPERTY(QString value READ value WRITE setValue)
	Q_PROPERTY(bool confidential READ confidential WRITE setConfidential)

public:
	Credential(QString key = {}, QString value = {}, bool confidential = false);
	Credential(const Credential &other);
	Credential(Credential &&other) noexcept;
	Credential &operator=(const Credential &other);
	Credential &operator=(Credential &&other) noexcept;
	~Credential();

	bool isValid() const;
	explicit operator bool() const;
	bool operator!() const;

	QString key() const;
	QString value() const;
	bool confidential() const;

	void setKey(QString key);
	void setValue(QString value);
	void setConfidential(bool confidential);

	bool operator==(const Credential &other) const;
	bool operator!=(const Credential &other) const;

private:
	QSharedDataPointer<CredentialData> d;
};

Q_DECLARE_METATYPE(Credential)
Q_DECLARE_TYPEINFO(Credential, Q_MOVABLE_TYPE);

#endif // CREDENTIAL_H
