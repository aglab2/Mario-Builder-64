#include "src/game/envfx_snow.h"
#include "actors/group0.h"

const GeoLayout wf_area_1_geo[] = {
	GEO_NODE_START(),
	GEO_OPEN_NODE(),
		GEO_TRANSLATE_ROTATE_WITH_DL(LAYER_OPAQUE, 2244, 3565, 1760, 67, -90, -90, wf_dl_bale_mesh),
		GEO_TRANSLATE_ROTATE_WITH_DL(LAYER_OPAQUE, -5224, 3565, -180, 67, -90, -90, wf_dl_bale_001_mesh),
		GEO_TRANSLATE_ROTATE_WITH_DL(LAYER_OPAQUE, -1116, 3565, 1763, 67, -90, -90, wf_dl_bale_002_mesh),
		GEO_TRANSLATE_ROTATE_WITH_DL(LAYER_OPAQUE, -487, 3565, 2303, 67, -90, -90, wf_dl_bale_003_mesh),
		GEO_TRANSLATE_ROTATE_WITH_DL(LAYER_OPAQUE, 371, 3565, 2040, 67, -90, -90, wf_dl_bale_004_mesh),
		GEO_TRANSLATE_ROTATE_WITH_DL(LAYER_OPAQUE, 7280, 3565, 168, 67, -90, -90, wf_dl_bale_005_mesh),
		GEO_TRANSLATE_NODE_WITH_DL(LAYER_OPAQUE, 0, 5189, 0, wf_dl_BarnOutside_mesh),
		GEO_TRANSLATE_NODE_WITH_DL(LAYER_ALPHA, -11959, 6705, 9231, wf_dl_fencer_mesh),
		GEO_TRANSLATE_ROTATE_WITH_DL(LAYER_ALPHA, 0, 3638, 8361, 90, 0, 0, wf_dl_FenceVisual_mesh),
		GEO_TRANSLATE_ROTATE_WITH_DL(LAYER_ALPHA, 9784, 5885, -6952, 90, 0, -45, wf_dl_FenceVisual_001_mesh),
		GEO_TRANSLATE_ROTATE_WITH_DL(LAYER_ALPHA, 9345, 5885, -7391, 90, 0, 135, wf_dl_FenceVisual_002_mesh),
		GEO_TRANSLATE_ROTATE_WITH_DL(LAYER_ALPHA, -5694, 3665, -790, 90, 79, -84, wf_dl_FenceVisual_003_mesh),
		GEO_TRANSLATE_ROTATE_WITH_DL(LAYER_OPAQUE, 0, 2501, 0, -180, 0, 0, wf_dl_Island_mesh),
		GEO_OPEN_NODE(),
			GEO_DISPLAY_LIST(LAYER_OPAQUE, wf_dl_Island_mesh_layer_1),
		GEO_CLOSE_NODE(),
		GEO_TRANSLATE_ROTATE_WITH_DL(LAYER_OPAQUE, 0, 2501, 0, -180, 0, 0, wf_dl_Island_001_mesh),
		GEO_TRANSLATE_NODE_WITH_DL(LAYER_OPAQUE, -10704, 4301, 5623, wf_dl_pog_mesh),
		GEO_TRANSLATE_NODE_WITH_DL(LAYER_TRANSPARENT, -10704, 4301, 5623, wf_dl_pog_001_mesh),
		GEO_TRANSLATE_ROTATE_WITH_DL(LAYER_OPAQUE, -218, 3665, -1918, 0, 45, 0, wf_dl_Rock_mesh),
		GEO_TRANSLATE_ROTATE_WITH_DL(LAYER_OPAQUE, -179, 3705, 1648, 0, 135, 0, wf_dl_Rock_001_mesh),
		GEO_TRANSLATE_ROTATE_WITH_DL(LAYER_OPAQUE, -1087, 3624, -2276, 0, 135, 0, wf_dl_Rock_002_mesh),
		GEO_TRANSLATE_ROTATE_WITH_DL(LAYER_OPAQUE, 1207, 3624, -1604, 0, 135, 0, wf_dl_Rock_003_mesh),
		GEO_TRANSLATE_ROTATE_WITH_DL(LAYER_OPAQUE, 2318, 3624, 749, 0, 135, 0, wf_dl_Rock_004_mesh),
		GEO_TRANSLATE_ROTATE_WITH_DL(LAYER_OPAQUE, -6944, 4185, 1162, 0, 135, 0, wf_dl_Rock_005_mesh),
		GEO_TRANSLATE_ROTATE_WITH_DL(LAYER_OPAQUE, -10595, 6594, 11152, 0, 135, 0, wf_dl_Rock_006_mesh),
		GEO_TRANSLATE_ROTATE_WITH_DL(LAYER_OPAQUE, -10865, 6577, 11298, 0, 135, 0, wf_dl_Rock_007_mesh),
		GEO_TRANSLATE_ROTATE_WITH_DL(LAYER_OPAQUE, 9794, 3624, -64, 0, 135, 0, wf_dl_Rock_008_mesh),
		GEO_TRANSLATE_ROTATE_WITH_DL(LAYER_OPAQUE, 11465, 4174, 966, 0, 135, 0, wf_dl_Rock_009_mesh),
		GEO_TRANSLATE_ROTATE_WITH_DL(LAYER_OPAQUE, -11305, 6525, 8068, 0, 135, 0, wf_dl_Rock_010_mesh),
		GEO_TRANSLATE_ROTATE_WITH_DL(LAYER_OPAQUE, -12010, 6503, 9738, 0, 135, 0, wf_dl_Rock_011_mesh),
		GEO_TRANSLATE_NODE_WITH_DL(LAYER_OPAQUE, -7250, 4407, -1349, wf_dl_silo_001_mesh),
		GEO_TRANSLATE_NODE_WITH_DL(LAYER_OPAQUE, 5367, 6669, -1752, wf_dl_silo_002_mesh),
		GEO_TRANSLATE_ROTATE_WITH_DL(LAYER_OPAQUE, -5445, 5203, 9212, 0, -11, 0, wf_dl_silo_003_mesh),
		GEO_TRANSLATE_NODE_WITH_DL(LAYER_OPAQUE, 10794, 4086, 2900, wf_dl_silo_004_mesh),
		GEO_TRANSLATE_NODE_WITH_DL(LAYER_OPAQUE, 7925, 6737, 8998, wf_dl_silo_005_mesh),
		GEO_TRANSLATE_NODE_WITH_DL(LAYER_OPAQUE, -11967, 6555, 9301, wf_dl_stump_mesh),
		GEO_TRANSLATE_NODE_WITH_DL(LAYER_OPAQUE, -12773, 6559, 7850, wf_dl_trough_mesh),
		GEO_TRANSLATE_NODE_WITH_DL(LAYER_OPAQUE, 6789, 6568, 5412, wf_dl_WoodFence_mesh),
		GEO_TRANSLATE_NODE_WITH_DL(LAYER_OPAQUE, 0, 5189, 0, wf_dl_BarnInside_mesh),
		GEO_TRANSLATE_ROTATE_WITH_DL(LAYER_ALPHA, 1117, 7544, -365, 0, 0, -90, wf_dl_Chain_mesh),
		GEO_TRANSLATE_ROTATE_WITH_DL(LAYER_ALPHA, 1117, 7544, 365, 0, 0, -90, wf_dl_Chain_029_mesh),
		GEO_TRANSLATE_ROTATE_WITH_DL(LAYER_ALPHA, 562, 7544, 368, 0, -180, -90, wf_dl_Chain_030_mesh),
		GEO_TRANSLATE_ROTATE_WITH_DL(LAYER_ALPHA, 562, 7544, -363, 0, -180, -90, wf_dl_Chain_031_mesh),
		GEO_TRANSLATE_ROTATE_WITH_DL(LAYER_ALPHA, 135, 7544, 296, 0, -135, -90, wf_dl_Chain_032_mesh),
		GEO_TRANSLATE_ROTATE_WITH_DL(LAYER_ALPHA, -382, 7544, -220, 0, -135, -90, wf_dl_Chain_033_mesh),
		GEO_TRANSLATE_ROTATE_WITH_DL(LAYER_ALPHA, -427, 7544, 309, 0, -90, -90, wf_dl_Chain_034_mesh),
		GEO_TRANSLATE_ROTATE_WITH_DL(LAYER_ALPHA, -1157, 7544, 309, 0, -90, -90, wf_dl_Chain_035_mesh),
		GEO_TRANSLATE_NODE_WITH_DL(LAYER_OPAQUE, 1238, 3724, 1139, wf_dl_Cube_mesh),
		GEO_OPEN_NODE(),
			GEO_DISPLAY_LIST(LAYER_ALPHA, wf_dl_Cube_mesh_layer_4),
		GEO_CLOSE_NODE(),
		GEO_TRANSLATE_NODE_WITH_DL(LAYER_OPAQUE, 0, 5189, 0, wf_dl_Darkness_mesh),
	GEO_CLOSE_NODE(),
	GEO_RETURN(),
};
const GeoLayout wf_area_1[] = {
	GEO_NODE_SCREEN_AREA(10, SCREEN_WIDTH/2, SCREEN_HEIGHT/2, SCREEN_WIDTH/2, SCREEN_HEIGHT/2),
	GEO_OPEN_NODE(),
		GEO_ZBUFFER(0),
		GEO_OPEN_NODE(),
			GEO_NODE_ORTHO(100.0000),
			GEO_OPEN_NODE(),
				GEO_BACKGROUND(BACKGROUND_BELOW_CLOUDS, geo_skybox_main),
			GEO_CLOSE_NODE(),
		GEO_CLOSE_NODE(),
		GEO_ZBUFFER(1),
		GEO_OPEN_NODE(),
			GEO_CAMERA_FRUSTUM_WITH_FUNC(45.0000, 100, 30000, geo_camera_fov),
			GEO_OPEN_NODE(),
				GEO_CAMERA(CAMERA_MODE_8_DIRECTIONS, 0, 0, 0, 0, -100, 0, geo_camera_main),
				GEO_OPEN_NODE(),
					GEO_BRANCH(1, wf_area_1_geo),
					GEO_RENDER_OBJ(),
					GEO_ASM(ENVFX_MODE_NONE, geo_envfx_main),
					GEO_ASM(0, geo_moon_move),
					GEO_TRANSLATE_NODE_WITH_DL(LAYER_FORCE, 0, 0, 0, executive__circle_mesh),
				GEO_CLOSE_NODE(),
			GEO_CLOSE_NODE(),
		GEO_CLOSE_NODE(),
		GEO_DISPLAY_LIST(LAYER_OPAQUE, wf_dl_material_revert_render_settings),
		GEO_DISPLAY_LIST(LAYER_ALPHA, wf_dl_material_revert_render_settings),
		GEO_DISPLAY_LIST(LAYER_TRANSPARENT, wf_dl_material_revert_render_settings),
	GEO_CLOSE_NODE(),
	GEO_END(),
};