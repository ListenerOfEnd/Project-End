#include "Response.h"


Response::Response()
	: r()
{}


Response::Response(const Response& other)
	: r(other.r)
{}


Response::Response(std::string r)
	: r(r)
{}


Response::~Response()
{
	r.clear();
}


std::string Response::GetResponse() const
{
	return r;
}


void Response::SetResponse(std::string res)
{
	r = res;
}
