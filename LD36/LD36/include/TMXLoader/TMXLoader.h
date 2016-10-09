#pragma once

#include "RapidXML\rapidxml.hpp"

using namespace std;
using namespace rapidxml;

struct MOBjectProperty
{//Object property
	string Name;
	string Value;
};

struct MTileProperty : MOBjectProperty
{//Tile property
};

struct MTile
{
	MTileProperty Prop; //Could possibly be a vector
	Int32 ID;
};

struct MTileset
{//Map Tileset
	string Name_;
	string FileName_;
	Int32 TileWidth_;
	Int32 TileHeight_;
	Int32 TileCount_;
	Int32 FirstGID_;
	vector<MTile>Tile;
	string GetTilePropertyName(Int32 tileID)
	{//Will return a tile's property based on it's ID
		string t("");
		bool found(false);
		Int32 counter(0);
		while (counter < static_cast<Int32> (Tile.size()) && !found)
		{
			if (Tile[counter].ID == tileID)
			{
				found = true;
				t = Tile[counter].Prop.Name;
			}
			++counter;
		}
		return(t);

	}
	string GetTilePropertyValue(Int32 tileID)
	{
		string t("");
		bool found(false);
		Int32 counter(0);
		while (counter < static_cast<Int32> (Tile.size()) && !found)
		{
			if (Tile[counter].ID == tileID)
			{
				found = true;
				t = Tile[counter].Prop.Value;
			}
			++counter;
		}
		return(t);
	}
};

struct MLayer
{
	string Name;
	Int32 Width;
	Int32 Height;
	vector<vector<Int32>> Data;
};

struct MObject
{
	MObject(Int32 id, Int32 x, Int32 y, Int32 width, Int32 height) :
		ID(id), X(x), Y(y), Width(width), Height(height)
	{

	}
	Int32 ID;
	Int32 X;
	Int32 Y;
	Int32 Width;
	Int32 Height;
	std::vector<MOBjectProperty> Properties;
};

struct MObjectGroup
{//Object layer
	string Name;
	vector<MObject> Objects;
};


class TMXLoader
{
public:

	~TMXLoader();
	
	const vector<MTileset*>& getTilesets() const;
	
	bool LoadMap(const std::string&);
	
	Int32 GetWidth() const { return Width_; }
	
	Int32 GetHeight() const { return Height_; }
	
	Int32 GetTileWidth() const { return TileWidth_; }
	
	Int32 GetTileHeight() const { return TileHeight_; }
	
	Int32 GetLayerCount() const { return Layers_.size(); }
	
	Int32 GetTilesetCount() const { return Tilesets_.size(); }
	
	//Returns the total number of object groups (object layers) in the tiled map
	Int32 GetObjectGroupCount() const { return ObjectGroups_.size(); }
	
	const vector<MLayer*>& GetLayer() const { return(Layers_); }
	
	const vector< MTileset*>& GetTileSet() const { return(Tilesets_); }
	
	MObjectGroup GetObjectGroup(Int32) const; //Param: id of the objectgroup 

private:

	string TMXToString(const std::string&);
	
	void SetupTileMap();
	
	void SetupTilesets(xml_node<>*);
	
	void SetupLayer(xml_node<>*);
	
	void SetupObjectGroups(xml_node<>*);
	
	Int32 Width_;  //Map width
	Int32 Height_; //Map height
	Int32 TileWidth_;
	Int32 TileHeight_;

	vector <MTileset*> Tilesets_;
	vector <MLayer*> Layers_;
	vector<MObjectGroup> ObjectGroups_;
	xml_document<> Doc_;
};