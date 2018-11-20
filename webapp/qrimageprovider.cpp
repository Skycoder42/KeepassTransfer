#include "qrimageprovider.h"
#include <QUrl>
#include <QUrlQuery>
#include <QSvgRenderer>
#include <QGuiApplication>
#include <QScreen>
#include <QPainter>
#include <QDebug>
#include <QrCode.hpp>
using namespace qrcodegen;

QrImageProvider::QrImageProvider() :
	QQuickImageProvider{QQmlImageProviderBase::Image}
{}

QImage QrImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
	QUrl requestUrl{QStringLiteral("qrcode:///") + id};
	const auto imageData = requestUrl.path().mid(1);
	const QUrlQuery params{requestUrl.query()};
	const auto eccLevel = params.queryItemValue(QStringLiteral("eccLevel"));
	const auto border = params.hasQueryItem(QStringLiteral("border")) ?
							params.queryItemValue(QStringLiteral("border")).toInt() :
							1;

	QrCode::Ecc ecc;
	if(eccLevel == QLatin1Char('L'))
		ecc = QrCode::Ecc::LOW;
	else if(eccLevel == QLatin1Char('M'))
		ecc = QrCode::Ecc::MEDIUM;
	else if(eccLevel == QLatin1Char('Q'))
		ecc = QrCode::Ecc::QUARTILE;
	else if(eccLevel == QLatin1Char('H'))
		ecc = QrCode::Ecc::HIGH;
	else
		ecc = QrCode::Ecc::LOW;

	const auto qrCode = QrCode::encodeText(qUtf8Printable(imageData), ecc);
	QSvgRenderer renderer{QByteArray::fromStdString(qrCode.toSvgString(border))};

	if(requestedSize.isValid())
		*size = requestedSize;
	else {
		*size = renderer.defaultSize();
		*size *= QGuiApplication::primaryScreen()->devicePixelRatio();
	}

	QImage image{*size, QImage::Format_Mono};
	image.fill(Qt::transparent);
	QPainter painter{&image};
	renderer.render(&painter);

	return image;
}
