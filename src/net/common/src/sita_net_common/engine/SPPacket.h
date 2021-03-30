#pragma once
#include "NEPacket.h"

namespace sita {
	enum class SPPacketType : uint16_t {
		None,
		Hello,
		Chat,
	};


	class SPPacket: NEPacket {
	public:
		using Type = SPPacketType;
		using PacketSize = uint32_t;

		static const size_t kMaxPacketSize = 32 * 1024;

		void toBuffer(std::vector<u8>& buf) {
			buf.clear();
			BinSerializer se(buf);
			onToBuffer(se);

			PacketSize packetSize = 0;
			{
				 se.buf();
				auto s = se.buf()->size();
				if (s > kMaxPacketSize)
					throw SITA_ERROR("excess MaxPacketSize");
				packetSize = static_cast<PacketSize>(s);
			}

			{
				// update the header size in buf
				auto* p = reinterpret_cast<PacketSize*>(se.buf()->data());
				*p = LittleEndian::FromHost::get(packetSize);
			}
		}

		void fromBuffer(const std::vector<u8>& buf) {
			BinDeserializer de(buf);
			onFromBuffer(de);
		}

		Type type() const { return onType(); }

	private:
		virtual Type onType() const = 0;
		virtual void onToBuffer(BinSerializer& se) = 0;
		virtual void onFromBuffer(BinDeserializer& se) = 0;

		void io(BinSerializer& se) {
			PacketSize size = 0;
			se.io_fixed(size);

			Type t = type();
			se.io_fixed(enum_int_ref(t));
		}

		void io(BinDeserializer& se) {
			PacketSize	size = 0;
			se.io_fixed(size);

			Type		t = Type::None;
			se.io_fixed(enum_int_ref(t));
			if (t != type())
				throw SITA_ERROR("Packet type mismatch");
		}
	};


	//class SPPacket_Hello : public SPPacket {
	//	using Base = SPPacket;
	//public:
	//	uint32_t version = 100;

	//protected:
	//	virtual Type onType() const { return Type::Hello; }
	//	virtual void onToBuffer(BinSerializer& se) override { io(se); }
	//	virtual void onFromBuffer(BinDeserializer& se) override { io(se); }

	//	template<typename SE>
	//	void io(SE& se) {
	//		Base::io(se);
	//		se.io(version);
	//	}
	//};

	//class SPPacket_Chat : public SPPacket {
	//	using Base = SPPacket;
	//public:
	//	std::string msg;
	//	std::vector< std::string > toUsers;

	//protected:
	//	virtual Type onType() const { return Type::Chat; }
	//	virtual void onToBuffer(BinSerializer& se) override { io(se); }
	//	virtual void onFromBuffer(BinDeserializer& se) override { io(se); }

	//	template<typename SE>
	//	void io(SE& se) {
	//		Base::io(se);
	//		se.io(msg);
	//		se.io(toUsers);
	//	}
	//};

} // namespace
