#include "transferloader.h"
#ifdef Q_OS_ANDROID
#include <QtAndroidExtras/QAndroidJniObject>
#include <QtAndroidExtras/QtAndroid>
#endif

QList<Credential> TransferLoader::loadCredentials()
{
#ifdef Q_OS_ANDROID
	auto activity = QtAndroid::androidActivity();
	auto entries = activity.callObjectMethod("getLastTransferEntries", "()Ljava/util/List;");
	if(entries.isValid()) {
		const auto size = entries.callMethod<jint>("size");
		QList<Credential> credentials;
		credentials.reserve(size);
		for(jint i = 0; i < size; ++i) {
			auto entry = entries.callObjectMethod("get", "(I)Ljava/lang/Object;",
												  i);
			Credential cred;
			cred.setKey(entry.getObjectField("key", "Ljava/lang/String;").toString());
			cred.setValue(entry.getObjectField("value", "Ljava/lang/String;").toString());
			cred.setConfidential(entry.getField<jboolean>("guarded"));
			credentials.append(cred);
		}
		return credentials;
	} else
		return {};
#else
	return {};
#endif
}
