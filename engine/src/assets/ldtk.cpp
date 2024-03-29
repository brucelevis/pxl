#include <pxl/assets/ldtk.h>
#include <pxl/filesystem.h>
#include <pxl/3rdparty/json.hpp>

using namespace pxl;

/*
struct EntityInstance
{
	string __identifier;
	int _grid[2];
	int _pivot[2];
	int width;
	int height;
	int px[2];
};

struct GridTile
{
	int px[2];
	int src[2];
	int f;
	int t;
	//int d; some ldtk internal thing
};
struct LayerInstance
{
	string __identifier;
	string __type;
	int __c_wid;
	int __c_hei;
	int __grid_size;
	float __opacity;
	int __px_total_offset_x;
	int __py_total_offset_y;
	int __tileset_def_uid;
	string __tileset_rel_path;

		string identifier;
		int uid;
		int world_x;
		int world_y;
		int px_wid;
		int px_hei;
		Color _bg_color;
};
*/
LDTKLevel::LDTKLevel()
{
}

LDTKLevel::LDTKLevel(const string& path)
{
	load(path);
}

void LDTKLevel::load(const string& path)
{
	file::FileStream file(path, file::FileMode::Read);
	string data = file.all();
	nlohmann::json ldtkLevelJson = nlohmann::json::parse(data);


	identifier = ldtkLevelJson["identifier"].get<string>();
	uid = ldtkLevelJson["uid"];
	world_x = ldtkLevelJson["worldX"];
	world_y = ldtkLevelJson["worldY"];
	px_wid = ldtkLevelJson["pxWid"];
	px_hei = ldtkLevelJson["pxHei"];

	if (ldtkLevelJson.contains("layerInstances") && ldtkLevelJson["layerInstances"].is_array())
	{
		auto layerInstancesJson = ldtkLevelJson["layerInstances"];
		for (auto &itLayer : layerInstancesJson)
		{
			LayerInstance layerInstance;
			layerInstance._identifier = itLayer["__identifier"].get<string>();

			string type = itLayer["__type"].get<string>();
			if (type == "Entities")
			{
				layerInstance._type = LayerInstance::Type::Entities;
			}
			else if (type == "Tiles")
			{
				layerInstance._type = LayerInstance::Type::Tiles;
			}

			layerInstance._c_wid = itLayer["__cWid"].get<int>();
			layerInstance._c_hei = itLayer["__cHei"].get<int>();
			layerInstance._grid_size = itLayer["__gridSize"].get<int>();
			layerInstance._opacity = itLayer["__opacity"].get<float>();
			layerInstance._px_total_offset_x = itLayer["__pxTotalOffsetX"].get<int>();
			layerInstance._py_total_offset_y = itLayer["__pxTotalOffsetX"].get<int>();

			if (layerInstance._type == LayerInstance::Type::Tiles)
			{
				auto gridTilesJosn = itLayer["gridTiles"];
				for (auto& itgridTile: gridTilesJosn)
				{
					GridTile tile;
					auto pxVector = itgridTile["px"].get<vector<int>>();
					tile.px[0] = pxVector[0];
					tile.px[1] = pxVector[1];
					auto srcVector = itgridTile["src"].get<vector<int>>();
					tile.src[0] = srcVector[0];
					tile.src[1] = srcVector[1];

					tile.f = itgridTile["f"].get<int>();
					tile.t = itgridTile["t"].get<int>();
					layerInstance.grid_tiles.emplace_back(tile);
				}
			}
			else if (layerInstance._type == LayerInstance::Type::Entities)
			{
				auto entityInstancesJson = itLayer["entityInstances"];
				for (auto& itEntity : entityInstancesJson)
				{
					EntityInstance entityInstance;
					entityInstance._identifier = itEntity["__identifier"].get<string>(); 
					auto grid = itEntity["__grid"].get<vector<int>>();
					entityInstance._grid[0] = grid[0];
					entityInstance._grid[1] = grid[1];

					auto pivot = itEntity["__pivot"].get<vector<int>>();
					entityInstance._pivot[0] = pivot[0];
					entityInstance._pivot[1] = pivot[1];

					entityInstance.width = itEntity["width"].get<int>();
					entityInstance.height = itEntity["height"].get<int>();

					auto px = itEntity["px"].get<vector<int>>();
					entityInstance.px[0] = px[0];
					entityInstance.px[1] = px[1];

					layerInstance.entity_instances.emplace_back(entityInstance);
				}
			}

			layer_instances.emplace_back(layerInstance);
		}
	}
}

const LDTKLevel::LayerInstance* LDTKLevel::tileLayer(const string& name)
{
	for (auto& it : layer_instances)
	{
		if (it._identifier == name && it._type == LayerInstance::Type::Tiles)
		{
			return &it;
		}
	}
	return nullptr;
}

const LDTKLevel::LayerInstance* LDTKLevel::entitiesLayer(const string& name)
{
	for (auto& it : layer_instances)
	{
		if (it._identifier == name && it._type == LayerInstance::Type::Entities)
		{
			return &it;
		}
	}
	return nullptr;
}