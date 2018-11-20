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
		template <typename TMessage, typename TFunc>
		void addVisitor(const TFunc &visitor);
		template <typename TMessage, typename TClass, typename... TArgs>
		void addVisitor(TClass *object, void(TClass::*visitor)(TMessage, TArgs...), TArgs... args);
		void clearVisitors();

		void addFallbackVisitor(std::function<void(int)> visitor);
		template <typename TClass, typename... TArgs>
		void addFallbackVisitor(TClass *object, void(TClass::*visitor)(int, TArgs...), TArgs... args);

		bool visit(const QByteArray &message) const;

	private:
		void addVisitorImpl(int typeId, const std::function<void(QVariant)> &visitor);

		QHash<int, std::function<void(QVariant)>> _visitors;
		std::function<void(int)> _fallbackVisitor;
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

template<typename TMessage, typename TFunc>
void KPTLib::MessageVisitor::addVisitor(const TFunc &visitor)
{
	addVisitorImpl(qMetaTypeId<TMessage>(), [visitor](const QVariant &message){
		visitor(message.template value<TMessage>());
	});
}

template<typename TMessage, typename TClass, typename... TArgs>
void KPTLib::MessageVisitor::addVisitor(TClass *object, void(TClass::*visitor)(TMessage, TArgs...), TArgs... args)
{
	return addVisitor<typename std::decay<TMessage>::type>(std::bind(visitor, object, std::placeholders::_1, args...));
}

template<typename TClass, typename... TArgs>
void KPTLib::MessageVisitor::addFallbackVisitor(TClass *object, void(TClass::*visitor)(int, TArgs...), TArgs... args)
{
	return addFallbackVisitor(std::bind(visitor, object, std::placeholders::_1, args...));
}

#endif // KPTLIB_H
