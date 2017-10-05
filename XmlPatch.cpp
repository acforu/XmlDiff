#include "StdAfx.h"
#include "XmlPatch.h"


XmlPatch::XmlPatch()
{
}


XmlPatch::~XmlPatch()
{
}

bool XmlPatch::LoadXml(const string& filename)
{
	srcXmlFile = make_shared<XmlFile>();
	return srcXmlFile->Parse(filename);
}

bool XmlPatch::ApplyPatch()
{
	if (!srcXmlFile)
		return false;
	

	//srcXmlFile->doc.first_node()
	return true;
}
