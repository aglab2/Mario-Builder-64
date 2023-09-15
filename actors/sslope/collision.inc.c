const Collision sslope_collision[] = {
	COL_INIT(),
	COL_VERTEX_INIT(6),
	COL_VERTEX(150, -150, -150),
	COL_VERTEX(150, 150, -150),
	COL_VERTEX(-150, 150, 150),
	COL_VERTEX(-150, -150, 150),
	COL_VERTEX(-150, 150, -150),
	COL_VERTEX(-150, -150, -150),
	COL_TRI_INIT(SURFACE_NOT_SLIPPERY, 8),
	COL_TRI(0, 1, 2),
	COL_TRI(0, 2, 3),
	COL_TRI(4, 1, 0),
	COL_TRI(4, 0, 5),
	COL_TRI(4, 2, 1),
	COL_TRI(5, 0, 3),
	COL_TRI(4, 5, 3),
	COL_TRI(4, 3, 2),
	COL_TRI_STOP(),
	COL_END()
};