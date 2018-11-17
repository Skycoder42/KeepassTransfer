#ifndef KPTLIB_H
#define KPTLIB_H

#include <QtGlobal>
#include <QMetaObject>
#include <QDataStream>
#include <QMetaProperty>

class KPTLib
{
public:
	class MessageVisitor
	{
	public:
		template <typename TMessage>
		void addVisitor(std::function<void(TMessage)> visitor);
		template <typename TClass, typename TMessage>
		void addVisitor(TClass *object, void(TClass::*visitor)(TMessage));

		bool visit(const QVariant &message) const;

	private:
		void addVisitorImpl(int typeId, const std::function<void(QVariant)> &visitor);

		QHash<int, std::function<void(QVariant)>> _visitors;
	};

	KPTLib() = delete;
	Q_DISABLE_COPY(KPTLib)

	static const quint32 ProtocolVersion;

	template <typename TMessage>
	static QByteArray serializeMessage(const TMessage &message);



private:
	static void setupStream(QDataStream &stream);

	static QByteArray serializeMessageImpl(const QVariant &message);
	static QVariant deserializeMessageImpl(const QByteArray &data);
};

template<typename TMessage>
QByteArray KPTLib::serializeMessage(const TMessage &message)
{
	return serializeMessageImpl(QVariant::fromValue(message));
}

template<typename TMessage>
void KPTLib::MessageVisitor::addVisitor(std::function<void(TMessage)> visitor)
{
	addVisitorImpl(qMetaTypeId<TMessage>(), [visitor](const QVariant &message){
		visitor(message.template value<TMessage>());
	});
}

template<typename TClass, typename TMessage>
void KPTLib::MessageVisitor::addVisitor(TClass *object, void (TClass::*visitor)(TMessage))
{
	return addVisitor(std::bind(visitor, object, std::placeholders::_1));
}

#endif // KPTLIB_H
