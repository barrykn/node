// Copyright 2021 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// CPU specific code for arm independent of OS goes here.

#ifdef __OpenBSD__
# include <sys/types.h>
# include <machine/sysarch.h>
#else
# include <sys/syscall.h>
#endif

#include <unistd.h>

#if V8_TARGET_ARCH_RISCV64

#include "src/codegen/cpu-features.h"

namespace v8 {
namespace internal {

void CpuFeatures::FlushICache(void* start, size_t size) {
#if !defined(USE_SIMULATOR)
# ifdef __OpenBSD__
	struct riscv_sync_icache_args args = { (u_int64_t)(uintptr_t)start, size };
	sysarch(RISCV_SYNC_ICACHE, &args);
# else
  char* end = reinterpret_cast<char*>(start) + size;
  // The definition of this syscall is
  // SYSCALL_DEFINE3(riscv_flush_icache, uintptr_t, start,
  //                 uintptr_t, end, uintptr_t, flags)
  // The flag here is set to be SYS_RISCV_FLUSH_ICACHE_LOCAL, which is
  // defined as 1 in the Linux kernel.
  syscall(SYS_riscv_flush_icache, start, end, 1);
# endif // !__OpenBSD__
#endif  // !USE_SIMULATOR.
}

}  // namespace internal
}  // namespace v8

#endif  // V8_TARGET_ARCH_RISCV64
