#include "src/game/envfx_snow.h"

const GeoLayout noteblock_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_DISPLAY_LIST(1, noteblock_Cube_007_mesh_layer_1),
		GEO_DISPLAY_LIST(1, noteblock_material_revert_render_settings),
	GEO_CLOSE_NODE(),
	GEO_END(),
};
