#include "stdafx.h"
#include "TMXLoader\TMXLoader.h"


TMXLoader::~TMXLoader()
{

	for (size_t i(0); i < Tilesets_.size(); ++i)
		delete Tilesets_[i];

	for (size_t i(0); i < Layers_.size(); ++i)
		delete Layers_[i];
}

bool TMXLoader::LoadMap(const string& filename)
{
	assert(filename.size() > 3);

	std::string map = TMXToString(filename);
	if (map.size() < 1)
		return(false);

	//cout << map << endl;
	char* a = new char[map.size() + 1];
	strcpy_s(a, map.size() + 1, map.c_str());

	Doc_.parse<0>(a);

	SetupTileMap();

	delete[] a;
	return(true);
}

string TMXLoader::TMXToString(const string& filename)
{
	string map; //Will contain read in data from the .tmx
	ifstream fromMap;

	fromMap.open(filename, ios::in);

	if (fromMap.fail())
	{//failure to open file
		cout << "Error! Failed to open: " << filename << endl;
		return(" "); //Will cause loadMap to return false
	}
	else
	{
		string temp;
		while (!fromMap.eof()) //While not at the end of a line
		{//fill the temp string with data
			getline(fromMap, temp);
			map += temp; //and append it onto the map string
		}
	}

	return(map);
}

void TMXLoader::SetupTileMap()
{
	std::istringstream is;

	//Find the map node in TMX file
	xml_node<>* mapNode = Doc_.first_node("map");
	//set the maps with to the value of this attibute
	is.str(mapNode->first_attribute("width")->value());
	is >> Width_;

	is.clear();
	is.str(mapNode->first_attribute("height")->value());
	is >> Height_; //set map height 

	is.clear();
	is.str(mapNode->first_attribute("tilewidth")->value());
	is >> TileWidth_;

	is.clear();
	is.str(mapNode->first_attribute("tileheight")->value());
	is >> TileHeight_;

	is.clear();

	SetupTilesets(mapNode);

	SetupLayer(mapNode);

	SetupObjectGroups(mapNode);

}

void TMXLoader::SetupTilesets(xml_node<>* mapNode)
{
	istringstream is;

	xml_node<>* tilesetNode = mapNode->first_node("tileset"); //the node which contains the details of tilesets
	xml_node<>* tileNode = tilesetNode->first_node("tile"); //tile information node

	Int32 counter(0);

	while (tilesetNode)
	{
		Tilesets_.push_back(new MTileset);
		//setting first grid
		is.clear();
		is.str(tilesetNode->first_attribute("firstgid")->value());
		is >> Tilesets_[counter]->FirstGID_; //set the tilsets first grid value

		Tilesets_[counter]->Name_ = tilesetNode->first_attribute("name")->value(); // set the tiles name
		Tilesets_[counter]->TileWidth_ = TileWidth_; //set tileWidth for the tileset
		Tilesets_[counter]->TileHeight_ = TileHeight_; //set tileHeight for the tileset

		is.clear();
		is.str(tilesetNode->first_attribute("tilecount")->value());
		is >> Tilesets_[counter]->TileCount_;


		while (tileNode)
		{//initialising the tile properties for each tileset
			MTile t;

			is.clear();
			is.str(tileNode->first_attribute("id")->value());
			is >> t.ID;

			xml_node<>* tileProp = tileNode->first_node("properties")->first_node("property");

			while (tileProp)
			{

				t.Prop.Name = tileProp->first_attribute("name")->value();
				t.Prop.Value = tileProp->first_attribute("value")->value();

				tileProp = tileProp->next_sibling("property");
			}

			Tilesets_[counter]->Tile.push_back(t);
			tileNode = tileNode->next_sibling("tile");
		}



		++counter;
		tilesetNode = tilesetNode->next_sibling("tileset");
	}
	cout << Tilesets_[0]->Tile.size() << endl;
}

void TMXLoader::SetupLayer(xml_node<>* mapNode)
{
	xml_node<>* layerNode = mapNode->first_node("layer");  //XML Node containing layer data
	istringstream is;

	Int32 counter(0);

	while (layerNode != nullptr)
	{
		Layers_.push_back(new MLayer);

		Layers_[counter]->Name = layerNode->first_attribute("name")->value();

		is.str(layerNode->first_attribute("width")->value());
		is >> Layers_[counter]->Width;

		is.clear();
		is.str(layerNode->first_attribute("height")->value());
		is >> Layers_[counter]->Height;

		string layerData = layerNode->first_node("data")->value(); //get the map data from the layer
		vector<Int32> a(Width_); //a temporary vector we'll use as an example, will be pushed Int32o a vector
		Int32 stringCounter(0); //Used to keep track of where in the string layerData we're looking 

		is.clear();
		is.str(layerData);
		layerData.clear();
		istringstream stream;
		vector<Int32> b;

		while (is)
		{
			//string s;
			if (!getline(is, layerData, ',')) break;
			//layerData.append(s + " ");
			stream.clear();
			stream.str(layerData);
			Int32 val;
			stream >> val;
			b.push_back(val);
		}

		cout << "\n\n\n";
		//char temp[2];
		for (size_t y(0); y < static_cast<size_t>(Height_); ++y)
		{

			Layers_[counter]->Data.push_back(a);
			for (size_t x(0); x < static_cast<size_t>(Width_); ++x)
			{
				Layers_[counter]->Data[y][x] = b[x + (y * Width_)];
			}
		}
		layerNode = layerNode->next_sibling("layer");
		++counter;
	}

}

void TMXLoader::SetupObjectGroups(xml_node<>* mapNode)
{
	xml_node<>* objectGroupNode(mapNode->first_node("objectgroup")); //Get the object group node 
	xml_node<>* objectNode(nullptr); //PoInt32er to access object data
	xml_node<>* objectPropertyNode(nullptr); // PoInt32er to access the object's properties

	istringstream is;

	Int32 counter(0);

	while (objectGroupNode != nullptr) //Loop through all objectgroups on the map file
	{
		ObjectGroups_.push_back(MObjectGroup()); //Add a new object group 

		ObjectGroups_[counter].Name = objectGroupNode->first_attribute("name")->value(); //Set this new object groups name to a value

		objectNode = objectGroupNode->first_node("object");

		Int32 objCounter(0);

		while (objectNode != nullptr)
		{
			Int32 id, x(0), y(0), width(0), height(0);
			if (objectNode->first_attribute("id"))
			{
				is.str(objectNode->first_attribute("id")->value());
				is >> id;
				is.clear();
			}

			if (objectNode->first_attribute("x"))
			{
				is.str(objectNode->first_attribute("x")->value());
				is >> x;
				is.clear();
			}

			if (objectNode->first_attribute("y"))
			{
				is.str(objectNode->first_attribute("y")->value());
				is >> y;
				is.clear();
			}
			if (objectNode->first_attribute("width"))
			{
				is.str(objectNode->first_attribute("width")->value());
				is >> width;
				is.clear();
			}
			if (objectNode->first_attribute("height"))
			{
				is.str(objectNode->first_attribute("height")->value());
				is >> height;
			}
			ObjectGroups_[counter].Objects.push_back(MObject(id, x, y, width, height));


			objectPropertyNode = objectNode->first_node("properties");
			if (objectPropertyNode)
				objectPropertyNode = objectPropertyNode->first_node("property");

			while (objectPropertyNode)
			{
				MOBjectProperty prop;
				if (objectPropertyNode->first_attribute("name"))
					prop.Name = objectPropertyNode->first_attribute("name")->value();
				if (objectPropertyNode->first_attribute("value"))
					prop.Value = objectPropertyNode->first_attribute("value")->value();
				ObjectGroups_[counter].Objects[ObjectGroups_[counter].Objects.size() - 1].Properties.push_back(prop);

				std::cout << prop.Name << " - " << prop.Value << std::endl;
				objectPropertyNode = objectPropertyNode->next_sibling("property");
			}
			objectNode = objectNode->next_sibling("object");
			++objCounter;
		}

		objectGroupNode = objectGroupNode->next_sibling("objectgroup");//Get the next sibling with this name (Will return null if nothing is found)
		++counter;
	}
}

MObjectGroup TMXLoader::GetObjectGroup(Int32 id) const
{
	assert(id >= 0 && id < static_cast<Int32>(ObjectGroups_.size()));

	return(ObjectGroups_[id]);
}