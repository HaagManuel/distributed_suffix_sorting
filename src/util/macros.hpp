// copied from: https://github.com/kurpicz/dsss/blob/master/dsss/util/macros.hpp

/*******************************************************************************
 * util/macros.hpp
 *
 * Copyright (C) 2018 Florian Kurpicz <florian.kurpicz@tu-dortmund.de>
 *
 * All rights reserved. Published under the BSD-2 license in the LICENSE file.
 ******************************************************************************/

#pragma once

#if defined(__GNUC__) || defined(__clang__)
#define DSSS_LIKELY(c)   __builtin_expect((c), 1)
#define DSSS_UNLIKELY(c) __builtin_expect((c), 0)
#else
#define DSSS_LIKELY(c)   c
#define DSSS_UNLIKELY(c) c
#endif

#if defined(__GNUC__) || defined(__clang__)
#define DSSS_ATTRIBUTE_PACKED __attribute__((packed))
#else
#define DSSS_ATTRIBUTE_PACKED
#endif

/******************************************************************************/