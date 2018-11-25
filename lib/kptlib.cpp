#include "kptlib.h"
#include "encrypteddata.h"
#include "credential.h"
#include "qrdata.h"

const quint32 KPTLib::ProtocolVersion = 0x01;

void KPTLib::setup()
{
	qRegisterMetaTypeStreamOperators<EncryptedData>();
	qRegisterMetaTypeStreamOperators<Credential>();
	qRegisterMetaTypeStreamOperators<QrData>();
}

void KPTLib::setupStream(QDataStream &stream)
{
	stream.setVersion(QDataStream::Qt_5_11);
}

QByteArray KPTLib::serializeMessageImpl(const QVariant &message)
{
	const auto typeId = message.userType();
	const auto metaObject = QMetaType::metaObjectForType(typeId);
	Q_ASSERT(metaObject && QMetaType::typeFlags(typeId).testFlag(QMetaType::IsGadget));

	QByteArray data;
	QDataStream stream{&data, QIODevice::WriteOnly};
	setupStream(stream);

	QByteArray msgName{metaObject->className()};
	msgName.resize(msgName.size() - 7); // truncate the "Message" part of the class name
	stream << msgName;

	for(auto i = 0; i < metaObject->propertyCount(); ++i) {
		auto prop = metaObject->property(i);
		if(!prop.isStored())
			continue;
		QMetaType::save(stream,
						prop.userType(),
						prop.readOnGadget(message.constData()).constData());
	}

	return data;
}

QVariant KPTLib::deserializeMessageImpl(const QByteArray &data)
{
	QDataStream stream{data};
	setupStream(stream);
	stream.startTransaction();

	QByteArray typeName;
	stream >> typeName;
	typeName += "Message"; //re-append the "Message" part of the class name
	const auto typeId = QMetaType::type(typeName.constData());
	if(typeId == QMetaType::UnknownType)
		return {};
	const auto metaObject = QMetaType::metaObjectForType(typeId);
	if(!metaObject || !QMetaType::typeFlags(typeId).testFlag(QMetaType::IsGadget))
		return {};

	QVariant message{typeId, nullptr};
	for(auto i = 0; i < metaObject->propertyCount(); ++i) {
		auto prop = metaObject->property(i);
		if(!prop.isStored())
			continue;
		const auto tId = prop.userType();
		QVariant tData(tId, nullptr);
		QMetaType::load(stream, tId, tData.data());
		prop.writeOnGadget(message.data(), tData);
	}

	if(stream.commitTransaction())
		return message;
	else
		return {};
}

// visitor

void KPTLib::MessageVisitor::clearVisitors()
{
	_visitors.clear();
}

void KPTLib::MessageVisitor::addFallbackVisitor(std::function<void(int)> visitor)
{
	_fallbackVisitor = std::move(visitor);
}

bool KPTLib::MessageVisitor::visit(const QByteArray &messageData) const
{
	const auto message = KPTLib::deserializeMessageImpl(messageData);
	auto visitor = _visitors.value(message.userType());
	if(visitor) {
		visitor(message);
		return true;
	} else {
		if(_fallbackVisitor)
			_fallbackVisitor(message.userType());
		return false;
	}
}

void KPTLib::MessageVisitor::addVisitorImpl(int typeId, const std::function<void (QVariant)> &visitor)
{
	_visitors.insert(typeId, visitor);
}
