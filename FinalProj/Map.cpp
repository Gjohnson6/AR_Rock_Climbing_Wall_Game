#include "Map.h"
#include <algorithm>
#include <functional>
#include "pugixml.hpp"

Map::Map()
{

}

Map::~Map()
{
}

vector<HitMarker> Map::getMarkers()
{
	return markers;
}

void Map::SaveMap(string filename)
{	
	pugi::xml_document doc;

	pugi::xml_node mapNode = doc.append_child("Map");
	pugi::xml_node markersNode = mapNode.append_child("Markers");

	for (auto marker : markers)
	{
		pugi::xml_node markerNode = markersNode.append_child("Marker");
		pugi::xml_attribute xAttr = markerNode.append_attribute("XPos");
		pugi::xml_attribute yAttr = markerNode.append_attribute("YPos");

		xAttr.set_value(marker.getX());
		yAttr.set_value(marker.getY());
	}

	filename.append(".xml");
	cout << filename << endl;
	doc.save_file(filename.c_str());
}

void Map::LoadMap(string filename)
{
	markers.clear();
	pugi::xml_document doc;

	pugi::xml_parse_result result = doc.load_file(filename.c_str());

	if (result)
	{
		//cout << "Loaded " << result.description() << doc.child("Map").value();
		//cout << doc.text();
		pugi::xml_node mapNode = doc.child("Map");
		cout << mapNode.text() << endl;
		pugi::xml_node markersNode = mapNode.child("Markers");
		int count = 1;
		for (pugi::xml_node marker : markersNode.children("Marker"))
		{
			int x, y;
			x = atoi(marker.attribute("XPos").value());
			y = atoi(marker.attribute("YPos").value());
			cout << x << "," << y << endl;
			HitMarker newMarker(count, x, y);
			markers.push_back(newMarker);
			count++;
		}
	}
	else
	{
		cout << "Error loading " << filename << endl;
	}
}