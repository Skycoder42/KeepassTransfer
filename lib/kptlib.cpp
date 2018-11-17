#include "kptlib.h"

const quint32 KPTLib::ProtocolVersion = 0x01;

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
						prop.readOnGadget(&message).constData());
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

	return message;
}

// visitor

bool KPTLib::MessageVisitor::visit(const QVariant &message) const
{
	auto visitor = _visitors.value(message.userType());
	if(visitor) {
		visitor(message);
		return true;
	} else
		return false;
}

void KPTLib::MessageVisitor::addVisitorImpl(int typeId, const std::function<void (QVariant)> &visitor)
{
	_visitors.insert(typeId, visitor);
}
