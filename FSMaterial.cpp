#include "FSMaterial.h"
#include "Application.h"
#include "SceneImporter.h"
#include "FSJsonIncluders.h"
#include "ComponentMaterial.h"
#include <iostream>
#include <fstream>
#include "Globals.h"

#include "MemoryLeakDetector.h"

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
	myfile.open(material->GetSerializedName());
	//LOG("MAT EXPORTED %s", material->GetSerializedName().c_str()); -> this generates mem leaks
	myfile << buffer.GetString();
	myfile.close();

}

ComponentMaterial* FSMaterial::ImportMaterial(std::string path)
{
	ComponentMaterial* result = nullptr;

    std::ifstream file(path);
    if (file.is_open()) {
		std::string aux; getline(file, aux);

		Document document;
		document.SetObject();
		document.Parse(aux.c_str());
		Value& material = document["material"];
		if (material.IsObject() && !material.ObjectEmpty()) {
			result = ComponentMaterial::Deserialize(material);	
		}

		file.close();
    }
    else {
		LOG("[ERROR] Cannot open file: %s", path.c_str());
    }

	return result;
}