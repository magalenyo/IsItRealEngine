#include "Component.h"
#include "GUIDGenerator.h"

Component::Component() : uid(GenerateUID()) {}

Component::~Component()
{
}
