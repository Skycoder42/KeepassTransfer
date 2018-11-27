#include "qrcodescanner.h"
#ifdef Q_OS_ANDROID
#include <QtAndroidExtras/QAndroidJniObject>
#include <QtAndroidExtras/QtAndroid>
#endif

QrCodeScanner::QrCodeScanner(QObject *parent) :
	QObject{parent}
{}

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
void JNICALL de_skycoder42_kpt_KptActivity_reportScanResult(JNIEnv*, jobject, jstring result)
{
	auto resStr = QAndroidJniObject::fromLocalRef(result);
	qDebug(qUtf8Printable(resStr.toString()));
}
#endif
