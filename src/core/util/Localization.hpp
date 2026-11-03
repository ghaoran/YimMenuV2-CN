#pragma once
#include <string>
#include <string_view>

namespace YimMenu::Localization
{
	// Returns the Simplified Chinese text for `text` when a translation exists,
	// otherwise the original text unchanged. Never used for internal command names.
	const char* Tr(std::string_view text);

	// Convenience wrapper for std::string members (Submenu/Category names).
	std::string TrStr(std::string_view text);

	// Returns "Chinese##English" so ImGui keeps a unique, stable widget id while
	// displaying only the Chinese part. Result is cached; pointer stays valid.
	const char* Label(std::string_view english);

	// Same as Label(), but uses the command's internal name as the ImGui id
	// suffix. The internal name is unique per command and never translated, so
	// two commands sharing one Chinese translation still get distinct widget ids.
	const char* LabelFor(std::string_view internalName, std::string_view english);

	// Notification helper: keeps TR() out of lambda/constexpr-sensitive contexts.
	const char* TrNotif(std::string_view text);

	// Number of loaded entries.
	std::size_t EntryCount();
}

// For widgets whose label doubles as the ImGui id (buttons, inputs, combos...).
#define LBL(s) ::YimMenu::Localization::Label(s)

// For pure display strings (ImGui::Text, SetTooltip). ImGui::Text is a printf
// style call, so the text must not gain a "##id" suffix.
#define TR(s) ::YimMenu::Localization::Tr(s)
