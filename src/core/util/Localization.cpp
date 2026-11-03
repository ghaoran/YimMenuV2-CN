#include "Localization.hpp"

#include "LocalizationData.hpp"

#include <algorithm>
#include <string>
#include <unordered_map>

namespace YimMenu::Localization
{
	namespace
	{
		constexpr bool Less(const Entry& e, std::string_view key)
		{
			return e.en < key;
		}

		const Entry* Find(std::string_view text)
		{
			auto begin = std::begin(kEntries);
			auto end = begin + kEntryCount;
			auto it = std::lower_bound(begin, end, text, Less);
			if (it != end && it->en == text)
				return &*it;
			return nullptr;
		}

		// Cache for Label()/LabelFor(). Bounded by the number of distinct labels,
		// so it never grows without limit. Needed because ImGui keeps the pointer.
		std::unordered_map<std::string, std::string>& Cache()
		{
			static std::unordered_map<std::string, std::string> cache;
			return cache;
		}

		const char* CacheComposite(const std::string& key, std::string&& value)
		{
			auto& cache = Cache();
			auto it = cache.find(key);
			if (it != cache.end())
				return it->second.c_str();

			return cache.emplace(key, std::move(value)).first->second.c_str();
		}
	}

	const char* Tr(std::string_view text)
	{
		if (text.empty())
			return text.data();

		// ImGui internal identifiers ("##suffix") must keep their original ASCII form;
		// translating them would change widget identity.
		if (text.find("##") != std::string_view::npos)
			return text.data();

		if (auto entry = Find(text))
			return entry->zh.data();

		return text.data();
	}

	std::string TrStr(std::string_view text)
	{
		if (auto entry = Find(text))
			return std::string(entry->zh);

		return std::string(text);
	}

	const char* Label(std::string_view english)
	{
		if (english.find("##") != std::string_view::npos)
			return english.data();

		auto entry = Find(english);
		if (!entry)
			return english.data();

		std::string value;
		value.reserve(entry->zh.size() + english.size() + 3);
		value.append(entry->zh);
		value.append("##");
		value.append(english);

		return CacheComposite(std::string(english), std::move(value));
	}

	const char* LabelFor(std::string_view internalName, std::string_view english)
	{
		auto entry = Find(english);
		if (!entry)
			return english.data();

		std::string key(internalName);
		key.push_back('\x1f');
		key.append(english);

		std::string value;
		value.reserve(entry->zh.size() + internalName.size() + 3);
		value.append(entry->zh);
		value.append("##");
		value.append(internalName);

		return CacheComposite(key, std::move(value));
	}

	std::size_t EntryCount()
	{
		return kEntryCount;
	}
}
