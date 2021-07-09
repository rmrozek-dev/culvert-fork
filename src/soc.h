/* SPDX-License-Identifier: Apache-2.0 */
/* Copyright (C) 2021 IBM Corp. */

#ifndef _SOC_H
#define _SOC_H

#include "ahb.h"
#include "rev.h"

#include <stddef.h>
#include <stdint.h>

struct soc_fdt {
	void *start;
	void *end;
};

struct soc {
	uint32_t rev;
	struct soc_fdt fdt;
	struct ahb *ahb;
};

int soc_probe(struct soc *ctx, struct ahb *ahb);

void soc_destroy(struct soc *ctx);

static inline enum ast_generation soc_generation(struct soc *ctx)
{
	return rev_generation(ctx->rev);
}

static inline int soc_stepping(struct soc *ctx)
{
	return rev_stepping(ctx->rev);
}

static inline ssize_t
soc_read(struct soc *ctx, uint32_t phys, void *buf, size_t len)
{
	return ahb_read(ctx->ahb, phys, buf, len);
}

static inline ssize_t
soc_write(struct soc *ctx, uint32_t phys, const void *buf, size_t len)
{
	return ahb_write(ctx->ahb, phys, buf, len);
}

static inline int soc_readl(struct soc *ctx, uint32_t phys, uint32_t *val)
{
	return ahb_readl(ctx->ahb, phys, val);
}

static inline int soc_writel(struct soc *ctx, uint32_t phys, uint32_t val)
{
	return ahb_writel(ctx->ahb, phys, val);
}

static inline ssize_t
soc_siphon_in(struct soc *ctx, uint32_t phys, size_t len, int outfd)
{
	return ahb_siphon_in(ctx->ahb, phys, len, outfd);
}

static inline ssize_t
soc_siphon_out(struct soc *ctx, uint32_t phys, int infd)
{
	return ahb_siphon_out(ctx->ahb, phys, infd);
}

struct soc_device_node {
	int offset;
};

struct soc_device_id {
	const char *compatible;
	const void *data;
};

int soc_device_match_node(struct soc *ctx,
			  const struct soc_device_id table[],
			  struct soc_device_node *dn);

int soc_device_from_name(struct soc *ctx, const char *name,
			 struct soc_device_node *dn);

int soc_device_from_type(struct soc *ctx, const char *type,
			 struct soc_device_node *dn);

int soc_device_is_compatible(struct soc *ctx,
			     const struct soc_device_id table[],
			     const struct soc_device_node *dn);

const void *soc_device_get_match_data(struct soc *ctx,
				      const struct soc_device_id table[],
				      const struct soc_device_node *dn);

struct soc_region {
	uint32_t start;
	uint32_t length;
};

int
soc_device_get_memory_index(struct soc *ctx, const struct soc_device_node *dn,
			    int index, struct soc_region *region);

static inline int
soc_device_get_memory(struct soc *ctx, const struct soc_device_node *dn,
		      struct soc_region *region)
{
	return soc_device_get_memory_index(ctx, dn, 0, region);
}
#endif