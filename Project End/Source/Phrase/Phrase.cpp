#include "Phrase.h"


Phrase::Phrase()
	: phrase(), possible_responses(), modified()
{}


Phrase::Phrase(const Phrase& other)
	: phrase(other.phrase), possible_responses(other.possible_responses), modified(other.modified)
{}


Phrase::Phrase(std::string p, std::vector<Response*>* pr, bool m)
	: phrase(p), possible_responses(pr), modified(m)
{}


Phrase::~Phrase()
{
	phrase.clear();
	
	possible_responses->clear();
	delete possible_responses;
	possible_responses = nullptr;
}


bool Phrase::Add(Response* r)
{
	// Check through pr and make sure the response being added isn't already added.
	for (unsigned int i = 0; i < possible_responses->size(); ++i)
	{
		if (r->GetResponse() == possible_responses->at(i)->GetResponse())
		{
			/* Handle mem leaks. */
			delete r;
			r = nullptr;
			return false;
		}
	}

	possible_responses->push_back(r);
	return true;
}


bool Phrase::Add(std::string s)
{
	// Check to make sure the string isn't a response already.
	for (unsigned int i = 0; i < possible_responses->size(); ++i)
	{
		if (s == possible_responses->at(i)->GetResponse())
		{
			return false;
		}
	}

	possible_responses->push_back(new Response(s));
	return true;
}


void Phrase::Remove(std::string r)
{
	for (unsigned int i = 0; i < possible_responses->size(); ++i)
	{
		if (r == possible_responses->at(i)->GetResponse())
		{
			possible_responses->erase(possible_responses->begin() + i);
			break;
		}
	}
}


std::string Phrase::GetPhrase() const
{
	return phrase;
}


void Phrase::SetPhrase(std::string p)
{
	phrase = p;
}


std::vector<Response*>& Phrase::GetResponseList() const
{
	return *possible_responses;
}


void Phrase::SetResponseList(std::vector<Response*>* list)
{
	possible_responses = list;
}


bool Phrase::Modified()
{
	return modified;
}
