#include "Text.h"


Text::Text()
	: text(), modified()
{}


Text::Text(const Text& other)
	: text(other.text), modified(other.modified)
{}


Text::Text(std::string t)
	: text(t), modified()
{}


Text::~Text()
{
	text.clear();
}


std::string Text::GetText() const
{
	return text;
}


void Text::SetText(std::string t)
{
	text = t;
}


bool Text::Modified()
{
	return modified;
}


void Text::SetModified(bool m)
{
	modified = m;
}
