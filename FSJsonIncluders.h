#pragma once

#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/prettywriter.h" // for stringify JSON

using namespace rapidjson;

#include <string>

/* CONSTANT KEYS TO IMPORT / EXPORT GAMEOBJECTS AND COMPONENTS */

/************************ GAMEOBJECT ***************************/
const std::string S_GAMEOBJECT_UID				= "uid";
const std::string S_GAMEOBJECT_NAME				= "name";
const std::string S_GAMEOBJECT_ENABLED			= "enabled";
const std::string S_GAMEOBJECT_COMPONENTLIST	= "components";
const std::string S_GAMEOBJECT_GAMEOBJECTLIST	= "gameObjects";
/************************* GAMEOBJECT ***************************/

/************************* COMPONENT ***************************/

/************************* COMPONENT ***************************/

/************************* MATERIAL ***************************/
/************************* MATERIAL ***************************/

/************************* TEXTURE ****************************/

//const std::string S_TEXTURE_WIDTH				= "width";
//const std::string S_TEXTURE_HEIGHT				= "height";
//const std::string S_TEXTURE_TYPE				= "type";
//const std::string S_TEXTURE_TEXTUREPATH			= "texturePath";
//const std::string S_TEXTURE_TEXTURETYPE			= "textureType";

/************************* TEXTURE ****************************/



/* CONSTANT KEYS TO IMPORT / EXPORT GAMEOBJECTS AND COMPONENTS */