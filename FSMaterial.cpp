#include "FSMaterial.h"
#include "FSJsonIncluders.h"
#include "FSConstants.h"
#include "ComponentMaterial.h"
#include <iostream>
#include <fstream>
#include "Globals.h"

void FSMaterial::ExportMaterial(ComponentMaterial* material)
{
	const char json[] = "";

	Document document;
	document.SetObject();
	document.Parse(json);
	Document::AllocatorType& allocator = document.GetAllocator();

	Value materialValue(kObjectType);
	material->SerializeExport(materialValue, allocator);
	document.AddMember("material", materialValue, allocator);

	StringBuffer buffer;
	//PrettyWriter<StringBuffer> writer(buffer);	// -> beautified version
	Writer<StringBuffer> writer(buffer);			// -> minified version
	document.Accept(writer);

	std::ofstream myfile;
	myfile.open(PATH_LIBRARY_MATERIALS + material->GetName() + FORMAT_MATERIAL);
	LOG("MAT EXPORTED %s", material->GetName().c_str());
	myfile << buffer.GetString();
	myfile.close();

	ImportMaterial("./Library/Materials/EyesMaterial.material");
	


}

ComponentMaterial* FSMaterial::ImportMaterial(std::string path)
{
	/*const char json[] = "{\"material\":{}}";
	document.Parse(json);*/



	return nullptr;
}
