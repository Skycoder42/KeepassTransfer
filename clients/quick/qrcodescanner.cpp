#include "qrcodescanner.h"
#include <QDebug>
#ifdef Q_OS_ANDROID
#include <QtAndroidExtras/QAndroidJniObject>
#include <QtAndroidExtras/QtAndroid>
#endif

QPointer<QrCodeScanner> QrCodeScanner::_instance;

QrCodeScanner::QrCodeScanner(QObject *parent) :
	QObject{parent}
{
	Q_ASSERT(!_instance);
	_instance = this;
}

QrCodeScanner *QrCodeScanner::instance()
{
	return _instance;
}

bool QrCodeScanner::canScan() const
{
#ifdef Q_OS_ANDROID
	return true;
#else
	return false;
#endif
}

void QrCodeScanner::initiateScan()
{
#ifdef Q_OS_ANDROID
	QtAndroid::runOnAndroidThread([](){
		auto activity = QtAndroid::androidActivity();
		activity.callMethod<void>("startQrScan");
	});
#else
	Q_UNIMPLEMENTED();
#endif
}

#ifdef Q_OS_ANDROID

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT void JNICALL Java_de_skycoder42_kpt_KptActivity_reportScanResult(JNIEnv*, jobject, jstring result)
{
	const auto instance = QrCodeScanner::instance();
	if(instance) {
		QMetaObject::invokeMethod(instance, "scanResultReady", Qt::QueuedConnection,
								  Q_ARG(QString, QAndroidJniObject::fromLocalRef(result).toString()));
	} else
		qWarning() << "Received QR-Scanresult, but no QrCodeScanner instance is available to report the result to";
}

#ifdef __cplusplus
}
#endif

#endif
