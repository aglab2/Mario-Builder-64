#include <ultra64.h>
#include <PR/os_internal_reg.h>

#include "sm64.h"
#include "gfx_dimensions.h"
#include "audio/external.h"
#include "buffers/buffers.h"
#include "buffers/gfx_output_buffer.h"
#include "buffers/framebuffers.h"
#include "buffers/zbuffer.h"
#include "engine/level_script.h"
#include "engine/math_util.h"
#include "game_init.h"
#include "main.h"
#include "memory.h"
#include "save_file.h"
#include "seq_ids.h"
#include "sound_init.h"
#include "print.h"
#include "segment2.h"
#include "segment_symbols.h"
#include "rumble_init.h"
#ifdef HVQM
#include <hvqm/hvqm.h>
#endif
#ifdef SRAM
#include "sram.h"
#endif
#include "puppyprint.h"
#include "puppycam2.h"
#include "debug_box.h"
#include "vc_check.h"
#include "profiling.h"
#include "puppycamold.h"
#include "cursed_mirror_maker.h"

#include "libcart/include/cart.h"
#include "libcart/ff/ff.h"

#include "libpl/libpl.h"

u8 painting_base_rgba16[] = {
	0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 
	0x40, 0xc1, 0x51, 0x41, 0x40, 0xc1, 0x28, 0x41, 
	0x28, 0x41, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 
	0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 
	0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 
	0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 
	0x20, 0x41, 0x38, 0xc1, 0x38, 0xc1, 0x38, 0xc1, 
	0x20, 0x41, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 
	0x00, 0x01, 0xab, 0x41, 0xf6, 0x01, 0xff, 0x41, 
	0xfe, 0x41, 0xfe, 0x81, 0xfe, 0x81, 0xfe, 0xc1, 
	0xff, 0x01, 0xff, 0x01, 0xff, 0x01, 0xfe, 0xc1, 
	0xfe, 0x81, 0xfe, 0x81, 0xfe, 0x81, 0xfe, 0x81, 
	0xfe, 0x81, 0xfe, 0x81, 0xfe, 0x81, 0xfe, 0x81, 
	0xfe, 0x41, 0xff, 0x01, 0xfe, 0xc1, 0xfe, 0x81, 
	0xfe, 0x81, 0xfe, 0x41, 0xfe, 0x41, 0xfd, 0xc1, 
	0xfd, 0xc1, 0xab, 0x41, 0x28, 0x41, 0x00, 0x01, 
	0x00, 0x01, 0xf6, 0x01, 0xdd, 0x43, 0x82, 0x41, 
	0x92, 0x81, 0x92, 0x81, 0x8a, 0x81, 0x8a, 0x41, 
	0x92, 0x81, 0x8a, 0x41, 0x8a, 0x41, 0x92, 0x81, 
	0x8a, 0x41, 0x92, 0x81, 0x92, 0x81, 0x92, 0x81, 
	0x92, 0x81, 0x92, 0x81, 0x92, 0x81, 0x92, 0x81, 
	0x92, 0x81, 0x7a, 0x01, 0x82, 0x01, 0x82, 0x01, 
	0x82, 0x01, 0x82, 0x01, 0x82, 0x01, 0x82, 0x01, 
	0x82, 0x41, 0xfd, 0x81, 0x38, 0xc1, 0x00, 0x01, 
	0x00, 0x01, 0xfd, 0x81, 0x92, 0x81, 0xbb, 0xc1, 
	0xab, 0x41, 0xbc, 0x01, 0xc4, 0x41, 0xcc, 0x81, 
	0xcc, 0x81, 0xcc, 0x81, 0xcc, 0x81, 0xc4, 0x41, 
	0xc4, 0x81, 0xc4, 0x41, 0xc4, 0x41, 0xc4, 0x41, 
	0xc4, 0x41, 0xc4, 0x41, 0xc4, 0x41, 0xc4, 0x41, 
	0xc4, 0x41, 0xbc, 0x01, 0xbc, 0x01, 0xb3, 0xc1, 
	0xab, 0x41, 0xab, 0x81, 0xa3, 0x41, 0xa3, 0x41, 
	0xd4, 0x41, 0xfd, 0x81, 0x51, 0x41, 0x00, 0x01, 
	0x00, 0x01, 0xf5, 0x41, 0x92, 0x81, 0xcc, 0x41, 
	0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 
	0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 
	0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 
	0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 
	0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 
	0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 0x92, 0x81, 
	0xf5, 0x81, 0xfe, 0x41, 0x61, 0x81, 0x00, 0x01, 
	0x00, 0x01, 0xed, 0x01, 0xa3, 0x01, 0xed, 0x01, 
	0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 
	0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 
	0x21, 0x09, 0x21, 0x09, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x21, 0x09, 0x21, 0x09, 
	0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 
	0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 0xcd, 0x09, 
	0xe4, 0xc1, 0xfe, 0xc1, 0x38, 0x81, 0x00, 0x01, 
	0x00, 0x01, 0xe4, 0xc1, 0xab, 0x01, 0xed, 0x41, 
	0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 
	0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 
	0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 0xdd, 0x09, 
	0xdc, 0xc1, 0xfe, 0x81, 0x30, 0x81, 0x00, 0x01, 
	0x00, 0x01, 0xdc, 0xc1, 0xab, 0x41, 0xf5, 0x81, 
	0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 
	0x21, 0x09, 0x21, 0x09, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x21, 0x09, 0x21, 0x09, 
	0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 0xd5, 0x07, 
	0xdc, 0x81, 0xfe, 0x81, 0x30, 0x81, 0x00, 0x01, 
	0x00, 0x01, 0xdc, 0xc1, 0xb3, 0x81, 0xed, 0x01, 
	0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 
	0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 
	0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 0xd5, 0x07, 
	0xd4, 0x81, 0xfe, 0x41, 0x30, 0x81, 0x00, 0x01, 
	0x00, 0x01, 0xdc, 0xc1, 0xab, 0x41, 0xed, 0x41, 
	0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 
	0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 
	0x9c, 0xe7, 0xde, 0xf7, 0xf7, 0xbd, 0xf7, 0xbd, 
	0xde, 0xf7, 0xa5, 0x29, 0x21, 0x09, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 
	0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 0xd5, 0x07, 
	0xdc, 0x81, 0xfe, 0x41, 0x28, 0x81, 0x00, 0x01, 
	0x00, 0x01, 0xd4, 0x41, 0xb3, 0x81, 0xe5, 0x01, 
	0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 0xb5, 0xad, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xce, 0x73, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x21, 0x09, 0x21, 0x09, 0xd5, 0x09, 
	0xd4, 0x81, 0xfe, 0x01, 0x28, 0x41, 0x00, 0x01, 
	0x00, 0x01, 0xd4, 0x41, 0xb3, 0x41, 0xe5, 0x01, 
	0x21, 0x09, 0x21, 0x09, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x52, 0x95, 0xff, 0xff, 
	0xff, 0xff, 0xb5, 0xad, 0x52, 0x95, 0x52, 0x95, 
	0xb5, 0xad, 0xff, 0xff, 0xff, 0xff, 0x7b, 0xdf, 
	0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x21, 0x09, 0xd5, 0x09, 
	0xd4, 0x81, 0xfe, 0x41, 0x28, 0x41, 0x00, 0x01, 
	0x00, 0x01, 0xd4, 0x41, 0xb3, 0x41, 0xed, 0x01, 
	0x21, 0x09, 0x21, 0x09, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0xad, 0x6b, 0xff, 0xff, 
	0xd6, 0xb5, 0x10, 0x85, 0x10, 0x85, 0x10, 0x85, 
	0x10, 0x85, 0xd6, 0xb5, 0xff, 0xff, 0xb5, 0xad, 
	0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x21, 0x09, 0xd5, 0x09, 
	0xd4, 0x81, 0xfe, 0x01, 0x28, 0x41, 0x00, 0x01, 
	0x00, 0x01, 0xd4, 0x41, 0xb3, 0x81, 0xe5, 0x01, 
	0x21, 0x09, 0x21, 0x09, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 0x21, 0x09, 
	0x21, 0x09, 0x10, 0x85, 0x10, 0x85, 0x10, 0x85, 
	0x10, 0x85, 0xce, 0x73, 0xff, 0xff, 0xa5, 0x29, 
	0x10, 0x85, 0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x21, 0x09, 0xd5, 0x09, 
	0xd4, 0x81, 0xfd, 0xc1, 0x28, 0x41, 0x00, 0x01, 
	0x00, 0x01, 0xc3, 0xc1, 0xab, 0x41, 0xdc, 0x81, 
	0x21, 0x09, 0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 0x10, 0x85, 
	0x10, 0x85, 0x10, 0x85, 0x10, 0x85, 0x08, 0x43, 
	0xad, 0x6b, 0xff, 0xff, 0xf7, 0xbd, 0x4a, 0x53, 
	0x10, 0x85, 0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 0xc4, 0x01, 
	0xd4, 0x41, 0xfd, 0xc1, 0x38, 0x81, 0x00, 0x01, 
	0x00, 0x01, 0xcc, 0x01, 0xa3, 0x01, 0xdc, 0x81, 
	0x21, 0x09, 0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 0x10, 0x85, 
	0x10, 0x85, 0x10, 0x85, 0x08, 0x43, 0xbd, 0xef, 
	0xff, 0xff, 0xf7, 0xbd, 0x7b, 0xdf, 0x10, 0x85, 
	0x10, 0x85, 0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 0xbb, 0xc1, 
	0xd4, 0x81, 0xfe, 0x01, 0x38, 0x81, 0x00, 0x01, 
	0x00, 0x01, 0xc3, 0xc1, 0xab, 0x01, 0xdc, 0x81, 
	0x21, 0x09, 0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 0x10, 0x85, 
	0x10, 0x85, 0x10, 0x85, 0xbd, 0xef, 0xff, 0xff, 
	0xf7, 0xbd, 0x73, 0x9d, 0x10, 0x85, 0x10, 0x85, 
	0x10, 0x85, 0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 0xbb, 0xc1, 
	0xd4, 0x81, 0xfd, 0xc1, 0x38, 0x81, 0x00, 0x01, 
	0x00, 0x01, 0xcc, 0x41, 0xa3, 0x01, 0xdc, 0x81, 
	0x21, 0x09, 0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 0x10, 0x85, 
	0x10, 0x85, 0x6b, 0x5b, 0xff, 0xff, 0xff, 0xff, 
	0x73, 0x9d, 0x10, 0x85, 0x10, 0x85, 0x10, 0x85, 
	0x10, 0x85, 0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 0xbb, 0xc1, 
	0xd4, 0x81, 0xfe, 0x01, 0x38, 0x81, 0x00, 0x01, 
	0x00, 0x01, 0xcc, 0x41, 0xa3, 0x01, 0xdc, 0x81, 
	0x21, 0x09, 0x21, 0x09, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 0x10, 0x85, 
	0x10, 0x85, 0xad, 0x6b, 0xff, 0xff, 0xd6, 0xb5, 
	0x10, 0x85, 0x10, 0x85, 0x10, 0x85, 0x10, 0x85, 
	0x10, 0x85, 0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x21, 0x09, 0xbb, 0x81, 
	0xd4, 0x81, 0xfe, 0x41, 0x38, 0x81, 0x00, 0x01, 
	0x00, 0x01, 0xcc, 0x41, 0xa3, 0x01, 0xdc, 0x81, 
	0x21, 0x09, 0x21, 0x09, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 
	0x10, 0x85, 0xbd, 0xef, 0xff, 0xff, 0xbd, 0xef, 
	0x10, 0x85, 0x10, 0x85, 0x10, 0x85, 0x10, 0x85, 
	0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x21, 0x09, 0xbb, 0x81, 
	0xd4, 0x81, 0xfe, 0x41, 0x40, 0xc1, 0x00, 0x01, 
	0x00, 0x01, 0xcc, 0x41, 0xa3, 0x01, 0xd4, 0x81, 
	0x21, 0x09, 0x21, 0x09, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x21, 0x09, 0x21, 0x09, 0x18, 0xc7, 
	0x10, 0x85, 0x10, 0x85, 0x10, 0x85, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x21, 0x09, 0xbb, 0x81, 
	0xdc, 0x81, 0xfe, 0x41, 0x40, 0xc1, 0x00, 0x01, 
	0x00, 0x01, 0xdc, 0x81, 0x9a, 0xc1, 0xd4, 0x81, 
	0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x21, 0x09, 0x21, 0x09, 0xb3, 0x81, 
	0xdc, 0x81, 0xfe, 0x41, 0x40, 0xc1, 0x00, 0x01, 
	0x00, 0x01, 0xdc, 0x81, 0xa3, 0x01, 0xdc, 0xc1, 
	0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 
	0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0xb5, 0xad, 0xff, 0xff, 0xb5, 0xad, 
	0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 
	0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 0xb3, 0x81, 
	0xdc, 0x81, 0xfe, 0x41, 0x48, 0xc1, 0x00, 0x01, 
	0x00, 0x01, 0xdc, 0x81, 0x9a, 0xc1, 0xd4, 0x41, 
	0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 
	0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0xbd, 0xef, 0xff, 0xff, 0xbd, 0xef, 
	0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 
	0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 0xb3, 0x81, 
	0xdc, 0xc1, 0xfe, 0x41, 0x48, 0xc1, 0x00, 0x01, 
	0x00, 0x01, 0xdc, 0xc1, 0x92, 0x81, 0xd4, 0x81, 
	0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 
	0x21, 0x09, 0x21, 0x09, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x21, 0x09, 0x21, 0x09, 
	0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 0xb3, 0x81, 
	0xdc, 0xc1, 0xfe, 0x81, 0x48, 0xc1, 0x00, 0x01, 
	0x00, 0x01, 0xed, 0x01, 0x8a, 0x81, 0xdc, 0x81, 
	0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 
	0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 
	0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 0xa3, 0x41, 
	0xe4, 0xc1, 0xfe, 0x81, 0x59, 0x41, 0x00, 0x01, 
	0x00, 0x01, 0xed, 0x01, 0x82, 0x41, 0xd4, 0x81, 
	0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 
	0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 
	0x21, 0x09, 0x21, 0x09, 0x18, 0xc7, 0x18, 0xc7, 
	0x18, 0xc7, 0x18, 0xc7, 0x21, 0x09, 0x21, 0x09, 
	0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 
	0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 0xb3, 0xc3, 
	0xe4, 0xc1, 0xfe, 0x41, 0x69, 0x81, 0x00, 0x01, 
	0x00, 0x01, 0xed, 0x41, 0x7a, 0x01, 0xcc, 0x41, 
	0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 
	0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 
	0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 
	0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 
	0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 
	0x21, 0x09, 0x21, 0x09, 0x21, 0x09, 0xbc, 0x45, 
	0xed, 0x01, 0xfe, 0x81, 0x69, 0x81, 0x00, 0x01, 
	0x00, 0x01, 0xe4, 0xc1, 0x82, 0x41, 0xd4, 0x41, 
	0xbb, 0xc1, 0xcc, 0xc1, 0xcc, 0xc1, 0xdd, 0x41, 
	0xed, 0xc1, 0xdd, 0x41, 0xed, 0xc1, 0xed, 0xc1, 
	0xed, 0xc1, 0xed, 0x81, 0xed, 0x81, 0xed, 0x81, 
	0xed, 0x81, 0xed, 0x81, 0xed, 0x81, 0xed, 0x81, 
	0xed, 0x81, 0xdd, 0x01, 0xdd, 0x01, 0xdd, 0x01, 
	0xdc, 0xc1, 0xcc, 0x81, 0xcc, 0x81, 0xf5, 0x81, 
	0xf5, 0x81, 0xfe, 0xc1, 0x69, 0x81, 0x00, 0x01, 
	0x00, 0x01, 0xe4, 0xc1, 0xa3, 0x01, 0xe4, 0xc1, 
	0xe5, 0x41, 0xdd, 0x01, 0xd4, 0xc1, 0xcc, 0x81, 
	0xcc, 0x81, 0xc4, 0x41, 0xc4, 0x41, 0xc4, 0x41, 
	0xc4, 0x01, 0xc4, 0x01, 0xc4, 0x01, 0xc4, 0x01, 
	0xc4, 0x41, 0xc4, 0x01, 0xc4, 0x01, 0xcc, 0x41, 
	0xcc, 0x41, 0xc4, 0x41, 0xcc, 0x41, 0xcc, 0x41, 
	0xd4, 0x81, 0xdc, 0xc1, 0xdd, 0x01, 0xe5, 0x41, 
	0xd4, 0xc1, 0xff, 0xc5, 0x38, 0x81, 0x00, 0x01, 
	0x00, 0x01, 0xa3, 0x01, 0xe4, 0xc1, 0xff, 0x01, 
	0xff, 0xc3, 0xff, 0xc5, 0xff, 0x83, 0xff, 0x43, 
	0xff, 0x03, 0xfe, 0xc3, 0xfe, 0x81, 0xfe, 0x81, 
	0xfe, 0xc1, 0xfe, 0x41, 0xfe, 0x41, 0xfe, 0x41, 
	0xfe, 0x41, 0xfe, 0x41, 0xfe, 0x41, 0xf6, 0x01, 
	0xf6, 0x41, 0xfe, 0x81, 0xfe, 0x81, 0xff, 0x03, 
	0xff, 0x03, 0xff, 0x03, 0xff, 0x43, 0xff, 0xc5, 
	0xff, 0xc7, 0xd4, 0xc1, 0x18, 0x41, 0x00, 0x01, 
	0x00, 0x01, 0x10, 0x01, 0x20, 0x41, 0x28, 0x41, 
	0x38, 0x81, 0x38, 0x81, 0x28, 0x41, 0x28, 0x41, 
	0x20, 0x41, 0x20, 0x41, 0x20, 0x41, 0x20, 0x41, 
	0x20, 0x41, 0x20, 0x41, 0x20, 0x41, 0x20, 0x41, 
	0x20, 0x41, 0x20, 0x41, 0x20, 0x41, 0x20, 0x41, 
	0x20, 0x41, 0x30, 0x81, 0x30, 0x81, 0x30, 0x81, 
	0x30, 0x81, 0x38, 0x81, 0x49, 0x01, 0x48, 0xc1, 
	0x38, 0x81, 0x18, 0x01, 0x00, 0x01, 0x00, 0x01, 
};

// First 3 controller slots
struct Controller gControllers[3];

// Gfx handlers
struct SPTask *gGfxSPTask;
Gfx *gDisplayListHead;
u8 *gGfxPoolEnd;
struct GfxPool *gGfxPool;

// OS Controllers
OSContStatus gControllerStatuses[4];
OSContPadEx gControllerPads[4];
u8 gControllerBits;
s8 gGamecubeControllerPort = -1; // HackerSM64: This is set to -1 if there's no GC controller, 0 if there's one in the first port and 1 if there's one in the second port.
u8 gIsConsole = TRUE; // Needs to be initialized before audio_reset_session is called
u8 gCacheEmulated = TRUE;
u8 gBorderHeight;
#ifdef VANILLA_STYLE_CUSTOM_DEBUG
u8 gCustomDebugMode;
#endif
#ifdef EEP
s8 gEepromProbe;
#endif
#ifdef SRAM
s8 gSramProbe;
#endif
OSMesgQueue gGameVblankQueue;
OSMesgQueue gGfxVblankQueue;
OSMesg gGameMesgBuf[1];
OSMesg gGfxMesgBuf[1];

// Vblank Handler
struct VblankHandler gGameVblankHandler;

// Buffers
uintptr_t gPhysicalFramebuffers[3];
uintptr_t gPhysicalZBuffer;

// Mario Anims and Demo allocation
void *gMarioAnimsMemAlloc;
void *gDemoInputsMemAlloc;
struct DmaHandlerList gMarioAnimsBuf;
struct DmaHandlerList gDemoInputsBuf;

// General timer that runs as the game starts
u32 gGlobalTimer = 0;
u8 *gAreaSkyboxStart[AREA_COUNT];
u8 *gAreaSkyboxEnd[AREA_COUNT];

// Framebuffer rendering values (max 3)
u16 sRenderedFramebuffer = 0;
u16 sRenderingFramebuffer = 0;

// Goddard Vblank Function Caller
void (*gGoddardVblankCallback)(void) = NULL;

// Defined controller slots
struct Controller *gPlayer1Controller = &gControllers[0];
struct Controller *gPlayer2Controller = &gControllers[1];
struct Controller *gPlayer3Controller = &gControllers[2]; // Probably debug only, see note below

// Title Screen Demo Handler
struct DemoInput *gCurrDemoInput = NULL;
u16 gDemoInputListID = 0;
struct DemoInput gRecordedDemoInput = { 0 };

// Display
// ----------------------------------------------------------------------------------------------------

/**
 * Sets the initial RDP (Reality Display Processor) rendering settings.
 */
const Gfx init_rdp[] = {
    gsDPPipeSync(),
    gsDPPipelineMode(G_PM_1PRIMITIVE),

    gsDPSetScissor(G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT),
    gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE),

    gsDPSetTextureLOD(G_TL_TILE),
    gsDPSetTextureLUT(G_TT_NONE),
    gsDPSetTextureDetail( G_TD_CLAMP),
    gsDPSetTexturePersp(G_TP_PERSP),
    gsDPSetTextureFilter( G_TF_BILERP),
    gsDPSetTextureConvert(G_TC_FILT),

    gsDPSetCombineKey(G_CK_NONE),
    gsDPSetAlphaCompare(G_AC_NONE),
    gsDPSetRenderMode(G_RM_OPA_SURF, G_RM_OPA_SURF2),
    gsDPSetColorDither(G_CD_MAGICSQ),
    gsDPSetCycleType(G_CYC_FILL),

// #ifdef VERSION_SH
    gsDPSetAlphaDither(G_AD_PATTERN),
// #endif
    gsSPEndDisplayList(),
};

/**
 * Sets the initial RSP (Reality Signal Processor) settings.
 */
const Gfx init_rsp[] = {
    gsDPPipeSync(),
    gsSPClearGeometryMode(G_CULL_FRONT | G_FOG | G_LIGHTING | G_TEXTURE_GEN | G_TEXTURE_GEN_LINEAR | G_LOD),
    gsSPSetGeometryMode(G_SHADE | G_SHADING_SMOOTH | G_CULL_BACK | G_LIGHTING),
    gsSPNumLights(NUMLIGHTS_1),
    gsSPTexture(0, 0, 0, G_TX_RENDERTILE, G_OFF),
    // @bug Failing to set the clip ratio will result in warped triangles in F3DEX2
    // without this change: https://jrra.zone/n64/doc/n64man/gsp/gSPClipRatio.htm
#ifdef F3DEX_GBI_2
    gsSPClipRatio(FRUSTRATIO_2),
#endif
    gsSPEndDisplayList(),
};

#ifdef S2DEX_TEXT_ENGINE
void my_rdp_init(void) {
    gSPDisplayList(gDisplayListHead++, init_rdp);
}

void my_rsp_init(void) {
    gSPDisplayList(gDisplayListHead++, init_rsp);
}
#endif

/**
 * Initialize the z buffer for the current frame.
 */
void init_z_buffer(s32 resetZB) {
    gDPPipeSync(gDisplayListHead++);

    gDPSetDepthSource(gDisplayListHead++, G_ZS_PIXEL);
    gDPSetDepthImage(gDisplayListHead++, gPhysicalZBuffer);

    gDPSetColorImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WIDTH, gPhysicalZBuffer);
    if (!resetZB)
        return;
    gDPSetFillColor(gDisplayListHead++,
                    GPACK_ZDZ(G_MAXFBZ, 0) << 16 | GPACK_ZDZ(G_MAXFBZ, 0));

    gDPFillRectangle(gDisplayListHead++, 0, gBorderHeight, SCREEN_WIDTH - 1,
                     SCREEN_HEIGHT - 1 - gBorderHeight);
}

/**
 * Tells the RDP which of the three framebuffers it shall draw to.
 */
void select_framebuffer(void) {
    gDPPipeSync(gDisplayListHead++);

    gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);
    gDPSetColorImage(gDisplayListHead++, G_IM_FMT_RGBA, G_IM_SIZ_16b, SCREEN_WIDTH,
                     gPhysicalFramebuffers[sRenderingFramebuffer]);
    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, gBorderHeight, SCREEN_WIDTH,
                  SCREEN_HEIGHT - gBorderHeight);
}

/**
 * Clear the framebuffer and fill it with a 32-bit color.
 * Information about the color argument: https://jrra.zone/n64/doc/n64man/gdp/gDPSetFillColor.htm
 */
void clear_framebuffer(s32 color) {
    gDPPipeSync(gDisplayListHead++);

    gDPSetRenderMode(gDisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    gDPSetCycleType(gDisplayListHead++, G_CYC_FILL);

    gDPSetFillColor(gDisplayListHead++, color);
    gDPFillRectangle(gDisplayListHead++,
                     GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(0), gBorderHeight,
                     GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(0) - 1, SCREEN_HEIGHT - gBorderHeight - 1);

    gDPPipeSync(gDisplayListHead++);

    gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);
}

/**
 * Resets the viewport, readying it for the final image.
 */
void clear_viewport(Vp *viewport, s32 color) {
    s16 vpUlx = (viewport->vp.vtrans[0] - viewport->vp.vscale[0]) / 4 + 1;
    s16 vpUly = (viewport->vp.vtrans[1] - viewport->vp.vscale[1]) / 4 + 1;
    s16 vpLrx = (viewport->vp.vtrans[0] + viewport->vp.vscale[0]) / 4 - 2;
    s16 vpLry = (viewport->vp.vtrans[1] + viewport->vp.vscale[1]) / 4 - 2;

#ifdef WIDESCREEN
    vpUlx = GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(vpUlx);
    vpLrx = GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(SCREEN_WIDTH - vpLrx);
#endif

    gDPPipeSync(gDisplayListHead++);

    gDPSetRenderMode(gDisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    gDPSetCycleType(gDisplayListHead++, G_CYC_FILL);

    gDPSetFillColor(gDisplayListHead++, color);
    gDPFillRectangle(gDisplayListHead++, vpUlx, vpUly, vpLrx, vpLry);

    gDPPipeSync(gDisplayListHead++);

    gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);
}

/**
 * Draw the horizontal screen borders.
 */
void draw_screen_borders(void) {
    gDPPipeSync(gDisplayListHead++);

    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
    gDPSetRenderMode(gDisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    gDPSetCycleType(gDisplayListHead++, G_CYC_FILL);

    gDPSetFillColor(gDisplayListHead++, GPACK_RGBA5551(0, 0, 0, 0) << 16 | GPACK_RGBA5551(0, 0, 0, 0));

    if (gBorderHeight) {
        gDPFillRectangle(gDisplayListHead++, GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(0), 0,
                        GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(0) - 1, gBorderHeight - 1);
        gDPFillRectangle(gDisplayListHead++,
                        GFX_DIMENSIONS_RECT_FROM_LEFT_EDGE(0), SCREEN_HEIGHT - gBorderHeight,
                        GFX_DIMENSIONS_RECT_FROM_RIGHT_EDGE(0) - 1, SCREEN_HEIGHT - 1);
    }
}

/**
 * Defines the viewport scissoring rectangle.
 * Scissoring: https://jrra.zone/n64/doc/pro-man/pro12/12-03.htm#01
 */
void make_viewport_clip_rect(Vp *viewport) {
    s16 vpUlx = (viewport->vp.vtrans[0] - viewport->vp.vscale[0]) / 4 + 1;
    s16 vpPly = (viewport->vp.vtrans[1] - viewport->vp.vscale[1]) / 4 + 1;
    s16 vpLrx = (viewport->vp.vtrans[0] + viewport->vp.vscale[0]) / 4 - 1;
    s16 vpLry = (viewport->vp.vtrans[1] + viewport->vp.vscale[1]) / 4 - 1;

    gDPSetScissor(gDisplayListHead++, G_SC_NON_INTERLACE, vpUlx, vpPly, vpLrx, vpLry);
}

/**
 * Initializes the Fast3D OSTask structure.
 * If you plan on using gSPLoadUcode, make sure to add OS_TASK_LOADABLE to the flags member.
 */
void create_gfx_task_structure(void) {
    s32 entries = gDisplayListHead - gGfxPool->buffer;

    gGfxSPTask->msgqueue = &gGfxVblankQueue;
    gGfxSPTask->msg = (OSMesg) 2;
    gGfxSPTask->task.t.type = M_GFXTASK;
    gGfxSPTask->task.t.ucode_boot = rspbootTextStart;
    gGfxSPTask->task.t.ucode_boot_size = ((u8 *) rspbootTextEnd - (u8 *) rspbootTextStart);
#if defined(F3DEX_GBI_SHARED) && defined(OBJECTS_REJ)
    gGfxSPTask->task.t.flags = (OS_TASK_LOADABLE | OS_TASK_DP_WAIT);
#else
    gGfxSPTask->task.t.flags = 0x0;
#endif
#ifdef  L3DEX2_ALONE
    gGfxSPTask->task.t.ucode = gspL3DEX2_fifoTextStart;
    gGfxSPTask->task.t.ucode_data = gspL3DEX2_fifoDataStart;
    gGfxSPTask->task.t.ucode_size = ((u8 *) gspL3DEX2_fifoTextEnd - (u8 *) gspL3DEX2_fifoTextStart);
    gGfxSPTask->task.t.ucode_data_size = ((u8 *) gspL3DEX2_fifoDataEnd - (u8 *) gspL3DEX2_fifoDataStart);
#elif  F3DZEX_GBI_2
    gGfxSPTask->task.t.ucode = gspF3DZEX2_PosLight_fifoTextStart;
    gGfxSPTask->task.t.ucode_data = gspF3DZEX2_PosLight_fifoDataStart;
    gGfxSPTask->task.t.ucode_size = ((u8 *) gspF3DZEX2_PosLight_fifoTextEnd - (u8 *) gspF3DZEX2_PosLight_fifoTextStart);
    gGfxSPTask->task.t.ucode_data_size = ((u8 *) gspF3DZEX2_PosLight_fifoDataEnd - (u8 *) gspF3DZEX2_PosLight_fifoDataStart);
#elif  F3DZEX_NON_GBI_2
    gGfxSPTask->task.t.ucode = gspF3DZEX2_NoN_PosLight_fifoTextStart;
    gGfxSPTask->task.t.ucode_data = gspF3DZEX2_NoN_PosLight_fifoDataStart;
    gGfxSPTask->task.t.ucode_size = ((u8 *) gspF3DZEX2_NoN_PosLight_fifoTextEnd - (u8 *) gspF3DZEX2_NoN_PosLight_fifoTextStart);
    gGfxSPTask->task.t.ucode_data_size = ((u8 *) gspF3DZEX2_NoN_PosLight_fifoDataEnd - (u8 *) gspF3DZEX2_NoN_PosLight_fifoDataStart);
#elif   F3DEX2PL_GBI
    gGfxSPTask->task.t.ucode = gspF3DEX2_PosLight_fifoTextStart;
    gGfxSPTask->task.t.ucode_data = gspF3DEX2_PosLight_fifoDataStart;
    gGfxSPTask->task.t.ucode_size = ((u8 *) gspF3DEX2_PosLight_fifoTextEnd - (u8 *) gspF3DEX2_PosLight_fifoTextStart);
    gGfxSPTask->task.t.ucode_data_size = ((u8 *) gspF3DEX2_PosLight_fifoDataEnd - (u8 *) gspF3DEX2_PosLight_fifoDataStart);
#elif   F3DEX_GBI_2
    gGfxSPTask->task.t.ucode = gspF3DEX2_fifoTextStart;
    gGfxSPTask->task.t.ucode_data = gspF3DEX2_fifoDataStart;
    gGfxSPTask->task.t.ucode_size = ((u8 *) gspF3DEX2_fifoTextEnd - (u8 *) gspF3DEX2_fifoTextStart);
    gGfxSPTask->task.t.ucode_data_size = ((u8 *) gspF3DEX2_fifoDataEnd - (u8 *) gspF3DEX2_fifoDataStart);
#elif   F3DEX_GBI
    gGfxSPTask->task.t.ucode = gspF3DEX_fifoTextStart;
    gGfxSPTask->task.t.ucode_data = gspF3DEX_fifoDataStart;
    gGfxSPTask->task.t.ucode_size = ((u8 *) gspF3DEX_fifoTextEnd - (u8 *) gspF3DEX_fifoTextStart);
    gGfxSPTask->task.t.ucode_data_size = ((u8 *) gspF3DEX_fifoDataEnd - (u8 *) gspF3DEX_fifoDataStart);
#elif   SUPER3D_GBI
    gGfxSPTask->task.t.ucode = gspSuper3DTextStart;
    gGfxSPTask->task.t.ucode_data = gspSuper3DDataStart;
    gGfxSPTask->task.t.ucode_size = ((u8 *) gspSuper3DTextEnd - (u8 *) gspSuper3DTextStart);
    gGfxSPTask->task.t.ucode_data_size = ((u8 *) gspSuper3DDataEnd - (u8 *) gspSuper3DDataStart);
#else
    gGfxSPTask->task.t.ucode = gspFast3D_fifoTextStart;
    gGfxSPTask->task.t.ucode_data = gspFast3D_fifoDataStart;
    gGfxSPTask->task.t.ucode_size = ((u8 *) gspFast3D_fifoTextEnd - (u8 *) gspFast3D_fifoTextStart);
    gGfxSPTask->task.t.ucode_data_size = ((u8 *) gspFast3D_fifoDataEnd - (u8 *) gspFast3D_fifoDataStart);
#endif
    gGfxSPTask->task.t.dram_stack = (u64 *) gGfxSPTaskStack;
    gGfxSPTask->task.t.dram_stack_size = SP_DRAM_STACK_SIZE8;
    gGfxSPTask->task.t.output_buff = gGfxSPTaskOutputBuffer;
    gGfxSPTask->task.t.output_buff_size =
        (u64 *)((u8 *) gGfxSPTaskOutputBuffer + sizeof(gGfxSPTaskOutputBuffer));
    gGfxSPTask->task.t.data_ptr = (u64 *) &gGfxPool->buffer;
    gGfxSPTask->task.t.data_size = entries * sizeof(Gfx);
    gGfxSPTask->task.t.yield_data_ptr = (u64 *) gGfxSPTaskYieldBuffer;
    gGfxSPTask->task.t.yield_data_size = OS_YIELD_DATA_SIZE;
}

/**
 * Set default RCP (Reality Co-Processor) settings.
 */
void init_rcp(s32 resetZB) {
    move_segment_table_to_dmem();
    gSPDisplayList(gDisplayListHead++, init_rdp);
    gSPDisplayList(gDisplayListHead++, init_rsp);
    init_z_buffer(resetZB);
    select_framebuffer();
}

/**
 * End the master display list and initialize the graphics task structure for the next frame to be rendered.
 */
void end_master_display_list(void) {
    draw_screen_borders();

    gDPFullSync(gDisplayListHead++);
    gSPEndDisplayList(gDisplayListHead++);

    create_gfx_task_structure();
}

/**
 * Draw the bars that appear when the N64 is soft reset.
 */
void draw_reset_bars(void) {
    s32 width, height;
    s32 fbNum;
    u64 *fbPtr;

    if (gResetTimer != 0 && gNmiResetBarsTimer < 15) {
        if (sRenderedFramebuffer == 0) {
            fbNum = 2;
        } else {
            fbNum = sRenderedFramebuffer - 1;
        }

        fbPtr = (u64 *) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[fbNum]);
        fbPtr += gNmiResetBarsTimer++ * (SCREEN_WIDTH / 4);

        for (width = 0; width < ((SCREEN_HEIGHT / 16) + 1); width++) {
            for (height = 0; height < (SCREEN_WIDTH / 4); height++) {
                *fbPtr++ = 0;
            }
            fbPtr += ((SCREEN_WIDTH / 4) * 14);
        }
    }

    osWritebackDCacheAll();
    osRecvMesg(&gGameVblankQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
    osRecvMesg(&gGameVblankQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
}

/**
 * Initial settings for the first rendered frame.
 */
void render_init(void) {
#ifdef DEBUG_FORCE_CRASH_ON_BOOT
    FORCE_CRASH
#endif
    gGfxPool = &gGfxPools[0];
    set_segment_base_addr(SEGMENT_RENDER, gGfxPool->buffer);
    gGfxSPTask = &gGfxPool->spTask;
    gDisplayListHead = gGfxPool->buffer;
    gGfxPoolEnd = (u8 *)(gGfxPool->buffer + GFX_POOL_SIZE);
    init_rcp(CLEAR_ZBUFFER);
    clear_framebuffer(0);
    end_master_display_list();
    exec_display_list(&gGfxPool->spTask);

    // Skip incrementing the initial framebuffer index on emulators so that they display immediately as the Gfx task finishes
    // VC probably emulates osViSwapBuffer accurately so instant patch breaks VC compatibility
    // Currently, Ares passes the cache emulation test and has issues with single buffering so disable it there as well.
    if (gIsConsole || gIsVC || gCacheEmulated) {
        sRenderingFramebuffer++;
    }
    gGlobalTimer++;
}

/**
 * Selects the location of the F3D output buffer (gDisplayListHead).
 */
void select_gfx_pool(void) {
    gGfxPool = &gGfxPools[gGlobalTimer % ARRAY_COUNT(gGfxPools)];
    set_segment_base_addr(SEGMENT_RENDER, gGfxPool->buffer);
    gGfxSPTask = &gGfxPool->spTask;
    gDisplayListHead = gGfxPool->buffer;
    gGfxPoolEnd = (u8 *) (gGfxPool->buffer + GFX_POOL_SIZE);
}

/**
 * This function:
 * - Sends the current master display list out to be rendered.
 * - Tells the VI which color framebuffer to be displayed.
 * - Yields to the VI framerate twice, locking the game at 30 FPS.
 * - Selects which framebuffer will be rendered and displayed to next time.
 */
void display_and_vsync(void) {
    osRecvMesg(&gGfxVblankQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
    if (gGoddardVblankCallback != NULL) {
        gGoddardVblankCallback();
        gGoddardVblankCallback = NULL;
    }
    exec_display_list(&gGfxPool->spTask);
#ifndef UNLOCK_FPS
    osRecvMesg(&gGameVblankQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
#endif
    osViSwapBuffer((void *) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[sRenderedFramebuffer]));
#ifndef UNLOCK_FPS
    osRecvMesg(&gGameVblankQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
#endif
    // Skip swapping buffers on inaccurate emulators other than VC so that they display immediately as the Gfx task finishes
    if (gIsConsole || gIsVC || gCacheEmulated) {
        if (++sRenderedFramebuffer == 3) {
            sRenderedFramebuffer = 0;
        }
        if (++sRenderingFramebuffer == 3) {
            sRenderingFramebuffer = 0;
        }
    }
    gGlobalTimer++;
}

// this function records distinct inputs over a 255-frame interval to RAM locations and was likely
// used to record the demo sequences seen in the final game. This function is unused.
UNUSED static void record_demo(void) {
    // record the player's button mask and current rawStickX and rawStickY.
    u8 buttonMask =
        ((gPlayer1Controller->buttonDown & (A_BUTTON | B_BUTTON | Z_TRIG | START_BUTTON)) >> 8)
        | (gPlayer1Controller->buttonDown & (U_CBUTTONS | D_CBUTTONS | L_CBUTTONS | R_CBUTTONS));
    s8 rawStickX = gPlayer1Controller->rawStickX;
    s8 rawStickY = gPlayer1Controller->rawStickY;

    // If the stick is in deadzone, set its value to 0 to
    // nullify the effects. We do not record deadzone inputs.
    if (rawStickX > -8 && rawStickX < 8) {
        rawStickX = 0;
    }

    if (rawStickY > -8 && rawStickY < 8) {
        rawStickY = 0;
    }

    // Rrecord the distinct input and timer so long as they are unique.
    // If the timer hits 0xFF, reset the timer for the next demo input.
    if (gRecordedDemoInput.timer == 0xFF || buttonMask != gRecordedDemoInput.buttonMask
        || rawStickX != gRecordedDemoInput.rawStickX || rawStickY != gRecordedDemoInput.rawStickY) {
        gRecordedDemoInput.timer = 0;
        gRecordedDemoInput.buttonMask = buttonMask;
        gRecordedDemoInput.rawStickX = rawStickX;
        gRecordedDemoInput.rawStickY = rawStickY;
    }
    gRecordedDemoInput.timer++;
}

/**
 * If a demo sequence exists, this will run the demo input list until it is complete.
 */
void run_demo_inputs(void) {
    // Eliminate the unused bits.
    gControllers[0].controllerData->button &= VALID_BUTTONS;

    // Check if a demo inputs list exists and if so,
    // run the active demo input list.
    if (gCurrDemoInput != NULL) {
        // Clear player 2's inputs if they exist. Player 2's controller
        // cannot be used to influence a demo. At some point, Nintendo
        // may have planned for there to be a demo where 2 players moved
        // around instead of just one, so clearing player 2's influence from
        // the demo had to have been necessary to perform this. Co-op mode, perhaps?
        if (gControllers[1].controllerData != NULL) {
            gControllers[1].controllerData->stick_x = 0;
            gControllers[1].controllerData->stick_y = 0;
            gControllers[1].controllerData->button = 0;
        }

        // The timer variable being 0 at the current input means the demo is over.
        // Set the button to the END_DEMO mask to end the demo.
        if (gCurrDemoInput->timer == 0) {
            gControllers[0].controllerData->stick_x = 0;
            gControllers[0].controllerData->stick_y = 0;
            gControllers[0].controllerData->button = END_DEMO;
        } else {
            // Backup the start button if it is pressed, since we don't want the
            // demo input to override the mask where start may have been pressed.
            u16 startPushed = gControllers[0].controllerData->button & START_BUTTON;

            // Perform the demo inputs by assigning the current button mask and the stick inputs.
            gControllers[0].controllerData->stick_x = 0;
            gControllers[0].controllerData->stick_y = 0;

            // To assign the demo input, the button information is stored in
            // an 8-bit mask rather than a 16-bit mask. this is because only
            // A, B, Z, Start, and the C-Buttons are used in a demo, as bits
            // in that order. In order to assign the mask, we need to take the
            // upper 4 bits (A, B, Z, and Start) and shift then left by 8 to
            // match the correct input mask. We then add this to the masked
            // lower 4 bits to get the correct button mask.
            gControllers[0].controllerData->button = 0;

            // If start was pushed, put it into the demo sequence being input to end the demo.
            gControllers[0].controllerData->button |= startPushed;

            // Run the current demo input's timer down. if it hits 0, advance the demo input list.
            if (--gCurrDemoInput->timer == 0) {
                gCurrDemoInput++;
            }
        }
    }
}

/**
 * Take the updated controller struct and calculate the new x, y, and distance floats.
 */
void adjust_analog_stick(struct Controller *controller) {
    // Reset the controller's x and y floats.
    controller->stickX = 0;
    controller->stickY = 0;

    // Modulate the rawStickX and rawStickY to be the new f32 values by adding/subtracting 6.
    if (controller->rawStickX <= -8) {
        controller->stickX = controller->rawStickX + 6;
    }

    if (controller->rawStickX >= 8) {
        controller->stickX = controller->rawStickX - 6;
    }

    if (controller->rawStickY <= -8) {
        controller->stickY = controller->rawStickY + 6;
    }

    if (controller->rawStickY >= 8) {
        controller->stickY = controller->rawStickY - 6;
    }

    // Calculate f32 magnitude from the center by vector length.
    controller->stickMag =
        sqrtf(controller->stickX * controller->stickX + controller->stickY * controller->stickY);

    // Magnitude cannot exceed 64.0f: if it does, modify the values
    // appropriately to flatten the values down to the allowed maximum value.
    if (controller->stickMag > 64) {
        controller->stickX *= 64 / controller->stickMag;
        controller->stickY *= 64 / controller->stickMag;
        controller->stickMag = 64;
    }
}

/**
 * Update the controller struct with available inputs if present.
 */
void read_controller_inputs(s32 threadID) {
    s32 i;

    // If any controllers are plugged in, update the controller information.
    if (gControllerBits) {
        if (threadID == THREAD_5_GAME_LOOP) {
            osRecvMesg(&gSIEventMesgQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
        }
        osContGetReadDataEx(&gControllerPads[0]);
#if ENABLE_RUMBLE
        release_rumble_pak_control();
#endif
    }
#if !defined(DISABLE_DEMO) && defined(KEEP_MARIO_HEAD)
    run_demo_inputs();
#endif

    for (i = 0; i < 2; i++) {
        struct Controller *controller = &gControllers[i];
        // if we're receiving inputs, update the controller struct with the new button info.
        if (controller->controllerData != NULL) {
            // HackerSM64: Swaps Z and L, only on console, and only when playing with a GameCube controller.
            if (gIsConsole && i == gGamecubeControllerPort) {
                u32 oldButton = controller->controllerData->button;
                u32 newButton = oldButton & ~(Z_TRIG | L_TRIG);
                if (oldButton & Z_TRIG) {
                    newButton |= L_TRIG;
                }
                if (controller->controllerData->l_trig > 85) { // How far the player has to press the L trigger for it to be considered a Z press. 64 is about 25%. 127 would be about 50%.
                    newButton |= Z_TRIG;
                }
                controller->controllerData->button = newButton;
            }
            controller->rawStickX = controller->controllerData->stick_x;
            controller->rawStickY = controller->controllerData->stick_y;
            controller->buttonPressed = controller->controllerData->button
                                        & (controller->controllerData->button ^ controller->buttonDown);
            // 0.5x A presses are a good meme
            controller->buttonDown = controller->controllerData->button;
            adjust_analog_stick(controller);
        } else { // otherwise, if the controllerData is NULL, 0 out all of the inputs.
            controller->rawStickX = 0;
            controller->rawStickY = 0;
            controller->buttonPressed = 0;
            controller->buttonDown = 0;
            controller->stickX = 0;
            controller->stickY = 0;
            controller->stickMag = 0;
        }
    }

    // For some reason, player 1's inputs are copied to player 3's port.
    // This potentially may have been a way the developers "recorded"
    // the inputs for demos, despite record_demo existing.
    gPlayer3Controller->rawStickX = gPlayer1Controller->rawStickX;
    gPlayer3Controller->rawStickY = gPlayer1Controller->rawStickY;
    gPlayer3Controller->stickX = gPlayer1Controller->stickX;
    gPlayer3Controller->stickY = gPlayer1Controller->stickY;
    gPlayer3Controller->stickMag = gPlayer1Controller->stickMag;
    gPlayer3Controller->buttonPressed = gPlayer1Controller->buttonPressed;
    gPlayer3Controller->buttonDown = gPlayer1Controller->buttonDown;
}

/**
 * Initialize the controller structs to point at the OSCont information.
 */
void init_controllers(void) {
    s16 port, cont;

    // Set controller 1 to point to the set of status/pads for input 1 and
    // init the controllers.
    gControllers[0].statusData = &gControllerStatuses[0];
    gControllers[0].controllerData = &gControllerPads[0];
    osContInit(&gSIEventMesgQueue, &gControllerBits, &gControllerStatuses[0]);

#ifdef EEP
    // strangely enough, the EEPROM probe for save data is done in this function.
    // save pak detection?
    gEepromProbe = osEepromProbe(&gSIEventMesgQueue);
#endif
#ifdef SRAM
    gSramProbe = nuPiInitSram();
#endif

    // Loop over the 4 ports and link the controller structs to the appropriate
    // status and pad. Interestingly, although there are pointers to 3 controllers,
    // only 2 are connected here. The third seems to have been reserved for debug
    // purposes and was never connected in the retail ROM, thus gPlayer3Controller
    // cannot be used, despite being referenced in various code.
    for (cont = 0, port = 0; port < 4 && cont < 2; port++) {
        // Is controller plugged in?
        if (gControllerBits & (1 << port)) {
            // The game allows you to have just 1 controller plugged
            // into any port in order to play the game. this was probably
            // so if any of the ports didn't work, you can have controllers
            // plugged into any of them and it will work.
            // gControllers[cont].port = port;
            gControllers[cont].statusData = &gControllerStatuses[port];
            gControllers[cont++].controllerData = &gControllerPads[port];
        }
    }
    if ((__osControllerTypes[1] == CONT_TYPE_GCN) && (gIsConsole)) {
        gGamecubeControllerPort = 1;
        gPlayer1Controller = &gControllers[1];
        gPlayer2Controller = &gControllers[0];
    } else {
        if (__osControllerTypes[0] == CONT_TYPE_GCN) {
            gGamecubeControllerPort = 0;
        }
        gPlayer1Controller = &gControllers[0];
        gPlayer2Controller = &gControllers[1];
    }
}

// Game thread core
// ----------------------------------------------------------------------------------------------------

/**
 * Setup main segments and framebuffers.
 */
void setup_game_memory(void) {
    // Setup general Segment 0
    set_segment_base_addr(SEGMENT_MAIN, (void *)RAM_START);
    // Create Mesg Queues
    osCreateMesgQueue(&gGfxVblankQueue, gGfxMesgBuf, ARRAY_COUNT(gGfxMesgBuf));
    osCreateMesgQueue(&gGameVblankQueue, gGameMesgBuf, ARRAY_COUNT(gGameMesgBuf));
    // Setup z buffer and framebuffer
    gPhysicalZBuffer = VIRTUAL_TO_PHYSICAL(gZBuffer);
    gPhysicalFramebuffers[0] = VIRTUAL_TO_PHYSICAL(gFramebuffer0);
    gPhysicalFramebuffers[1] = VIRTUAL_TO_PHYSICAL(gFramebuffer1);
    gPhysicalFramebuffers[2] = VIRTUAL_TO_PHYSICAL(gFramebuffer2);
    // Setup Mario Animations
    gMarioAnimsMemAlloc = main_pool_alloc(MARIO_ANIMS_POOL_SIZE, MEMORY_POOL_LEFT);
    set_segment_base_addr(SEGMENT_MARIO_ANIMS, (void *) gMarioAnimsMemAlloc);
    setup_dma_table_list(&gMarioAnimsBuf, gMarioAnims, gMarioAnimsMemAlloc);
    // Setup Demo Inputs List
    gDemoInputsMemAlloc = main_pool_alloc(DEMO_INPUTS_POOL_SIZE, MEMORY_POOL_LEFT);
    set_segment_base_addr(SEGMENT_DEMO_INPUTS, (void *) gDemoInputsMemAlloc);
    setup_dma_table_list(&gDemoInputsBuf, gDemoInputs, gDemoInputsMemAlloc);
    // Setup Level Script Entry
    load_segment(SEGMENT_LEVEL_ENTRY, _entrySegmentRomStart, _entrySegmentRomEnd, MEMORY_POOL_LEFT, NULL, NULL);
    // Setup Segment 2 (Fonts, Text, etc)
    load_segment_decompress(SEGMENT_SEGMENT2, _segment2_mio0SegmentRomStart, _segment2_mio0SegmentRomEnd);
}

/**
 * Main game loop thread. Runs forever as long as the game continues.
 */
Bool32 gSupportsLibpl = FALSE;
Bool32 gIsGliden = FALSE;

FATFS fs;
DIR cmm_dir;
FRESULT mount_success;
FRESULT directory_success;
FILINFO cmm_dir_info;

#define MAX_FILES 20
FILINFO cmm_level_entries[MAX_FILES];
u16 cmm_level_entry_piktcher[MAX_FILES][64][64];
struct cmm_level_save temp_cmm_save;


u8 cmm_level_entry_count = 0;
FRESULT global_code;

TCHAR cmm_dir_name[] = {"Mario Builder 64 Levels"};

void load_level_files_from_sd_card(void) {
    f_opendir(&cmm_dir,"");
    s8 i = -1;
    do {
        i++;
        if (f_readdir(&cmm_dir,&cmm_level_entries[i]) == FR_OK) {
            u32 bytes_read;
            FIL read_file;
            f_open(&read_file,&cmm_level_entries[i].fname, FA_READ);
            f_read(&read_file,&temp_cmm_save,sizeof(temp_cmm_save),&bytes_read);
            f_close(&read_file);

            s16 x;
            s16 y;
            u16 *u16_array = cmm_level_entry_piktcher[i];
            for (x = 0; x < 64; x++) {
                for (y = 0; y < 64; y++) {
                    u16_array[(y*64)+x] = temp_cmm_save.piktcher[y][x];
                } 
            }
        }

    } while (cmm_level_entries[i].fname[0] != 0);

    cmm_level_entry_count = i;
    f_closedir(&cmm_dir);
}

void thread5_game_loop(UNUSED void *arg) {
    setup_game_memory();
#if ENABLE_RUMBLE
    init_rumble_pak_scheduler_queue();
#endif
    init_controllers();
#if ENABLE_RUMBLE
    create_thread_6();
#endif
#ifdef HVQM
    createHvqmThread();
#endif
    save_file_load_all();
#ifdef PUPPYCAM
    puppycam_boot();
#endif

    set_vblank_handler(2, &gGameVblankHandler, &gGameVblankQueue, (OSMesg) 1);

    // Point address to the entry point into the level script data.
    struct LevelCommand *addr = segmented_to_virtual(level_script_entry);

    play_music(SEQ_PLAYER_SFX, SEQUENCE_ARGS(0, SEQ_SOUND_PLAYER), 0);
    set_sound_mode(0);
#ifdef WIDE
    gConfig.widescreen = save_file_get_widescreen_mode();
#endif
    render_init();

    gSupportsLibpl = libpl_is_supported( LPL_ABI_VERSION_CURRENT );
    if (gSupportsLibpl) {
        libpl_create_auto_sd_card(16,255);
        lpl_plugin_info *pluginInfo = libpl_get_graphics_plugin();
        gIsGliden = ((pluginInfo->plugin_id == LPL_GLN64)||(pluginInfo->plugin_id == LPL_GLIDEN64));
    }
    //init cmm file structure
    cart_init();
    mount_success = f_mount(&fs, "", 1);
    if (mount_success == FR_OK) {
        //mount is successful

        //create directory if not exist, otherwise open it if it does
        FRESULT chdir_success = FR_NO_FILE; //init it with something that we won't actually check
        directory_success = f_stat(&cmm_dir_name,&cmm_dir_info);
        if (directory_success == FR_NO_FILE) {
            //does not exist, therefore make
            f_mkdir(&cmm_dir_name);
            f_opendir(&cmm_dir,&cmm_dir_name);
            chdir_success = f_chdir(&cmm_dir_name);
        }
        if (directory_success == FR_OK) {
            chdir_success = f_chdir(&cmm_dir_name);
        }

        if (chdir_success != FR_OK) {
            while(TRUE){}
            //freeze the game if directory changing went wrong
        } else {
            load_level_files_from_sd_card();
        }
    }

    while (TRUE) {
        profiler_frame_setup();
        // If the reset timer is active, run the process to reset the game.
        if (gResetTimer != 0) {
            draw_reset_bars();
            continue;
        }

        // If any controllers are plugged in, start read the data for when
        // read_controller_inputs is called later.
        if (gControllerBits) {
#if ENABLE_RUMBLE
            block_until_rumble_pak_free();
#endif
            osContStartReadDataEx(&gSIEventMesgQueue);
        }

        audio_game_loop_tick();
        select_gfx_pool();
        read_controller_inputs(THREAD_5_GAME_LOOP);
        profiler_update(PROFILER_TIME_CONTROLLERS);
        addr = level_script_execute(addr);
#if !PUPPYPRINT_DEBUG && defined(VISUAL_DEBUG)
        debug_box_input();
#endif
#if PUPPYPRINT_DEBUG
        puppyprint_profiler_process();
#endif

        display_and_vsync();
#ifdef VANILLA_DEBUG
        // when debug info is enabled, print the "BUF %d" information.
        if (gShowDebugText) {
            // subtract the end of the gfx pool with the display list to obtain the
            // amount of free space remaining.
            print_text_fmt_int(180, 20, "BUF %d", gGfxPoolEnd - (u8 *) gDisplayListHead);
        }
#endif
#if 0
        if (gPlayer1Controller->buttonPressed & L_TRIG) {
            osStartThread(&hvqmThread);
            osRecvMesg(&gDmaMesgQueue, NULL, OS_MESG_BLOCK);
        }
#endif
    }
}
