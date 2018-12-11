/* Lebed Pavel, IPMM, 23631\2, 27.02.2018 */

#ifndef __COLORDEF_H_INCLUDED_
#define __COLORDEF_H_INCLUDED_
#pragma once

#include <vector>

using color_t = std::vector<float>;

static const color_t Red{1.0, 0, 0};
static const color_t Green{0, 1, 0};
static const color_t Blue{0.0, 0, 1};
static const color_t Black{0, 0, 0};
static const color_t SkyBlue{135.0 / 256, 206.0 / 256, 235.0 / 256};


#endif /* __COLORDEF_H_INCLUDED_*/

/* END OF 'colordef.h' FILE */

