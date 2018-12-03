#ifndef TRANSFERLOADER_H
#define TRANSFERLOADER_H

#include <credential.h>

class TransferLoader
{
	Q_DISABLE_COPY(TransferLoader)

public:
	TransferLoader() = delete;

	static QList<Credential> loadCredentials();
};

#endif // TRANSFERLOADER_H
